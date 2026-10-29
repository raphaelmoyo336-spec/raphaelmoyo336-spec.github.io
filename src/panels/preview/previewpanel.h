#pragma once

#include <QWidget>

class QLineEdit;
class QWebEngineView;

class PreviewPanel final : public QWidget
{
public:
    explicit PreviewPanel(QWidget *parent = nullptr);

private:
    void openUrl();

    QLineEdit *urlInput_ = nullptr;
    QWebEngineView *webView_ = nullptr;
};
