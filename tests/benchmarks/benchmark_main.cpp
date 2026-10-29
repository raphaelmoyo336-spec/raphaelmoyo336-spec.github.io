#include "editorpanel.h"
#include "fileexplorerwidget.h"

#include <DockManager.h>
#include <DockWidget.h>

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <Qsci/qsciscintilla.h>
#include <QSaveFile>
#include <QSysInfo>
#include <QTemporaryDir>
#include <QTextStream>
#include <QTime>
#include <QVector>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>

#ifdef Q_OS_LINUX
#include <unistd.h>
#endif

#ifndef KOGENCE_GIT_REVISION
#define KOGENCE_GIT_REVISION "unknown"
#endif

#ifndef KOGENCE_BUILD_TYPE
#define KOGENCE_BUILD_TYPE "unspecified"
#endif

#ifndef KOGENCE_COMPILER_ID
#define KOGENCE_COMPILER_ID "unknown"
#endif

#ifndef KOGENCE_COMPILER_VERSION
#define KOGENCE_COMPILER_VERSION "unknown"
#endif

namespace
{
QString readCpuModel()
{
#ifdef Q_OS_LINUX
    QFile file(QStringLiteral("/proc/cpuinfo"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            const QString line = QString::fromLocal8Bit(file.readLine()).trimmed();
            if (line.startsWith(QStringLiteral("model name"))) {
                const qsizetype separator = line.indexOf(QLatin1Char(':'));
                if (separator >= 0)
                    return line.mid(separator + 1).trimmed();
            }
        }
    }
#endif
    return QSysInfo::currentCpuArchitecture();
}

qint64 totalMemoryKiB()
{
#ifdef Q_OS_LINUX
    QFile file(QStringLiteral("/proc/meminfo"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            const QByteArray line = file.readLine();
            if (line.startsWith("MemTotal:")) {
                const QList<QByteArray> fields = line.simplified().split(' ');
                if (fields.size() > 1)
                    return fields.at(1).toLongLong();
            }
        }
    }
#endif
    return -1;
}

qint64 residentMemoryKiB()
{
#ifdef Q_OS_LINUX
    QFile file(QStringLiteral("/proc/self/statm"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QList<QByteArray> fields = file.readAll().simplified().split(' ');
        if (fields.size() > 1) {
            const long pages = fields.at(1).toLong();
            const long pageSize = sysconf(_SC_PAGESIZE);
            if (pages >= 0 && pageSize > 0)
                return (qint64(pages) * pageSize) / 1024;
        }
    }
#endif
    return -1;
}

QJsonObject environmentInfo()
{
    return {
        {QStringLiteral("git_revision"), QString::fromLatin1(KOGENCE_GIT_REVISION)},
        {QStringLiteral("qt_version"), QString::fromLatin1(qVersion())},
        {QStringLiteral("os"), QSysInfo::prettyProductName()},
        {QStringLiteral("cpu"), readCpuModel()},
        {QStringLiteral("system_memory_kib"), totalMemoryKiB()},
        {QStringLiteral("build_type"), QString::fromLatin1(KOGENCE_BUILD_TYPE)},
        {QStringLiteral("compiler"),
         QStringLiteral("%1 %2").arg(QString::fromLatin1(KOGENCE_COMPILER_ID),
                                      QString::fromLatin1(KOGENCE_COMPILER_VERSION))}
    };
}

struct Measurement
{
    QString name;
    QJsonObject workload;
    QVector<double> samplesMs;
    qint64 residentBeforeKiB = -1;
    qint64 residentAfterKiB = -1;

    QJsonObject toJson(int warmupCount) const
    {
        QVector<double> sorted = samplesMs;
        std::sort(sorted.begin(), sorted.end());
        const double mean = std::accumulate(sorted.cbegin(), sorted.cend(), 0.0)
            / sorted.size();
        double squaredDeviation = 0.0;
        for (double sample : sorted)
            squaredDeviation += (sample - mean) * (sample - mean);
        const double standardDeviation =
            std::sqrt(squaredDeviation / sorted.size());
        const double median = sorted.size() % 2
            ? sorted.at(sorted.size() / 2)
            : (sorted.at(sorted.size() / 2 - 1) + sorted.at(sorted.size() / 2)) / 2.0;
        QJsonArray rawSamples;
        for (double sample : samplesMs)
            rawSamples.append(sample);
        return {
            {QStringLiteral("name"), name},
            {QStringLiteral("status"), QStringLiteral("completed")},
            {QStringLiteral("workload"), workload},
            {QStringLiteral("iterations"), samplesMs.size()},
            {QStringLiteral("warmup_iterations"), warmupCount},
            {QStringLiteral("raw_samples_ms"), rawSamples},
            {QStringLiteral("median_ms"), median},
            {QStringLiteral("minimum_ms"), sorted.first()},
            {QStringLiteral("maximum_ms"), sorted.last()},
            {QStringLiteral("mean_ms"), mean},
            {QStringLiteral("standard_deviation_ms"), standardDeviation},
            {QStringLiteral("resident_memory_before_kib"), residentBeforeKiB},
            {QStringLiteral("resident_memory_after_kib"), residentAfterKiB},
            {QStringLiteral("resident_memory_measurement"),
             QStringLiteral("Linux process RSS from /proc/self/statm; allocator-retained pages may remain")}
        };
    }
};

Measurement measure(const QString &name,
                    const QJsonObject &workload,
                    int warmupCount,
                    int iterations,
                    const std::function<void()> &operation)
{
    for (int index = 0; index < warmupCount; ++index)
        operation();

    Measurement result{name, workload};
    result.residentBeforeKiB = residentMemoryKiB();
    for (int index = 0; index < iterations; ++index) {
        QElapsedTimer timer;
        timer.start();
        operation();
        result.samplesMs.append(timer.nsecsElapsed() / 1'000'000.0);
    }
    result.residentAfterKiB = residentMemoryKiB();
    return result;
}

bool openEditorFile(EditorPanel &panel, const QString &path)
{
    auto *explorer = panel.findChild<FileExplorerWidget *>();
    return explorer
        && QMetaObject::invokeMethod(explorer,
                                     "fileActivated",
                                     Qt::DirectConnection,
                                     Q_ARG(QString, path));
}

void printUsage()
{
    std::cerr << "Usage: kogence_benchmarks [--iterations=N] [--warmup=N] "
                 "[--output=FILE]\n";
}
} // namespace

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication application(argc, argv);
    int iterations = 25;
    int warmupCount = 3;
    QString outputPath;
    for (const QString &argument : application.arguments().mid(1)) {
        if (argument.startsWith(QStringLiteral("--iterations=")))
            iterations = argument.mid(QStringLiteral("--iterations=").size()).toInt();
        else if (argument.startsWith(QStringLiteral("--warmup=")))
            warmupCount = argument.mid(QStringLiteral("--warmup=").size()).toInt();
        else if (argument.startsWith(QStringLiteral("--output=")))
            outputPath = argument.mid(QStringLiteral("--output=").size());
        else {
            printUsage();
            return 2;
        }
    }
    if (iterations < 1 || warmupCount < 0) {
        printUsage();
        return 2;
    }

    QTemporaryDir temporaryDirectory;
    if (!temporaryDirectory.isValid()) {
        std::cerr << "Unable to create isolated benchmark data directory.\n";
        return 1;
    }

    const int fileBytes = 256 * 1024;
    const QString payload(fileBytes, QLatin1Char('k'));
    const QString firstPath = temporaryDirectory.filePath(QStringLiteral("first.txt"));
    const QString secondPath = temporaryDirectory.filePath(QStringLiteral("second.txt"));
    for (const QString &path : {firstPath, secondPath}) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)
            || file.write(payload.toUtf8()) != fileBytes) {
            std::cerr << "Unable to write the isolated editor benchmark fixture.\n";
            return 1;
        }
    }

    EditorPanel editor;
    if (!editor.openProjectDirectory(temporaryDirectory.path())
        || !openEditorFile(editor, firstPath)) {
        std::cerr << "Unable to initialize the editor benchmark fixture.\n";
        return 1;
    }
    auto *textEditor = editor.findChild<QsciScintilla *>(QStringLiteral("sourceEditor"));
    if (!textEditor) {
        std::cerr << "Editor benchmark could not locate the source editor.\n";
        return 1;
    }

    QVector<Measurement> measurements;
    measurements.append(measure(
        QStringLiteral("editor.set_plain_text"),
        {{QStringLiteral("document_bytes"), fileBytes}},
        warmupCount,
        iterations,
        [&]() {
            textEditor->setText(payload);
            textEditor->setModified(false);
        }));

    measurements.append(measure(
        QStringLiteral("editor.open_file"),
        {{QStringLiteral("file_bytes"), fileBytes}},
        warmupCount,
        iterations,
        [&, nextPath = false]() mutable {
            const QString &path = nextPath ? firstPath : secondPath;
            if (!openEditorFile(editor, path))
                qFatal("Could not activate a file during editor-open benchmark.");
            nextPath = !nextPath;
        }));

    measurements.append(measure(
        QStringLiteral("editor.save_file"),
        {{QStringLiteral("file_bytes"), fileBytes}},
        warmupCount,
        iterations,
        [&]() {
            if (!editor.saveCurrentFile())
                qFatal("Editor save failed during benchmark.");
        }));

    ads::CDockManager::setConfigFlags(ads::CDockManager::DefaultOpaqueConfig);
    QWidget dockHost;
    auto *dockManager = new ads::CDockManager(&dockHost);
    ads::CDockWidget *firstDock = nullptr;
    for (int index = 0; index < 12; ++index) {
        auto *dock = dockManager->createDockWidget(
            QStringLiteral("Benchmark panel %1").arg(index));
        dock->setObjectName(QStringLiteral("benchmarkPanel%1").arg(index));
        dock->setWidget(new QWidget);
        if (index == 0) {
            firstDock = dock;
            dockManager->addDockWidget(ads::CenterDockWidgetArea, dock);
        } else {
            dockManager->addDockWidget(ads::RightDockWidgetArea,
                                       dock,
                                       firstDock->dockAreaWidget());
        }
    }
    const QByteArray savedDockState = dockManager->saveState();
    if (savedDockState.isEmpty()) {
        std::cerr << "ADS returned an empty layout state.\n";
        return 1;
    }
    measurements.append(measure(
        QStringLiteral("ads.restore_layout"),
        {{QStringLiteral("dock_widgets"), 12},
         {QStringLiteral("serialized_state_bytes"), savedDockState.size()}},
        warmupCount,
        iterations,
        [&]() {
            if (!dockManager->restoreState(savedDockState))
                qFatal("ADS layout restore failed during benchmark.");
        }));

    QJsonArray benchmarkResults;
    for (const Measurement &measurement : measurements)
        benchmarkResults.append(measurement.toJson(warmupCount));
    const QJsonObject report{
        {QStringLiteral("schema_version"), 1},
        {QStringLiteral("generated_utc"),
         QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs)},
        {QStringLiteral("environment"), environmentInfo()},
        {QStringLiteral("configuration"),
         QJsonObject{{QStringLiteral("iterations"), iterations},
                     {QStringLiteral("warmup_iterations"), warmupCount},
                     {QStringLiteral("clock"), QStringLiteral("QElapsedTimer monotonic nanoseconds")},
                     {QStringLiteral("filesystem_cache"),
                      QStringLiteral("OS cache is not flushed; repeated file operations may be cached")}}},
        {QStringLiteral("benchmarks"), benchmarkResults}
    };
    const QByteArray json = QJsonDocument(report).toJson(QJsonDocument::Indented);
    if (outputPath.isEmpty()) {
        std::cout << json.constData();
        return 0;
    }

    QFile output(outputPath);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Truncate)
        || output.write(json) != json.size()) {
        std::cerr << "Unable to write benchmark report: "
                  << output.errorString().toStdString() << '\n';
        return 1;
    }
    return 0;
}
