#include "theme_manager.hpp"
#include <QGuiApplication>
#include <QStyleHints>

namespace PinkReader {

ThemeManager::ThemeManager(QObject* parent) : QObject(parent) {
    load();
}

void ThemeManager::setDark(bool dark) {
    if (m_dark != dark) {
        m_dark = dark;
        save();
        emit themeChanged();
    }
}

void ThemeManager::toggle() {
    setDark(!m_dark);
}

void ThemeManager::load() {
    QSettings settings;
    m_dark = settings.value("theme/dark", true).toBool();
    emit themeChanged();
}

void ThemeManager::save() {
    QSettings settings;
    settings.setValue("theme/dark", m_dark);
}

// Dark palette: Deep blue/purple with pink accent
// Light palette: White/light gray with pink accent

QColor ThemeManager::background() const {
    return m_dark ? QColor("#0f0f1a") : QColor("#f5f5f5");
}

QColor ThemeManager::surface() const {
    return m_dark ? QColor("#1a1a2e") : QColor("#ffffff");
}

QColor ThemeManager::primary() const {
    return QColor("#e94560"); // Pink accent - same in both themes
}

QColor ThemeManager::accent() const {
    return m_dark ? QColor("#0f3460") : QColor("#e8e8f0");
}

QColor ThemeManager::text() const {
    return m_dark ? QColor("#e0e0e0") : QColor("#1a1a1a");
}

QColor ThemeManager::textSecondary() const {
    return m_dark ? QColor("#888888") : QColor("#666666");
}

QColor ThemeManager::divider() const {
    return m_dark ? QColor("#16213e") : QColor("#e0e0e0");
}

QColor ThemeManager::error() const {
    return QColor("#e94560");
}

QColor ThemeManager::success() const {
    return QColor("#00b894");
}

QColor ThemeManager::warning() const {
    return QColor("#f0a500");
}

} // namespace PinkReader
