#pragma once

#include <QWidget>

class QTermWidget;

class TerminalPanel final : public QWidget
{
public:
    explicit TerminalPanel(QWidget *parent = nullptr);

private:
    QTermWidget *terminal_ = nullptr;
};
