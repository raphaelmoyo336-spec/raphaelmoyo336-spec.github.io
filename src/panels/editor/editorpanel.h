#pragma once

#include <QByteArray>
#include <QDateTime>
#include <QHash>
#include <QString>
#include <QWidget>

#include <Qsci/qscidocument.h>
#include "editorpalette.h"

class QAction;
class FileExplorerWidget;
class QLabel;
class QSplitter;
class QTabBar;
class QsciLexer;
class QsciScintilla;

class EditorPanel final : public QWidget
{
    Q_OBJECT

public:
    explicit EditorPanel(QWidget *parent = nullptr);
    ~EditorPanel() override;
    bool openProjectDirectory(const QString &directory);
    QString projectDirectory() const;
    bool openFilePath(const QString &filePath);
    bool saveCurrentFile();
    bool saveCurrentFileAs(const QString &filePath);
    bool maybeSaveCurrentFile();
    bool maybeSaveAllFiles();
    bool saveAllFiles();
    bool closeCurrentDocument();
    bool closeAllDocuments();
    void openFileDialog();
    void newDocument();
    QAction *saveAction() const;
    QAction *saveAsAction() const;
    QAction *newFileAction() const;
    QAction *openFileAction() const;
    QAction *saveAllAction() const;
    FileExplorerWidget *fileExplorer() const;
    void setEditorTheme(const QString &id);
    QString editorTheme() const;

public slots:
    void handleFileRenamed(const QString &oldPath, const QString &newPath);

signals:
    void currentFileChanged(bool hasFile);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    struct DocumentState
    {
        QString id;
        QString filePath;
        QString displayName;
        QsciDocument document;
        QByteArray savedContentHash;
        bool dirty = false;
        bool newlyCreated = false;
    };

    struct EditorPane
    {
        QWidget *container = nullptr;
        QLabel *label = nullptr;
        QsciScintilla *editor = nullptr;
        QsciLexer *lexer = nullptr;
        DocumentState *documentState = nullptr;
        QString filePath;
        QByteArray savedContentHash;
        int activeLineMarkerHandle = -1;
        int activeLineMarkerLine = -1;
    };

    EditorPane *createEditorPane(QWidget *parent, bool primary);
    void activatePane(EditorPane *pane);
    void saveActivePaneState();
    void loadPaneState(EditorPane *pane);
    void updatePaneIndicator(EditorPane *pane);
    void updateDocumentTab(DocumentState *document);
    void updateAllDocumentTabs();
    void activateDocument(DocumentState *document, EditorPane *pane);
    DocumentState *documentForPath(const QString &absolutePath) const;
    DocumentState *documentForTab(int index) const;
    DocumentState *createDocument(const QString &filePath,
                                  const QByteArray &contents,
                                  bool newlyCreated);
    int documentTabIndex(const DocumentState *document) const;
    void removeDocument(DocumentState *document, bool alreadyConfirmed = false);
    void clearDocuments();
    bool confirmCloseDocument(DocumentState *document);
    void splitEditor(Qt::Orientation orientation);
    void closeSplit();
    void configureEditor();
    void applyEditorColors(QsciScintilla *editor, QsciLexer *lexer);
    void chooseFileToOpen();
    void createNewFile();
    bool saveAs();
    void showFindDialog(bool replace);
    void updateDocumentIndicator();
    void updateActionStates();
    void updateActiveLineMarker();
    void selectLexerForFile(const QString &filePath);
    bool fileHasChangedExternally() const;
    bool writeDocumentToPath(const QString &filePath);

    FileExplorerWidget *explorer_ = nullptr;
    QLabel *currentFileLabel_ = nullptr;
    QSplitter *editorSplitter_ = nullptr;
    QTabBar *documentTabs_ = nullptr;
    EditorPane *primaryPane_ = nullptr;
    EditorPane *secondaryPane_ = nullptr;
    EditorPane *activePane_ = nullptr;
    QsciScintilla *editor_ = nullptr;
    QsciLexer *lexer_ = nullptr;
    QAction *saveAction_ = nullptr;
    QAction *saveAsAction_ = nullptr;
    QAction *newFileAction_ = nullptr;
    QAction *openFileAction_ = nullptr;
    QAction *saveAllAction_ = nullptr;
    QAction *undoAction_ = nullptr;
    QAction *redoAction_ = nullptr;
    QAction *cutAction_ = nullptr;
    QAction *copyAction_ = nullptr;
    QAction *pasteAction_ = nullptr;
    QAction *selectAllAction_ = nullptr;
    QAction *findAction_ = nullptr;
    QAction *replaceAction_ = nullptr;
    QAction *commentAction_ = nullptr;
    QAction *indentAction_ = nullptr;
    QAction *unindentAction_ = nullptr;
    QString currentFilePath_;
    QByteArray savedContentHash_;
    QHash<QString, DocumentState *> documentsById_;
    QHash<QString, DocumentState *> documentsByPath_;
    quint64 nextDocumentId_ = 1;
    int activeLineMarkerHandle_ = -1;
    int activeLineMarkerLine_ = -1;
    QString editorThemeId_ = QStringLiteral("metal");
    EditorPalette editorPalette_ = editorPaletteForId(editorThemeId_);
};
