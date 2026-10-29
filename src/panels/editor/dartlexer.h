#pragma once

#include <Qsci/qscilexercustom.h>

class DartLexer final : public QsciLexerCustom
{
    Q_OBJECT

public:
    explicit DartLexer(QObject *parent = nullptr);
    const char *language() const override;
    QString description(int style) const override;
    void styleText(int start, int end) override;
};
