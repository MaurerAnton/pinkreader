#include "models.hpp"

#include "json_parser.hpp"

namespace PinkReader {

Post Post::fromJson(const QJsonObject& obj) {
    return JsonParser::parsePost(obj);
}

QJsonObject Post::toJson() const {
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = fullname;
    obj["title"] = title;
    obj["selftext"] = selftext;
    obj["author"] = author;
    obj["subreddit"] = subreddit;
    obj["permalink"] = permalink;
    obj["url"] = url.toString();
    obj["score"] = score;
    obj["num_comments"] = commentCount;
    obj["domain"] = domain;
    obj["created_utc"] = createdUtc.toSecsSinceEpoch();
    obj["saved"] = saved;
    obj["stickied"] = stickied;
    obj["nsfw"] = nsfw;
    obj["spoiler"] = spoiler;
    obj["is_self"] = isSelf;
    return obj;
}

Comment Comment::fromJson(const QJsonObject& obj, int depth) {
    return JsonParser::parseComment(obj, depth);
}

QJsonObject Comment::toJson() const {
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = fullname;
    obj["parent_id"] = parentId;
    obj["link_id"] = linkId;
    obj["author"] = author;
    obj["body"] = body;
    obj["score"] = score;
    obj["created_utc"] = createdUtc.toSecsSinceEpoch();
    obj["depth"] = depth;
    obj["saved"] = saved;
    obj["stickied"] = stickied;
    obj["is_submitter"] = isSubmitter;
    return obj;
}

Subreddit Subreddit::fromJson(const QJsonObject& obj) {
    return JsonParser::parseSubreddit(obj);
}

QJsonObject Subreddit::toJson() const {
    QJsonObject obj;
    obj["id"] = id;
    obj["display_name"] = name;
    obj["title"] = title;
    obj["description"] = description;
    obj["subscribers"] = subscriberCount;
    obj["created_utc"] = createdUtc.toSecsSinceEpoch();
    obj["over18"] = over18;
    obj["icon_img"] = iconUrl.toString();
    return obj;
}

Listing Listing::fromJson(const QJsonObject& obj) {
    Q_UNUSED(obj)
    return Listing{};
}

}  // namespace PinkReader
