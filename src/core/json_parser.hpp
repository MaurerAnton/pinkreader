#pragma once

#include "models.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace PinkReader {
namespace JsonParser {

Post parsePost(const QJsonObject& obj);
Comment parseComment(const QJsonObject& obj, int depth = 0);
Subreddit parseSubreddit(const QJsonObject& obj);
Listing parseListing(const QJsonDocument& doc);

}  // namespace JsonParser
}  // namespace PinkReader
