#pragma once

#include <QString>

namespace PinkReader {
namespace Platform {

QString cachePath();
QString dataPath();
void shareUrl(const QString& url);
void openBrowser(const QString& url);
bool isNetworkAvailable();
void setKeepScreenOn(bool on);
QString deviceId();

} // namespace Platform
} // namespace PinkReader
