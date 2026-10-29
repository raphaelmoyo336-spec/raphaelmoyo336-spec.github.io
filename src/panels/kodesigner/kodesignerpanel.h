#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <QVector>
#include <QWidget>

class QAction;
class QLabel;
class QTabBar;
class QWebChannel;
class QWebEngineView;

class KoDesignerBridge final : public QObject
{
    Q_OBJECT

public:
    explicit KoDesignerBridge(QObject *parent = nullptr);

public slots:
    void documentChanged();
    void designerReady(const QString &projectData);

signals:
    void changed();
    void ready(const QString &projectData);
};

class KoDesignerPanel final : public QWidget
{
    Q_OBJECT

public:
    explicit KoDesignerPanel(QWidget *parent = nullptr);
    ~KoDesignerPanel() override;

    QAction *newDocumentAction() const;
    QAction *openDocumentAction() const;
    QAction *importHtmlAction() const;
    QAction *saveDocumentAction() const;
    QAction *saveDocumentAsAction() const;
    QAction *saveAllDocumentsAction() const;
    QAction *exportHtmlAction() const;
    QAction *resetDocumentAction() const;
    QAction *closeDocumentAction() const;
    QAction *closeAllDocumentsAction() const;

    void setProjectDirectory(const QString &directory);
    bool openFilePath(const QString &filePath);
    bool saveCurrentDocument();
    bool saveCurrentDocumentAs(const QString &filePath);
    bool exportCurrentDocumentAs(const QString &filePath);
    bool saveAllDocuments();
    bool maybeSaveAllDocuments();
    void newDocument();
    void resetCurrentDocument();
    void handleFileRenamed(const QString &oldPath, const QString &newPath);

private:
    struct DesignDocument
    {
        QString id;
        QString filePath;
        QString displayName;
        QString projectData;
        QString savedProjectData;
        bool dirty = false;
        bool newlyCreated = true;
    };

    QString captureProjectData();
    QString exportProjectHtml(const QString &projectData);
    QString runJavaScriptAndWait(const QString &script);
    bool loadProjectData(const QString &projectData);
    bool writeProjectData(DesignDocument *document, const QString &path);
    bool importHtmlFile(const QString &absolutePath);
    bool validateProjectData(const QByteArray &contents, QString *projectData) const;
    DesignDocument *documentForTab(int index) const;
    DesignDocument *documentForPath(const QString &absolutePath) const;
    int tabIndexForDocument(const DesignDocument *document) const;
    void activateDocument(DesignDocument *document);
    void updateDocumentTab(DesignDocument *document);
    void updateDocumentTitle();
    void createActions();
    void openDocumentDialog();
    void importHtmlDialog();
    void exportHtmlDialog();
    bool saveDocumentAsDialog(DesignDocument *document);
    bool confirmSaveDocument(DesignDocument *document);
    void discardDocumentChanges(DesignDocument *document);
    bool closeDocument(DesignDocument *document);
    void closeAllDocuments();

    QLabel *loadError_ = nullptr;
    QTabBar *documentTabs_ = nullptr;
    QWebEngineView *webView_ = nullptr;
    QWebChannel *webChannel_ = nullptr;
    KoDesignerBridge *bridge_ = nullptr;
    QAction *newDocumentAction_ = nullptr;
    QAction *openDocumentAction_ = nullptr;
    QAction *importHtmlAction_ = nullptr;
    QAction *saveDocumentAction_ = nullptr;
    QAction *saveDocumentAsAction_ = nullptr;
    QAction *saveAllDocumentsAction_ = nullptr;
    QAction *exportHtmlAction_ = nullptr;
    QAction *resetDocumentAction_ = nullptr;
    QAction *closeDocumentAction_ = nullptr;
    QAction *closeAllDocumentsAction_ = nullptr;
    QString projectDirectory_;
    QVector<DesignDocument *> documents_;
    QHash<QString, DesignDocument *> documentsByPath_;
    DesignDocument *activeDocument_ = nullptr;
    quint64 nextDocumentId_ = 1;
    bool suppressTabChange_ = false;
    bool loadingProject_ = false;
};
