// PinkReader Native Library - NDK-compilable C++20 core

#include <jni.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstring>
#include <cstdlib>

// ============================================================================
// Simple JSON parser (no external deps)
// ============================================================================
// PinkReader utils prefix to avoid name collisions
#define PR_JSON  pinkreader_json_


enum class JsonType { Null, Bool, Number, String, Array, Object };

class JsonValue {
public:
    JsonType type = JsonType::Null;
    bool boolVal = false;
    double numberVal = 0.0;
    std::string stringVal;
    std::vector<JsonValue> arrayVal;
    std::map<std::string, JsonValue> objectVal;

    static JsonValue parse(const std::string &json);
    std::string toString() const;
    bool has(const std::string &key) const {
        return type == JsonType::Object && objectVal.count(key) > 0;
    }
    JsonValue operator[](const std::string &key) const {
        if (type == JsonType::Object) {
            auto it = objectVal.find(key);
            if (it != objectVal.end()) return it->second;
        }
        return JsonValue();
    }
    std::string asString() const { return stringVal; }
    int asInt() const { return static_cast<int>(numberVal); }
    double asDouble() const { return numberVal; }
    bool asBool() const { return boolVal; }
    int size() const {
        if (type == JsonType::Array) return arrayVal.size();
        if (type == JsonType::Object) return objectVal.size();
        return 0;
    }
    JsonValue at(int i) const {
        if (type == JsonType::Array && i >= 0 && i < (int)arrayVal.size())
            return arrayVal[i];
        return JsonValue();
    }
};

// Minimal recursive descent JSON parser
class JsonParser {
    const std::string &s;
    size_t pos = 0;

    void skipWhitespace() {
        while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\n' ||
               s[pos] == '\r' || s[pos] == '\t')) pos++;
    }

    JsonValue parseValue() {
        skipWhitespace();
        if (pos >= s.size()) return JsonValue();
        char c = s[pos];
        if (c == '"') return parseString();
        if (c == '{') return parseObject();
        if (c == '[') return parseArray();
        if (c == 't' || c == 'f') return parseBool();
        if (c == 'n') return parseNull();
        return parseNumber();
    }

    JsonValue parseNull() {
        if (s.substr(pos, 4) == "null") { pos += 4; return JsonValue(); }
        return JsonValue();
    }

    JsonValue parseBool() {
        JsonValue v;
        v.type = JsonType::Bool;
        if (s.substr(pos, 4) == "true") { v.boolVal = true; pos += 4; }
        else if (s.substr(pos, 5) == "false") { v.boolVal = false; pos += 5; }
        return v;
    }

    JsonValue parseNumber() {
        JsonValue v;
        v.type = JsonType::Number;
        size_t start = pos;
        if (s[pos] == '-') pos++;
        while (pos < s.size() && isdigit(s[pos])) pos++;
        if (pos < s.size() && s[pos] == '.') {
            pos++;
            while (pos < s.size() && isdigit(s[pos])) pos++;
        }
        if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E')) {
            pos++;
            if (s[pos] == '+' || s[pos] == '-') pos++;
            while (pos < s.size() && isdigit(s[pos])) pos++;
        }
        v.numberVal = std::strtod(s.substr(start, pos - start).c_str(), nullptr);
        return v;
    }

    JsonValue parseString() {
        JsonValue v;
        v.type = JsonType::String;
        pos++; // skip opening quote
        while (pos < s.size() && s[pos] != '"') {
            if (s[pos] == '\\') {
                pos++;
                if (pos < s.size()) {
                    switch (s[pos]) {
                        case 'n': v.stringVal += '\n'; break;
                        case 't': v.stringVal += '\t'; break;
                        case 'r': v.stringVal += '\r'; break;
                        case '\\': v.stringVal += '\\'; break;
                        case '"': v.stringVal += '"'; break;
                        case '/': v.stringVal += '/'; break;
                        case 'u': {
                            // Skip unicode escapes for simplicity
                            std::string hex = s.substr(pos + 1, 4);
                            unsigned int cp = std::strtoul(hex.c_str(), nullptr, 16);
                            if (cp < 0x80) v.stringVal += (char)cp;
                            else if (cp < 0x800) {
                                v.stringVal += (char)(0xC0 | (cp >> 6));
                                v.stringVal += (char)(0x80 | (cp & 0x3F));
                            } else {
                                v.stringVal += (char)(0xE0 | (cp >> 12));
                                v.stringVal += (char)(0x80 | ((cp >> 6) & 0x3F));
                                v.stringVal += (char)(0x80 | (cp & 0x3F));
                            }
                            pos += 4;
                            break;
                        }
                    }
                }
            } else {
                v.stringVal += s[pos];
            }
            pos++;
        }
        if (pos < s.size()) pos++; // skip closing quote
        return v;
    }

    JsonValue parseArray() {
        JsonValue v;
        v.type = JsonType::Array;
        pos++; // skip [
        skipWhitespace();
        if (pos < s.size() && s[pos] == ']') { pos++; return v; }
        while (true) {
            v.arrayVal.push_back(parseValue());
            skipWhitespace();
            if (pos < s.size() && s[pos] == ',') { pos++; continue; }
            if (pos < s.size() && s[pos] == ']') { pos++; break; }
            break;
        }
        return v;
    }

    JsonValue parseObject() {
        JsonValue v;
        v.type = JsonType::Object;
        pos++; // skip {
        skipWhitespace();
        if (pos < s.size() && s[pos] == '}') { pos++; return v; }
        while (true) {
            JsonValue key = parseString();
            skipWhitespace();
            if (pos < s.size() && s[pos] == ':') pos++;
            skipWhitespace();
            v.objectVal[key.stringVal] = parseValue();
            skipWhitespace();
            if (pos < s.size() && s[pos] == ',') { pos++; skipWhitespace(); continue; }
            if (pos < s.size() && s[pos] == '}') { pos++; break; }
            break;
        }
        return v;
    }

public:
    JsonParser(const std::string &str) : s(str) {}
    JsonValue parse() { return parseValue(); }
};

JsonValue JsonValue::parse(const std::string &json) {
    return JsonParser(json).parse();
}

std::string JsonValue::toString() const {
    switch (type) {
        case JsonType::Null: return "null";
        case JsonType::Bool: return boolVal ? "true" : "false";
        case JsonType::Number: return std::to_string(numberVal);
        case JsonType::String: return "\"" + stringVal + "\"";
        case JsonType::Array: {
            std::string r = "[";
            for (size_t i = 0; i < arrayVal.size(); i++) {
                if (i > 0) r += ",";
                r += arrayVal[i].toString();
            }
            return r + "]";
        }
        case JsonType::Object: {
            std::string r = "{";
            bool first = true;
            for (auto &kv : objectVal) {
                if (!first) r += ",";
                first = false;
                r += "\"" + kv.first + "\":" + kv.second.toString();
            }
            return r + "}";
        }
    }
    return "null";
}

// ============================================================================
// Reddit data models
// ============================================================================

struct RedditPost {
    std::string id;
    std::string title;
    std::string author;
    std::string subreddit;
    std::string selftext;
    std::string url;
    std::string permalink;
    std::string thumbnail;
    int score = 0;
    int numComments = 0;
    double createdUtc = 0.0;
    bool over18 = false;
    bool spoiler = false;
    bool stickied = false;
    bool locked = false;
    bool saved = false;
    bool hidden = false;
    int likes = 0; // -1, 0, 1

    static RedditPost fromJson(const JsonValue &data) {
        RedditPost p;
        p.id = data["id"].asString();
        p.title = data["title"].asString();
        p.author = data["author"].asString();
        p.subreddit = data["subreddit"].asString();
        p.selftext = data["selftext"].asString();
        p.url = data["url"].asString();
        p.permalink = data["permalink"].asString();
        p.thumbnail = data["thumbnail"].asString();
        p.score = data["score"].asInt();
        p.numComments = data["num_comments"].asInt();
        p.createdUtc = data["created_utc"].asDouble();
        p.over18 = data["over_18"].asBool();
        p.spoiler = data["spoiler"].asBool();
        p.stickied = data["stickied"].asBool();
        p.locked = data["locked"].asBool();
        p.saved = data["saved"].asBool();
        p.hidden = data["hidden"].asBool();
        p.likes = data["likes"].asInt();
        return p;
    }
};

struct RedditComment {
    std::string id;
    std::string author;
    std::string body;
    std::string bodyHtml;
    std::string parentId;
    std::string linkId;
    std::string subreddit;
    int score = 0;
    double createdUtc = 0.0;
    int depth = 0;
    bool stickied = false;
    std::vector<RedditComment> replies;

    static RedditComment fromJson(const JsonValue &data) {
        RedditComment c;
        c.id = data["id"].asString();
        c.author = data["author"].asString();
        c.body = data["body"].asString();
        c.bodyHtml = data["body_html"].asString();
        c.parentId = data["parent_id"].asString();
        c.linkId = data["link_id"].asString();
        c.subreddit = data["subreddit"].asString();
        c.score = data["score"].asInt();
        c.createdUtc = data["created_utc"].asDouble();
        c.depth = data["depth"].asInt();
        c.stickied = data["stickied"].asBool();

        // Parse replies from "replies" field (can be empty string or object)
        if (data["replies"].type == JsonType::Object) {
            JsonValue repliesData = data["replies"]["data"];
            if (repliesData.type == JsonType::Object) {
                JsonValue children = repliesData["children"];
                for (int i = 0; i < children.size(); i++) {
                    JsonValue childData = children.at(i)["data"];
                    if (childData.type == JsonType::Object) {
                        RedditComment reply = RedditComment::fromJson(childData);
                        reply.depth = c.depth + 1;
                        c.replies.push_back(reply);
                    }
                }
            }
        }
        return c;
    }
};

struct RedditAccount {
    std::string username;
    std::string accessToken;
    std::string refreshToken;
    double tokenExpiry = 0.0;

    bool isAuthorized() const { return !accessToken.empty(); }
};

// ============================================================================
// Simple HTTP client using POSIX sockets
// ============================================================================

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

// OpenSSL not available on Android NDK - HTTPS handled via JNI
#ifndef __ANDROID__
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

class HttpClient {
    std::mutex m_mutex;

    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

public:
    static HttpClient& instance() {
        static HttpClient inst;
        return inst;
    }

    std::string get(const std::string &url, const std::string &authHeader = "") {
        // Parse URL
        std::string host, path;
        bool useHttps = false;

        if (url.find("https://") == 0) {
            useHttps = true;
            size_t pos = 8;
            size_t slash = url.find('/', pos);
            if (slash == std::string::npos) {
                host = url.substr(pos);
                path = "/";
            } else {
                host = url.substr(pos, slash - pos);
                path = url.substr(slash);
            }
        } else if (url.find("http://") == 0) {
            size_t pos = 7;
            size_t slash = url.find('/', pos);
            if (slash == std::string::npos) {
                host = url.substr(pos);
                path = "/";
            } else {
                host = url.substr(pos, slash - pos);
                path = url.substr(slash);
            }
        } else {
            host = url;
            path = "/";
        }

        int port = useHttps ? 443 : 80;

        // Resolve hostname
        struct hostent *server = gethostbyname(host.c_str());
        if (!server) return "";

        // Create socket
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) return "";

        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
        serv_addr.sin_port = htons(port);

        // Set timeout
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            close(sockfd);
            return "";
        }

        std::string response;

        if (useHttps) {
#ifdef __ANDROID__
            // On Android, HTTPS is handled via platform/JNI
            close(sockfd);
            return "";
#else
            // SSL connection
            SSL_library_init();
            SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
            if (!ctx) { close(sockfd); return ""; }

            SSL *ssl = SSL_new(ctx);
            SSL_set_fd(ssl, sockfd);

            if (SSL_connect(ssl) <= 0) {
                SSL_free(ssl);
                SSL_CTX_free(ctx);
                close(sockfd);
                return "";
            }

            // Build HTTP request
            std::string request = "GET " + path + " HTTP/1.1\r\n";
            request += "Host: " + host + "\r\n";
            request += "User-Agent: PinkReader/1.0.0 (Android)\r\n";
            request += "Accept: application/json\r\n";
            if (!authHeader.empty()) {
                request += "Authorization: " + authHeader + "\r\n";
            }
            request += "Connection: close\r\n\r\n";

            SSL_write(ssl, request.c_str(), request.size());

            char buffer[4096];
            int bytes;
            while ((bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytes] = '\0';
                response += buffer;
            }

            SSL_shutdown(ssl);
            SSL_free(ssl);
            SSL_CTX_free(ctx);
#endif
        } else {
            // Plain HTTP
            std::string request = "GET " + path + " HTTP/1.1\r\n";
            request += "Host: " + host + "\r\n";
            request += "User-Agent: PinkReader/1.0.0 (Android)\r\n";
            request += "Accept: application/json\r\n";
            request += "Connection: close\r\n\r\n";

            send(sockfd, request.c_str(), request.size(), 0);

            char buffer[4096];
            int bytes;
            while ((bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
                buffer[bytes] = '\0';
                response += buffer;
            }
        }

        close(sockfd);

        // Extract body from HTTP response
        size_t headerEnd = response.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            return response.substr(headerEnd + 4);
        }

        return response;
    }
};

// ============================================================================
// Reddit API client
// ============================================================================

class RedditAPI {
    RedditAccount m_account;
    std::string m_baseUrl = "https://oauth.reddit.com";

public:
    RedditAPI() {}

    void setAccount(const RedditAccount &account) { m_account = account; }
    const RedditAccount& account() const { return m_account; }

    std::vector<RedditPost> fetchSubreddit(const std::string &subreddit,
                                            const std::string &sort = "hot",
                                            const std::string &after = "",
                                            int limit = 25) {
        std::vector<RedditPost> posts;
        std::string url = m_baseUrl;

        if (subreddit.empty() || subreddit == "frontpage") {
            url += "/" + sort + ".json";
        } else {
            url += "/r/" + subreddit + "/" + sort + ".json";
        }

        url += "?limit=" + std::to_string(limit);
        if (!after.empty()) url += "&after=" + after;
        url += "&raw_json=1";

        std::string auth = "Bearer " + m_account.accessToken;
        std::string response = HttpClient::instance().get(url, auth);

        if (response.empty()) return posts;

        JsonValue root = JsonValue::parse(response);
        JsonValue data = root["data"];
        JsonValue children = data["children"];

        for (int i = 0; i < children.size(); i++) {
            JsonValue childData = children.at(i)["data"];
            if (childData.type == JsonType::Object) {
                posts.push_back(RedditPost::fromJson(childData));
            }
        }

        return posts;
    }

    std::vector<RedditComment> fetchComments(const std::string &subreddit,
                                               const std::string &postId,
                                               const std::string &sort = "confidence",
                                               int limit = 200) {
        std::vector<RedditComment> comments;
        std::string url = m_baseUrl + "/r/" + subreddit + "/comments/" + postId + ".json";
        url += "?sort=" + sort + "&limit=" + std::to_string(limit) + "&raw_json=1";

        std::string auth = "Bearer " + m_account.accessToken;
        std::string response = HttpClient::instance().get(url, auth);

        if (response.empty()) return comments;

        // Reddit returns an array: [post_data, comments_listing]
        JsonValue root = JsonValue::parse(response);
        if (root.type != JsonType::Array || root.size() < 2) return comments;

        JsonValue commentsListing = root.at(1);
        JsonValue commentsData = commentsListing["data"];
        JsonValue children = commentsData["children"];

        for (int i = 0; i < children.size(); i++) {
            JsonValue child = children.at(i);
            std::string kind = child["kind"].asString();
            if (kind == "t1") {
                JsonValue commentData = child["data"];
                comments.push_back(RedditComment::fromJson(commentData));
            }
        }

        return comments;
    }
};

// ============================================================================
// Simple LRU Cache
// ============================================================================

#include <list>
#include <unordered_map>

template<typename K, typename V>
class LRUCache {
    size_t m_maxSize;
    std::list<std::pair<K, V>> m_list;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> m_map;
    std::mutex m_mutex;

public:
    LRUCache(size_t maxSize = 100) : m_maxSize(maxSize) {}

    void put(const K &key, const V &value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            m_list.erase(it->second);
            m_map.erase(it);
        }
        m_list.push_front({key, value});
        m_map[key] = m_list.begin();
        if (m_map.size() > m_maxSize) {
            auto last = m_list.end();
            last--;
            m_map.erase(last->first);
            m_list.pop_back();
        }
    }

    bool get(const K &key, V &value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_map.find(key);
        if (it == m_map.end()) return false;
        m_list.splice(m_list.begin(), m_list, it->second);
        value = it->second->second;
        return true;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_list.clear();
        m_map.clear();
    }

    size_t size() const { return m_map.size(); }
};

// ============================================================================
// Global application state
// ============================================================================

class PinkReaderApp {
    RedditAPI m_api;
    LRUCache<std::string, std::string> m_cache{200};
    std::vector<RedditPost> m_currentPosts;
    std::string m_currentSubreddit = "all";
    std::string m_after;
    std::mutex m_mutex;

    PinkReaderApp() = default;

public:
    static PinkReaderApp& instance() {
        static PinkReaderApp app;
        return app;
    }

    RedditAPI& api() { return m_api; }

    void setAccount(const std::string &username, const std::string &accessToken) {
        RedditAccount acc;
        acc.username = username;
        acc.accessToken = accessToken;
        m_api.setAccount(acc);
    }

    std::vector<RedditPost> fetchPosts() {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto posts = m_api.fetchSubreddit(m_currentSubreddit, "hot", m_after, 25);
        if (!posts.empty()) {
            m_after = posts.back().id;
            m_currentPosts = posts;
        }
        return posts;
    }

    std::string getCurrentSubreddit() const { return m_currentSubreddit; }
    void setCurrentSubreddit(const std::string &sub) { m_currentSubreddit = sub; }
};


// ============================================================================
// JNI Bridge - Exposes native functions to Kotlin/Java
// ============================================================================

extern "C" {

JNIEXPORT jstring JNICALL
Java_org_pinkreader_app_MainActivity_nativeGetVersion(
    JNIEnv *env, jobject /* this */) {
    return env->NewStringUTF("PinkReader v1.0.0 (C++20 native)");
}

JNIEXPORT jstring JNICALL
Java_org_pinkreader_app_MainActivity_nativeFetchPosts(
    JNIEnv *env, jobject /* this */, jstring subreddit) {
    const char *sub = env->GetStringUTFChars(subreddit, nullptr);
    PinkReaderApp::instance().setCurrentSubreddit(sub);
    auto posts = PinkReaderApp::instance().fetchPosts();
    env->ReleaseStringUTFChars(subreddit, sub);

    // Serialize posts to JSON for Java layer
    std::string json = "[";
    for (size_t i = 0; i < posts.size(); i++) {
        if (i > 0) json += ",";
        json += "{";
        json += "\"id\":\"" + posts[i].id + "\",";
        json += "\"title\":\"" + posts[i].title + "\",";
        json += "\"author\":\"" + posts[i].author + "\",";
        json += "\"subreddit\":\"" + posts[i].subreddit + "\",";
        json += "\"score\":" + std::to_string(posts[i].score) + ",";
        json += "\"num_comments\":" + std::to_string(posts[i].numComments) + ",";
        json += "\"url\":\"" + posts[i].url + "\",";
        json += "\"permalink\":\"" + posts[i].permalink + "\"";
        json += "}";
    }
    json += "]";

    return env->NewStringUTF(json.c_str());
}

JNIEXPORT void JNICALL
Java_org_pinkreader_app_MainActivity_nativeInit(
    JNIEnv *env, jobject /* this */, jstring cachePath) {
    const char *path = env->GetStringUTFChars(cachePath, nullptr);
    // Initialize native components
    env->ReleaseStringUTFChars(cachePath, path);
}

JNIEXPORT void JNICALL
Java_org_pinkreader_app_MainActivity_nativeResume(
    JNIEnv * /* env */, jobject /* this */) {
    // Resume native operations
}

JNIEXPORT void JNICALL
Java_org_pinkreader_app_MainActivity_nativePause(
    JNIEnv * /* env */, jobject /* this */) {
    // Pause native operations
}

JNIEXPORT void JNICALL
Java_org_pinkreader_app_MainActivity_nativeDestroy(
    JNIEnv * /* env */, jobject /* this */) {
    // Cleanup native resources
}

} // extern "C"
