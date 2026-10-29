#pragma once

#include <QColor>
#include <QString>

struct EditorPalette
{
    QColor background;
    QColor secondaryBackground;
    QColor foreground;
    QColor secondaryText;
    QColor mutedText;
    QColor amber;
    QColor brightAccent;
    QColor selection;
    QColor currentLine;
    QColor keyword;
    QColor string;
    QColor number;
    QColor comment;
    QColor function;
    QColor type;
    QColor operation;
    QColor preprocessor;
    QColor constant;
    QColor escape;
    QColor documentation;
};

inline EditorPalette editorPaletteForId(const QString &id)
{
    if (id == QStringLiteral("glass-light")) {
        return {
            QColor(QStringLiteral("#f7fafb")), QColor(QStringLiteral("#e8f0f3")),
            QColor(QStringLiteral("#20313a")), QColor(QStringLiteral("#526873")),
            QColor(QStringLiteral("#788990")), QColor(QStringLiteral("#027f93")),
            QColor(QStringLiteral("#153a42")), QColor(QStringLiteral("#c6e7eb")),
            QColor(QStringLiteral("#eaf3f5")), QColor(QStringLiteral("#9b4f00")),
            QColor(QStringLiteral("#617d16")), QColor(QStringLiteral("#8b4db2")),
            QColor(QStringLiteral("#73838a")), QColor(QStringLiteral("#744b21")),
            QColor(QStringLiteral("#153a42")), QColor(QStringLiteral("#027f93")),
            QColor(QStringLiteral("#a30502")), QColor(QStringLiteral("#8b4db2")),
            QColor(QStringLiteral("#b45f00")), QColor(QStringLiteral("#718078"))
        };
    }
    if (id == QStringLiteral("glass-dark")) {
        return {
            QColor(QStringLiteral("#101820")), QColor(QStringLiteral("#18272f")),
            QColor(QStringLiteral("#dce8eb")), QColor(QStringLiteral("#a4b7bd")),
            QColor(QStringLiteral("#72878d")), QColor(QStringLiteral("#027f93")),
            QColor(QStringLiteral("#62c3ce")), QColor(QStringLiteral("#25434b")),
            QColor(QStringLiteral("#16242b")), QColor(QStringLiteral("#f78b04")),
            QColor(QStringLiteral("#b7d28f")), QColor(QStringLiteral("#dfb184")),
            QColor(QStringLiteral("#82959a")), QColor(QStringLiteral("#e4c9a6")),
            QColor(QStringLiteral("#70c1c8")), QColor(QStringLiteral("#8be0dc")),
            QColor(QStringLiteral("#d98263")), QColor(QStringLiteral("#d8ad77")),
            QColor(QStringLiteral("#f4c27a")), QColor(QStringLiteral("#8e9a8b"))
        };
    }
    return {
        QColor(QStringLiteral("#021324")), QColor(QStringLiteral("#00172f")),
        QColor(QStringLiteral("#87fbc8")), QColor(QStringLiteral("#858783")),
        QColor(QStringLiteral("#626560")), QColor(QStringLiteral("#ff7b00")),
        QColor(QStringLiteral("#f0a65a")), QColor(QStringLiteral("#493426")),
        QColor(QStringLiteral("#1b1d1e")), QColor(QStringLiteral("#ff7700")),
        QColor(QStringLiteral("#a9b987")), QColor(QStringLiteral("#c99b74")),
        QColor(QStringLiteral("#777b76")), QColor(QStringLiteral("#d8c6a8")),
        QColor(QStringLiteral("#d3a16f")), QColor(QStringLiteral("#9df1df")),
        QColor(QStringLiteral("#c18a66")), QColor(QStringLiteral("#c6a17a")),
        QColor(QStringLiteral("#e3b878")), QColor(QStringLiteral("#858d7c"))
    };
}
