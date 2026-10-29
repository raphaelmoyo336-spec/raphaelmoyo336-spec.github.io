#include "kodesignerpanel.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QKeySequence>
#include <QLabel>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QSaveFile>
#include <QTabBar>
#include <QTimer>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QRegularExpression>

#include <algorithm>

KoDesignerBridge::KoDesignerBridge(QObject *parent)
    : QObject(parent)
{
}

void KoDesignerBridge::documentChanged()
{
    emit changed();
}

void KoDesignerBridge::designerReady(const QString &projectData)
{
    emit ready(projectData);
}

KoDesignerPanel::KoDesignerPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *toolbar = new QToolBar(QStringLiteral("Design Documents"), this);
    toolbar->setObjectName(QStringLiteral("koDesignerToolbar"));
    toolbar->setMovable(false);
    layout->addWidget(toolbar);

    documentTabs_ = new QTabBar(this);
    documentTabs_->setObjectName(QStringLiteral("koDesignerDocumentTabs"));
    documentTabs_->setTabsClosable(true);
    documentTabs_->setMovable(true);
    layout->addWidget(documentTabs_);

    loadError_ = new QLabel(
        QStringLiteral("KoDesigner could not load its bundled designer assets."),
        this);
    loadError_->setObjectName(QStringLiteral("koDesignerLoadError"));
    loadError_->setWordWrap(true);
    loadError_->setStyleSheet(
        QStringLiteral("QLabel { background: #5a2828; color: #ffffff; padding: 8px; }"));
    loadError_->hide();
    layout->addWidget(loadError_);

    webView_ = new QWebEngineView(this);
    webView_->setObjectName(QStringLiteral("koDesignerWebView"));
    webView_->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    webView_->setProperty("koDesignerLoadFinished", false);
    webView_->setProperty("koDesignerLoadSucceeded", false);
    layout->addWidget(webView_);

    webChannel_ = new QWebChannel(this);
    bridge_ = new KoDesignerBridge(this);
    webChannel_->registerObject(QStringLiteral("koDesignerBridge"), bridge_);
    webView_->page()->setWebChannel(webChannel_);
    connect(bridge_, &KoDesignerBridge::changed, this, [this]() {
        if (loadingProject_ || !activeDocument_)
            return;
        activeDocument_->dirty = true;
        updateDocumentTab(activeDocument_);
        saveAllDocumentsAction_->setEnabled(true);
        updateDocumentTitle();
    });
    connect(bridge_, &KoDesignerBridge::ready, this, [this](const QString &data) {
        if (activeDocument_)
            return;
        QString initialData;
        if (!validateProjectData(data.toUtf8(), &initialData)) {
            QMessageBox::warning(this, QStringLiteral("Unable to initialize koDesigner"),
                                 QStringLiteral("KoDesigner could not read its initial "
                                                "project data."));
            return;
        }
        auto *document = new DesignDocument;
        document->id = QString::number(nextDocumentId_++);
        document->displayName = QStringLiteral("Untitled");
        document->projectData = initialData;
        document->savedProjectData = initialData;
        documents_.append(document);
        const int index = documentTabs_->addTab(document->displayName);
        documentTabs_->setTabData(index, document->id);
        activeDocument_ = document;
        documentTabs_->setCurrentIndex(index);
        updateDocumentTitle();
    });

    createActions();
    connect(documentTabs_, &QTabBar::currentChanged, this, [this](int index) {
        if (suppressTabChange_)
            return;
        DesignDocument *document = documentForTab(index);
        if (!document || document == activeDocument_)
            return;
        DesignDocument *previous = activeDocument_;
        activateDocument(document);
        if (activeDocument_ != document) {
            suppressTabChange_ = true;
            documentTabs_->setCurrentIndex(tabIndexForDocument(previous));
            suppressTabChange_ = false;
        }
    });
    connect(documentTabs_, &QTabBar::tabCloseRequested, this, [this](int index) {
        closeDocument(documentForTab(index));
    });

    connect(webView_, &QWebEngineView::loadFinished, this, [this](bool succeeded) {
        webView_->setProperty("koDesignerLoadFinished", true);
        webView_->setProperty("koDesignerLoadSucceeded", succeeded);
        loadError_->setVisible(!succeeded);
        if (!succeeded)
            qWarning() << "KoDesigner failed to load" << webView_->url();
    });

    webView_->load(QUrl(QStringLiteral("qrc:/kodesigner/index.html")));
}

KoDesignerPanel::~KoDesignerPanel()
{
    qDeleteAll(documents_);
}

void KoDesignerPanel::createActions()
{
    newDocumentAction_ = new QAction(QStringLiteral("New Design"), this);
    newDocumentAction_->setObjectName(QStringLiteral("koDesignerNewAction"));
    connect(newDocumentAction_, &QAction::triggered, this, &KoDesignerPanel::newDocument);

    openDocumentAction_ = new QAction(QStringLiteral("Open Design..."), this);
    openDocumentAction_->setObjectName(QStringLiteral("koDesignerOpenAction"));
    connect(openDocumentAction_, &QAction::triggered, this, &KoDesignerPanel::openDocumentDialog);

    importHtmlAction_ = new QAction(QStringLiteral("Import HTML..."), this);
    importHtmlAction_->setObjectName(QStringLiteral("koDesignerImportHtmlAction"));
    connect(importHtmlAction_, &QAction::triggered, this, &KoDesignerPanel::importHtmlDialog);

    saveDocumentAction_ = new QAction(QStringLiteral("Save Design"), this);
    saveDocumentAction_->setObjectName(QStringLiteral("koDesignerSaveAction"));
    saveDocumentAction_->setShortcut(QKeySequence::Save);
    saveDocumentAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(saveDocumentAction_, &QAction::triggered, this, [this]() {
        saveCurrentDocument();
    });

    saveDocumentAsAction_ = new QAction(QStringLiteral("Save Design As..."), this);
    saveDocumentAsAction_->setObjectName(QStringLiteral("koDesignerSaveAsAction"));
    saveDocumentAsAction_->setShortcut(QKeySequence::SaveAs);
    saveDocumentAsAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(saveDocumentAsAction_, &QAction::triggered, this, [this]() {
        if (activeDocument_)
            saveDocumentAsDialog(activeDocument_);
    });

    saveAllDocumentsAction_ = new QAction(QStringLiteral("Save All Designs"), this);
    saveAllDocumentsAction_->setObjectName(QStringLiteral("koDesignerSaveAllAction"));
    saveAllDocumentsAction_->setShortcut(QKeySequence(QStringLiteral("Ctrl+Alt+S")));
    saveAllDocumentsAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    saveAllDocumentsAction_->setEnabled(false);
    connect(saveAllDocumentsAction_, &QAction::triggered, this, [this]() {
        saveAllDocuments();
    });

    exportHtmlAction_ = new QAction(QStringLiteral("Export HTML..."), this);
    exportHtmlAction_->setObjectName(QStringLiteral("koDesignerExportHtmlAction"));
    connect(exportHtmlAction_, &QAction::triggered, this, &KoDesignerPanel::exportHtmlDialog);

    resetDocumentAction_ = new QAction(QStringLiteral("Reset Current Design"), this);
    resetDocumentAction_->setObjectName(QStringLiteral("koDesignerResetAction"));
    connect(resetDocumentAction_, &QAction::triggered,
            this, &KoDesignerPanel::resetCurrentDocument);

    closeDocumentAction_ = new QAction(QStringLiteral("Close Design"), this);
    closeDocumentAction_->setObjectName(QStringLiteral("koDesignerCloseAction"));
    closeDocumentAction_->setShortcut(QKeySequence(QStringLiteral("Ctrl+W")));
    closeDocumentAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(closeDocumentAction_, &QAction::triggered, this, [this]() {
        closeDocument(activeDocument_);
    });
    closeAllDocumentsAction_ = new QAction(QStringLiteral("Close All Designs"), this);
    closeAllDocumentsAction_->setObjectName(QStringLiteral("koDesignerCloseAllAction"));
    connect(closeAllDocumentsAction_, &QAction::triggered,
            this, &KoDesignerPanel::closeAllDocuments);

    auto *toolbar = findChild<QToolBar *>(QStringLiteral("koDesignerToolbar"));
    toolbar->addAction(newDocumentAction_);
    toolbar->addAction(openDocumentAction_);
    toolbar->addAction(importHtmlAction_);
    toolbar->addSeparator();
    toolbar->addAction(saveDocumentAction_);
    toolbar->addAction(saveDocumentAsAction_);
    toolbar->addAction(saveAllDocumentsAction_);
    toolbar->addAction(exportHtmlAction_);
    toolbar->addAction(resetDocumentAction_);
}

QAction *KoDesignerPanel::newDocumentAction() const
{
    return newDocumentAction_;
}

QAction *KoDesignerPanel::openDocumentAction() const
{
    return openDocumentAction_;
}

QAction *KoDesignerPanel::importHtmlAction() const
{
    return importHtmlAction_;
}

QAction *KoDesignerPanel::saveDocumentAction() const
{
    return saveDocumentAction_;
}

QAction *KoDesignerPanel::saveDocumentAsAction() const
{
    return saveDocumentAsAction_;
}

QAction *KoDesignerPanel::saveAllDocumentsAction() const
{
    return saveAllDocumentsAction_;
}

QAction *KoDesignerPanel::exportHtmlAction() const
{
    return exportHtmlAction_;
}

QAction *KoDesignerPanel::resetDocumentAction() const
{
    return resetDocumentAction_;
}

QAction *KoDesignerPanel::closeDocumentAction() const
{
    return closeDocumentAction_;
}

QAction *KoDesignerPanel::closeAllDocumentsAction() const
{
    return closeAllDocumentsAction_;
}

void KoDesignerPanel::setProjectDirectory(const QString &directory)
{
    const QFileInfo info(directory);
    projectDirectory_ = info.isDir() ? info.absoluteFilePath() : QString();
}

QString KoDesignerPanel::runJavaScriptAndWait(const QString &script)
{
    QEventLoop loop;
    QTimer timeout;
    timeout.setSingleShot(true);
    QString result;
    bool completed = false;
    connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);

    timeout.start(10000);
    webView_->page()->runJavaScript(script, [&loop, &result, &completed](const QVariant &value) {
        result = value.toString();
        completed = true;
        loop.quit();
    });
    if (!completed)
        loop.exec();
    if (!completed)
        qWarning() << "Timed out waiting for koDesigner JavaScript";
    return completed ? result : QString();
}

QString KoDesignerPanel::captureProjectData()
{
    const QString data = runJavaScriptAndWait(
        QStringLiteral("window.koDesignerGetProjectData ? "
                       "window.koDesignerGetProjectData() : ''"));
    QString validatedData;
    if (data.isEmpty() || !validateProjectData(data.toUtf8(), &validatedData))
        return QString();
    return validatedData;
}

QString KoDesignerPanel::exportProjectHtml(const QString &projectData)
{
    const QByteArray encodedProject = QJsonDocument(
        QJsonArray{QJsonValue(projectData)}).toJson(QJsonDocument::Compact);
    return runJavaScriptAndWait(
        QStringLiteral("window.koDesignerExportProjectHtml"
                       "(JSON.parse((%1)[0]))")
            .arg(QString::fromUtf8(encodedProject)));
}

bool KoDesignerPanel::validateProjectData(const QByteArray &contents,
                                          QString *projectData) const
{
    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(contents, &error);
    if (error.error != QJsonParseError::NoError || !document.isObject()
        || !document.object().value(QStringLiteral("pages")).isArray())
        return false;
    if (projectData)
        *projectData = QString::fromUtf8(document.toJson(QJsonDocument::Indented));
    return true;
}

bool KoDesignerPanel::loadProjectData(const QString &projectData)
{
    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(projectData.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError || !document.isObject())
        return false;
    const QString serialized = QString::fromUtf8(
        QJsonValue(projectData).toJson(QJsonDocument::Compact));
    loadingProject_ = true;
    const QString result = runJavaScriptAndWait(
        QStringLiteral("window.koDesignerLoadProject(JSON.parse(%1)); 'ok'")
            .arg(serialized));
    loadingProject_ = false;
    return result == QStringLiteral("ok");
}

KoDesignerPanel::DesignDocument *KoDesignerPanel::documentForTab(int index) const
{
    if (index < 0 || index >= documentTabs_->count())
        return nullptr;
    const QString id = documentTabs_->tabData(index).toString();
    for (DesignDocument *document : documents_) {
        if (document->id == id)
            return document;
    }
    return nullptr;
}

KoDesignerPanel::DesignDocument *KoDesignerPanel::documentForPath(
    const QString &absolutePath) const
{
    return documentsByPath_.value(absolutePath, nullptr);
}

int KoDesignerPanel::tabIndexForDocument(const DesignDocument *document) const
{
    if (!document)
        return -1;
    for (int index = 0; index < documentTabs_->count(); ++index) {
        if (documentTabs_->tabData(index).toString() == document->id)
            return index;
    }
    return -1;
}

void KoDesignerPanel::activateDocument(DesignDocument *document)
{
    if (!document || document == activeDocument_)
        return;
    if (activeDocument_) {
        const QString currentData = captureProjectData();
        if (currentData.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Unable to switch design"),
                                 QStringLiteral("KoDesigner could not read the current "
                                                "design data."));
            return;
        }
        activeDocument_->projectData = currentData;
    }
    if (!loadProjectData(document->projectData)) {
        QMessageBox::warning(this, QStringLiteral("Unable to load design"),
                             QStringLiteral("KoDesigner could not load this project file."));
        return;
    }
    activeDocument_ = document;
    suppressTabChange_ = true;
    documentTabs_->setCurrentIndex(tabIndexForDocument(document));
    suppressTabChange_ = false;
    updateDocumentTitle();
}

void KoDesignerPanel::updateDocumentTab(DesignDocument *document)
{
    const int index = tabIndexForDocument(document);
    if (index < 0)
        return;
    documentTabs_->setTabText(
        index, document->dirty
            ? QStringLiteral("* %1").arg(document->displayName)
            : document->displayName);
    documentTabs_->setTabToolTip(index, document->filePath);
}

void KoDesignerPanel::updateDocumentTitle()
{
    if (!activeDocument_)
        return;
    const QString title = activeDocument_->dirty
        ? QStringLiteral("* %1").arg(activeDocument_->displayName)
        : activeDocument_->displayName;
    webView_->setToolTip(activeDocument_->filePath);
    setWindowTitle(title);
}

void KoDesignerPanel::openDocumentDialog()
{
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Open Design"),
        projectDirectory_.isEmpty() ? QDir::homePath() : projectDirectory_,
        QStringLiteral("Design files (*.json *.html *.htm);;"
                       "GrapesJS project files (*.json);;HTML files (*.html *.htm);;"
                       "All files (*)"));
    if (!path.isEmpty())
        openFilePath(path);
}

void KoDesignerPanel::importHtmlDialog()
{
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Import HTML Design"),
        projectDirectory_.isEmpty() ? QDir::homePath() : projectDirectory_,
        QStringLiteral("HTML files (*.html *.htm);;All files (*)"));
    if (!path.isEmpty())
        openFilePath(path);
}

void KoDesignerPanel::exportHtmlDialog()
{
    if (!activeDocument_)
        return;
    QString suggestion = activeDocument_->filePath;
    if (suggestion.isEmpty() || QFileInfo(suggestion).suffix().compare(
                                    QStringLiteral("html"), Qt::CaseInsensitive) != 0) {
        suggestion = QDir(projectDirectory_.isEmpty() ? QDir::homePath()
                                                       : projectDirectory_)
                         .filePath(activeDocument_->displayName
                                   + QStringLiteral(".html"));
    }
    const QString path = QFileDialog::getSaveFileName(
        this, QStringLiteral("Export Design as HTML"), suggestion,
        QStringLiteral("HTML files (*.html)"));
    if (!path.isEmpty())
        exportCurrentDocumentAs(path);
}

bool KoDesignerPanel::openFilePath(const QString &filePath)
{
    const QString absolutePath = QFileInfo(filePath).absoluteFilePath();
    const QString suffix = QFileInfo(absolutePath).suffix().toLower();
    if (suffix == QStringLiteral("html") || suffix == QStringLiteral("htm"))
        return importHtmlFile(absolutePath);
    if (suffix != QStringLiteral("json")) {
        QMessageBox::warning(this, QStringLiteral("Unsupported design file"),
                             QStringLiteral("KoDesigner opens GrapesJS project JSON "
                                            "or HTML files."));
        return false;
    }
    if (DesignDocument *existing = documentForPath(absolutePath)) {
        if (existing != activeDocument_)
            activateDocument(existing);
        return existing == activeDocument_;
    }

    QFile file(absolutePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, QStringLiteral("Unable to open design"),
                             QStringLiteral("Could not open %1:\n%2")
                                 .arg(absolutePath, file.errorString()));
        return false;
    }
    const QByteArray contents = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        QMessageBox::warning(this, QStringLiteral("Unable to read design"),
                             QStringLiteral("Could not read %1:\n%2")
                                 .arg(absolutePath, file.errorString()));
        return false;
    }

    QString projectData;
    if (!validateProjectData(contents, &projectData)) {
        QMessageBox::warning(
            this, QStringLiteral("Unsupported design file"),
            QStringLiteral("%1 is not a valid GrapesJS project JSON file.")
                .arg(absolutePath));
        return false;
    }
    if (activeDocument_) {
        const QString currentData = captureProjectData();
        if (currentData.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Unable to open design"),
                                 QStringLiteral("KoDesigner could not read the current "
                                                "design data."));
            return false;
        }
        activeDocument_->projectData = currentData;
    }

    auto *document = new DesignDocument;
    document->id = QString::number(nextDocumentId_++);
    document->filePath = absolutePath;
    document->displayName = QFileInfo(absolutePath).fileName();
    document->projectData = projectData;
    document->savedProjectData = projectData;
    document->newlyCreated = false;
    documents_.append(document);
    documentsByPath_.insert(absolutePath, document);
    const int index = documentTabs_->addTab(document->displayName);
    documentTabs_->setTabData(index, document->id);
    documentTabs_->setTabToolTip(index, absolutePath);
    activateDocument(document);
    if (activeDocument_ != document) {
        documentsByPath_.remove(absolutePath);
        documents_.removeOne(document);
        documentTabs_->removeTab(index);
        delete document;
        return false;
    }
    saveAllDocumentsAction_->setEnabled(
        std::any_of(documents_.cbegin(), documents_.cend(),
                    [](const DesignDocument *item) { return item->dirty; }));
    return true;
}

bool KoDesignerPanel::importHtmlFile(const QString &absolutePath)
{
    if (DesignDocument *existing = documentForPath(absolutePath)) {
        if (existing != activeDocument_)
            activateDocument(existing);
        return existing == activeDocument_;
    }

    QFile file(absolutePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, QStringLiteral("Unable to import HTML"),
                             QStringLiteral("Could not open %1:\n%2")
                                 .arg(absolutePath, file.errorString()));
        return false;
    }
    const QByteArray contents = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        QMessageBox::warning(this, QStringLiteral("Unable to read HTML"),
                             QStringLiteral("Could not read %1:\n%2")
                                 .arg(absolutePath, file.errorString()));
        return false;
    }

    if (activeDocument_) {
        const QString currentData = captureProjectData();
        if (currentData.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Unable to import HTML"),
                                 QStringLiteral("KoDesigner could not read the current "
                                                "design data."));
            return false;
        }
        activeDocument_->projectData = currentData;
    }

    QJsonObject stylesheets;
    QJsonObject assets;
    QJsonObject assetReferences;
    QStringList missingStylesheets;
    QStringList missingAssets;
    const QString html = QString::fromUtf8(contents);
    const QString htmlDirectory = QFileInfo(absolutePath).absolutePath();
    const QRegularExpression tagExpression(
        QStringLiteral("<link\\b[^>]*>"),
        QRegularExpression::CaseInsensitiveOption);
    const QRegularExpression relExpression(
        QStringLiteral("\\brel\\s*=\\s*(?:\"([^\"]*)\"|'([^']*)'|([^\\s>]+))"),
        QRegularExpression::CaseInsensitiveOption);
    const QRegularExpression hrefExpression(
        QStringLiteral("\\bhref\\s*=\\s*(?:\"([^\"]*)\"|'([^']*)'|([^\\s>]+))"),
        QRegularExpression::CaseInsensitiveOption);
    const QRegularExpression resourceAttributeExpression(
        QStringLiteral("(?:^|\\s)(?:src|poster)\\s*=\\s*(?:\"([^\"]*)\"|'([^']*)'|([^\\s>]+))"),
        QRegularExpression::CaseInsensitiveOption);
    const QRegularExpression cssUrlExpression(
        QStringLiteral("url\\(\\s*(['\"]?)([^'\")]+)\\1\\s*\\)"),
        QRegularExpression::CaseInsensitiveOption);
    const auto addAsset = [&](const QString &reference, const QString &baseDirectory) {
        const QUrl resourceUrl(reference);
        if (reference.isEmpty() || !resourceUrl.isRelative()
            || reference.startsWith(QStringLiteral("//")))
            return QString();
        const QString resourcePath = resourceUrl.path();
        if (resourcePath.isEmpty())
            return QString();
        const QFileInfo resourceInfo(
            QDir(baseDirectory).absoluteFilePath(resourcePath));
        if (!resourceInfo.isFile()) {
            missingAssets.append(reference);
            return QString();
        }
        QFile assetFile(resourceInfo.absoluteFilePath());
        if (!assetFile.open(QIODevice::ReadOnly)) {
            missingAssets.append(reference);
            return QString();
        }
        const QByteArray data = assetFile.readAll();
        if (assetFile.error() != QFileDevice::NoError) {
            missingAssets.append(reference);
            return QString();
        }
        const QString mimeType = QMimeDatabase()
                                     .mimeTypeForFile(resourceInfo)
                                     .name();
        const QString dataUrl = QStringLiteral("data:%1;base64,%2")
                                    .arg(mimeType,
                                         QString::fromLatin1(data.toBase64()));
        assets.insert(reference, dataUrl);
        const QString exportedPath = QDir(htmlDirectory)
                                         .relativeFilePath(resourceInfo.absoluteFilePath());
        assetReferences.insert(dataUrl, exportedPath);
        return dataUrl;
    };
    const auto rewriteCssAssets = [&](const QString &css, const QString &baseDirectory) {
        QString rewritten = css;
        QVector<QRegularExpressionMatch> matches;
        auto iterator = cssUrlExpression.globalMatch(css);
        while (iterator.hasNext())
            matches.append(iterator.next());
        for (auto match = matches.crbegin(); match != matches.crend(); ++match) {
            const QString dataUrl = addAsset(match->captured(2).trimmed(), baseDirectory);
            if (!dataUrl.isEmpty()) {
                rewritten.replace(match->capturedStart(),
                                  match->capturedLength(),
                                  QStringLiteral("url(\"%1\")").arg(dataUrl));
            }
        }
        return rewritten;
    };
    for (auto match = resourceAttributeExpression.globalMatch(html);
         match.hasNext();) {
        const auto resourceMatch = match.next();
        const QString reference = !resourceMatch.captured(1).isEmpty()
            ? resourceMatch.captured(1)
            : (!resourceMatch.captured(2).isEmpty() ? resourceMatch.captured(2)
                                                    : resourceMatch.captured(3));
        addAsset(reference, htmlDirectory);
    }
    rewriteCssAssets(html, htmlDirectory);

    for (auto tagMatch = tagExpression.globalMatch(html); tagMatch.hasNext();) {
        const QString tag = tagMatch.next().captured();
        const auto relMatch = relExpression.match(tag);
        const QString rel = !relMatch.captured(1).isEmpty()
            ? relMatch.captured(1)
            : (!relMatch.captured(2).isEmpty() ? relMatch.captured(2)
                                               : relMatch.captured(3));
        if (!rel.split(QRegularExpression(QStringLiteral("\\s+")),
                       Qt::SkipEmptyParts)
                 .contains(QStringLiteral("stylesheet"), Qt::CaseInsensitive))
            continue;
        const auto hrefMatch = hrefExpression.match(tag);
        const QString href = !hrefMatch.captured(1).isEmpty()
            ? hrefMatch.captured(1)
            : (!hrefMatch.captured(2).isEmpty() ? hrefMatch.captured(2)
                                                 : hrefMatch.captured(3));
        const QUrl resourceUrl(href);
        if (href.isEmpty() || resourceUrl.isRelative() == false
            || href.startsWith(QStringLiteral("//")))
            continue;

        const QString cssPath = QDir(QFileInfo(absolutePath).absolutePath())
                                    .filePath(resourceUrl.path());
        QFile cssFile(cssPath);
        if (!cssFile.open(QIODevice::ReadOnly)) {
            missingStylesheets.append(href);
            continue;
        }
        const QByteArray cssContents = cssFile.readAll();
        if (cssFile.error() != QFileDevice::NoError) {
            missingStylesheets.append(href);
            continue;
        }
        stylesheets.insert(
            href,
            rewriteCssAssets(QString::fromUtf8(cssContents),
                             QFileInfo(cssPath).absolutePath()));
    }

    const QJsonArray encodedHtml{QJsonValue(html)};
    const QJsonArray encodedBase{
        QJsonValue(QUrl::fromLocalFile(
                       QDir::cleanPath(QFileInfo(absolutePath).absolutePath())
                       + QDir::separator())
                       .toString())};
    loadingProject_ = true;
    const QString importedData = runJavaScriptAndWait(
        QStringLiteral("window.koDesignerImportHtml((%1)[0], %2, (%3)[0], %4, %5)")
            .arg(QString::fromUtf8(
                     QJsonDocument(encodedHtml).toJson(QJsonDocument::Compact)),
                 QString::fromUtf8(
                     QJsonDocument(stylesheets).toJson(QJsonDocument::Compact)),
                 QString::fromUtf8(
                     QJsonDocument(encodedBase).toJson(QJsonDocument::Compact)),
                 QString::fromUtf8(
                     QJsonDocument(assets).toJson(QJsonDocument::Compact)),
                 QString::fromUtf8(
                     QJsonDocument(assetReferences).toJson(QJsonDocument::Compact))));
    loadingProject_ = false;
    QString projectData;
    if (importedData.isEmpty()
        || !validateProjectData(importedData.toUtf8(), &projectData)) {
        if (activeDocument_)
            loadProjectData(activeDocument_->projectData);
        QMessageBox::warning(this, QStringLiteral("Unable to import HTML"),
                             QStringLiteral("KoDesigner could not parse this HTML design."));
        return false;
    }
    if (activeDocument_)
        loadProjectData(activeDocument_->projectData);

    auto *document = new DesignDocument;
    document->id = QString::number(nextDocumentId_++);
    document->filePath = absolutePath;
    document->displayName = QFileInfo(absolutePath).fileName();
    document->projectData = projectData;
    document->savedProjectData = projectData;
    document->newlyCreated = false;
    documents_.append(document);
    documentsByPath_.insert(absolutePath, document);
    const int index = documentTabs_->addTab(document->displayName);
    documentTabs_->setTabData(index, document->id);
    documentTabs_->setTabToolTip(index, absolutePath);
    activateDocument(document);
    if (activeDocument_ != document) {
        documentsByPath_.remove(absolutePath);
        documents_.removeOne(document);
        documentTabs_->removeTab(index);
        delete document;
        return false;
    }
    missingAssets.removeDuplicates();
    if (!missingStylesheets.isEmpty() || !missingAssets.isEmpty()) {
        QStringList unavailableResources = missingStylesheets;
        unavailableResources.append(missingAssets);
        QMessageBox::warning(
            this, QStringLiteral("Some local resources are unavailable"),
            QStringLiteral("These local resources could not be read. Their original "
                           "references were preserved:\n%1")
                .arg(unavailableResources.join(QLatin1Char('\n'))));
    }
    return true;
}

void KoDesignerPanel::newDocument()
{
    if (activeDocument_) {
        const QString currentData = captureProjectData();
        if (currentData.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Unable to create design"),
                                 QStringLiteral("KoDesigner could not read the current "
                                                "design data."));
            return;
        }
        activeDocument_->projectData = currentData;
    }
    loadingProject_ = true;
    const QString projectData = runJavaScriptAndWait(
        QStringLiteral("window.koDesignerNewProject ? "
                       "window.koDesignerNewProject() : ''"));
    loadingProject_ = false;
    QString validatedData;
    if (projectData.isEmpty()
        || !validateProjectData(projectData.toUtf8(), &validatedData)) {
        QMessageBox::warning(this, QStringLiteral("Unable to create design"),
                             QStringLiteral("KoDesigner could not initialize a new design."));
        return;
    }

    auto *document = new DesignDocument;
    document->id = QString::number(nextDocumentId_++);
    document->displayName = QStringLiteral("Untitled");
    document->projectData = validatedData;
    document->savedProjectData = validatedData;
    documents_.append(document);
    suppressTabChange_ = true;
    const int index = documentTabs_->addTab(document->displayName);
    documentTabs_->setTabData(index, document->id);
    activeDocument_ = document;
    documentTabs_->setCurrentIndex(index);
    suppressTabChange_ = false;
    updateDocumentTitle();
}

void KoDesignerPanel::resetCurrentDocument()
{
    if (!activeDocument_)
        return;
    const QMessageBox::StandardButton choice = QMessageBox::warning(
        this, QStringLiteral("Reset Current Design"),
        QStringLiteral("Reset %1 and start with a blank design?\n"
                       "Save changes first, discard them, or cancel.")
            .arg(activeDocument_->displayName),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Cancel);
    if (choice == QMessageBox::Cancel)
        return;
    if (choice == QMessageBox::Save && !saveCurrentDocument())
        return;

    loadingProject_ = true;
    const QString projectData = runJavaScriptAndWait(
        QStringLiteral("window.koDesignerNewProject ? "
                       "window.koDesignerNewProject() : ''"));
    loadingProject_ = false;
    QString validatedData;
    if (projectData.isEmpty()
        || !validateProjectData(projectData.toUtf8(), &validatedData)) {
        QMessageBox::warning(this, QStringLiteral("Unable to reset design"),
                             QStringLiteral("KoDesigner could not initialize a blank design."));
        return;
    }
    activeDocument_->projectData = validatedData;
    activeDocument_->dirty = true;
    updateDocumentTab(activeDocument_);
    saveAllDocumentsAction_->setEnabled(true);
    updateDocumentTitle();
}

bool KoDesignerPanel::writeProjectData(DesignDocument *document, const QString &path)
{
    if (!document)
        return false;
    QString contentsText = document->projectData;
    const QString suffix = QFileInfo(path).suffix().toLower();
    if (suffix == QStringLiteral("html") || suffix == QStringLiteral("htm")) {
        contentsText = exportProjectHtml(document->projectData);
        if (contentsText.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Unable to export design"),
                                 QStringLiteral("KoDesigner could not generate HTML for %1.")
                                     .arg(path));
            return false;
        }
    } else if (suffix != QStringLiteral("json")) {
        QMessageBox::warning(this, QStringLiteral("Unsupported file extension"),
                             QStringLiteral("Design files must use .json, .html, or .htm."));
        return false;
    }

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("Unable to save design"),
                             QStringLiteral("Could not save %1:\n%2")
                                 .arg(path, file.errorString()));
        return false;
    }
    const QByteArray contents = contentsText.toUtf8();
    if (file.write(contents) != contents.size() || !file.commit()) {
        QMessageBox::warning(this, QStringLiteral("Unable to save design"),
                             QStringLiteral("Could not save %1:\n%2")
                                 .arg(path, file.errorString()));
        return false;
    }

    if (!document->filePath.isEmpty())
        documentsByPath_.remove(document->filePath);
    document->filePath = QFileInfo(path).absoluteFilePath();
    document->displayName = QFileInfo(document->filePath).fileName();
    document->savedProjectData = document->projectData;
    document->newlyCreated = false;
    document->dirty = false;
    documentsByPath_.insert(document->filePath, document);
    updateDocumentTab(document);
    updateDocumentTitle();
    saveAllDocumentsAction_->setEnabled(
        std::any_of(documents_.cbegin(), documents_.cend(),
                    [](const DesignDocument *item) { return item->dirty; }));
    return true;
}

bool KoDesignerPanel::saveCurrentDocument()
{
    if (!activeDocument_)
        return false;
    if (activeDocument_->filePath.isEmpty())
        return saveDocumentAsDialog(activeDocument_);

    const QString currentData = captureProjectData();
    if (currentData.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Unable to save design"),
                             QStringLiteral("KoDesigner could not read the current "
                                            "design data."));
        return false;
    }
    activeDocument_->projectData = currentData;
    return writeProjectData(activeDocument_, activeDocument_->filePath);
}

bool KoDesignerPanel::saveCurrentDocumentAs(const QString &filePath)
{
    if (!activeDocument_ || filePath.isEmpty())
        return false;
    QString path = QFileInfo(filePath).absoluteFilePath();
    if (QFileInfo(path).suffix().isEmpty())
        path += QStringLiteral(".json");
    const QString suffix = QFileInfo(path).suffix().toLower();
    if (suffix != QStringLiteral("json") && suffix != QStringLiteral("html")
        && suffix != QStringLiteral("htm")) {
        QMessageBox::warning(this, QStringLiteral("Unsupported file extension"),
                             QStringLiteral("Design files must use .json, .html, or .htm."));
        return false;
    }
    DesignDocument *existing = documentForPath(path);
    if (existing && existing != activeDocument_) {
        QMessageBox::warning(this, QStringLiteral("Design already open"),
                             QStringLiteral("%1 is already open in another tab.")
                                 .arg(path));
        return false;
    }

    const QString currentData = captureProjectData();
    if (currentData.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Unable to save design"),
                             QStringLiteral("KoDesigner could not read the current "
                                            "design data."));
        return false;
    }
    activeDocument_->projectData = currentData;
    return writeProjectData(activeDocument_, path);
}

bool KoDesignerPanel::exportCurrentDocumentAs(const QString &filePath)
{
    if (!activeDocument_ || filePath.isEmpty())
        return false;
    QString path = QFileInfo(filePath).absoluteFilePath();
    if (QFileInfo(path).suffix().isEmpty())
        path += QStringLiteral(".html");
    const QString suffix = QFileInfo(path).suffix().toLower();
    if (suffix != QStringLiteral("html") && suffix != QStringLiteral("htm")) {
        QMessageBox::warning(this, QStringLiteral("Unsupported file extension"),
                             QStringLiteral("HTML exports must use .html or .htm."));
        return false;
    }

    const QString currentData = captureProjectData();
    if (currentData.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Unable to export design"),
                             QStringLiteral("KoDesigner could not read the current "
                                            "design data."));
        return false;
    }
    const QString html = exportProjectHtml(currentData);
    if (html.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Unable to export design"),
                             QStringLiteral("KoDesigner could not generate HTML."));
        return false;
    }

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("Unable to export design"),
                             QStringLiteral("Could not export %1:\n%2")
                                 .arg(path, file.errorString()));
        return false;
    }
    const QByteArray contents = html.toUtf8();
    if (file.write(contents) != contents.size() || !file.commit()) {
        QMessageBox::warning(this, QStringLiteral("Unable to export design"),
                             QStringLiteral("Could not export %1:\n%2")
                                 .arg(path, file.errorString()));
        return false;
    }
    return true;
}

bool KoDesignerPanel::saveDocumentAsDialog(DesignDocument *document)
{
    if (!document)
        return false;
    QString suggestion = document->filePath;
    if (suggestion.isEmpty()) {
        suggestion = projectDirectory_.isEmpty()
            ? QDir::home().filePath(QStringLiteral("Untitled.json"))
            : QDir(projectDirectory_).filePath(QStringLiteral("Untitled.json"));
    }
    const QString path = QFileDialog::getSaveFileName(
        this, QStringLiteral("Save Design As"), suggestion,
        QStringLiteral("GrapesJS project files (*.json);;HTML files (*.html *.htm)"));
    if (path.isEmpty())
        return false;

    QString normalizedPath = QFileInfo(path).absoluteFilePath();
    if (QFileInfo(normalizedPath).suffix().isEmpty())
        normalizedPath += QStringLiteral(".json");
    const QString suffix = QFileInfo(normalizedPath).suffix().toLower();
    if (suffix != QStringLiteral("json") && suffix != QStringLiteral("html")
        && suffix != QStringLiteral("htm")) {
        QMessageBox::warning(this, QStringLiteral("Unsupported file extension"),
                             QStringLiteral("Design files must use .json, .html, or .htm."));
        return false;
    }
    DesignDocument *existing = documentForPath(normalizedPath);
    if (existing && existing != document) {
        QMessageBox::warning(this, QStringLiteral("Design already open"),
                             QStringLiteral("%1 is already open in another tab.")
                                 .arg(normalizedPath));
        return false;
    }
    if (document == activeDocument_) {
        const QString currentData = captureProjectData();
        if (currentData.isEmpty()) {
            QMessageBox::warning(this, QStringLiteral("Unable to save design"),
                                 QStringLiteral("KoDesigner could not read the current "
                                                "design data."));
            return false;
        }
        document->projectData = currentData;
    }
    return writeProjectData(document, normalizedPath);
}

bool KoDesignerPanel::saveAllDocuments()
{
    DesignDocument *previous = activeDocument_;
    for (DesignDocument *document : documents_) {
        if (!document->dirty)
            continue;
        if (document == activeDocument_) {
            const QString currentData = captureProjectData();
            if (currentData.isEmpty()) {
                QMessageBox::warning(this, QStringLiteral("Unable to save design"),
                                     QStringLiteral("KoDesigner could not read the current "
                                                    "design data."));
                return false;
            }
            document->projectData = currentData;
        }
        if (document->filePath.isEmpty()) {
            if (!saveDocumentAsDialog(document))
                return false;
        } else if (!writeProjectData(document, document->filePath)) {
            return false;
        }
    }
    if (previous)
        updateDocumentTitle();
    return true;
}

bool KoDesignerPanel::confirmSaveDocument(DesignDocument *document)
{
    if (!document || !document->dirty)
        return true;
    const QMessageBox::StandardButton choice = QMessageBox::warning(
        this, QStringLiteral("Unsaved Design"),
        QStringLiteral("Save changes to %1?")
            .arg(document->displayName),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);
    if (choice == QMessageBox::Save) {
        if (document == activeDocument_)
            return saveCurrentDocument();
        if (document->filePath.isEmpty()) {
            return saveDocumentAsDialog(document);
        }
        return writeProjectData(document, document->filePath);
    }
    if (choice == QMessageBox::Discard) {
        discardDocumentChanges(document);
        return true;
    }
    return false;
}

void KoDesignerPanel::discardDocumentChanges(DesignDocument *document)
{
    document->projectData = document->savedProjectData;
    document->dirty = false;
    if (document == activeDocument_)
        loadProjectData(document->projectData);
    updateDocumentTab(document);
    saveAllDocumentsAction_->setEnabled(
        std::any_of(documents_.cbegin(), documents_.cend(),
                    [](const DesignDocument *item) { return item->dirty; }));
}

bool KoDesignerPanel::closeDocument(DesignDocument *document)
{
    if (!document || !confirmSaveDocument(document))
        return false;
    const int index = tabIndexForDocument(document);
    const bool wasActive = activeDocument_ == document;
    suppressTabChange_ = true;
    documentTabs_->removeTab(index);
    documents_.removeOne(document);
    if (!document->filePath.isEmpty())
        documentsByPath_.remove(document->filePath);
    delete document;

    DesignDocument *next = documentForTab(documentTabs_->currentIndex());
    if (wasActive) {
        activeDocument_ = nullptr;
        if (next) {
            activeDocument_ = next;
            loadProjectData(next->projectData);
        }
    }
    suppressTabChange_ = false;
    if (!activeDocument_ && !documents_.isEmpty()) {
        activeDocument_ = documents_.first();
        loadProjectData(activeDocument_->projectData);
        documentTabs_->setCurrentIndex(tabIndexForDocument(activeDocument_));
    }
    updateDocumentTitle();
    saveAllDocumentsAction_->setEnabled(
        std::any_of(documents_.cbegin(), documents_.cend(),
                    [](const DesignDocument *item) { return item->dirty; }));
    return true;
}

void KoDesignerPanel::closeAllDocuments()
{
    const QVector<DesignDocument *> documents = documents_;
    for (DesignDocument *document : documents) {
        if (!confirmSaveDocument(document))
            return;
    }
    for (DesignDocument *document : documents_) {
        if (!document->filePath.isEmpty())
            documentsByPath_.remove(document->filePath);
        delete document;
    }
    documents_.clear();
    while (documentTabs_->count() > 0)
        documentTabs_->removeTab(documentTabs_->count() - 1);
    activeDocument_ = nullptr;
    saveAllDocumentsAction_->setEnabled(false);
    updateDocumentTitle();
    newDocument();
}

bool KoDesignerPanel::maybeSaveAllDocuments()
{
    const QVector<DesignDocument *> documents = documents_;
    for (DesignDocument *document : documents) {
        if (!confirmSaveDocument(document))
            return false;
    }
    return true;
}

void KoDesignerPanel::handleFileRenamed(const QString &oldPath,
                                        const QString &newPath)
{
    const QString absoluteOldPath = QFileInfo(oldPath).absoluteFilePath();
    const QString absoluteNewPath = QFileInfo(newPath).absoluteFilePath();
    const bool renamedDirectory = QFileInfo(absoluteNewPath).isDir();
    const QString descendantPrefix = absoluteOldPath + QDir::separator();
    for (DesignDocument *document : documents_) {
        if (document->filePath != absoluteOldPath
            && (!renamedDirectory || !document->filePath.startsWith(descendantPrefix)))
            continue;
        documentsByPath_.remove(document->filePath);
        const QString suffix = document->filePath.mid(absoluteOldPath.size());
        document->filePath = absoluteNewPath + suffix;
        document->displayName = QFileInfo(document->filePath).fileName();
        documentsByPath_.insert(document->filePath, document);
        updateDocumentTab(document);
    }
    updateDocumentTitle();
}
