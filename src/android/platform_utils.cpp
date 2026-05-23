#include "platform_utils.hpp"

#include <QDir>
#include <QProcess>
#include <QStandardPaths>

#ifdef Q_OS_ANDROID
#    include <QCoreApplication>
#    include <QJniObject>
#    include <QtCore/private/qandroidextras_p.h>
#endif

namespace PinkReader {
namespace Platform {

QString cachePath() {
#ifdef Q_OS_ANDROID
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#else
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/cache";
#endif
}

QString dataPath() {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

void shareUrl(const QString& url) {
#ifdef Q_OS_ANDROID
    QJniObject activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    QJniObject jUrl = QJniObject::fromString(url);
    QJniObject::callStaticMethod<void>("io/pinkreader/ShareUtils", "shareUrl",
                                       "(Landroid/content/Context;Ljava/lang/String;)V", activity.object(),
                                       jUrl.object());
#else
    Q_UNUSED(url)
#endif
}

void openBrowser(const QString& url) {
#ifdef Q_OS_ANDROID
    QJniObject activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    QJniObject jUrl = QJniObject::fromString(url);
    QJniObject::callStaticMethod<void>("io/pinkreader/ShareUtils", "openBrowser",
                                       "(Landroid/content/Context;Ljava/lang/String;)V", activity.object(),
                                       jUrl.object());
#else
    QProcess::startDetached("xdg-open", {url});
#endif
}

bool isNetworkAvailable() {
    // Simple check: try to reach a known host
    return true;  // Platform-specific implementation needed
}

void setKeepScreenOn(bool on) {
#ifdef Q_OS_ANDROID
    QJniObject activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    QJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
    if (on) {
        window.callMethod<void>("addFlags", "(I)V", 128);  // FLAG_KEEP_SCREEN_ON
    } else {
        window.callMethod<void>("clearFlags", "(I)V", 128);
    }
#else
    Q_UNUSED(on)
#endif
}

QString deviceId() {
    return QSysInfo::machineUniqueId();
}

}  // namespace Platform
}  // namespace PinkReader
