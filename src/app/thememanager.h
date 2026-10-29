#pragma once

#include <QColor>
#include <QString>
#include <QVector>

struct ThemeDefinition
{
    QString id;
    QString displayName;
    QColor windowBackground;
    QColor panelBackground;
    QColor surfaceBackground;
    QColor primaryColor;
    QColor accentColor;
    QColor textPrimary;
    QColor textSecondary;
    QColor borderColor;
    bool glassEnabled = false;
    bool light = false;
};

class ThemeManager final
{
public:
    static const QVector<ThemeDefinition> &themes();
    static const ThemeDefinition *theme(const QString &id);
    static QString stylesheet(const ThemeDefinition &theme);
    static void applyPalette(const ThemeDefinition &theme, class QPalette *palette);
};
