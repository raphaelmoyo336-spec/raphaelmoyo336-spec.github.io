#include "editorpanel.h"

#include "dartlexer.h"
#include "fileexplorerwidget.h"

#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerjson.h>
#include <Qsci/qscilexermarkdown.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexeryaml.h>
#include <Qsci/qscidocument.h>
#include <Qsci/qsciscintilla.h>

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QCryptographicHash>
#include <QColor>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
#include <QKeySequence>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QPushButton>
#include <QRegularExpression>
#include <QSaveFile>
#include <QSet>
#include <QSize>
#include <QSplitter>
#include <QStyle>
#include <QTabBar>
#include <QToolBar>
#include <QVBoxLayout>

#include <algorithm>
#include <functional>

namespace {

QFont editorFont()
{
    const QStringList installedFamilies = QFontDatabase::families();
    const QStringList preferredFamilies = {
        QStringLiteral("JetBrains Mono"),
        QStringLiteral("Iosevka"),
        QStringLiteral("Fira Code")
    };

    QFont font;
    bool preferredFontFound = false;
    for (const QString &preferred : preferredFamilies) {
        const auto found = std::find_if(
            installedFamilies.cbegin(), installedFamilies.cend(),
            [&preferred](const QString &family) {
                return family.compare(preferred, Qt::CaseInsensitive) == 0;
            });
        if (found != installedFamilies.cend()) {
            font = QFont(*found);
            preferredFontFound = true;
            break;
        }
    }

    if (!preferredFontFound)
        font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSizeF(10.0);
    return font;
}

QColor colorForLexerStyle(const QString &description, const EditorPalette &palette)
{
    const QString style = description.toLower();
    if (style.contains(QStringLiteral("comment"))) {
        if (style.contains(QStringLiteral("doc")))
            return palette.documentation;
        return palette.comment;
    }
    if (style.contains(QStringLiteral("escape")))
        return palette.escape;
    if (style.contains(QStringLiteral("string"))
        || style.contains(QStringLiteral("character")))
        return palette.string;
    if (style.contains(QStringLiteral("number")))
        return palette.number;
    if (style.contains(QStringLiteral("keyword")))
        return palette.keyword;
    if (style.contains(QStringLiteral("preprocessor")))
        return palette.preprocessor;
    if (style.contains(QStringLiteral("function"))
        || style.contains(QStringLiteral("method")))
        return palette.function;
    if (style.contains(QStringLiteral("class"))
        || style.contains(QStringLiteral("type"))
        || style.contains(QStringLiteral("tag")))
        return palette.type;
    if (style.contains(QStringLiteral("attribute")))
        return palette.keyword;
    if (style.contains(QStringLiteral("constant")))
        return palette.constant;
    if (style.contains(QStringLiteral("operator")))
        return palette.operation;
    return palette.foreground;
}

QAction *createToolbarAction(QToolBar *toolbar,
                             QWidget *owner,
                             const QString &text,
                             QStyle::StandardPixmap icon,
                             const std::function<void()> &callback)
{
    auto *action = new QAction(owner->style()->standardIcon(icon), text, owner);
    action->setToolTip(text);
    toolbar->addAction(action);
    QObject::connect(action, &QAction::triggered, owner, callback);
    return action;
}

QIcon splitEditorIcon(Qt::Orientation orientation)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(QColor(QStringLiteral("#858783")), 1));
    painter.drawRect(1, 2, 13, 12);
    painter.setPen(QPen(QColor(QStringLiteral("#d9954b")), 1));
    if (orientation == Qt::Horizontal)
        painter.drawLine(8, 3, 8, 13);
    else
        painter.drawLine(2, 8, 13, 8);
    return QIcon(pixmap);
}

QString commentPrefix(const QString &path)
{
    const QString extension = QFileInfo(path).suffix().toLower();
    static const QSet<QString> slashCommentExtensions = {
        QStringLiteral("c"), QStringLiteral("cc"), QStringLiteral("cpp"),
        QStringLiteral("cxx"), QStringLiteral("h"), QStringLiteral("hh"),
        QStringLiteral("hpp"), QStringLiteral("hxx"), QStringLiteral("dart"),
        QStringLiteral("js"), QStringLiteral("jsx"), QStringLiteral("mjs"),
        QStringLiteral("cjs"), QStringLiteral("ts"), QStringLiteral("tsx"),
        QStringLiteral("css"), QStringLiteral("java"), QStringLiteral("go"),
        QStringLiteral("rust")
    };
    static const QSet<QString> hashCommentExtensions = {
        QStringLiteral("py"), QStringLiteral("sh"), QStringLiteral("bash"),
        QStringLiteral("zsh"), QStringLiteral("yaml"), QStringLiteral("yml")
    };

    if (slashCommentExtensions.contains(extension))
        return QStringLiteral("//");
    if (hashCommentExtensions.contains(extension))
        return QStringLiteral("#");
    return QString();
}

QByteArray contentsForEditor(const QsciScintilla *editor)
{
    return editor->text().toUtf8();
}

} // namespace

EditorPanel::EditorPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *splitter = new QSplitter(Qt::Horizontal, this);
    explorer_ = new FileExplorerWidget(splitter);

    auto *editorContainer = new QWidget(splitter);
    auto *editorLayout = new QVBoxLayout(editorContainer);
    editorLayout->setContentsMargins(0, 0, 0, 0);
    editorLayout->setSpacing(0);

    auto *toolbar = new QToolBar(QStringLiteral("Editor Actions"), editorContainer);
    toolbar->setObjectName(QStringLiteral("editorToolbar"));
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(16, 16));
    editorLayout->addWidget(toolbar);

    documentTabs_ = new QTabBar(editorContainer);
    documentTabs_->setObjectName(QStringLiteral("documentTabs"));
    documentTabs_->setExpanding(false);
    documentTabs_->setTabsClosable(true);
    documentTabs_->setMovable(true);
    documentTabs_->hide();
    editorLayout->addWidget(documentTabs_);
    connect(documentTabs_, &QTabBar::currentChanged, this, [this](int index) {
        if (DocumentState *document = documentForTab(index))
            activateDocument(document, activePane_);
    });
    connect(documentTabs_, &QTabBar::tabCloseRequested, this, [this](int index) {
        if (DocumentState *document = documentForTab(index))
            removeDocument(document);
    });

    editorSplitter_ = new QSplitter(Qt::Horizontal, editorContainer);
    editorSplitter_->setObjectName(QStringLiteral("splitEditorSplitter"));
    editorSplitter_->setChildrenCollapsible(false);
    editorSplitter_->setHandleWidth(3);
    primaryPane_ = createEditorPane(editorSplitter_, true);
    editorSplitter_->addWidget(primaryPane_->container);
    editorLayout->addWidget(editorSplitter_);
    loadPaneState(primaryPane_);
    configureEditor();

    newFileAction_ = createToolbarAction(toolbar, this, QStringLiteral("New File"),
                                         QStyle::SP_FileIcon, [this]() { newDocument(); });
    newFileAction_->setObjectName(QStringLiteral("newFileAction"));
    newFileAction_->setShortcut(QKeySequence::New);
    newFileAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    openFileAction_ = createToolbarAction(toolbar, this, QStringLiteral("Open File"),
                                          QStyle::SP_DialogOpenButton, [this]() { openFileDialog(); });
    openFileAction_->setObjectName(QStringLiteral("openFileAction"));
    openFileAction_->setShortcut(QKeySequence::Open);
    openFileAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);

    toolbar->addSeparator();
    auto *splitVerticalAction = new QAction(
        splitEditorIcon(Qt::Horizontal), QStringLiteral("Split Vertically"), this);
    splitVerticalAction->setToolTip(QStringLiteral("Split Vertically"));
    toolbar->addAction(splitVerticalAction);
    connect(splitVerticalAction, &QAction::triggered,
            this, [this]() { splitEditor(Qt::Horizontal); });
    splitVerticalAction->setObjectName(QStringLiteral("splitVerticallyAction"));
    auto *splitHorizontalAction = new QAction(
        splitEditorIcon(Qt::Vertical), QStringLiteral("Split Horizontally"), this);
    splitHorizontalAction->setToolTip(QStringLiteral("Split Horizontally"));
    toolbar->addAction(splitHorizontalAction);
    connect(splitHorizontalAction, &QAction::triggered,
            this, [this]() { splitEditor(Qt::Vertical); });
    splitHorizontalAction->setObjectName(QStringLiteral("splitHorizontallyAction"));
    auto *closeSplitAction = createToolbarAction(
        toolbar, this, QStringLiteral("Close Split"),
        QStyle::SP_TitleBarCloseButton,
        [this]() { closeSplit(); });
    closeSplitAction->setObjectName(QStringLiteral("closeSplitAction"));
    closeSplitAction->setEnabled(false);

    saveAction_ = createToolbarAction(toolbar, this, QStringLiteral("Save"),
                                      QStyle::SP_DialogSaveButton, [this]() { saveCurrentFile(); });
    saveAction_->setObjectName(QStringLiteral("saveAction"));
    saveAction_->setShortcut(QKeySequence::Save);
    saveAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);

    saveAsAction_ = createToolbarAction(toolbar, this, QStringLiteral("Save As"),
                                        QStyle::SP_DialogSaveButton, [this]() { saveAs(); });
    saveAsAction_->setObjectName(QStringLiteral("saveAsAction"));
    saveAsAction_->setShortcut(QKeySequence::SaveAs);
    saveAsAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    saveAllAction_ = createToolbarAction(toolbar, this, QStringLiteral("Save All"),
                                         QStyle::SP_DialogSaveButton,
                                         [this]() { saveAllFiles(); });
    saveAllAction_->setObjectName(QStringLiteral("saveAllAction"));
    saveAllAction_->setShortcut(QKeySequence(QStringLiteral("Ctrl+Alt+S")));
    saveAllAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);

    toolbar->addSeparator();
    undoAction_ = createToolbarAction(toolbar, this, QStringLiteral("Undo"),
                                      QStyle::SP_ArrowBack, [this]() { editor_->undo(); });
    undoAction_->setObjectName(QStringLiteral("undoAction"));
    undoAction_->setShortcut(QKeySequence::Undo);
    undoAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    redoAction_ = createToolbarAction(toolbar, this, QStringLiteral("Redo"),
                                      QStyle::SP_ArrowForward, [this]() { editor_->redo(); });
    redoAction_->setObjectName(QStringLiteral("redoAction"));
    redoAction_->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+Z")));
    redoAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);

    toolbar->addSeparator();
    cutAction_ = createToolbarAction(toolbar, this, QStringLiteral("Cut"),
                                     QStyle::SP_TrashIcon, [this]() { editor_->cut(); });
    cutAction_->setObjectName(QStringLiteral("cutAction"));
    cutAction_->setShortcut(QKeySequence::Cut);
    cutAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    copyAction_ = createToolbarAction(toolbar, this, QStringLiteral("Copy"),
                                      QStyle::SP_FileDialogDetailedView, [this]() { editor_->copy(); });
    copyAction_->setObjectName(QStringLiteral("copyAction"));
    copyAction_->setShortcut(QKeySequence::Copy);
    copyAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    pasteAction_ = createToolbarAction(toolbar, this, QStringLiteral("Paste"),
                                       QStyle::SP_DialogOpenButton, [this]() { editor_->paste(); });
    pasteAction_->setObjectName(QStringLiteral("pasteAction"));
    pasteAction_->setShortcut(QKeySequence::Paste);
    pasteAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    selectAllAction_ = createToolbarAction(toolbar, this, QStringLiteral("Select All"),
                                           QStyle::SP_DialogYesButton,
                                           [this]() { editor_->selectAll(); });
    selectAllAction_->setObjectName(QStringLiteral("selectAllAction"));
    selectAllAction_->setShortcut(QKeySequence::SelectAll);
    selectAllAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);

    toolbar->addSeparator();
    findAction_ = createToolbarAction(toolbar, this, QStringLiteral("Find"),
                                      QStyle::SP_FileDialogContentsView,
                                      [this]() { showFindDialog(false); });
    findAction_->setObjectName(QStringLiteral("findAction"));
    findAction_->setShortcut(QKeySequence::Find);
    findAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    replaceAction_ = createToolbarAction(toolbar, this, QStringLiteral("Replace"),
                                         QStyle::SP_BrowserReload,
                                         [this]() { showFindDialog(true); });
    replaceAction_->setObjectName(QStringLiteral("replaceAction"));
    replaceAction_->setShortcut(QKeySequence(QStringLiteral("Ctrl+H")));
    replaceAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    commentAction_ = createToolbarAction(toolbar, this, QStringLiteral("Toggle Line Comment"),
                                         QStyle::SP_FileDialogListView,
                                         [this]() {
        const QString prefix = commentPrefix(currentFilePath_);
        if (prefix.isEmpty())
            return;

        int fromLine = -1;
        int fromIndex = -1;
        int toLine = -1;
        int toIndex = -1;
        editor_->getSelection(&fromLine, &fromIndex, &toLine, &toIndex);
        if (fromLine < 0) {
            editor_->getCursorPosition(&fromLine, &fromIndex);
            toLine = fromLine;
        } else if (toIndex == 0 && toLine > fromLine) {
            --toLine;
        }

        bool allCommented = true;
        for (int line = fromLine; line <= toLine; ++line) {
            const QString content = editor_->text(line);
            const int indentation = content.indexOf(QRegularExpression(QStringLiteral("\\S")));
            const int position = indentation < 0 ? content.size() : indentation;
            if (!content.mid(position).startsWith(prefix)) {
                allCommented = false;
                break;
            }
        }

        editor_->beginUndoAction();
        for (int line = toLine; line >= fromLine; --line) {
            const QString content = editor_->text(line);
            const int indentation = content.indexOf(QRegularExpression(QStringLiteral("\\S")));
            const int position = indentation < 0 ? content.size() : indentation;
            if (allCommented) {
                editor_->setSelection(line, position, line, position + prefix.size());
                editor_->replaceSelectedText(QString());
            } else {
                editor_->insertAt(prefix + QLatin1Char(' '), line, position);
            }
        }
        editor_->endUndoAction();
    });
    commentAction_->setObjectName(QStringLiteral("toggleCommentAction"));
    commentAction_->setShortcut(QKeySequence(QStringLiteral("Ctrl+/")));
    commentAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);

    indentAction_ = createToolbarAction(toolbar, this, QStringLiteral("Indent"),
                                        QStyle::SP_ArrowRight,
                                        [this]() {
        int fromLine = -1;
        int toLine = -1;
        int fromIndex = -1;
        int toIndex = -1;
        editor_->getSelection(&fromLine, &fromIndex, &toLine, &toIndex);
        if (fromLine < 0)
            editor_->getCursorPosition(&fromLine, &fromIndex), toLine = fromLine;
        else if (toIndex == 0 && toLine > fromLine)
            --toLine;
        editor_->beginUndoAction();
        for (int line = fromLine; line <= toLine; ++line)
            editor_->indent(line);
        editor_->endUndoAction();
    });
    indentAction_->setObjectName(QStringLiteral("indentAction"));
    unindentAction_ = createToolbarAction(toolbar, this, QStringLiteral("Unindent"),
                                          QStyle::SP_ArrowLeft,
                                          [this]() {
        int fromLine = -1;
        int toLine = -1;
        int fromIndex = -1;
        int toIndex = -1;
        editor_->getSelection(&fromLine, &fromIndex, &toLine, &toIndex);
        if (fromLine < 0)
            editor_->getCursorPosition(&fromLine, &fromIndex), toLine = fromLine;
        else if (toIndex == 0 && toLine > fromLine)
            --toLine;
        editor_->beginUndoAction();
        for (int line = fromLine; line <= toLine; ++line)
            editor_->unindent(line);
        editor_->endUndoAction();
    });
    unindentAction_->setObjectName(QStringLiteral("unindentAction"));

    splitter->addWidget(explorer_);
    splitter->addWidget(editorContainer);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({240, 760});
    layout->addWidget(splitter);

    connect(explorer_, &FileExplorerWidget::fileActivated,
            this, &EditorPanel::openFilePath);
    connect(explorer_, &FileExplorerWidget::fileRenamed,
            this, &EditorPanel::handleFileRenamed);
    connect(primaryPane_->editor, &QsciScintilla::modificationChanged, this,
            [this, pane = primaryPane_](bool) {
        updatePaneIndicator(pane);
        if (pane == activePane_)
            updateActionStates();
    });
    connect(primaryPane_->editor, &QsciScintilla::copyAvailable, this,
            [this, pane = primaryPane_](bool) {
        if (pane == activePane_)
            updateActionStates();
    });
    connect(primaryPane_->editor, &QsciScintilla::SCN_UPDATEUI, this,
            [this, pane = primaryPane_](int) {
        if (pane == activePane_) {
            updateActionStates();
            updateActiveLineMarker();
        }
    });
    connect(QApplication::clipboard(), &QClipboard::dataChanged,
            this, &EditorPanel::updateActionStates);

    updateActionStates();
}

EditorPanel::~EditorPanel()
{
    clearDocuments();
    delete secondaryPane_;
    delete primaryPane_;
}

EditorPanel::EditorPane *EditorPanel::createEditorPane(QWidget *parent, bool primary)
{
    auto *pane = new EditorPane;
    pane->container = new QWidget(parent);
    auto *paneLayout = new QVBoxLayout(pane->container);
    paneLayout->setContentsMargins(0, 0, 0, 0);
    paneLayout->setSpacing(0);

    pane->label = new QLabel(QStringLiteral("No file open"), pane->container);
    pane->label->setObjectName(primary ? QStringLiteral("currentFileLabel")
                                      : QStringLiteral("secondaryFileLabel"));
    pane->label->setContentsMargins(10, 6, 10, 6);
    paneLayout->addWidget(pane->label);

    pane->editor = new QsciScintilla(pane->container);
    pane->editor->setObjectName(primary ? QStringLiteral("sourceEditor")
                                       : QStringLiteral("secondarySourceEditor"));
    pane->editor->setReadOnly(true);
    pane->editor->installEventFilter(this);
    paneLayout->addWidget(pane->editor);
    return pane;
}

void EditorPanel::saveActivePaneState()
{
    if (!activePane_)
        return;
    activePane_->lexer = lexer_;
    activePane_->filePath = currentFilePath_;
    activePane_->savedContentHash = savedContentHash_;
    activePane_->activeLineMarkerHandle = activeLineMarkerHandle_;
    activePane_->activeLineMarkerLine = activeLineMarkerLine_;
    if (activePane_->documentState) {
        activePane_->documentState->filePath = currentFilePath_;
        activePane_->documentState->savedContentHash = savedContentHash_;
        activePane_->documentState->dirty = editor_->isModified();
    }
}

void EditorPanel::loadPaneState(EditorPane *pane)
{
    activePane_ = pane;
    editor_ = pane->editor;
    currentFileLabel_ = pane->label;
    lexer_ = pane->lexer;
    currentFilePath_ = pane->filePath;
    savedContentHash_ = pane->savedContentHash;
    activeLineMarkerHandle_ = pane->activeLineMarkerHandle;
    activeLineMarkerLine_ = pane->activeLineMarkerLine;
}

EditorPanel::DocumentState *EditorPanel::documentForPath(
    const QString &absolutePath) const
{
    return documentsByPath_.value(absolutePath, nullptr);
}

EditorPanel::DocumentState *EditorPanel::documentForTab(int index) const
{
    if (index < 0 || index >= documentTabs_->count())
        return nullptr;
    return documentsById_.value(documentTabs_->tabData(index).toString(), nullptr);
}

int EditorPanel::documentTabIndex(const DocumentState *document) const
{
    if (!document)
        return -1;
    for (int index = 0; index < documentTabs_->count(); ++index) {
        if (documentTabs_->tabData(index).toString() == document->id)
            return index;
    }
    return -1;
}

EditorPanel::DocumentState *EditorPanel::createDocument(
    const QString &filePath,
    const QByteArray &contents,
    bool newlyCreated)
{
    auto *document = new DocumentState;
    document->id = QString::number(nextDocumentId_++);
    document->filePath = filePath;
    document->displayName = filePath.isEmpty()
        ? QStringLiteral("Untitled")
        : QFileInfo(filePath).fileName();
    document->document = editor_->document();
    document->savedContentHash = newlyCreated
        ? QByteArray()
        : QCryptographicHash::hash(contents, QCryptographicHash::Sha256);
    document->newlyCreated = newlyCreated;
    document->dirty = editor_->isModified();
    documentsById_.insert(document->id, document);
    if (!filePath.isEmpty())
        documentsByPath_.insert(filePath, document);

    activePane_->documentState = document;
    activePane_->filePath = filePath;
    activePane_->savedContentHash = document->savedContentHash;
    currentFilePath_ = filePath;
    savedContentHash_ = document->savedContentHash;
    const int index = documentTabs_->addTab(document->displayName);
    documentTabs_->setTabData(index, document->id);
    documentTabs_->setTabToolTip(index, filePath);
    documentTabs_->show();
    documentTabs_->setCurrentIndex(index);
    return document;
}

void EditorPanel::activateDocument(DocumentState *document, EditorPane *pane)
{
    if (!document || !pane)
        return;
    if (pane == activePane_)
        saveActivePaneState();

    pane->editor->setLexer(nullptr);
    pane->editor->setDocument(document->document);
    pane->documentState = document;
    pane->filePath = document->filePath;
    pane->savedContentHash = document->savedContentHash;
    pane->activeLineMarkerHandle = -1;
    pane->activeLineMarkerLine = -1;
    if (pane == activePane_) {
        loadPaneState(pane);
        selectLexerForFile(currentFilePath_);
        editor_->setReadOnly(false);
        updateDocumentIndicator();
        updateActionStates();
        emit currentFileChanged(true);
    }
    updateDocumentTab(document);
}

void EditorPanel::updateDocumentTab(DocumentState *document)
{
    const int index = documentTabIndex(document);
    if (index < 0)
        return;
    const QString title = document->dirty
        ? QStringLiteral("* %1").arg(document->displayName)
        : document->displayName;
    documentTabs_->setTabText(index, title);
    documentTabs_->setTabToolTip(index, document->filePath);
}

void EditorPanel::updateAllDocumentTabs()
{
    for (DocumentState *document : documentsById_)
        updateDocumentTab(document);
}

void EditorPanel::removeDocument(DocumentState *document, bool alreadyConfirmed)
{
    if (!document || (!alreadyConfirmed && !confirmCloseDocument(document)))
        return;

    const int index = documentTabIndex(document);
    const bool wasActive = activePane_ && activePane_->documentState == document;
    if (primaryPane_->documentState == document) {
        primaryPane_->editor->setDocument(QsciDocument());
        primaryPane_->documentState = nullptr;
        primaryPane_->filePath.clear();
        primaryPane_->savedContentHash.clear();
        primaryPane_->editor->setReadOnly(true);
    }
    if (secondaryPane_ && secondaryPane_->documentState == document) {
        secondaryPane_->editor->setDocument(QsciDocument());
        secondaryPane_->documentState = nullptr;
        secondaryPane_->filePath.clear();
        secondaryPane_->savedContentHash.clear();
        secondaryPane_->editor->setReadOnly(true);
    }
    if (!document->filePath.isEmpty())
        documentsByPath_.remove(document->filePath);
    documentsById_.remove(document->id);
    if (index >= 0)
        documentTabs_->removeTab(index);
    delete document;

    if (wasActive) {
        DocumentState *nextDocument = documentForTab(documentTabs_->currentIndex());
        if (nextDocument) {
            activateDocument(nextDocument, activePane_);
        } else {
            activePane_->documentState = nullptr;
            activePane_->filePath.clear();
            activePane_->savedContentHash.clear();
            currentFilePath_.clear();
            savedContentHash_.clear();
            editor_->setDocument(QsciDocument());
            editor_->setText(QString());
            editor_->setReadOnly(true);
            selectLexerForFile(QString());
            updateDocumentIndicator();
            updateActionStates();
            emit currentFileChanged(false);
        }
    }
    if (documentTabs_->count() == 0)
        documentTabs_->hide();
    updateAllDocumentTabs();
}

bool EditorPanel::confirmCloseDocument(DocumentState *document)
{
    if (!document || !document->dirty)
        return true;

    const QMessageBox::StandardButton choice = QMessageBox::warning(
        this,
        QStringLiteral("Unsaved Changes"),
        QStringLiteral("Save changes to %1?").arg(document->displayName),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);
    if (choice == QMessageBox::Discard)
        return true;
    if (choice != QMessageBox::Save)
        return false;

    EditorPane *previousPane = activePane_;
    DocumentState *previousDocument = previousPane
        ? previousPane->documentState
        : nullptr;
    activateDocument(document, previousPane);
    const bool saved = saveCurrentFile();
    if (saved && previousPane && previousDocument != document)
        activateDocument(previousDocument, previousPane);
    return saved;
}

void EditorPanel::clearDocuments()
{
    if (primaryPane_) {
        primaryPane_->editor->setDocument(QsciDocument());
        primaryPane_->documentState = nullptr;
    }
    if (secondaryPane_) {
        secondaryPane_->editor->setDocument(QsciDocument());
        secondaryPane_->documentState = nullptr;
    }
    qDeleteAll(documentsById_);
    documentsById_.clear();
    documentsByPath_.clear();
    if (documentTabs_) {
        while (documentTabs_->count() > 0)
            documentTabs_->removeTab(documentTabs_->count() - 1);
        documentTabs_->hide();
    }
    currentFilePath_.clear();
    savedContentHash_.clear();
}

void EditorPanel::activatePane(EditorPane *pane)
{
    if (!pane || pane == activePane_)
        return;
    saveActivePaneState();
    loadPaneState(pane);
    updatePaneIndicator(pane);
    updateActionStates();
    emit currentFileChanged(!currentFilePath_.isEmpty() || !editor_->isReadOnly());
}

bool EditorPanel::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::FocusIn || event->type() == QEvent::MouseButtonPress) {
        if (primaryPane_ && watched == primaryPane_->editor)
            activatePane(primaryPane_);
        else if (secondaryPane_ && watched == secondaryPane_->editor)
            activatePane(secondaryPane_);
    }
    return QWidget::eventFilter(watched, event);
}

void EditorPanel::configureEditor()
{
    const QFont font = editorFont();
    editor_->setFont(font);
    editor_->setUtf8(true);
    editor_->setIndentationsUseTabs(false);
    editor_->setTabWidth(4);
    editor_->setIndentationWidth(4);
    editor_->setAutoIndent(true);
    editor_->setIndentationGuides(true);
    editor_->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    editor_->setCaretLineVisible(true);
    editor_->setMarginType(0, QsciScintilla::NumberMargin);
    editor_->setMarginType(1, QsciScintilla::SymbolMargin);
    editor_->setMarginWidth(1, 3);
    editor_->setMarginMarkerMask(1, 1 << 8);
    editor_->markerDefine(QsciScintilla::VLine, 8);
    editor_->setMarginWidth(
        0, QString(qMax(1, QString::number(editor_->lines()).size()), QLatin1Char('0')));
    connect(editor_, &QsciScintilla::linesChanged, editor_, [configuredEditor = editor_]() {
        const int digits = qMax(1, QString::number(configuredEditor->lines()).size());
        configuredEditor->setMarginWidth(0, QString(digits, QLatin1Char('0')));
    });
    editor_->setFolding(QsciScintilla::BoxedTreeFoldStyle, 2);
    editor_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    editor_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    editor_->setWhitespaceVisibility(QsciScintilla::WsInvisible);
    applyEditorColors(editor_, lexer_);
}

void EditorPanel::applyEditorColors(QsciScintilla *editor, QsciLexer *lexer)
{
    if (!editor)
        return;

    editor->setMatchedBraceBackgroundColor(editorPalette_.selection);
    editor->setMatchedBraceForegroundColor(editorPalette_.brightAccent);
    editor->setCaretLineBackgroundColor(editorPalette_.currentLine);
    editor->setCaretForegroundColor(editorPalette_.brightAccent);
    editor->setSelectionBackgroundColor(editorPalette_.selection);
    editor->setSelectionForegroundColor(editorPalette_.foreground);
    editor->setMarginsBackgroundColor(editorPalette_.secondaryBackground);
    editor->setMarginsForegroundColor(editorPalette_.mutedText);
    editor->setMarginBackgroundColor(0, editorPalette_.secondaryBackground);
    editor->setMarginBackgroundColor(1, editorPalette_.secondaryBackground);
    editor->setFoldMarginColors(editorPalette_.secondaryText,
                                editorPalette_.secondaryBackground);
    editor->setMarkerBackgroundColor(editorPalette_.amber, 8);
    editor->setMarkerForegroundColor(editorPalette_.amber, 8);
    editor->SendScintilla(QsciScintilla::SCI_STYLESETBACK,
                          QsciScintilla::STYLE_DEFAULT,
                          editorPalette_.background.rgb());
    editor->SendScintilla(QsciScintilla::SCI_STYLESETFORE,
                          QsciScintilla::STYLE_DEFAULT,
                          editorPalette_.foreground.rgb());
    editor->SendScintilla(QsciScintilla::SCI_STYLECLEARALL);

    if (lexer) {
        lexer->setDefaultColor(editorPalette_.foreground);
        lexer->setDefaultPaper(editorPalette_.background);
        lexer->setPaper(editorPalette_.background);
        lexer->setColor(editorPalette_.foreground);
        for (int style = 0; style <= QsciScintilla::STYLE_MAX; ++style) {
            const QString description = lexer->description(style);
            if (!description.isEmpty())
                lexer->setColor(colorForLexerStyle(description, editorPalette_), style);
        }
        editor->recolor();
    } else {
        editor->SendScintilla(QsciScintilla::SCI_STYLESETBACK,
                              QsciScintilla::STYLE_DEFAULT,
                              editorPalette_.background.rgb());
        editor->SendScintilla(QsciScintilla::SCI_STYLESETFORE,
                              QsciScintilla::STYLE_DEFAULT,
                              editorPalette_.foreground.rgb());
    }
}

void EditorPanel::splitEditor(Qt::Orientation orientation)
{
    if (!secondaryPane_) {
        secondaryPane_ = createEditorPane(editorSplitter_, false);
        applyEditorColors(secondaryPane_->editor, nullptr);
        saveActivePaneState();
        editorSplitter_->addWidget(secondaryPane_->container);

        connect(secondaryPane_->editor, &QsciScintilla::modificationChanged, this,
                [this, pane = secondaryPane_](bool) {
            updatePaneIndicator(pane);
            if (pane == activePane_)
                updateActionStates();
        });
        connect(secondaryPane_->editor, &QsciScintilla::copyAvailable, this,
                [this, pane = secondaryPane_](bool) {
            if (pane == activePane_)
                updateActionStates();
        });
        connect(secondaryPane_->editor, &QsciScintilla::SCN_UPDATEUI, this,
                [this, pane = secondaryPane_](int) {
            if (pane == activePane_) {
                updateActionStates();
                updateActiveLineMarker();
            }
        });

        loadPaneState(secondaryPane_);
        configureEditor();
        saveActivePaneState();
        if (auto *action = findChild<QAction *>(QStringLiteral("closeSplitAction")))
            action->setEnabled(true);
    }

    editorSplitter_->setOrientation(orientation);
    editorSplitter_->setSizes({1, 1});
    activatePane(secondaryPane_);
    secondaryPane_->editor->setFocus();
}

void EditorPanel::closeSplit()
{
    if (!secondaryPane_)
        return;

    EditorPane *previousPane = activePane_;
    activatePane(secondaryPane_);
    if (!maybeSaveCurrentFile()) {
        activatePane(previousPane);
        return;
    }

    EditorPane *closingPane = secondaryPane_;
    secondaryPane_ = nullptr;
    if (activePane_ == closingPane)
        activatePane(primaryPane_);
    delete closingPane->container;
    delete closingPane;
    if (auto *action = findChild<QAction *>(QStringLiteral("closeSplitAction")))
        action->setEnabled(false);
    updateActionStates();
}

bool EditorPanel::maybeSaveAllFiles()
{
    EditorPane *pane = activePane_;
    DocumentState *previousDocument = pane ? pane->documentState : nullptr;
    const QList<DocumentState *> documents = documentsById_.values();
    for (DocumentState *document : documents) {
        activateDocument(document, pane);
        if (!maybeSaveCurrentFile()) {
            activateDocument(previousDocument, pane);
            return false;
        }
    }
    if (previousDocument)
        activateDocument(previousDocument, pane);
    return true;
}

bool EditorPanel::saveAllFiles()
{
    EditorPane *pane = activePane_;
    DocumentState *previousDocument = pane ? pane->documentState : nullptr;
    const QList<DocumentState *> documents = documentsById_.values();
    for (DocumentState *document : documents) {
        if (!document->dirty)
            continue;
        activateDocument(document, pane);
        if (!saveCurrentFile()) {
            if (previousDocument)
                activateDocument(previousDocument, pane);
            return false;
        }
    }
    if (previousDocument)
        activateDocument(previousDocument, pane);
    return true;
}

bool EditorPanel::closeCurrentDocument()
{
    DocumentState *document = activePane_ ? activePane_->documentState : nullptr;
    if (!document)
        return true;
    const QString documentId = document->id;
    removeDocument(document);
    return !documentsById_.contains(documentId);
}

bool EditorPanel::closeAllDocuments()
{
    while (documentTabs_->count() > 0) {
        DocumentState *document = documentForTab(0);
        if (!document || !confirmCloseDocument(document))
            return false;
        removeDocument(document, true);
    }
    return true;
}

bool EditorPanel::openProjectDirectory(const QString &directory)
{
    const QFileInfo info(directory);
    if (!info.exists() || !info.isDir() || !maybeSaveAllFiles())
        return false;

    if (!explorer_->setRootDirectory(directory))
        return false;

    clearDocuments();
    for (EditorPane *pane : {primaryPane_, secondaryPane_}) {
        if (!pane)
            continue;
        pane->filePath.clear();
        pane->savedContentHash.clear();
        pane->activeLineMarkerHandle = -1;
        pane->activeLineMarkerLine = -1;
        pane->editor->setDocument(QsciDocument());
        pane->editor->setText(QString());
        pane->editor->setReadOnly(true);
    }
    selectLexerForFile(QString());
    updateDocumentIndicator();
    updateActionStates();
    emit currentFileChanged(false);
    return true;
}

QString EditorPanel::projectDirectory() const
{
    return explorer_->rootDirectory();
}

void EditorPanel::handleFileRenamed(const QString &oldPath, const QString &newPath)
{
    const QString absoluteOldPath = QFileInfo(oldPath).absoluteFilePath();
    const QString absoluteNewPath = QFileInfo(newPath).absoluteFilePath();
    const bool renamedDirectory = QFileInfo(absoluteNewPath).isDir();
    const QString descendantPrefix = absoluteOldPath + QDir::separator();
    for (DocumentState *document : documentsById_) {
        if (document->filePath != absoluteOldPath
            && (!renamedDirectory || !document->filePath.startsWith(descendantPrefix)))
            continue;
        documentsByPath_.remove(document->filePath);
        const QString suffix = document->filePath.mid(absoluteOldPath.size());
        document->filePath = absoluteNewPath + suffix;
        document->displayName = QFileInfo(document->filePath).fileName();
        documentsByPath_.insert(document->filePath, document);
        for (EditorPane *pane : {primaryPane_, secondaryPane_}) {
            if (pane && pane->documentState == document)
                pane->filePath = document->filePath;
        }
        if (activePane_ && activePane_->documentState == document)
            currentFilePath_ = document->filePath;
        updateDocumentTab(document);
    }
    updatePaneIndicator(primaryPane_);
    updatePaneIndicator(secondaryPane_);
}

bool EditorPanel::saveCurrentFile()
{
    if (currentFilePath_.isEmpty())
        return saveAs();

    if (fileHasChangedExternally()) {
        const QMessageBox::StandardButton choice = QMessageBox::warning(
            this,
            QStringLiteral("File changed externally"),
            QStringLiteral("%1 has changed or was removed outside Kogence IDE. "
                           "Overwrite the current file with your editor contents?")
                .arg(currentFilePath_),
            QMessageBox::Yes | QMessageBox::Cancel,
            QMessageBox::Cancel);
        if (choice != QMessageBox::Yes)
            return false;
    }

    if (!writeDocumentToPath(currentFilePath_))
        return false;

    savedContentHash_ = QCryptographicHash::hash(contentsForEditor(editor_),
                                                  QCryptographicHash::Sha256);
    editor_->setModified(false);
    if (activePane_->documentState) {
        activePane_->documentState->savedContentHash = savedContentHash_;
        activePane_->documentState->dirty = false;
        activePane_->documentState->newlyCreated = false;
    }
    updateDocumentIndicator();
    updateAllDocumentTabs();
    return true;
}

bool EditorPanel::saveCurrentFileAs(const QString &filePath)
{
    if (filePath.isEmpty())
        return false;

    const QString absolutePath = QFileInfo(filePath).absoluteFilePath();
    DocumentState *existing = documentForPath(absolutePath);
    if (existing && existing != activePane_->documentState) {
        QMessageBox::warning(this,
                             QStringLiteral("File already open"),
                             QStringLiteral("%1 is already open in another document.")
                                 .arg(absolutePath));
        return false;
    }
    if (!writeDocumentToPath(absolutePath))
        return false;

    DocumentState *document = activePane_->documentState;
    if (document && !document->filePath.isEmpty())
        documentsByPath_.remove(document->filePath);
    currentFilePath_ = absolutePath;
    savedContentHash_ = QCryptographicHash::hash(contentsForEditor(editor_),
                                                  QCryptographicHash::Sha256);
    editor_->setModified(false);
    if (document) {
        document->filePath = absolutePath;
        document->displayName = QFileInfo(absolutePath).fileName();
        document->savedContentHash = savedContentHash_;
        document->dirty = false;
        document->newlyCreated = false;
        documentsByPath_.insert(absolutePath, document);
        activePane_->filePath = absolutePath;
        activePane_->savedContentHash = savedContentHash_;
    }
    selectLexerForFile(currentFilePath_);
    updateDocumentIndicator();
    updateAllDocumentTabs();
    emit currentFileChanged(true);
    return true;
}

bool EditorPanel::maybeSaveCurrentFile()
{
    if (!editor_->isModified())
        return true;

    const QString displayName = currentFilePath_.isEmpty()
        ? QStringLiteral("Untitled")
        : QFileInfo(currentFilePath_).fileName();
    const QMessageBox::StandardButton choice = QMessageBox::warning(
        this,
        QStringLiteral("Unsaved Changes"),
        QStringLiteral("Save changes to %1?").arg(displayName),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);
    if (choice == QMessageBox::Save)
        return saveCurrentFile();
    return choice == QMessageBox::Discard;
}

QAction *EditorPanel::saveAction() const
{
    return saveAction_;
}

QAction *EditorPanel::saveAsAction() const
{
    return saveAsAction_;
}

QAction *EditorPanel::newFileAction() const
{
    return newFileAction_;
}

QAction *EditorPanel::openFileAction() const
{
    return openFileAction_;
}

QAction *EditorPanel::saveAllAction() const
{
    return saveAllAction_;
}

FileExplorerWidget *EditorPanel::fileExplorer() const
{
    return explorer_;
}

void EditorPanel::setEditorTheme(const QString &id)
{
    if (id != QStringLiteral("metal")
        && id != QStringLiteral("glass-light")
        && id != QStringLiteral("glass-dark")) {
        return;
    }
    if (editorThemeId_ == id)
        return;

    editorThemeId_ = id;
    editorPalette_ = editorPaletteForId(id);
    applyEditorColors(primaryPane_->editor, primaryPane_->lexer);
    if (secondaryPane_)
        applyEditorColors(secondaryPane_->editor, secondaryPane_->lexer);
}

QString EditorPanel::editorTheme() const
{
    return editorThemeId_;
}

void EditorPanel::openFileDialog()
{
    chooseFileToOpen();
}

bool EditorPanel::openFilePath(const QString &filePath)
{
    const QString absolutePath = QFileInfo(filePath).absoluteFilePath();
    if (DocumentState *existing = documentForPath(absolutePath)) {
        const int index = documentTabIndex(existing);
        if (index >= 0)
            documentTabs_->setCurrentIndex(index);
        activateDocument(existing, activePane_);
        return true;
    }

    QFile file(absolutePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                             QStringLiteral("Unable to open file"),
                             QStringLiteral("Could not open %1:\n%2")
                                 .arg(absolutePath, file.errorString()));
        return false;
    }

    const QByteArray contents = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        QMessageBox::warning(this,
                             QStringLiteral("Unable to read file"),
                             QStringLiteral("Could not read %1:\n%2")
                                 .arg(absolutePath, file.errorString()));
        return false;
    }
    if (contents.contains('\0')) {
        QMessageBox::warning(this,
                             QStringLiteral("Unsupported file"),
                             QStringLiteral("%1 appears to be a binary file and cannot be edited.")
                                 .arg(absolutePath));
        return false;
    }

    saveActivePaneState();
    editor_->setLexer(nullptr);
    editor_->setDocument(QsciDocument());
    if (contents.contains("\r\n"))
        editor_->setEolMode(QsciScintilla::EolWindows);
    else if (contents.contains('\r'))
        editor_->setEolMode(QsciScintilla::EolMac);
    else
        editor_->setEolMode(QsciScintilla::EolUnix);
    editor_->setText(QString::fromUtf8(contents));
    editor_->convertEols(editor_->eolMode());
    editor_->recolor();
    editor_->setModified(false);
    DocumentState *document = createDocument(absolutePath, contents, false);
    activateDocument(document, activePane_);
    selectLexerForFile(currentFilePath_);
    editor_->setReadOnly(false);
    updateDocumentIndicator();
    updateActionStates();
    return true;
}

void EditorPanel::chooseFileToOpen()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Open File"),
        currentFilePath_.isEmpty() ? projectDirectory() : currentFilePath_);
    if (!path.isEmpty())
        openFilePath(path);
}

void EditorPanel::createNewFile()
{
    newDocument();
}

void EditorPanel::newDocument()
{
    saveActivePaneState();
    editor_->setLexer(nullptr);
    editor_->setDocument(QsciDocument());
    editor_->setText(QString());
    editor_->setModified(false);
    DocumentState *document = createDocument(QString(), QByteArray(), true);
    activateDocument(document, activePane_);
    editor_->setReadOnly(false);
    selectLexerForFile(QString());
    editor_->setFocus();
}

bool EditorPanel::saveAs()
{
    const QString suggestion = currentFilePath_.isEmpty()
        ? projectDirectory()
        : currentFilePath_;
    const QString path = QFileDialog::getSaveFileName(
        this, QStringLiteral("Save File As"), suggestion);
    return !path.isEmpty() && saveCurrentFileAs(path);
}

void EditorPanel::showFindDialog(bool replace)
{
    if (editor_->isReadOnly())
        return;

    QDialog dialog(this);
    dialog.setObjectName(QStringLiteral("editorSearchDialog"));
    dialog.setWindowTitle(replace ? QStringLiteral("Find and Replace")
                                  : QStringLiteral("Find"));
    auto *layout = new QVBoxLayout(&dialog);
    auto *findEdit = new QLineEdit(editor_->selectedText(), &dialog);
    findEdit->setPlaceholderText(QStringLiteral("Find"));
    layout->addWidget(findEdit);

    QLineEdit *replaceEdit = nullptr;
    if (replace) {
        replaceEdit = new QLineEdit(&dialog);
        replaceEdit->setPlaceholderText(QStringLiteral("Replace with"));
        layout->addWidget(replaceEdit);
    }

    auto *caseSensitive = new QCheckBox(QStringLiteral("Case sensitive"), &dialog);
    layout->addWidget(caseSensitive);
    auto *buttons = new QDialogButtonBox(&dialog);
    QPushButton *findNextButton = buttons->addButton(
        replace ? QStringLiteral("Replace Next") : QStringLiteral("Find Next"),
        QDialogButtonBox::ActionRole);
    QPushButton *replaceAllButton = replace
        ? buttons->addButton(QStringLiteral("Replace All"), QDialogButtonBox::ActionRole)
        : nullptr;
    buttons->addButton(QDialogButtonBox::Close);
    layout->addWidget(buttons);

    bool searchInitialized = false;
    const auto findNext = [this, findEdit, caseSensitive, &searchInitialized]() {
        if (findEdit->text().isEmpty())
            return false;
        if (!searchInitialized) {
            searchInitialized = true;
            return editor_->findFirst(findEdit->text(), false,
                                      caseSensitive->isChecked(), false, true);
        }
        return editor_->findNext();
    };
    connect(findNextButton, &QPushButton::clicked, &dialog, [&, replace]() {
        if (!replace) {
            findNext();
            return;
        }
        if (searchInitialized && editor_->hasSelectedText())
            editor_->replace(replaceEdit->text());
        if (findEdit->text().isEmpty())
            return;
        searchInitialized = true;
        editor_->findFirst(findEdit->text(), false,
                           caseSensitive->isChecked(), false, true);
    });
    if (replaceAllButton) {
        connect(replaceAllButton, &QPushButton::clicked, &dialog, [this, findEdit,
                                                                   replaceEdit,
                                                                   caseSensitive,
                                                                   &searchInitialized]() {
            if (findEdit->text().isEmpty())
                return;
            editor_->beginUndoAction();
            bool found = editor_->findFirst(findEdit->text(), false,
                                            caseSensitive->isChecked(), false,
                                            false, true, 0, 0);
            while (found) {
                editor_->replace(replaceEdit->text());
                found = editor_->findNext();
            }
            editor_->endUndoAction();
            searchInitialized = false;
        });
    }
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    findEdit->setFocus();
    dialog.exec();
}

void EditorPanel::updateDocumentIndicator()
{
    saveActivePaneState();
    updatePaneIndicator(activePane_);
}

void EditorPanel::updatePaneIndicator(EditorPane *pane)
{
    if (!pane)
        return;
    const QString displayName = pane->documentState
        ? pane->documentState->displayName
        : QStringLiteral("No file open");
    if (pane->documentState) {
        pane->documentState->dirty = pane->editor->isModified();
        updateDocumentTab(pane->documentState);
    }
    const QString markedName = pane->documentState && pane->documentState->dirty
        ? QStringLiteral("* %1").arg(displayName)
        : displayName;
    pane->label->setText(markedName);
    pane->label->setToolTip(pane->filePath);
}

void EditorPanel::updateActionStates()
{
    saveActivePaneState();
    const bool hasDocument = !editor_->isReadOnly();
    saveAction_->setEnabled(hasDocument);
    saveAsAction_->setEnabled(hasDocument);
    saveAllAction_->setEnabled(std::any_of(
        documentsById_.cbegin(), documentsById_.cend(),
        [](const DocumentState *document) { return document->dirty; }));
    undoAction_->setEnabled(hasDocument && editor_->isUndoAvailable());
    redoAction_->setEnabled(hasDocument && editor_->isRedoAvailable());
    cutAction_->setEnabled(hasDocument && editor_->hasSelectedText());
    copyAction_->setEnabled(hasDocument && editor_->hasSelectedText());
    pasteAction_->setEnabled(hasDocument
                             && !QApplication::clipboard()->text().isEmpty());
    selectAllAction_->setEnabled(hasDocument);
    findAction_->setEnabled(hasDocument);
    replaceAction_->setEnabled(hasDocument);
    commentAction_->setEnabled(hasDocument && !commentPrefix(currentFilePath_).isEmpty());
    indentAction_->setEnabled(hasDocument);
    unindentAction_->setEnabled(hasDocument);
    if (auto *action = findChild<QAction *>(QStringLiteral("closeSplitAction")))
        action->setEnabled(secondaryPane_ != nullptr);
}

void EditorPanel::updateActiveLineMarker()
{
    int line = 0;
    int index = 0;
    editor_->getCursorPosition(&line, &index);
    if (line == activeLineMarkerLine_)
        return;

    if (activeLineMarkerHandle_ >= 0)
        editor_->markerDeleteHandle(activeLineMarkerHandle_);
    activeLineMarkerHandle_ = editor_->markerAdd(line, 8);
    activeLineMarkerLine_ = line;
    saveActivePaneState();
}

void EditorPanel::selectLexerForFile(const QString &filePath)
{
    if (lexer_) {
        editor_->setLexer(nullptr);
        delete lexer_;
        lexer_ = nullptr;
    }

    const QString extension = QFileInfo(filePath).suffix().toLower();
    QsciLexer *lexer = nullptr;
    if (extension == QStringLiteral("dart")) {
        lexer = new DartLexer(editor_);
    } else if (extension == QStringLiteral("html")
               || extension == QStringLiteral("htm")
               || extension == QStringLiteral("vue")) {
        lexer = new QsciLexerHTML(editor_);
    } else if (extension == QStringLiteral("css")
               || extension == QStringLiteral("scss")
               || extension == QStringLiteral("less")) {
        lexer = new QsciLexerCSS(editor_);
    } else if (extension == QStringLiteral("js")
               || extension == QStringLiteral("jsx")
               || extension == QStringLiteral("mjs")
               || extension == QStringLiteral("cjs")
               || extension == QStringLiteral("ts")
               || extension == QStringLiteral("tsx")) {
        lexer = new QsciLexerJavaScript(editor_);
    } else if (extension == QStringLiteral("json")
               || extension == QStringLiteral("jsonc")) {
        lexer = new QsciLexerJSON(editor_);
    } else if (extension == QStringLiteral("xml")
               || extension == QStringLiteral("xsd")
               || extension == QStringLiteral("svg")) {
        lexer = new QsciLexerXML(editor_);
    } else if (extension == QStringLiteral("py")) {
        lexer = new QsciLexerPython(editor_);
    } else if (extension == QStringLiteral("c") || extension == QStringLiteral("h")
               || extension == QStringLiteral("cc") || extension == QStringLiteral("cpp")
               || extension == QStringLiteral("cxx") || extension == QStringLiteral("hh")
               || extension == QStringLiteral("hpp") || extension == QStringLiteral("hxx")
               || extension == QStringLiteral("ipp") || extension == QStringLiteral("inl")
               || extension == QStringLiteral("tpp") || extension == QStringLiteral("ino")) {
        lexer = new QsciLexerCPP(editor_);
    } else if (extension == QStringLiteral("java")) {
        lexer = new QsciLexerJava(editor_);
    } else if (extension == QStringLiteral("sh") || extension == QStringLiteral("bash")
               || extension == QStringLiteral("zsh")) {
        lexer = new QsciLexerBash(editor_);
    } else if (extension == QStringLiteral("md") || extension == QStringLiteral("markdown")) {
        lexer = new QsciLexerMarkdown(editor_);
    } else if (extension == QStringLiteral("yaml") || extension == QStringLiteral("yml")) {
        lexer = new QsciLexerYAML(editor_);
    }

    if (lexer) {
        const QFont font = editorFont();
        lexer->setDefaultFont(font);
        lexer->setFont(font);
        lexer_ = lexer;
        if (activePane_)
            activePane_->lexer = lexer;
        editor_->setLexer(lexer_);
        applyEditorColors(editor_, lexer_);
    } else {
        editor_->SendScintilla(QsciScintilla::SCI_STYLESETBACK,
                               QsciScintilla::STYLE_DEFAULT,
                               editorPalette_.background.rgb());
        editor_->SendScintilla(QsciScintilla::SCI_STYLESETFORE,
                               QsciScintilla::STYLE_DEFAULT,
                               editorPalette_.foreground.rgb());
        const QByteArray family = editor_->font().family().toUtf8();
        editor_->SendScintilla(QsciScintilla::SCI_STYLESETFONT,
                               QsciScintilla::STYLE_DEFAULT,
                               family.constData());
    }
    saveActivePaneState();
}

bool EditorPanel::fileHasChangedExternally() const
{
    QFile file(currentFilePath_);
    if (!file.open(QIODevice::ReadOnly))
        return true;
    const QByteArray contents = file.readAll();
    return file.error() != QFileDevice::NoError
        || QCryptographicHash::hash(contents, QCryptographicHash::Sha256)
               != savedContentHash_;
}

bool EditorPanel::writeDocumentToPath(const QString &filePath)
{
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             QStringLiteral("Unable to save file"),
                             QStringLiteral("Could not save %1:\n%2")
                                 .arg(filePath, file.errorString()));
        return false;
    }

    const QByteArray contents = contentsForEditor(editor_);
    if (file.write(contents) != contents.size() || !file.commit()) {
        QMessageBox::warning(this,
                             QStringLiteral("Unable to save file"),
                             QStringLiteral("Could not save %1:\n%2")
                                 .arg(filePath, file.errorString()));
        return false;
    }
    return true;
}
