#include "mainwindow.h"
#include "editorpanel.h"
#include "kodesignerpanel.h"
#include "thememanager.h"

#include <DockManager.h>
#include <DockWidget.h>

#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDir>
#include <QDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>
#include <QTemporaryDir>
#include <QTabBar>
#include <QTimer>
#include <QPushButton>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <Qsci/qsciscintilla.h>
#include <QtTest>

class MainWindowTests final : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void createsAndTogglesTheImplementedPanels();
    void loadsKoDesignerFromBundledAssets();
    void opensAndSavesKoDesignerProjectDocuments();
    void importsAndExportsExistingHtmlDesigns();
    void floatingPanelCanBeReattached();
    void savesAndRestoresDockLayout();
    void editorToolbarAndNavbarShareSaveAction();
    void floatingEditorPreservesItsDocument();
    void themeSelectorChangesApplicationAndEditorIndependently();

private:
    QTemporaryDir settingsDirectory_;
};

void MainWindowTests::initTestCase()
{
    QVERIFY(settingsDirectory_.isValid());
    QCoreApplication::setOrganizationName(QStringLiteral("KogenceTest"));
    QCoreApplication::setApplicationName(QStringLiteral("KogenceIDETests"));
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat,
                       QSettings::UserScope,
                       settingsDirectory_.path());
    QSettings::setPath(QSettings::NativeFormat,
                       QSettings::UserScope,
                       settingsDirectory_.path());
}

void MainWindowTests::init()
{
    QSettings().clear();
}

void MainWindowTests::cleanup()
{
    QSettings().clear();
}

void MainWindowTests::createsAndTogglesTheImplementedPanels()
{
    MainWindow window;
    auto *manager = window.findChild<ads::CDockManager *>();
    QVERIFY(manager);

    const QStringList expectedNames = {
        QStringLiteral("editorDock"),
        QStringLiteral("terminalDock"),
        QStringLiteral("previewDock"),
        QStringLiteral("koDesignerDock")
    };
    for (const QString &name : expectedNames) {
        auto *dock = manager->findDockWidget(name);
        QVERIFY2(dock, qPrintable(QStringLiteral("Missing dock: %1").arg(name)));
        QVERIFY(!dock->isInFloatingContainer());
        QVERIFY(dock->toggleViewAction());
    }

    auto *terminal = manager->findDockWidget(QStringLiteral("terminalDock"));
    terminal->toggleViewAction()->trigger();
    QTRY_VERIFY(!terminal->toggleViewAction()->isChecked());
    terminal->toggleViewAction()->trigger();
    QTRY_VERIFY(terminal->toggleViewAction()->isChecked());

    auto *designer = manager->findDockWidget(QStringLiteral("koDesignerDock"));
    QAction *designerAction = designer->toggleViewAction();
    const bool wasChecked = designerAction->isChecked();
    designerAction->trigger();
    QTRY_VERIFY(designerAction->isChecked() != wasChecked);
    designerAction->trigger();
    QTRY_COMPARE(designerAction->isChecked(), wasChecked);
}

void MainWindowTests::loadsKoDesignerFromBundledAssets()
{
    MainWindow window;
    auto *view = window.findChild<QWebEngineView *>(QStringLiteral("koDesignerWebView"));
    QVERIFY(view);
    QTRY_VERIFY_WITH_TIMEOUT(view->property("koDesignerLoadFinished").toBool(), 15000);
    QVERIFY(view->property("koDesignerLoadSucceeded").toBool());

    bool javascriptFinished = false;
    QVariant ready;
    view->page()->runJavaScript(
        QStringLiteral("window.koDesignerReady === true"),
        [&javascriptFinished, &ready](const QVariant &result) {
            ready = result;
            javascriptFinished = true;
        });
    QTRY_VERIFY_WITH_TIMEOUT(javascriptFinished, 15000);
    QVERIFY(ready.toBool());

    bool exportFinished = false;
    QVariant generatedCode;
    view->page()->runJavaScript(
        QStringLiteral(
            "(() => { document.getElementById('export-button').click(); return {"
            "html: document.getElementById('html-output').value,"
            "css: document.getElementById('css-output').value,"
            "blocks: document.querySelectorAll('#blocks .gjs-block').length"
            "}; })()"),
        [&exportFinished, &generatedCode](const QVariant &result) {
            generatedCode = result;
            exportFinished = true;
        });
    QTRY_VERIFY_WITH_TIMEOUT(exportFinished, 15000);
    const QVariantMap output = generatedCode.toMap();
    QVERIFY(output.value(QStringLiteral("html"))
                .toString()
                .contains(QStringLiteral("Design your page")));
    QVERIFY(output.value(QStringLiteral("css"))
                .toString()
                .contains(QStringLiteral(".kd-page")));
    QVERIFY(output.value(QStringLiteral("blocks")).toInt() >= 5);
}

void MainWindowTests::opensAndSavesKoDesignerProjectDocuments()
{
    QTemporaryDir project;
    QVERIFY(project.isValid());
    MainWindow window;
    auto *designer = window.findChild<KoDesignerPanel *>();
    QVERIFY(designer);
    auto *view = window.findChild<QWebEngineView *>(QStringLiteral("koDesignerWebView"));
    QVERIFY(view);
    QTRY_VERIFY_WITH_TIMEOUT(view->property("koDesignerLoadFinished").toBool(), 15000);
    QTRY_COMPARE_WITH_TIMEOUT(
        designer->findChild<QTabBar *>(QStringLiteral("koDesignerDocumentTabs"))->count(),
        1, 15000);

    qInfo() << "designer ready";
    designer->newDocument();
    qInfo() << "new document";
    auto *tabs = designer->findChild<QTabBar *>(QStringLiteral("koDesignerDocumentTabs"));
    QVERIFY(tabs);
    QCOMPARE(tabs->count(), 2);
    const QString firstPath = project.filePath(QStringLiteral("first-design.json"));
    QVERIFY(designer->saveCurrentDocumentAs(firstPath));
    qInfo() << "saved first";
    QFile firstFile(firstPath);
    QVERIFY(firstFile.open(QIODevice::ReadOnly));
    QJsonParseError parseError;
    const QJsonDocument savedDocument = QJsonDocument::fromJson(firstFile.readAll(),
                                                                &parseError);
    QCOMPARE(parseError.error, QJsonParseError::NoError);
    QVERIFY(savedDocument.isObject());
    QVERIFY(savedDocument.object().value(QStringLiteral("pages")).isArray());

    designer->newDocument();
    qInfo() << "new second";
    QCOMPARE(tabs->count(), 3);
    QVERIFY(designer->openFilePath(firstPath));
    qInfo() << "opened first";
    QCOMPARE(tabs->count(), 3);
    QVERIFY(designer->openFilePath(firstPath));
    QCOMPARE(tabs->count(), 3);
    QCOMPARE(tabs->tabText(tabs->currentIndex()), QStringLiteral("first-design.json"));

    const QString secondPath = project.filePath(QStringLiteral("second-design.json"));
    QVERIFY(QFile::copy(firstPath, secondPath));
    QVERIFY(designer->openFilePath(secondPath));
    qInfo() << "opened second";
    QCOMPARE(tabs->count(), 4);
    QCOMPARE(tabs->tabText(tabs->currentIndex()), QStringLiteral("second-design.json"));
    for (int index = 0; index < tabs->count(); ++index)
        qInfo() << "tab" << tabs->tabText(index);
    qInfo() << "closing";
    window.close();
}

void MainWindowTests::importsAndExportsExistingHtmlDesigns()
{
    QTemporaryDir project;
    QVERIFY(project.isValid());
    const QByteArray originalHtml = R"(<!doctype html>
<html lang="en"><head><meta charset="utf-8"><title>Imported landing page</title>
<script>window.preservedHeadScript = true;</script>
<link rel="stylesheet" href="styles/site.css"></head>
<body class="landing"><nav id="primary-nav"><a href="#content">Home</a></nav>
<main id="content"><section class="hero" style="padding: 24px">
<h1>Welcome</h1><img src="assets/logo.svg" alt="Logo">
<form action="/subscribe"><input name="email"><button>Subscribe</button></form>
</section></main></body></html>)";
    const QString stylesDirectory = project.filePath(QStringLiteral("styles"));
    const QString assetsDirectory = project.filePath(QStringLiteral("assets"));
    QVERIFY(QDir().mkpath(stylesDirectory));
    QVERIFY(QDir().mkpath(assetsDirectory));
    QFile stylesheet(QDir(stylesDirectory).filePath(QStringLiteral("site.css")));
    const QByteArray stylesheetContents =
        ".hero { color: #123456; font-size: 32px; "
        "background-image: url('../assets/logo.svg'); }\n";
    QVERIFY(stylesheet.open(QIODevice::WriteOnly));
    QCOMPARE(stylesheet.write(stylesheetContents), qint64(stylesheetContents.size()));
    stylesheet.close();
    QFile image(QDir(assetsDirectory).filePath(QStringLiteral("logo.svg")));
    const QByteArray imageContents = "<svg xmlns=\"http://www.w3.org/2000/svg\"/>";
    QVERIFY(image.open(QIODevice::WriteOnly));
    QCOMPARE(image.write(imageContents), qint64(imageContents.size()));
    image.close();
    const QString htmlPath = project.filePath(QStringLiteral("landing.html"));
    QFile source(htmlPath);
    QVERIFY(source.open(QIODevice::WriteOnly));
    QCOMPARE(source.write(originalHtml), qint64(originalHtml.size()));
    source.close();

    MainWindow window;
    auto *designer = window.findChild<KoDesignerPanel *>();
    auto *view = window.findChild<QWebEngineView *>(QStringLiteral("koDesignerWebView"));
    QVERIFY(designer && view);
    QTRY_VERIFY_WITH_TIMEOUT(view->property("koDesignerLoadFinished").toBool(), 15000);
    QVERIFY(designer->openFilePath(htmlPath));
    auto *tabs = designer->findChild<QTabBar *>(QStringLiteral("koDesignerDocumentTabs"));
    QVERIFY(tabs);
    QCOMPARE(tabs->count(), 2);

    bool inspected = false;
    QVariant imported;
    view->page()->runJavaScript(
        QStringLiteral(
            "JSON.stringify({html: window.koDesignerEditor.getHtml(), "
            "css: window.koDesignerEditor.getCss(), "
            "nav: window.koDesignerEditor.getWrapper().find('nav').length, "
            "section: window.koDesignerEditor.getWrapper().find('.hero').length, "
            "form: window.koDesignerEditor.getWrapper().find('form').length, "
            "image: window.koDesignerEditor.getWrapper().find('img')[0].getAttributes().src, "
            "project: window.koDesignerGetProjectData()})"),
        [&inspected, &imported](const QVariant &result) {
            imported = result;
            inspected = true;
        });
    QTRY_VERIFY_WITH_TIMEOUT(inspected, 15000);
    const QJsonObject importResult = QJsonDocument::fromJson(
        imported.toString().toUtf8()).object();
    QVERIFY(importResult.value(QStringLiteral("html")).toString()
                .contains(QStringLiteral("data:image/svg+xml;base64,")));
    QVERIFY(importResult.value(QStringLiteral("css")).toString()
                .contains(QStringLiteral("rgb(18, 52, 86)")));
    QVERIFY(importResult.value(QStringLiteral("image")).toString()
                .startsWith(QStringLiteral("data:image/svg+xml;base64,")));
    QCOMPARE(importResult.value(QStringLiteral("nav")).toInt(), 1);
    QCOMPARE(importResult.value(QStringLiteral("section")).toInt(), 1);
    QCOMPARE(importResult.value(QStringLiteral("form")).toInt(), 1);

    QFile unchangedSource(htmlPath);
    QVERIFY(unchangedSource.open(QIODevice::ReadOnly));
    QCOMPARE(unchangedSource.readAll(), originalHtml);
    unchangedSource.close();

    bool edited = false;
    view->page()->runJavaScript(
        QStringLiteral(
            "(() => { const hero = window.koDesignerEditor.getWrapper().find('.hero')[0]; "
            "hero.addAttributes({'data-edited': 'true'}); "
            "hero.addStyle({'background-color': '#fefefe'}); "
            "hero.components().add('<p class=\"added\">Added in KoDesigner</p>'); "
            "return true; })()"),
        [&edited](const QVariant &) { edited = true; });
    QTRY_VERIFY_WITH_TIMEOUT(edited, 15000);

    const QString exportPath = project.filePath(QStringLiteral("exported.html"));
    QVERIFY(designer->exportCurrentDocumentAs(exportPath));
    QFile exported(exportPath);
    QVERIFY(exported.open(QIODevice::ReadOnly));
    const QByteArray exportedHtml = exported.readAll();
    QVERIFY(exportedHtml.contains("<title>Imported landing page</title>"));
    QVERIFY(exportedHtml.contains("window.preservedHeadScript"));
    QVERIFY(exportedHtml.contains("data-edited=\"true\""));
    QVERIFY(exportedHtml.contains("Added in KoDesigner"));
    QVERIFY(exportedHtml.contains("rgb(18, 52, 86)"));
    QVERIFY(exportedHtml.contains("background-color"));
    QVERIFY(exportedHtml.contains("assets/logo.svg"));

    QVERIFY(designer->saveCurrentDocument());
    QFile savedSource(htmlPath);
    QVERIFY(savedSource.open(QIODevice::ReadOnly));
    const QByteArray savedHtml = savedSource.readAll();
    QVERIFY(savedHtml.contains("data-edited=\"true\""));
    QVERIFY(savedHtml.contains("Added in KoDesigner"));
    QVERIFY(savedHtml.contains("rgb(18, 52, 86)"));

    const QString malformedPath = project.filePath(QStringLiteral("partial.html"));
    const QByteArray malformedContents = "<html><body><main><h2 class=\"recovered\">Partial";
    QFile malformed(malformedPath);
    QVERIFY(malformed.open(QIODevice::WriteOnly));
    QCOMPARE(malformed.write(malformedContents), qint64(malformedContents.size()));
    malformed.close();
    QVERIFY(designer->openFilePath(malformedPath));
    bool malformedLoaded = false;
    QVariant recovered;
    view->page()->runJavaScript(
        QStringLiteral(
            "window.koDesignerEditor.getWrapper().find('.recovered').length"),
        [&malformedLoaded, &recovered](const QVariant &result) {
            recovered = result;
            malformedLoaded = true;
        });
    QTRY_VERIFY_WITH_TIMEOUT(malformedLoaded, 15000);
    QCOMPARE(recovered.toInt(), 1);

    QVERIFY(designer->openFilePath(exportPath));
    bool reopened = false;
    QVariant reopenedTree;
    view->page()->runJavaScript(
        QStringLiteral(
            "JSON.stringify({section: window.koDesignerEditor.getWrapper().find('.hero').length, "
            "added: window.koDesignerEditor.getWrapper().find('.added').length, "
            "css: window.koDesignerEditor.getCss()})"),
        [&reopened, &reopenedTree](const QVariant &result) {
            reopenedTree = result;
            reopened = true;
        });
    QTRY_VERIFY_WITH_TIMEOUT(reopened, 15000);
    const QJsonObject reopenedResult = QJsonDocument::fromJson(
        reopenedTree.toString().toUtf8()).object();
    QCOMPARE(reopenedResult.value(QStringLiteral("section")).toInt(), 1);
    QCOMPARE(reopenedResult.value(QStringLiteral("added")).toInt(), 1);
    QVERIFY(reopenedResult.value(QStringLiteral("css")).toString()
                .contains(QStringLiteral("rgb(18, 52, 86)")));
    window.close();
}

void MainWindowTests::floatingPanelCanBeReattached()
{
    MainWindow window;
    window.show();
    auto *manager = window.findChild<ads::CDockManager *>();
    QVERIFY(manager);
    auto *preview = manager->findDockWidget(QStringLiteral("previewDock"));
    auto *terminal = manager->findDockWidget(QStringLiteral("terminalDock"));
    QVERIFY(preview);
    QVERIFY(terminal);

    preview->setFloating();
    QTRY_VERIFY(preview->isInFloatingContainer());
    manager->addDockWidget(ads::CenterDockWidgetArea,
                           terminal,
                           preview->dockAreaWidget());
    QVERIFY(terminal->isInFloatingContainer());

    QAction *reattachAction = nullptr;
    QTRY_VERIFY(([&]() {
        for (QAction *action : preview->titleBarActions()) {
            if (action->text() == QStringLiteral("Reattach to Main Window")
                && action->isVisible()) {
                reattachAction = action;
                return true;
            }
        }
        return false;
    })());

    reattachAction->trigger();
    QTRY_VERIFY(!preview->isInFloatingContainer());
    QTRY_VERIFY(!terminal->isInFloatingContainer());
    QVERIFY(manager->floatingWidgets().isEmpty());
    window.close();
}

void MainWindowTests::savesAndRestoresDockLayout()
{
    QByteArray savedState;
    {
        MainWindow window;
        window.show();
        auto *manager = window.findChild<ads::CDockManager *>();
        QVERIFY(manager);
        auto *terminal = manager->findDockWidget(QStringLiteral("terminalDock"));
        QVERIFY(terminal);
        QVERIFY(terminal->toggleViewAction()->isChecked());
        terminal->toggleViewAction()->trigger();
        QVERIFY(window.close());
        savedState = QSettings().value(QStringLiteral("window/dockingState")).toByteArray();
        QVERIFY(!savedState.isEmpty());
        QCOMPARE(QSettings().value(QStringLiteral("window/dockingLayoutVersion")).toInt(), 3);
    }

    MainWindow restoredWindow;
    auto *restoredManager = restoredWindow.findChild<ads::CDockManager *>();
    QVERIFY(restoredManager);
    auto *restoredTerminal = restoredManager->findDockWidget(QStringLiteral("terminalDock"));
    QVERIFY2(restoredTerminal,
             qPrintable(QStringLiteral("Restored dock names: %1")
                            .arg(restoredManager->dockWidgetsMap().keys().join(','))));
    QVERIFY(!restoredTerminal->toggleViewAction()->isChecked());
}

void MainWindowTests::editorToolbarAndNavbarShareSaveAction()
{
    QTemporaryDir project;
    QVERIFY(project.isValid());
    const QString filePath = project.filePath(QStringLiteral("main.cpp"));
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QCOMPARE(file.write("int main() {}\n"), qint64(14));
    file.close();

    MainWindow window;
    auto *panel = window.findChild<EditorPanel *>();
    QVERIFY(panel);
    QVERIFY(panel->openProjectDirectory(project.path()));
    QVERIFY(panel->openFilePath(filePath));
    auto *editor = panel->findChild<QsciScintilla *>(QStringLiteral("sourceEditor"));
    QVERIFY(editor);
    editor->append(QStringLiteral("// saved through shared action"));
    QVERIFY(editor->isModified());

    QAction *toolbarSave = panel->saveAction();
    QAction *navbarSave = window.findChild<QAction *>(QStringLiteral("saveAction"));
    QVERIFY(toolbarSave);
    QCOMPARE(navbarSave, toolbarSave);
    navbarSave->trigger();
    QVERIFY(!editor->isModified());
    QVERIFY(file.open(QIODevice::ReadOnly));
    QVERIFY(file.readAll().contains("// saved through shared action"));
    QVERIFY(window.close());
}

void MainWindowTests::floatingEditorPreservesItsDocument()
{
    MainWindow window;
    auto *manager = window.findChild<ads::CDockManager *>();
    auto *panel = window.findChild<EditorPanel *>();
    QVERIFY(manager && panel);
    auto *editor = panel->findChild<QsciScintilla *>(QStringLiteral("sourceEditor"));
    QVERIFY(editor);
    QVERIFY(panel->openProjectDirectory(QDir::tempPath()));
    auto *dock = manager->findDockWidget(QStringLiteral("editorDock"));
    QVERIFY(dock);

    dock->setFloating();
    QTRY_VERIFY(dock->isInFloatingContainer());
    QCOMPARE(panel->findChild<QsciScintilla *>(QStringLiteral("sourceEditor")), editor);
    editor->setText(QStringLiteral("kept while floating"));
    QCOMPARE(editor->text(), QStringLiteral("kept while floating"));
    editor->setModified(false);
    manager->addDockWidget(ads::CenterDockWidgetArea, dock);
    QTRY_VERIFY(!dock->isInFloatingContainer());
    QCOMPARE(editor->text(), QStringLiteral("kept while floating"));
    QVERIFY(window.close());
}

void MainWindowTests::themeSelectorChangesApplicationAndEditorIndependently()
{
    QCOMPARE(ThemeManager::themes().size(), 8);
    for (const ThemeDefinition &theme : ThemeManager::themes())
        QVERIFY(!ThemeManager::stylesheet(theme).isEmpty());
    QCOMPARE(ThemeManager::theme(QStringLiteral("crimson"))->primaryColor,
             QColor(QStringLiteral("#a30502")));
    QCOMPARE(ThemeManager::theme(QStringLiteral("amber"))->primaryColor,
             QColor(QStringLiteral("#f78b04")));
    QCOMPARE(ThemeManager::theme(QStringLiteral("obsidian"))->primaryColor,
             QColor(QStringLiteral("#2b1718")));
    QCOMPARE(ThemeManager::theme(QStringLiteral("ocean"))->primaryColor,
             QColor(QStringLiteral("#153a42")));
    QCOMPARE(ThemeManager::theme(QStringLiteral("cyan"))->primaryColor,
             QColor(QStringLiteral("#027f93")));

    MainWindow window;
    auto *editorPanel = window.findChild<EditorPanel *>();
    QVERIFY(editorPanel);
    QCOMPARE(editorPanel->editorTheme(), QStringLiteral("metal"));

    QDialog *selector = nullptr;
    QTimer::singleShot(0, &window, [&]() {
        selector = window.findChild<QDialog *>(QStringLiteral("themeSelectorDialog"));
        QVERIFY(selector);
        auto *ocean = selector->findChild<QPushButton *>(
            QStringLiteral("themeCard_ocean"));
        auto *editorTheme = selector->findChild<QComboBox *>(
            QStringLiteral("editorThemeCombo"));
        QVERIFY(ocean);
        QVERIFY(editorTheme);
        ocean->click();
        editorTheme->setCurrentIndex(editorTheme->findData(QStringLiteral("glass-light")));
        selector->accept();
    });
    auto *appearanceAction = window.findChild<QAction *>(
        QStringLiteral("appearanceAction"));
    QVERIFY(appearanceAction);
    appearanceAction->trigger();
    QVERIFY(selector);

    QCOMPARE(QSettings().value(QStringLiteral("appearance/applicationTheme")).toString(),
             QStringLiteral("ocean"));
    QCOMPARE(QSettings().value(QStringLiteral("appearance/editorTheme")).toString(),
             QStringLiteral("glass-light"));
    QCOMPARE(editorPanel->editorTheme(), QStringLiteral("glass-light"));
    QVERIFY(qApp->styleSheet().contains(QStringLiteral("rgba(21, 58, 66")));

    MainWindow restoredWindow;
    auto *restoredEditor = restoredWindow.findChild<EditorPanel *>();
    QVERIFY(restoredEditor);
    QCOMPARE(restoredEditor->editorTheme(), QStringLiteral("glass-light"));
    QVERIFY(qApp->styleSheet().contains(QStringLiteral("rgba(21, 58, 66")));
}

QTEST_MAIN(MainWindowTests)

#include "test_main_window.moc"
