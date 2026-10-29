#include "thememanager.h"

#include <QFile>
#include <QPalette>

namespace {

QColor color(const char *hex)
{
    return QColor(QString::fromLatin1(hex));
}

QString rgba(const QColor &value, int alpha)
{
    return QStringLiteral("rgba(%1, %2, %3, %4)")
        .arg(value.red()).arg(value.green()).arg(value.blue()).arg(alpha);
}

QString glassRules(const ThemeDefinition &theme)
{
    const QColor atmosphere = theme.primaryColor;
    const QColor panel = theme.panelBackground;
    const QColor surface = theme.surfaceBackground;
    const QColor border = theme.borderColor;
    const QColor accent = theme.accentColor;
    const QColor text = theme.textPrimary;
    QString rules = QStringLiteral(R"(
QMainWindow {
    background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,
        stop: 0 {ATMOSPHERE}, stop: 0.48 {PANEL}, stop: 1 {SURFACE});
}
QMenuBar, QToolBar, QStatusBar {
    background: {GLASS_PANEL};
    border-color: {BORDER};
}
QMenu, QComboBox QAbstractItemView {
    background: {GLASS_SURFACE};
    border: 1px solid {BORDER};
    border-radius: 6px;
}
QPushButton, QLineEdit, QComboBox {
    border-radius: 0px;
}
QLineEdit:focus, QComboBox:focus {
    border: 1px solid {ACCENT};
}
QPushButton:hover, QToolButton:hover, QTreeView::item:hover {
    border-color: {ACCENT};
}
ads--CDockContainerWidget, ads--CDockAreaWidget, ads--CDockWidget {
    background: {GLASS_PANEL};
    border-color: {BORDER};
}
ads--CDockAreaTitleBar, ads--CFloatingWidgetTitleBar {
    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
        stop: 0 {GLASS_SURFACE}, stop: 1 {GLASS_PANEL});
    border-bottom: 1px solid {BORDER};
}
ads--CDockAreaWidget[focused="true"] ads--CDockAreaTitleBar,
ads--CDockWidgetTab[activeTab="true"] {
    border-top: 2px solid {ACCENT};
    border-bottom-color: {ACCENT};
}
ads--CDockWidgetTab, ads--CAutoHideTab {
    background: {GLASS_SURFACE};
    border-color: {BORDER};
}
ads--CDockWidgetTab[activeTab="true"], ads--CDockWidgetTab:hover,
ads--CAutoHideTab:hover, ads--CAutoHideTab[activeTab="true"] {
    background: {ACCENT_WASH};
}
ads--CAutoHideDockContainer {
    background: {GLASS_SURFACE};
    border: 1px solid {BORDER};
    border-radius: 8px;
}
QSplitter::handle:hover {
    background: {ACCENT};
}
QToolTip {
    background: {GLASS_SURFACE};
    color: {TEXT};
    border: 1px solid {BORDER};
    border-radius: 5px;
}
)");
    const QVector<QPair<QString, QString>> tokens = {
        {QStringLiteral("{ATMOSPHERE}"), rgba(atmosphere, theme.light ? 22 : 34)},
        {QStringLiteral("{PANEL}"), rgba(panel, theme.light ? 238 : 244)},
        {QStringLiteral("{SURFACE}"), rgba(surface, theme.light ? 246 : 248)},
        {QStringLiteral("{GLASS_PANEL}"), rgba(panel, theme.light ? 232 : 230)},
        {QStringLiteral("{GLASS_SURFACE}"), rgba(surface, theme.light ? 248 : 250)},
        {QStringLiteral("{ACCENT}"), accent.name(QColor::HexRgb)},
        {QStringLiteral("{ACCENT_WASH}"), rgba(accent, theme.light ? 25 : 42)},
        {QStringLiteral("{BORDER}"), border.name(QColor::HexRgb)},
        {QStringLiteral("{TEXT}"), text.name(QColor::HexRgb)}
    };
    for (const auto &token : tokens)
        rules.replace(token.first, token.second);
    return rules;
}

} // namespace

const QVector<ThemeDefinition> &ThemeManager::themes()
{
    static const QVector<ThemeDefinition> definitions = {
        {QStringLiteral("metal"), QStringLiteral("Metal"),
         color("#111315"), color("#202327"), color("#292d32"),
         color("#a96537"), color("#d9954b"), color("#e7e9ec"), color("#a0a7b0"),
         color("#454b52"), false, false},
        {QStringLiteral("glass-light"), QStringLiteral("Glass Light"),
         color("#dce8ee"), color("#f1f5f7"), color("#ffffff"),
         color("#153a42"), color("#027f93"), color("#172a33"), color("#526873"),
         color("#b6c8cf"), true, true},
        {QStringLiteral("glass-dark"), QStringLiteral("Glass Dark"),
         color("#10151b"), color("#1b242c"), color("#26323a"),
         color("#2b1718"), color("#56b9c5"), color("#e8f0f2"), color("#a4b3b9"),
         color("#46565e"), true, false},
        {QStringLiteral("crimson"), QStringLiteral("Crimson"),
         color("#171315"), color("#241b1c"), color("#312426"),
         color("#a30502"), color("#d05c50"), color("#f0e9e9"), color("#b4a3a4"),
         color("#584143"), true, false},
        {QStringLiteral("amber"), QStringLiteral("Amber"),
         color("#1a1713"), color("#28231c"), color("#373027"),
         color("#f78b04"), color("#ffad3d"), color("#f5eee4"), color("#bdb09d"),
         color("#5c4d38"), true, false},
        {QStringLiteral("obsidian"), QStringLiteral("Obsidian"),
         color("#100d0e"), color("#1b1416"), color("#281b1e"),
         color("#2b1718"), color("#a64c4c"), color("#eae2e3"), color("#a99b9d"),
         color("#46383b"), true, false},
        {QStringLiteral("ocean"), QStringLiteral("Ocean"),
         color("#10191d"), color("#17282d"), color("#20363c"),
         color("#153a42"), color("#4a9eaa"), color("#e4eff0"), color("#a2b9bc"),
         color("#3d5960"), true, false},
        {QStringLiteral("cyan"), QStringLiteral("Cyan"),
         color("#10191c"), color("#17272b"), color("#20383d"),
         color("#027f93"), color("#18b5c5"), color("#e3f2f3"), color("#9ebdc1"),
         color("#3c6166"), true, false}
    };
    return definitions;
}

const ThemeDefinition *ThemeManager::theme(const QString &id)
{
    for (const ThemeDefinition &definition : themes()) {
        if (definition.id == id)
            return &definition;
    }
    return nullptr;
}

QString ThemeManager::stylesheet(const ThemeDefinition &theme)
{
    QFile file(QStringLiteral(":/kogence/styles/kogence-metal.qss"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    QString result = QString::fromUtf8(file.readAll());
    if (theme.id == QStringLiteral("metal"))
        return result;

    const QString editorSelector = QStringLiteral("QsciScintilla#sourceEditor");
    const int editorStart = result.indexOf(editorSelector);
    const int editorEnd = editorStart < 0 ? -1 : result.indexOf(QLatin1Char('}'), editorStart);
    const QString editorRule = editorEnd < 0
        ? QString()
        : result.mid(editorStart, editorEnd - editorStart + 1);
    if (!editorRule.isEmpty())
        result.replace(editorStart, editorRule.size(), QStringLiteral("{EDITOR_QSS}"));

    const QColor window = theme.windowBackground;
    const QColor panel = theme.panelBackground;
    const QColor surface = theme.surfaceBackground;
    const QColor accent = theme.accentColor;
    const QColor primary = theme.primaryColor;
    const QColor text = theme.textPrimary;
    const QColor secondaryText = theme.textSecondary;
    const QColor border = theme.borderColor;
    const QColor deep = theme.light ? color("#e7eef1") : window.darker(150);
    const QColor mid = theme.light ? color("#e6edf0") : surface.lighter(115);
    const QColor hover = theme.light ? color("#dfe9ed") : surface.lighter(130);
    const QColor mutedBorder = theme.light ? color("#d0dce1") : border.lighter(120);
    const QColor copper = theme.light ? color("#416c75") : primary.darker(125);
    const QColor selected = theme.light ? accent.lighter(175) : accent.darker(220);
    const QColor active = theme.light ? accent.lighter(155) : accent.darker(180);

    const QVector<QPair<QString, QColor>> replacements = {
        {QStringLiteral("#111315"), window}, {QStringLiteral("#090b0d"), deep},
        {QStringLiteral("#202327"), panel}, {QStringLiteral("#292d32"), surface},
        {QStringLiteral("#34393e"), hover}, {QStringLiteral("#454b52"), border},
        {QStringLiteral("#60666d"), mutedBorder}, {QStringLiteral("#e7e9ec"), text},
        {QStringLiteral("#f0f2f4"), text}, {QStringLiteral("#cbd0d5"), secondaryText},
        {QStringLiteral("#d4d8dc"), text}, {QStringLiteral("#a0a7b0"), secondaryText},
        {QStringLiteral("#68717b"), secondaryText.darker(115)},
        {QStringLiteral("#70afc4"), accent}, {QStringLiteral("#a96537"), primary},
        {QStringLiteral("#d9954b"), accent}, {QStringLiteral("#d68a45"), accent},
        {QStringLiteral("#1c1e20"), panel}, {QStringLiteral("#17191c"), panel.darker(105)},
        {QStringLiteral("#17191b"), panel.darker(105)}, {QStringLiteral("#151719"), mid},
        {QStringLiteral("#0d0f11"), window}, {QStringLiteral("#080a0b"), deep},
        {QStringLiteral("#a65f3d"), copper}, {QStringLiteral("#704b31"), selected},
        {QStringLiteral("#493426"), selected}, {QStringLiteral("#39454c"), active},
        {QStringLiteral("#34444a"), active}, {QStringLiteral("#222426"), hover},
        {QStringLiteral("#292b2b"), border.darker(110)}, {QStringLiteral("#393a38"), border},
        {QStringLiteral("#858783"), secondaryText}, {QStringLiteral("#626560"), secondaryText},
        {QStringLiteral("#d6d2ca"), text}, {QStringLiteral("#f4eee8"), text},
        {QStringLiteral("#d8dde2"), text}, {QStringLiteral("#e0e3e6"), text},
        {QStringLiteral("#b9c2c9"), secondaryText}
    };

    for (const auto &replacement : replacements) {
        result.replace(replacement.first, replacement.second.name(QColor::HexRgb),
                       Qt::CaseInsensitive);
    }

    result.replace(QStringLiteral("{EDITOR_QSS}"), editorRule);
    if (theme.glassEnabled)
        result.append(glassRules(theme));
    return result;
}

void ThemeManager::applyPalette(const ThemeDefinition &theme, QPalette *palette)
{
    if (!palette)
        return;
    if (theme.id == QStringLiteral("metal")) {
        palette->setColor(QPalette::Window, color("#111315"));
        palette->setColor(QPalette::WindowText, color("#e7e9ec"));
        palette->setColor(QPalette::Base, color("#090b0d"));
        palette->setColor(QPalette::AlternateBase, color("#202327"));
        palette->setColor(QPalette::ToolTipBase, color("#202327"));
        palette->setColor(QPalette::ToolTipText, color("#e7e9ec"));
        palette->setColor(QPalette::Text, color("#e7e9ec"));
        palette->setColor(QPalette::Button, color("#202327"));
        palette->setColor(QPalette::ButtonText, color("#e7e9ec"));
        palette->setColor(QPalette::BrightText, color("#d8e3ea"));
        palette->setColor(QPalette::Highlight, color("#a96537"));
        palette->setColor(QPalette::HighlightedText, color("#f4eee8"));
        palette->setColor(QPalette::Light, color("#454b52"));
        palette->setColor(QPalette::Midlight, color("#292d32"));
        palette->setColor(QPalette::Mid, color("#202327"));
        palette->setColor(QPalette::Dark, color("#090b0d"));
        palette->setColor(QPalette::Shadow, color("#050607"));
        palette->setColor(QPalette::Disabled, QPalette::WindowText, color("#68717b"));
        palette->setColor(QPalette::Disabled, QPalette::Text, color("#68717b"));
        palette->setColor(QPalette::Disabled, QPalette::ButtonText, color("#68717b"));
        return;
    }
    palette->setColor(QPalette::Window, theme.windowBackground);
    palette->setColor(QPalette::WindowText, theme.textPrimary);
    palette->setColor(QPalette::Base, theme.light ? theme.surfaceBackground
                                                  : theme.windowBackground.darker(145));
    palette->setColor(QPalette::AlternateBase, theme.panelBackground);
    palette->setColor(QPalette::ToolTipBase, theme.surfaceBackground);
    palette->setColor(QPalette::ToolTipText, theme.textPrimary);
    palette->setColor(QPalette::Text, theme.textPrimary);
    palette->setColor(QPalette::Button, theme.panelBackground);
    palette->setColor(QPalette::ButtonText, theme.textPrimary);
    palette->setColor(QPalette::Highlight, theme.accentColor);
    palette->setColor(QPalette::HighlightedText, theme.light ? Qt::white : theme.textPrimary);
    palette->setColor(QPalette::Mid, theme.surfaceBackground);
    palette->setColor(QPalette::Dark, theme.windowBackground);
    palette->setColor(QPalette::Disabled, QPalette::Text, theme.textSecondary);
    palette->setColor(QPalette::Disabled, QPalette::ButtonText, theme.textSecondary);
}
