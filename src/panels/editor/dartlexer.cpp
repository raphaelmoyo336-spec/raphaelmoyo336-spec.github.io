#include "dartlexer.h"
#include "editorpalette.h"

#include <Qsci/qsciscintilla.h>

#include <QColor>
#include <QFontDatabase>
#include <QSet>

namespace {

enum DartStyle {
    DefaultStyle = 0,
    KeywordStyle = 1,
    StringStyle = 2,
    CommentStyle = 3,
    NumberStyle = 4,
    TypeStyle = 5
};

bool isIdentifierStart(unsigned char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
        || ch == '_' || ch == '$' || ch >= 0x80;
}

bool isIdentifierPart(unsigned char ch)
{
    return isIdentifierStart(ch) || (ch >= '0' && ch <= '9');
}

const QSet<QByteArray> &dartKeywords()
{
    static const QSet<QByteArray> words = {
        "abstract", "as", "assert", "async", "await", "break", "case",
        "catch", "class", "const", "continue", "covariant", "default",
        "deferred", "do", "dynamic", "else", "enum", "export", "extends",
        "extension", "external", "factory", "false", "final", "finally",
        "for", "Function", "get", "hide", "if", "implements", "import",
        "in", "interface", "is", "late", "library", "mixin", "native",
        "new", "null", "on", "operator", "part", "required", "rethrow",
        "return", "sealed", "set", "show", "static", "super", "switch",
        "sync", "this", "throw", "true", "try", "typedef", "var", "void",
        "while", "with", "yield"
    };
    return words;
}

} // namespace

DartLexer::DartLexer(QObject *parent)
    : QsciLexerCustom(parent)
{
    const EditorPalette palette = editorPaletteForId(QStringLiteral("metal"));
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setDefaultFont(fixedFont);
    setDefaultColor(palette.foreground);
    setDefaultPaper(palette.background);
    setColor(palette.keyword, KeywordStyle);
    setColor(palette.string, StringStyle);
    setColor(palette.comment, CommentStyle);
    setColor(palette.number, NumberStyle);
    setColor(palette.type, TypeStyle);
}

const char *DartLexer::language() const
{
    return "Dart";
}

QString DartLexer::description(int style) const
{
    switch (style) {
    case DefaultStyle:
        return QStringLiteral("Default");
    case KeywordStyle:
        return QStringLiteral("Keyword");
    case StringStyle:
        return QStringLiteral("String");
    case CommentStyle:
        return QStringLiteral("Comment");
    case NumberStyle:
        return QStringLiteral("Number");
    case TypeStyle:
        return QStringLiteral("Type");
    default:
        return QString();
    }
}

void DartLexer::styleText(int start, int end)
{
    if (!editor() || end <= start)
        return;

    const QByteArray source = editor()->bytes(start, end - 1);
    int index = 0;
    int style = DefaultStyle;
    if (start > 0
        && editor()->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, start - 1)
               == CommentStyle) {
        style = CommentStyle;
    }

    startStyling(start);
    while (index < source.size()) {
        const unsigned char ch = static_cast<unsigned char>(source.at(index));
        int tokenEnd = index + 1;
        int tokenStyle = style;

        if (style == CommentStyle) {
            const int close = source.indexOf("*/", index);
            if (close < 0) {
                tokenEnd = source.size();
            } else {
                tokenEnd = close + 2;
                style = DefaultStyle;
            }
            tokenStyle = CommentStyle;
        } else if (ch == '/' && index + 1 < source.size()
                   && source.at(index + 1) == '/') {
            tokenEnd = source.indexOf('\n', index);
            if (tokenEnd < 0 || tokenEnd >= source.size())
                tokenEnd = source.size();
            tokenStyle = CommentStyle;
        } else if (ch == '/' && index + 1 < source.size()
                   && source.at(index + 1) == '*') {
            tokenEnd = source.indexOf("*/", index + 2);
            if (tokenEnd < 0) {
                tokenEnd = source.size();
                style = CommentStyle;
            } else {
                tokenEnd += 2;
            }
            tokenStyle = CommentStyle;
        } else if (ch == '\'' || ch == '"') {
            const bool triple = index + 2 < source.size()
                && source.at(index + 1) == ch && source.at(index + 2) == ch;
            const int delimiterSize = triple ? 3 : 1;
            tokenEnd = index + delimiterSize;
            while (tokenEnd < source.size()) {
                if (source.at(tokenEnd) == '\\') {
                    tokenEnd = qMin(tokenEnd + 2, source.size());
                    continue;
                }
                if (source.at(tokenEnd) == ch
                    && (!triple
                        || (tokenEnd + 2 < source.size()
                            && source.at(tokenEnd + 1) == ch
                            && source.at(tokenEnd + 2) == ch))) {
                    tokenEnd += delimiterSize;
                    break;
                }
                if (!triple && source.at(tokenEnd) == '\n')
                    break;
                ++tokenEnd;
            }
            tokenStyle = StringStyle;
        } else if (ch >= '0' && ch <= '9') {
            while (tokenEnd < source.size()) {
                const unsigned char next =
                    static_cast<unsigned char>(source.at(tokenEnd));
                if (!isIdentifierPart(next) && next != '.')
                    break;
                ++tokenEnd;
            }
            tokenStyle = NumberStyle;
        } else if (isIdentifierStart(ch)) {
            while (tokenEnd < source.size()
                   && isIdentifierPart(static_cast<unsigned char>(source.at(tokenEnd))))
                ++tokenEnd;
            const QByteArray word = source.mid(index, tokenEnd - index);
            if (dartKeywords().contains(word))
                tokenStyle = KeywordStyle;
            else if (!word.isEmpty()
                     && word.at(0) >= 'A' && word.at(0) <= 'Z')
                tokenStyle = TypeStyle;
        }

        setStyling(tokenEnd - index, tokenStyle);
        index = tokenEnd;
    }
}
