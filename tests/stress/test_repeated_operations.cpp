#include "editorpanel.h"
#include "fileexplorerwidget.h"
#include "test_workload.h"

#include <DockManager.h>
#include <DockWidget.h>

#include <QDir>
#include <QFile>
#include <Qsci/qsciscintilla.h>
#include <QTemporaryDir>
#include <QtTest>

class RepeatedOperationTests final : public QObject
{
    Q_OBJECT

private slots:
    void repeatedEditorSavesPreserveTheLatestContent();
    void repeatedDockFloatAndRedockKeepsWidgetsRegistered();
};

void RepeatedOperationTests::repeatedEditorSavesPreserveTheLatestContent()
{
    const TestWorkload workload = testWorkload();
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString projectPath = directory.filePath(QStringLiteral("project"));
    QVERIFY(QDir().mkpath(projectPath));

    const QString filePath = QDir(projectPath).filePath(QStringLiteral("stress.txt"));
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();

    EditorPanel panel;
    QVERIFY(panel.openProjectDirectory(projectPath));
    auto *explorer = panel.findChild<FileExplorerWidget *>();
    auto *editor = panel.findChild<QsciScintilla *>(QStringLiteral("sourceEditor"));
    QVERIFY(explorer);
    QVERIFY(editor);
    QVERIFY(QMetaObject::invokeMethod(explorer,
                                      "fileActivated",
                                      Qt::DirectConnection,
                                      Q_ARG(QString, filePath)));

    const QString padding(workload.documentBytes, QLatin1Char('x'));
    for (int iteration = 0; iteration < workload.repetitions; ++iteration) {
        const QString content =
            QStringLiteral("iteration=%1\n").arg(iteration) + padding;
        editor->setText(content);
        QVERIFY2(panel.saveCurrentFile(),
                 qPrintable(QStringLiteral("Save failed on iteration %1").arg(iteration)));
    }

    QVERIFY(file.open(QIODevice::ReadOnly));
    const QString persisted = QString::fromUtf8(file.readAll());
    QVERIFY(persisted.startsWith(
        QStringLiteral("iteration=%1\n").arg(workload.repetitions - 1)));
    QCOMPARE(persisted.size(), workload.documentBytes
                                   + QStringLiteral("iteration=%1\n")
                                         .arg(workload.repetitions - 1)
                                         .size());
}

void RepeatedOperationTests::repeatedDockFloatAndRedockKeepsWidgetsRegistered()
{
    const TestWorkload workload = testWorkload();
    QWidget host;
    auto *manager = new ads::CDockManager(&host);
    QList<ads::CDockWidget *> docks;
    for (int index = 0; index < workload.dockWidgets; ++index) {
        auto *dock = manager->createDockWidget(
            QStringLiteral("Test panel %1").arg(index));
        dock->setObjectName(QStringLiteral("testPanel%1").arg(index));
        dock->setWidget(new QWidget);
        if (docks.isEmpty())
            manager->addDockWidget(ads::CenterDockWidgetArea, dock);
        else
            manager->addDockWidget(ads::CenterDockWidgetArea,
                                   dock,
                                   docks.first()->dockAreaWidget());
        docks.append(dock);
    }

    host.resize(800, 600);
    host.show();
    QVERIFY(!manager->saveState().isEmpty());
    const int roundTrips = qMin(workload.repetitions, docks.size());
    for (int index = 0; index < roundTrips; ++index) {
        auto *dock = docks.at(index);
        manager->addDockWidgetFloating(dock);
        QTRY_VERIFY(dock->isInFloatingContainer());
        auto *targetDock = docks.at(index == docks.size() - 1 ? 0 : docks.size() - 1);
        manager->addDockWidget(ads::CenterDockWidgetArea,
                               dock,
                               targetDock->dockAreaWidget());
        QTRY_VERIFY(!dock->isInFloatingContainer());
        QCOMPARE(manager->findDockWidget(dock->objectName()), dock);
    }

    const QByteArray savedState = manager->saveState();
    QVERIFY(!savedState.isEmpty());
    QVERIFY(manager->restoreState(savedState));
    QCOMPARE(manager->dockWidgetsMap().size(), workload.dockWidgets);
    host.close();
}

QTEST_MAIN(RepeatedOperationTests)

#include "test_repeated_operations.moc"
