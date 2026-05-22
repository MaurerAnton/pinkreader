#pragma once

#include "models.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace PinkReader {
namespace JsonParser {

Post parsePost(const QJsonObject& obj);
Comment parseComment(const QJsonObject& obj, int depth = 0);
Subreddit parseSubreddit(const QJsonObject& obj);
Listing parseListing(const QJsonDocument& doc);

} // namespace JsonParser
} // namespace PinkReader
