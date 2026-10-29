#include "themeselectordialog.h"

#include "thememanager.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

namespace {

class ThemeCard final : public QPushButton
{
public:
    explicit ThemeCard(const ThemeDefinition &theme, QWidget *parent = nullptr)
        : QPushButton(parent), theme_(theme)
    {
        setObjectName(QStringLiteral("themeCard_%1").arg(theme.id));
        setCheckable(true);
        setMinimumSize(142, 82);
        setCursor(Qt::PointingHandCursor);
        setAccessibleName(theme.displayName);
        setToolTip(QStringLiteral("Apply %1 application theme").arg(theme.displayName));
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF bounds = rect().adjusted(1, 1, -1, -1);
        const QColor base = theme_.panelBackground;
        painter.setPen(QPen(isChecked() ? theme_.accentColor : theme_.borderColor,
                            isChecked() ? 2 : 1));
        painter.setBrush(base);
        painter.drawRoundedRect(bounds, 9, 9);

        const QRectF preview(bounds.left() + 9, bounds.top() + 8,
                             bounds.width() - 18, 30);
        QLinearGradient gradient(preview.topLeft(), preview.bottomRight());
        gradient.setColorAt(0, theme_.windowBackground);
        gradient.setColorAt(1, theme_.primaryColor);
        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        painter.drawRoundedRect(preview, 5, 5);
        painter.setBrush(QColor(theme_.surfaceBackground).lighter(115));
        painter.drawRoundedRect(QRectF(preview.left() + 5, preview.top() + 5, 43, 20), 3, 3);
        painter.setBrush(theme_.accentColor);
        painter.drawEllipse(QPointF(preview.right() - 10, preview.center().y()), 4, 4);

        painter.setPen(theme_.textPrimary);
        QFont labelFont = font();
        labelFont.setPointSizeF(labelFont.pointSizeF() * 0.92);
        labelFont.setWeight(isChecked() ? QFont::DemiBold : QFont::Medium);
        painter.setFont(labelFont);
        painter.drawText(QRectF(bounds.left() + 9, preview.bottom() + 5,
                                bounds.width() - 18, 21),
                         Qt::AlignVCenter | Qt::AlignLeft, theme_.displayName);
        if (isChecked()) {
            painter.setBrush(theme_.accentColor);
            painter.drawEllipse(QPointF(bounds.right() - 12, bounds.bottom() - 12), 3, 3);
        }
    }

private:
    const ThemeDefinition &theme_;
};

} // namespace

ThemeSelectorDialog::ThemeSelectorDialog(const QString &applicationTheme,
                                         const QString &editorTheme,
                                         QWidget *parent)
    : QDialog(parent), selectedApplicationTheme_(applicationTheme)
{
    setObjectName(QStringLiteral("themeSelectorDialog"));
    setWindowTitle(QStringLiteral("Appearance"));
    setMinimumWidth(630);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(18, 16, 18, 14);
    layout->setSpacing(10);

    auto *heading = new QLabel(QStringLiteral("Application theme"), this);
    QFont headingFont = heading->font();
    headingFont.setWeight(QFont::DemiBold);
    heading->setFont(headingFont);
    layout->addWidget(heading);

    auto *grid = new QGridLayout;
    grid->setHorizontalSpacing(9);
    grid->setVerticalSpacing(9);
    int index = 0;
    for (const ThemeDefinition &theme : ThemeManager::themes()) {
        auto *card = new ThemeCard(theme, this);
        card->setChecked(theme.id == selectedApplicationTheme_);
        connect(card, &QPushButton::clicked, this, [this, id = theme.id]() {
            if (selectedApplicationTheme_ == id)
                return;
            selectedApplicationTheme_ = id;
            updateSelectedCard();
            emit applicationThemeSelected(id);
        });
        grid->addWidget(card, index / 4, index % 4);
        ++index;
    }
    layout->addLayout(grid);

    auto *editorHeading = new QLabel(QStringLiteral("Editor theme (independent)"), this);
    editorHeading->setFont(headingFont);
    layout->addWidget(editorHeading);
    editorThemeCombo_ = new QComboBox(this);
    editorThemeCombo_->setObjectName(QStringLiteral("editorThemeCombo"));
    editorThemeCombo_->addItem(QStringLiteral("Metal — current editor colors"),
                               QStringLiteral("metal"));
    editorThemeCombo_->addItem(QStringLiteral("Glass Light — high-contrast editor"),
                               QStringLiteral("glass-light"));
    editorThemeCombo_->addItem(QStringLiteral("Glass Dark — cool editor"),
                               QStringLiteral("glass-dark"));
    const int editorIndex = editorThemeCombo_->findData(editorTheme);
    editorThemeCombo_->setCurrentIndex(qMax(0, editorIndex));
    connect(editorThemeCombo_, &QComboBox::currentIndexChanged, this, [this](int index) {
        emit editorThemeSelected(editorThemeCombo_->itemData(index).toString());
    });
    layout->addWidget(editorThemeCombo_);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    layout->addWidget(buttons);
}

void ThemeSelectorDialog::updateSelectedCard()
{
    const auto cards = findChildren<QPushButton *>();
    for (QPushButton *card : cards) {
        if (!card->objectName().startsWith(QStringLiteral("themeCard_")))
            continue;
        card->setChecked(card->objectName() == QStringLiteral("themeCard_%1")
                                                   .arg(selectedApplicationTheme_));
        card->update();
    }
    update();
}
