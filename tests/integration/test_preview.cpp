#include "previewpanel.h"

#include <QFile>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QWebEngineView>
#include <QtTest>

class PreviewTests final : public QObject
{
    Q_OBJECT

private slots:
    void loadsAndReloadsALocalPage();
};

void PreviewTests::loadsAndReloadsALocalPage()
{
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    const QString pagePath = directory.filePath(QStringLiteral("preview.html"));
    QFile page(pagePath);
    QVERIFY(page.open(QIODevice::WriteOnly));
    const QByteArray html("<!doctype html><title>Kogence local preview</title>");
    QCOMPARE(page.write(html), qint64(html.size()));
    page.close();

    PreviewPanel panel;
    auto *urlInput = panel.findChild<QLineEdit *>(QStringLiteral("previewUrlInput"));
    auto *openButton = panel.findChild<QPushButton *>(QStringLiteral("previewOpenButton"));
    auto *reloadButton = panel.findChild<QPushButton *>(QStringLiteral("previewReloadButton"));
    auto *view = panel.findChild<QWebEngineView *>(QStringLiteral("previewWebView"));
    QVERIFY(urlInput);
    QVERIFY(openButton);
    QVERIFY(reloadButton);
    QVERIFY(view);

    QSignalSpy loadSpy(view, &QWebEngineView::loadFinished);
    QVERIFY(loadSpy.isValid());
    const QUrl localUrl = QUrl::fromLocalFile(pagePath);
    urlInput->setText(localUrl.toString());
    QTest::mouseClick(openButton, Qt::LeftButton);
    QTRY_COMPARE_WITH_TIMEOUT(view->title(),
                              QStringLiteral("Kogence local preview"),
                              15000);
    QCOMPARE(view->url(), localUrl);

    QTest::mouseClick(reloadButton, Qt::LeftButton);
    if (loadSpy.isEmpty())
        QVERIFY(loadSpy.wait(15000));
    QCOMPARE(loadSpy.takeFirst().at(0).toBool(), true);
    QCOMPARE(view->title(), QStringLiteral("Kogence local preview"));
}

QTEST_MAIN(PreviewTests)

#include "test_preview.moc"
