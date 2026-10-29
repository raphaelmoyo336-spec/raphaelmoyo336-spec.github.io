#include "terminalpanel.h"

#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QFontInfo>
#include <QFontMetrics>
#include <QFontMetricsF>
#include <QImage>
#include <qtermwidget.h>
#include <QSignalSpy>
#include <QtTest>

class TerminalTests final : public QObject
{
    Q_OBJECT

private slots:
    void terminalUsesKogenceTypography();
    void shellStartsAndRunsCommands();
    void terminalRendersDistinctCharacters();
    void shellExitIsReported();
};

void TerminalTests::terminalUsesKogenceTypography()
{
    TerminalPanel terminal;
    auto *emulator = terminal.findChild<QTermWidget *>(QStringLiteral("terminalEmulator"));
    QVERIFY(emulator);
    const QFont font = emulator->getTerminalFont();
    QCOMPARE(font.pointSize(), 10);
    QCOMPARE(font.styleHint(), QFont::Monospace);
    QVERIFY(QFontInfo(font).fixedPitch());
    const QString expectedFamily = QFontDatabase::families().contains(
                                       QStringLiteral("DejaVu Sans Mono"))
                                       ? QStringLiteral("DejaVu Sans Mono")
                                       : QFontInfo(QFontDatabase::systemFont(
                                           QFontDatabase::FixedFont)).family();
    QCOMPARE(QFontInfo(font).family(), expectedFamily);
    const QFontMetricsF metrics(font, emulator);
    for (const QChar character : QStringLiteral("iW0 "))
        QCOMPARE(metrics.horizontalAdvance(character),
                 qreal(qRound(metrics.horizontalAdvance(character))));
    QCOMPARE(metrics.horizontalAdvance(QLatin1Char('i')),
             metrics.horizontalAdvance(QLatin1Char('W')));
    QCOMPARE(metrics.horizontalAdvance(QLatin1Char('0')),
             metrics.horizontalAdvance(QLatin1Char(' ')));
    QVERIFY(QFile::exists(QString::fromUtf8(KOGENCE_TERMINAL_COLOR_SCHEME)));
    QVERIFY(QTermWidget::availableColorSchemes().contains(QStringLiteral("kogence-terminal")));
}

void TerminalTests::shellStartsAndRunsCommands()
{
    TerminalPanel terminal;
    auto *emulator = terminal.findChild<QTermWidget *>(QStringLiteral("terminalEmulator"));
    QVERIFY(emulator);
    QTRY_VERIFY_WITH_TIMEOUT(emulator->getShellPID() > 0, 5000);

    QString received;
    connect(emulator, &QTermWidget::receivedData, this, [&received](const QString &text) {
        received.append(text);
    });
    const QString shellPid = QString::number(emulator->getShellPID());
    emulator->sendText(QStringLiteral("printf '%s\\n' \"$$\"\n"));

    QTRY_VERIFY_WITH_TIMEOUT(received.contains(shellPid), 5000);
}

void TerminalTests::terminalRendersDistinctCharacters()
{
    TerminalPanel terminal;
    terminal.resize(800, 400);
    terminal.show();
    auto *emulator = terminal.findChild<QTermWidget *>(QStringLiteral("terminalEmulator"));
    QVERIFY(emulator);
    QTRY_VERIFY_WITH_TIMEOUT(emulator->getShellPID() > 0, 5000);

    QString received;
    connect(emulator, &QTermWidget::receivedData, this, [&received](const QString &text) {
        received.append(text);
    });
    emulator->sendText(QStringLiteral("printf '\\033[2J\\033[H'; printf 'iW0X\\n'\n"));
    QTRY_VERIFY_WITH_TIMEOUT(received.contains(QStringLiteral("iW0X\r\n")), 5000);
    QTest::qWait(100);

    const QFontMetrics metrics(emulator->getTerminalFont());
    const int cellWidth = metrics.horizontalAdvance(QLatin1Char('M'));
    const int cellHeight = metrics.height();
    const QImage rendered = emulator->grab().toImage();
    const QRect wCell(1 + cellWidth, 1, cellWidth, cellHeight);
    const QRect zeroCell(1 + 2 * cellWidth, 1, cellWidth, cellHeight);
    QVERIFY(rendered.rect().contains(wCell));
    QVERIFY(rendered.rect().contains(zeroCell));
    QVERIFY(rendered.copy(wCell) != rendered.copy(zeroCell));
}

void TerminalTests::shellExitIsReported()
{
    TerminalPanel terminal;
    auto *emulator = terminal.findChild<QTermWidget *>(QStringLiteral("terminalEmulator"));
    QVERIFY(emulator);
    QTRY_VERIFY_WITH_TIMEOUT(emulator->getShellPID() > 0, 5000);

    QSignalSpy finished(emulator, &QTermWidget::finished);
    QVERIFY(finished.isValid());
    emulator->sendText(QStringLiteral("exit\n"));
    QTRY_COMPARE_WITH_TIMEOUT(finished.count(), 1, 5000);
}

QTEST_MAIN(TerminalTests)

#include "test_terminal.moc"
