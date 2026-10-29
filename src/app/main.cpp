#include "mainwindow.h"

#include <QApplication>
#include <QColor>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication application(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Kogence IDE"));
    QApplication::setOrganizationName(QStringLiteral("Binhlako Kogence"));

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(QStringLiteral("#111315")));
    palette.setColor(QPalette::WindowText, QColor(QStringLiteral("#e7e9ec")));
    palette.setColor(QPalette::Base, QColor(QStringLiteral("#090b0d")));
    palette.setColor(QPalette::AlternateBase, QColor(QStringLiteral("#202327")));
    palette.setColor(QPalette::ToolTipBase, QColor(QStringLiteral("#202327")));
    palette.setColor(QPalette::ToolTipText, QColor(QStringLiteral("#e7e9ec")));
    palette.setColor(QPalette::Text, QColor(QStringLiteral("#e7e9ec")));
    palette.setColor(QPalette::Button, QColor(QStringLiteral("#202327")));
    palette.setColor(QPalette::ButtonText, QColor(QStringLiteral("#e7e9ec")));
    palette.setColor(QPalette::BrightText, QColor(QStringLiteral("#d8e3ea")));
    palette.setColor(QPalette::Highlight, QColor(QStringLiteral("#a96537")));
    palette.setColor(QPalette::HighlightedText, QColor(QStringLiteral("#f4eee8")));
    palette.setColor(QPalette::Light, QColor(QStringLiteral("#454b52")));
    palette.setColor(QPalette::Midlight, QColor(QStringLiteral("#292d32")));
    palette.setColor(QPalette::Mid, QColor(QStringLiteral("#202327")));
    palette.setColor(QPalette::Dark, QColor(QStringLiteral("#090b0d")));
    palette.setColor(QPalette::Shadow, QColor(QStringLiteral("#050607")));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(QStringLiteral("#68717b")));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(QStringLiteral("#68717b")));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(QStringLiteral("#68717b")));
    application.setPalette(palette);

    MainWindow window;
    window.show();
    return application.exec();
}
