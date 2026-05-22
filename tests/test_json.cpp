#include <QtTest>
#include <QJsonObject>
#include <QJsonDocument>
#include "../src/core/json_parser.hpp"
#include "../src/core/models.hpp"

class TestJsonParser : public QObject {
    Q_OBJECT

private slots:
    void testParsePost() {
        QJsonObject data;
        data["id"] = "abc123";
        data["name"] = "t3_abc123";
        data["title"] = "Test Post Title";
        data["author"] = "testuser";
        data["subreddit"] = "testsub";
        data["score"] = 42;
        data["num_comments"] = 7;
        data["created_utc"] = 1700000000.0;
        data["is_self"] = true;
        data["over_18"] = false;
        data["spoiler"] = false;
        data["stickied"] = false;

        auto post = PinkReader::JsonParser::parsePost(data);

        QCOMPARE(post.id, QString("abc123"));
        QCOMPARE(post.fullname, QString("t3_abc123"));
        QCOMPARE(post.title, QString("Test Post Title"));
        QCOMPARE(post.author, QString("testuser"));
        QCOMPARE(post.subreddit, QString("testsub"));
        QCOMPARE(post.score, 42);
        QCOMPARE(post.commentCount, 7);
        QVERIFY(post.isSelf);
        QVERIFY(!post.nsfw);
    }

    void testParsePostNsfw() {
        QJsonObject data;
        data["id"] = "nsfw1";
        data["over_18"] = true;
        data["is_self"] = false;
        auto post = PinkReader::JsonParser::parsePost(data);
        QVERIFY(post.nsfw);
    }

    void testParsePostVoteState() {
        QJsonObject data;
        data["id"] = "v1";
        data["likes"] = true;
        auto upvoted = PinkReader::JsonParser::parsePost(data);
        QCOMPARE(static_cast<int>(upvoted.voteState), static_cast<int>(PinkReader::VoteState::Upvoted));

        QJsonObject data2;
        data2["id"] = "v2";
        data2["likes"] = false;
        auto downvoted = PinkReader::JsonParser::parsePost(data2);
        QCOMPARE(static_cast<int>(downvoted.voteState), static_cast<int>(PinkReader::VoteState::Downvoted));

        QJsonObject data3;
        data3["id"] = "v3";
        auto none = PinkReader::JsonParser::parsePost(data3);
        QCOMPARE(static_cast<int>(none.voteState), static_cast<int>(PinkReader::VoteState::None));
    }

    void testParseComment() {
        QJsonObject data;
        data["id"] = "c1";
        data["name"] = "t1_c1";
        data["author"] = "commenter";
        data["body"] = "Hello world";
        data["score"] = 10;
        data["created_utc"] = 1700000000.0;

        auto comment = PinkReader::JsonParser::parseComment(data);
        QCOMPARE(comment.id, QString("c1"));
        QCOMPARE(comment.author, QString("commenter"));
        QCOMPARE(comment.body, QString("Hello world"));
        QCOMPARE(comment.score, 10);
    }

    void testParseSubreddit() {
        QJsonObject data;
        data["display_name"] = "programming";
        data["title"] = "Programming";
        data["description"] = "Computer programming";
        data["subscribers"] = 5000000;
        data["over18"] = false;

        auto sr = PinkReader::JsonParser::parseSubreddit(data);
        QCOMPARE(sr.name, QString("programming"));
        QCOMPARE(sr.title, QString("Programming"));
        QCOMPARE(sr.subscriberCount, 5000000);
        QVERIFY(!sr.over18);
    }
};

QTEST_MAIN(TestJsonParser)
#include "test_json.moc"
