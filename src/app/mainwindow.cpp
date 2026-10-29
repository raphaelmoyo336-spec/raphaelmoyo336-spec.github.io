#include "mainwindow.h"

#include "editorpanel.h"
#include "fileexplorerwidget.h"
#include "kodesignerpanel.h"
#include "previewpanel.h"
#include "terminalpanel.h"
#include "thememanager.h"
#include "themeselectordialog.h"

#include <DockManager.h>
#include <DockWidget.h>
#include <FloatingDockContainer.h>

#include <QAction>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QStatusBar>
#include <QStyle>
#include <QToolBar>
#include <QTimer>
#include <QApplication>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSettings settings;
    applicationThemeId_ = settings.value(QStringLiteral("appearance/applicationTheme"),
                                         QStringLiteral("metal")).toString();
    if (!ThemeManager::theme(applicationThemeId_))
        applicationThemeId_ = QStringLiteral("metal");
    editorThemeId_ = settings.value(QStringLiteral("appearance/editorTheme"),
                                    QStringLiteral("metal")).toString();
    if (editorThemeId_ != QStringLiteral("metal")
        && editorThemeId_ != QStringLiteral("glass-light")
        && editorThemeId_ != QStringLiteral("glass-dark")) {
        editorThemeId_ = QStringLiteral("metal");
    }

    ads::CDockManager::setConfigFlags(ads::CDockManager::DefaultOpaqueConfig);
    ads::CDockManager::setAutoHideConfigFlags(ads::CDockManager::DefaultAutoHideConfig);
    m_DockManager = new ads::CDockManager(this);

    setWindowTitle(QStringLiteral("Kogence IDE"));
    resize(1440, 900);
    applyApplicationTheme(applicationThemeId_);

    const auto createPanel = [this](const QString &title,
                                    const QString &objectName,
                                    QWidget *content) {
        auto *dock = m_DockManager->createDockWidget(title);
        dock->setObjectName(objectName);
        dock->setFeature(ads::CDockWidget::DockWidgetPinnable, true);
        dock->setFeature(ads::CDockWidget::DockWidgetFloatable, true);
        dock->setWidget(content);

        auto *reattachAction = new QAction(
            style()->standardIcon(QStyle::SP_TitleBarNormalButton),
            QStringLiteral("Reattach to Main Window"),
            dock);
        reattachAction->setToolTip(QStringLiteral("Reattach this floating group to the main window"));
        reattachAction->setVisible(false);
        dock->setTitleBarActions({reattachAction});
        connect(reattachAction, &QAction::triggered, this, [this, dock]() {
            if (!dock->isInFloatingContainer() || !dock->dockContainer())
                return;

            auto *floatingContainer = dock->dockContainer()->floatingWidget();
            if (!floatingContainer)
                return;

            const QList<ads::CDockWidget *> floatingDocks = floatingContainer->dockWidgets();
            ads::CDockAreaWidget *targetArea = nullptr;
            const QList<ads::CDockWidget *> mainDocks = {
                editorPanel_, previewPanel_, terminalPanel_, koDesignerPanel_};
            for (auto *mainDock : mainDocks) {
                if (mainDock && !mainDock->isInFloatingContainer()
                    && !floatingDocks.contains(mainDock)) {
                    targetArea = mainDock->dockAreaWidget();
                    if (targetArea)
                        break;
                }
            }

            for (auto *floatingDock : floatingDocks) {
                targetArea = m_DockManager->addDockWidget(
                    ads::CenterDockWidgetArea, floatingDock, targetArea);
                for (QAction *action : floatingDock->titleBarActions()) {
                    if (action->text() == QStringLiteral("Reattach to Main Window"))
                        action->setVisible(false);
                }
            }
        });
        return dock;
    };
    editorContent_ = new EditorPanel;
    editorContent_->setEditorTheme(editorThemeId_);
    editorPanel_ = createPanel(QStringLiteral("Text Editor"),
                               QStringLiteral("editorDock"),
                               editorContent_);
    terminalPanel_ = createPanel(QStringLiteral("Terminal"),
                                 QStringLiteral("terminalDock"),
                                 new TerminalPanel);
    previewPanel_ = createPanel(QStringLiteral("Preview"),
                                QStringLiteral("previewDock"),
                                new PreviewPanel);
    designerContent_ = new KoDesignerPanel;
    koDesignerPanel_ = createPanel(QStringLiteral("KoDesigner"),
                                   QStringLiteral("koDesignerDock"),
                                   designerContent_);

    FileExplorerWidget *explorer = editorContent_->fileExplorer();
    disconnect(explorer, &FileExplorerWidget::fileActivated,
               editorContent_, &EditorPanel::openFilePath);
    connect(explorer, &FileExplorerWidget::fileActivated, this, [this](const QString &path) {
        if (QFileInfo(path).suffix().compare(QStringLiteral("json"),
                                              Qt::CaseInsensitive) == 0)
            designerContent_->openFilePath(path);
        else
            editorContent_->openFilePath(path);
    });
    connect(explorer, &FileExplorerWidget::fileRenamed,
            designerContent_, &KoDesignerPanel::handleFileRenamed);

    connect(m_DockManager,
            &ads::CDockManager::floatingWidgetCreated,
            this,
            [](ads::CFloatingDockContainer *floatingWidget) {
                QTimer::singleShot(0, floatingWidget, [floatingWidget]() {
                    for (auto *floatingDock : floatingWidget->dockWidgets()) {
                        for (QAction *action : floatingDock->titleBarActions()) {
                            if (action->text() == QStringLiteral("Reattach to Main Window"))
                                action->setVisible(true);
                        }
                    }
                });
            });

    if (settings.contains(QStringLiteral("window/geometry")))
        restoreGeometry(settings.value(QStringLiteral("window/geometry")).toByteArray());
    const QByteArray dockState = settings.value(QStringLiteral("window/dockingState")).toByteArray();
    const int dockLayoutVersion = settings.value(QStringLiteral("window/dockingLayoutVersion")).toInt();
    auto *editorArea = m_DockManager->addDockWidget(ads::CenterDockWidgetArea, editorPanel_);
    m_DockManager->addDockWidget(ads::RightDockWidgetArea, previewPanel_, editorArea);
    m_DockManager->addDockWidget(ads::BottomDockWidgetArea, terminalPanel_, editorArea);
    m_DockManager->addDockWidget(ads::CenterDockWidgetArea, koDesignerPanel_, editorArea);

    if (dockLayoutVersion == 3 && !dockState.isEmpty()) {
        const QByteArray defaultDockState = m_DockManager->saveState();
        if (!m_DockManager->restoreState(dockState))
            m_DockManager->restoreState(defaultDockState);
    }

    createNavigation();
    createProjectsMenu();

    statusBar()->showMessage(QStringLiteral("Ready"));
    const QString lastProjectDirectory =
        settings.value(QStringLiteral("projects/lastDirectory")).toString();
    if (!lastProjectDirectory.isEmpty() && QFileInfo(lastProjectDirectory).isDir())
        openProjectDirectory(lastProjectDirectory);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!editorContent_->maybeSaveAllFiles()) {
        event->ignore();
        return;
    }
    if (!designerContent_->maybeSaveAllDocuments()) {
        event->ignore();
        return;
    }

    QSettings settings;
    settings.setValue(QStringLiteral("window/geometry"), saveGeometry());
    settings.setValue(QStringLiteral("window/dockingState"), m_DockManager->saveState());
    settings.setValue(QStringLiteral("window/dockingLayoutVersion"), 3);
    QMainWindow::closeEvent(event);
}

void MainWindow::createNavigation()
{
    auto *navigation = addToolBar(QStringLiteral("Navigate"));
    navigation->setObjectName(QStringLiteral("navigationToolbar"));
    navigation->setMovable(true);
    navigation->addAction(editorPanel_->toggleViewAction());
    navigation->addAction(terminalPanel_->toggleViewAction());
    navigation->addAction(previewPanel_->toggleViewAction());
    navigation->addAction(koDesignerPanel_->toggleViewAction());

    auto *viewMenu = menuBar()->addMenu(QStringLiteral("&View"));
    viewMenu->addAction(editorPanel_->toggleViewAction());
    viewMenu->addAction(terminalPanel_->toggleViewAction());
    viewMenu->addAction(previewPanel_->toggleViewAction());
    viewMenu->addAction(koDesignerPanel_->toggleViewAction());
    viewMenu->addSeparator();
    auto *appearanceAction = viewMenu->addAction(QStringLiteral("Appearance..."));
    appearanceAction->setObjectName(QStringLiteral("appearanceAction"));
    connect(appearanceAction, &QAction::triggered,
            this, &MainWindow::showThemeSelector);
    navigation->addSeparator();
    auto *toolbarAppearanceAction = navigation->addAction(
        style()->standardIcon(QStyle::SP_ComputerIcon), QStringLiteral("Appearance"));
    toolbarAppearanceAction->setObjectName(QStringLiteral("appearanceToolbarAction"));
    toolbarAppearanceAction->setToolTip(QStringLiteral("Choose application and editor themes"));
    connect(toolbarAppearanceAction, &QAction::triggered,
            this, &MainWindow::showThemeSelector);

    auto *fileMenu = menuBar()->addMenu(QStringLiteral("&File"));
    fileMenu->addAction(editorContent_->newFileAction());
    fileMenu->addAction(editorContent_->openFileAction());
    fileMenu->addAction(editorContent_->saveAction());
    fileMenu->addAction(editorContent_->saveAsAction());
    fileMenu->addAction(editorContent_->saveAllAction());
    fileMenu->addSeparator();
    fileMenu->addAction(designerContent_->newDocumentAction());
    fileMenu->addAction(designerContent_->openDocumentAction());
    fileMenu->addAction(designerContent_->importHtmlAction());
    fileMenu->addAction(designerContent_->saveDocumentAction());
    fileMenu->addAction(designerContent_->saveDocumentAsAction());
    fileMenu->addAction(designerContent_->saveAllDocumentsAction());
    fileMenu->addAction(designerContent_->exportHtmlAction());
    fileMenu->addAction(designerContent_->resetDocumentAction());
    fileMenu->addAction(designerContent_->closeDocumentAction());
    fileMenu->addAction(designerContent_->closeAllDocumentsAction());
    fileMenu->addSeparator();
    auto *quitAction = fileMenu->addAction(QStringLiteral("E&xit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::showThemeSelector()
{
    ThemeSelectorDialog dialog(applicationThemeId_, editorThemeId_, this);
    connect(&dialog, &ThemeSelectorDialog::applicationThemeSelected,
            this, [this](const QString &id) {
        applyApplicationTheme(id);
        applicationThemeId_ = id;
        QSettings().setValue(QStringLiteral("appearance/applicationTheme"), id);
    });
    connect(&dialog, &ThemeSelectorDialog::editorThemeSelected,
            this, [this](const QString &id) {
        editorContent_->setEditorTheme(id);
        editorThemeId_ = id;
        QSettings().setValue(QStringLiteral("appearance/editorTheme"), id);
    });
    dialog.exec();
}

void MainWindow::applyApplicationTheme(const QString &id)
{
    const ThemeDefinition *definition = ThemeManager::theme(id);
    if (!definition)
        return;
    const QString stylesheet = ThemeManager::stylesheet(*definition);
    if (stylesheet.isEmpty())
        qFatal("Unable to load the Kogence application theme stylesheet");

    qApp->setStyleSheet(stylesheet);
    QPalette palette = qApp->palette();
    ThemeManager::applyPalette(*definition, &palette);
    qApp->setPalette(palette);
    if (m_DockManager) {
        m_DockManager->setColorSchemeMode(
            definition->light ? ads::CDockManager::ColorSchemeMode::Light
                              : ads::CDockManager::ColorSchemeMode::Dark);
        m_DockManager->setStyleSheet(stylesheet);
    }
}

void MainWindow::createProjectsMenu()
{
    auto *projectsMenu = menuBar()->addMenu(QStringLiteral("&Projects"));
    auto *openFolderAction = projectsMenu->addAction(QStringLiteral("Open Folder..."));
    openFolderAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+O")));
    connect(openFolderAction, &QAction::triggered, this, [this]() {
        const QString directory = QFileDialog::getExistingDirectory(
            this,
            QStringLiteral("Open Project Folder"),
            editorContent_->projectDirectory());
        if (!directory.isEmpty())
            openProjectDirectory(directory);
    });
    projectsMenu->addAction(QStringLiteral("New Project..."), this, &MainWindow::createProject);

    projectsMenu->addSeparator();
    currentProjectAction_ = projectsMenu->addAction(QStringLiteral("No project open"));
    currentProjectAction_->setEnabled(false);
}

void MainWindow::openProjectDirectory(const QString &directory)
{
    if (!QFileInfo(directory).isDir()) {
        QMessageBox::warning(this, QStringLiteral("Unable to open project"),
                             QStringLiteral("%1 is not an accessible directory.")
                                 .arg(directory));
        return;
    }
    if (!designerContent_->maybeSaveAllDocuments())
        return;
    if (!editorContent_->openProjectDirectory(directory)) {
        return;
    }

    const QString projectDirectory = editorContent_->projectDirectory();
    designerContent_->setProjectDirectory(projectDirectory);
    const QString projectName = QDir(projectDirectory).dirName();
    currentProjectAction_->setText(
        QStringLiteral("Current: %1").arg(projectName));
    currentProjectAction_->setToolTip(projectDirectory);

    QSettings settings;
    settings.setValue(QStringLiteral("projects/lastDirectory"), projectDirectory);
    statusBar()->showMessage(QStringLiteral("Opened project: %1").arg(projectDirectory));
}

void MainWindow::createProject()
{
    bool accepted = false;
    const QString name = QInputDialog::getText(
        this, QStringLiteral("New Project"), QStringLiteral("Project name:"),
        QLineEdit::Normal, QString(), &accepted);
    if (!accepted || name.isEmpty())
        return;
    if (QFileInfo(name).fileName() != name || name == QStringLiteral(".")
        || name == QStringLiteral("..")) {
        QMessageBox::warning(this, QStringLiteral("Invalid project name"),
                             QStringLiteral("Enter a project name, not a path."));
        return;
    }
    const QString parentDirectory = QFileDialog::getExistingDirectory(
        this, QStringLiteral("Choose Project Destination"), QDir::homePath());
    if (parentDirectory.isEmpty())
        return;

    const QString projectDirectory = QDir(parentDirectory).filePath(name);
    if (!QDir().mkdir(projectDirectory)) {
        QMessageBox::warning(this, QStringLiteral("Unable to create project"),
                             QStringLiteral("Could not create %1. Check that the name "
                                            "is available and the destination is writable.")
                                 .arg(projectDirectory));
        return;
    }
    openProjectDirectory(projectDirectory);
}
