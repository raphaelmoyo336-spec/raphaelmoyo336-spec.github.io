#include "terminalpanel.h"

#include <QAction>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
#include <QKeySequence>
#include <qtermwidget.h>
#include <QVBoxLayout>

TerminalPanel::TerminalPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    terminal_ = new QTermWidget(0, this);
    terminal_->setObjectName(QStringLiteral("terminalEmulator"));

    const QString preferredFont = QStringLiteral("DejaVu Sans Mono");
    QFont font = QFontDatabase::families().contains(preferredFont)
                     ? QFont(preferredFont)
                     : QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(10);
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setKerning(false);
    font.setStyleStrategy(QFont::PreferAntialias);
    terminal_->setTerminalFont(font);
    const QString installedColorScheme = QDir(QCoreApplication::applicationDirPath())
                                             .filePath(QStringLiteral("../share/kogence-ide/"
                                                                      "kogence-terminal.colorscheme"));
    const QString colorScheme = QFileInfo::exists(installedColorScheme)
                                    ? installedColorScheme
                                    : QString::fromUtf8(KOGENCE_TERMINAL_COLOR_SCHEME);
    terminal_->setColorScheme(colorScheme);
    terminal_->setScrollBarPosition(QTermWidget::ScrollBarRight);
    terminal_->setWorkingDirectory(QDir::currentPath());

    auto addTerminalAction = [this](const QString &text,
                                    const QKeySequence &shortcut,
                                    auto slot) {
        auto *action = new QAction(text, terminal_);
        action->setShortcut(shortcut);
        action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
        terminal_->addAction(action);
        connect(action, &QAction::triggered, terminal_, slot);
    };
    addTerminalAction(tr("Find..."),
                      QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F),
                      &QTermWidget::toggleShowSearchBar);
    addTerminalAction(tr("Copy"),
                      QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C),
                      &QTermWidget::copyClipboard);
    addTerminalAction(tr("Paste"),
                      QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_V),
                      &QTermWidget::pasteClipboard);

    layout->addWidget(terminal_);
    terminal_->startShellProgram();
}
