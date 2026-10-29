#include "dartlexer.h"
#include "editorpanel.h"
#include "fileexplorerwidget.h"

#include <Qsci/qscilexer.h>
#include <Qsci/qsciscintilla.h>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFocusEvent>
#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QSignalSpy>
#include <QSplitter>
#include <QTemporaryDir>
#include <QTimer>
#include <QtTest>

class EditorTests final : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void invalidProjectDirectoryIsRejected();
    void editsSaveUtf8AndTrackDocumentState();
    void copyCutPasteAndUndoActionsOperateOnTheEditor();
    void lexerSelectionTracksFileExtensions();
    void saveAsChangesTheDocumentPath();
    void failedSaveKeepsTheDocumentModified();
    void switchingFilesReplacesTheActiveDocument();
    void splitPanesSaveIndependentFilesAndClose();
    void duplicateFileViewsShareDocument();

private:
    bool activateFile(const QString &path);
    QString createFile(const QString &name, const QByteArray &contents);

    QTemporaryDir temporaryDirectory_;
    EditorPanel *panel_ = nullptr;
    QsciScintilla *editor_ = nullptr;
};

void EditorTests::init()
{
    QVERIFY(temporaryDirectory_.isValid());
    panel_ = new EditorPanel;
    editor_ = panel_->findChild<QsciScintilla *>(QStringLiteral("sourceEditor"));
    QVERIFY(editor_);
}

void EditorTests::cleanup()
{
    delete panel_;
    panel_ = nullptr;
    editor_ = nullptr;
}

bool EditorTests::activateFile(const QString &path)
{
    auto *explorer = panel_->findChild<FileExplorerWidget *>();
    return explorer
        && QMetaObject::invokeMethod(explorer,
                                     "fileActivated",
                                     Qt::DirectConnection,
                                     Q_ARG(QString, path));
}

QString EditorTests::createFile(const QString &name, const QByteArray &contents)
{
    const QString path = temporaryDirectory_.filePath(name);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
        return QString();
    if (file.write(contents) != contents.size())
        return QString();
    return path;
}

void EditorTests::invalidProjectDirectoryIsRejected()
{
    const QString missingPath =
        temporaryDirectory_.filePath(QStringLiteral("does-not-exist"));
    QVERIFY(!panel_->openProjectDirectory(missingPath));
    QVERIFY(panel_->projectDirectory().isEmpty());
}

void EditorTests::editsSaveUtf8AndTrackDocumentState()
{
    const QString projectPath = temporaryDirectory_.filePath(QStringLiteral("project"));
    QVERIFY(QDir().mkpath(projectPath));
    const QString filePath = QDir(projectPath).filePath(QStringLiteral("notes.txt"));
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    const QByteArray original("first line\nsecond line\n");
    QCOMPARE(file.write(original), qint64(original.size()));
    file.close();

    QVERIFY(panel_->openProjectDirectory(projectPath));
    QCOMPARE(panel_->projectDirectory(), QDir(projectPath).absolutePath());
    QSignalSpy fileChangedSpy(panel_, &EditorPanel::currentFileChanged);
    QVERIFY(activateFile(filePath));
    QCOMPARE(editor_->text(), QStringLiteral("first line\nsecond line\n"));
    QVERIFY(!editor_->isReadOnly());
    QVERIFY(!editor_->isModified());
    QVERIFY(editor_->lexer() == nullptr);

    QCOMPARE(fileChangedSpy.count(), 1);
    QCOMPARE(fileChangedSpy.takeFirst().at(0).toBool(), true);
    const QString editedText = QString::fromUtf8("Kogence: café 🚀\nsecond line");
    editor_->selectAll();
    editor_->replaceSelectedText(editedText);
    QVERIFY(editor_->isModified());
    QVERIFY(panel_->findChild<QLabel *>(QStringLiteral("currentFileLabel"))
                ->text().startsWith(QLatin1Char('*')));
    editor_->undo();
    QCOMPARE(editor_->text(), QStringLiteral("first line\nsecond line\n"));
    editor_->redo();
    QCOMPARE(editor_->text(), editedText);
    QVERIFY(panel_->saveCurrentFile());
    QVERIFY(!editor_->isModified());
    QVERIFY(!panel_->findChild<QLabel *>(QStringLiteral("currentFileLabel"))
                 ->text().startsWith(QLatin1Char('*')));
    QCOMPARE(fileChangedSpy.count(), 0);

    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), editedText.toUtf8());
}

void EditorTests::copyCutPasteAndUndoActionsOperateOnTheEditor()
{
    const QString path = createFile(QStringLiteral("actions.txt"), "hello");
    QVERIFY(!path.isEmpty());
    QVERIFY(activateFile(path));

    auto *copy = panel_->findChild<QAction *>(QStringLiteral("copyAction"));
    auto *cut = panel_->findChild<QAction *>(QStringLiteral("cutAction"));
    auto *paste = panel_->findChild<QAction *>(QStringLiteral("pasteAction"));
    auto *undo = panel_->findChild<QAction *>(QStringLiteral("undoAction"));
    auto *redo = panel_->findChild<QAction *>(QStringLiteral("redoAction"));
    QVERIFY(copy && cut && paste && undo && redo);

    editor_->selectAll();
    QVERIFY(copy->isEnabled());
    copy->trigger();
    QCOMPARE(QApplication::clipboard()->text(), QStringLiteral("hello"));
    cut->trigger();
    QVERIFY(editor_->text().isEmpty());
    QApplication::clipboard()->setText(QStringLiteral("pasted"));
    paste->trigger();
    QCOMPARE(editor_->text(), QStringLiteral("pasted"));
    QVERIFY(undo->isEnabled());
    undo->trigger();
    QVERIFY(editor_->text().isEmpty());
    QVERIFY(redo->isEnabled());
    redo->trigger();
    QCOMPARE(editor_->text(), QStringLiteral("pasted"));
}

void EditorTests::lexerSelectionTracksFileExtensions()
{
    const QString dartPath = createFile(QStringLiteral("sample.dart"),
                                         "class Sample { void run() {} }\n");
    const QString jsPath = createFile(QStringLiteral("sample.ts"),
                                       "const answer: number = 42;\n");
    const QString javaPath = createFile(QStringLiteral("Sample.java"),
                                         "public class Sample { }\n");
    const QString cppPath = createFile(QStringLiteral("sample.cpp"),
                                        "class Sample { };\n");
    const QString htmlPath = createFile(QStringLiteral("sample.html"),
                                         "<html></html>\n");
    const QString cssPath = createFile(QStringLiteral("sample.css"),
                                        "body { color: red; }\n");
    const QString xmlPath = createFile(QStringLiteral("sample.xml"),
                                        "<root />\n");
    const QString textPath = createFile(QStringLiteral("sample.unknown"),
                                          "plain text\n");
    QVERIFY(!dartPath.isEmpty());
    QVERIFY(!jsPath.isEmpty());
    QVERIFY(!javaPath.isEmpty());
    QVERIFY(!cppPath.isEmpty());
    QVERIFY(!htmlPath.isEmpty());
    QVERIFY(!cssPath.isEmpty());
    QVERIFY(!xmlPath.isEmpty());
    QVERIFY(!textPath.isEmpty());

    QVERIFY(activateFile(dartPath));
    auto *dartLexer = qobject_cast<DartLexer *>(editor_->lexer());
    QVERIFY(dartLexer);
    QCOMPARE(QByteArray(dartLexer->language()), QByteArray("Dart"));
    QVERIFY(editor_->folding() != QsciScintilla::NoFoldStyle);
    QCoreApplication::processEvents();
    const long defaultColor = editor_->SendScintilla(
        QsciScintilla::SCI_STYLEGETFORE, QsciScintilla::STYLE_DEFAULT);
    const long dartKeywordStyle = editor_->SendScintilla(
        QsciScintilla::SCI_GETSTYLEAT, 0);
    QVERIFY(dartKeywordStyle > 0);
    QVERIFY(dartKeywordStyle < QsciScintilla::STYLE_DEFAULT);
    QVERIFY(editor_->SendScintilla(QsciScintilla::SCI_STYLEGETFORE,
                                   dartKeywordStyle) != defaultColor);

    QVERIFY(activateFile(jsPath));
    QVERIFY(editor_->lexer());
    QCOMPARE(QByteArray(editor_->lexer()->language()), QByteArray("JavaScript"));
    QCoreApplication::processEvents();
    const long javascriptKeywordStyle = editor_->SendScintilla(
        QsciScintilla::SCI_GETSTYLEAT, 0);
    QVERIFY(javascriptKeywordStyle > 0);
    QVERIFY(javascriptKeywordStyle < QsciScintilla::STYLE_DEFAULT);
    QVERIFY(editor_->SendScintilla(QsciScintilla::SCI_STYLEGETFORE,
                                   javascriptKeywordStyle) != defaultColor);

    const QList<QPair<QString, QByteArray>> lexerCases = {
        {javaPath, "Java"},
        {cppPath, "C++"},
        {htmlPath, "HTML"},
        {cssPath, "CSS"},
        {xmlPath, "XML"}
    };
    for (const auto &testCase : lexerCases) {
        QVERIFY(activateFile(testCase.first));
        QVERIFY(editor_->lexer());
        QCOMPARE(QByteArray(editor_->lexer()->language()), testCase.second);
        if (testCase.second == QByteArray("Java")
            || testCase.second == QByteArray("C++")
            || testCase.second == QByteArray("HTML")) {
            QCoreApplication::processEvents();
            const long keywordStyle = editor_->SendScintilla(
                QsciScintilla::SCI_GETSTYLEAT, 0);
            QVERIFY(keywordStyle > 0);
            QVERIFY(keywordStyle < QsciScintilla::STYLE_DEFAULT);
            QVERIFY(editor_->SendScintilla(QsciScintilla::SCI_STYLEGETFORE,
                                           keywordStyle) != defaultColor);
        }
    }

    QVERIFY(activateFile(textPath));
    QVERIFY(!editor_->lexer());
    QCOMPARE(editor_->text(), QStringLiteral("plain text\n"));
}

void EditorTests::saveAsChangesTheDocumentPath()
{
    const QString originalPath = createFile(QStringLiteral("original.txt"), "old");
    const QString saveAsPath = temporaryDirectory_.filePath(QStringLiteral("renamed.json"));
    QVERIFY(activateFile(originalPath));
    editor_->selectAll();
    editor_->replaceSelectedText(QStringLiteral("{\"ok\": true}\n"));
    QVERIFY(panel_->saveCurrentFileAs(saveAsPath));
    QVERIFY(!editor_->isModified());
    QCOMPARE(panel_->findChild<QLabel *>(QStringLiteral("currentFileLabel"))->text(),
             QStringLiteral("renamed.json"));
    QVERIFY(editor_->lexer());
    QCOMPARE(QByteArray(editor_->lexer()->language()), QByteArray("JSON"));

    QFile savedFile(saveAsPath);
    QVERIFY(savedFile.open(QIODevice::ReadOnly));
    QCOMPARE(savedFile.readAll(), QByteArray("{\"ok\": true}\n"));
}

void EditorTests::failedSaveKeepsTheDocumentModified()
{
    const QString originalPath = createFile(QStringLiteral("failure.txt"), "before");
    QVERIFY(activateFile(originalPath));
    editor_->append(QStringLiteral(" after"));
    QVERIFY(editor_->isModified());

    const QString invalidPath =
        temporaryDirectory_.filePath(QStringLiteral("missing/target.txt"));
    QTimer::singleShot(0, []() {
        auto *message = qobject_cast<QMessageBox *>(QApplication::activeModalWidget());
        if (message)
            message->accept();
    });
    QVERIFY(!panel_->saveCurrentFileAs(invalidPath));
    QVERIFY(editor_->isModified());
    QVERIFY(QFileInfo::exists(originalPath));
}

void EditorTests::switchingFilesReplacesTheActiveDocument()
{
    const QString pythonPath = createFile(QStringLiteral("module.py"),
                                          "print('first')\n");
    const QString yamlPath = createFile(QStringLiteral("config.yml"),
                                        "key: value\n");
    QVERIFY(activateFile(pythonPath));
    QCOMPARE(QByteArray(editor_->lexer()->language()), QByteArray("Python"));
    QCOMPARE(editor_->text(), QStringLiteral("print('first')\n"));

    QVERIFY(activateFile(yamlPath));
    QCOMPARE(QByteArray(editor_->lexer()->language()), QByteArray("YAML"));
    QCOMPARE(editor_->text(), QStringLiteral("key: value\n"));
}

void EditorTests::splitPanesSaveIndependentFilesAndClose()
{
    const QString firstPath = createFile(QStringLiteral("first.txt"), "first");
    const QString secondPath = createFile(QStringLiteral("second.txt"), "second");
    QVERIFY(activateFile(firstPath));

    auto *splitVertical =
        panel_->findChild<QAction *>(QStringLiteral("splitVerticallyAction"));
    auto *splitHorizontal =
        panel_->findChild<QAction *>(QStringLiteral("splitHorizontallyAction"));
    auto *closeSplit = panel_->findChild<QAction *>(QStringLiteral("closeSplitAction"));
    QVERIFY(splitVertical && splitHorizontal && closeSplit);
    splitVertical->trigger();

    auto *splitter = panel_->findChild<QSplitter *>(QStringLiteral("splitEditorSplitter"));
    auto *secondEditor =
        panel_->findChild<QsciScintilla *>(QStringLiteral("secondarySourceEditor"));
    QVERIFY(splitter && secondEditor);
    QCOMPARE(splitter->orientation(), Qt::Horizontal);
    panel_->resize(800, 600);
    panel_->show();
    QCoreApplication::processEvents();
    splitter->setSizes({300, 500});
    QCoreApplication::processEvents();
    const QList<int> paneSizes = splitter->sizes();
    QCOMPARE(paneSizes.size(), 2);
    QVERIFY(paneSizes.at(0) > 0);
    QVERIFY(paneSizes.at(1) > 0);
    QVERIFY(activateFile(secondPath));
    secondEditor->insertAt(QStringLiteral(" updated"), 0, 6);
    auto *undo = panel_->findChild<QAction *>(QStringLiteral("undoAction"));
    auto *redo = panel_->findChild<QAction *>(QStringLiteral("redoAction"));
    QVERIFY(undo && redo);
    undo->trigger();
    QCOMPARE(secondEditor->text(), QStringLiteral("second"));
    redo->trigger();
    QCOMPARE(secondEditor->text(), QStringLiteral("second updated"));
    QCOMPARE(editor_->text(), QStringLiteral("first"));
    QVERIFY(panel_->saveCurrentFile());

    QFocusEvent focusPrimary(QEvent::FocusIn);
    QApplication::sendEvent(editor_, &focusPrimary);
    editor_->append(QStringLiteral(" updated"));
    QVERIFY(panel_->saveCurrentFile());

    QFile firstFile(firstPath);
    QFile secondFile(secondPath);
    QVERIFY(firstFile.open(QIODevice::ReadOnly));
    QVERIFY(secondFile.open(QIODevice::ReadOnly));
    QCOMPARE(firstFile.readAll(), QByteArray("first updated"));
    QCOMPARE(secondFile.readAll(), QByteArray("second updated"));

    splitHorizontal->trigger();
    QCOMPARE(splitter->orientation(), Qt::Vertical);
    closeSplit->trigger();
    QVERIFY(!panel_->findChild<QsciScintilla *>(
        QStringLiteral("secondarySourceEditor")));
    QCOMPARE(editor_->text(), QStringLiteral("first updated"));
    editor_->append(QStringLiteral(" still works"));
    QVERIFY(panel_->saveCurrentFile());
}

void EditorTests::duplicateFileViewsShareDocument()
{
    const QString path = createFile(QStringLiteral("shared.txt"), "shared");
    QVERIFY(activateFile(path));
    panel_->findChild<QAction *>(QStringLiteral("splitVerticallyAction"))->trigger();

    auto *secondEditor =
        panel_->findChild<QsciScintilla *>(QStringLiteral("secondarySourceEditor"));
    QVERIFY(secondEditor);
    QVERIFY(activateFile(path));
    QCOMPARE(secondEditor->text(), QStringLiteral("shared"));
    secondEditor->insertAt(QStringLiteral(" changed"), 0, 6);
    QCOMPARE(editor_->text(), QStringLiteral("shared changed"));
    QVERIFY(editor_->isModified());

    auto *undo = panel_->findChild<QAction *>(QStringLiteral("undoAction"));
    auto *redo = panel_->findChild<QAction *>(QStringLiteral("redoAction"));
    QVERIFY(undo && redo);
    undo->trigger();
    QCOMPARE(editor_->text(), QStringLiteral("shared"));
    QCOMPARE(secondEditor->text(), QStringLiteral("shared"));
    redo->trigger();
    QCOMPARE(editor_->text(), QStringLiteral("shared changed"));
    QVERIFY(panel_->saveCurrentFile());

    QFile file(path);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), QByteArray("shared changed"));

    const QString otherPath = createFile(QStringLiteral("other.txt"), "other");
    QVERIFY(activateFile(otherPath));
    QCOMPARE(editor_->text(), QStringLiteral("shared changed"));
    QCOMPARE(secondEditor->text(), QStringLiteral("other"));
    secondEditor->insertAt(QStringLiteral(" independent"), 0, 5);
    QVERIFY(panel_->saveCurrentFile());
    QCOMPARE(editor_->text(), QStringLiteral("shared changed"));

    panel_->findChild<QAction *>(QStringLiteral("closeSplitAction"))->trigger();
    QCOMPARE(editor_->text(), QStringLiteral("shared changed"));
}

QTEST_MAIN(EditorTests)

#include "test_editor.moc"
