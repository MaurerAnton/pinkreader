#pragma once

#include <QObject>
#include <QColor>
#include <QSettings>

namespace PinkReader {

class ThemeManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool dark READ dark WRITE setDark NOTIFY themeChanged)
    Q_PROPERTY(QColor background READ background NOTIFY themeChanged)
    Q_PROPERTY(QColor surface READ surface NOTIFY themeChanged)
    Q_PROPERTY(QColor primary READ primary NOTIFY themeChanged)
    Q_PROPERTY(QColor accent READ accent NOTIFY themeChanged)
    Q_PROPERTY(QColor text READ text NOTIFY themeChanged)
    Q_PROPERTY(QColor textSecondary READ textSecondary NOTIFY themeChanged)
    Q_PROPERTY(QColor divider READ divider NOTIFY themeChanged)
    Q_PROPERTY(QColor error READ error NOTIFY themeChanged)
    Q_PROPERTY(QColor success READ success NOTIFY themeChanged)
    Q_PROPERTY(QColor warning READ warning NOTIFY themeChanged)
    Q_PROPERTY(int elevation READ elevation NOTIFY themeChanged)
    Q_PROPERTY(qreal radius READ radius NOTIFY themeChanged)

public:
    explicit ThemeManager(QObject* parent = nullptr);

    bool dark() const { return m_dark; }
    void setDark(bool dark);

    Q_INVOKABLE void toggle();
    Q_INVOKABLE void load();
    Q_INVOKABLE void save();

    // Colors
    QColor background() const;
    QColor surface() const;
    QColor primary() const;
    QColor accent() const;
    QColor text() const;
    QColor textSecondary() const;
    QColor divider() const;
    QColor error() const;
    QColor success() const;
    QColor warning() const;

    int elevation() const { return 2; }
    qreal radius() const { return 8.0; }

signals:
    void themeChanged();

private:
    bool m_dark = true;
};

} // namespace PinkReader
