#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DockManager.h"

class QCloseEvent;
class QAction;
class QString;
class EditorPanel;
class KoDesignerPanel;
class ThemeSelectorDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    void createNavigation();
    void createProjectsMenu();
    void showThemeSelector();
    void applyApplicationTheme(const QString &id);
    void openProjectDirectory(const QString &directory);
    void createProject();

protected:
    void closeEvent(QCloseEvent *event) override;


private:
    ads::CDockManager* m_DockManager;
    EditorPanel *editorContent_ = nullptr;
    KoDesignerPanel *designerContent_ = nullptr;
    ads::CDockWidget *editorPanel_ = nullptr;
    ads::CDockWidget *terminalPanel_ = nullptr;
    ads::CDockWidget *previewPanel_ = nullptr;
    ads::CDockWidget *koDesignerPanel_ = nullptr;
    QAction *currentProjectAction_ = nullptr;
    QString applicationThemeId_ = QStringLiteral("metal");
    QString editorThemeId_ = QStringLiteral("metal");
};

#endif // MAINWINDOW_H