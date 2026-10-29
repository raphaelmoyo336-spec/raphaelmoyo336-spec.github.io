#include "previewpanel.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWidget>

PreviewPanel::PreviewPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *container = new QWidget(this);
    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(6);

    auto *urlRow = new QHBoxLayout;
    urlInput_ = new QLineEdit(QStringLiteral("http://localhost:3000"), container);
    urlInput_->setObjectName(QStringLiteral("previewUrlInput"));
    urlInput_->setPlaceholderText(QStringLiteral("Enter a URL"));
    auto *openButton = new QPushButton(QStringLiteral("Open"), container);
    openButton->setObjectName(QStringLiteral("previewOpenButton"));
    auto *reloadButton = new QPushButton(QStringLiteral("Reload"), container);
    reloadButton->setObjectName(QStringLiteral("previewReloadButton"));
    urlRow->addWidget(urlInput_);
    urlRow->addWidget(openButton);
    urlRow->addWidget(reloadButton);
    layout->addLayout(urlRow);

    webView_ = new QWebEngineView(container);
    webView_->setObjectName(QStringLiteral("previewWebView"));
    layout->addWidget(webView_);
    auto *panelLayout = new QVBoxLayout(this);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->addWidget(container);

    connect(openButton, &QPushButton::clicked, this, &PreviewPanel::openUrl);
    connect(urlInput_, &QLineEdit::returnPressed, this, &PreviewPanel::openUrl);
    connect(reloadButton, &QPushButton::clicked, webView_, &QWebEngineView::reload);
    connect(webView_, &QWebEngineView::urlChanged, this, [this](const QUrl &currentUrl) {
        urlInput_->setText(currentUrl.toString());
    });

    webView_->load(QUrl(QStringLiteral("http://localhost:3000")));
}

void PreviewPanel::openUrl()
{
    const QUrl target = QUrl::fromUserInput(urlInput_->text().trimmed());
    if (!target.isValid() || target.isEmpty())
        return;

    webView_->load(target);
    urlInput_->setText(target.toString());
}
