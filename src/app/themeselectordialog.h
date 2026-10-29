#pragma once

#include <QDialog>
#include <QString>

class QComboBox;

class ThemeSelectorDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit ThemeSelectorDialog(const QString &applicationTheme,
                                 const QString &editorTheme,
                                 QWidget *parent = nullptr);

signals:
    void applicationThemeSelected(const QString &id);
    void editorThemeSelected(const QString &id);

private:
    void updateSelectedCard();

    QString selectedApplicationTheme_;
    QComboBox *editorThemeCombo_ = nullptr;
};
