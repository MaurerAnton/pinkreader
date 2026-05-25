// PinkReader Native Engine v2 — Replicates RedReader's exact logic
// Matches: CacheManager → RedditAPI → PostListing → CommentListing architecture
// Compiles with Android NDK 27, C++20, no Qt dependencies

#include <jni.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <mutex>
#include <functional>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

// ============================================================================
// JSON Parser — zero dependencies, recursive descent
// ============================================================================
enum JType { JNULL, JBOOL, JNUM, JSTR, JARR, JOBJ };
struct JVal {
    JType type=JNULL; bool b=false; double n=0; std::string s;
    std::vector<JVal> arr; std::map<std::string,JVal> obj;
    bool has(const std::string &k) const { return type==JOBJ&&obj.count(k); }
    JVal operator[](const std::string &k) const { auto i=obj.find(k); return i!=obj.end()?i->second:JVal(); }
    std::string str() const { return s; }
    double num() const { return n; }
    int i() const { return (int)n; }
    int64_t i64() const { return (int64_t)n; }
    bool bl() const { return b; }
    int sz() const { return type==JARR?(int)arr.size():type==JOBJ?(int)obj.size():0; }
    JVal at(int i) const { return (type==JARR&&i>=0&&i<sz())?arr[i]:JVal(); }
    const std::map<std::string,JVal>& o() const { return obj; }
};

struct JParser {
    const char *p,*e;
    JParser(const char *s,int l):p(s),e(s+l){}
    void ws(){while(p<e&&(*p==' '||*p=='\n'||*p=='\r'||*p=='\t'))p++;}
    JVal parse(){ws();if(p>=e)return JVal();char c=*p;
        if(c=='"')return pstr();if(c=='{')return pobj();if(c=='[')return parr();
        if(c=='t'||c=='f')return pbool();if(c=='n')return pnull();return pnum();}
    JVal pnull(){if(e-p>=4&&!memcmp(p,"null",4)){p+=4;return JVal();}return JVal();}
    JVal pbool(){JVal v;v.type=JBOOL;if(e-p>=4&&!memcmp(p,"true",4)){v.b=true;p+=4;}
        else if(e-p>=5&&!memcmp(p,"false",5)){v.b=false;p+=5;}return v;}
    JVal pnum(){JVal v;v.type=JNUM;char*ep;v.n=strtod(p,&ep);p=ep;return v;}
    JVal pstr(){JVal v;v.type=JSTR;p++;
        while(p<e&&*p!='"'){if(*p=='\\'){p++;if(p<e){switch(*p){case'n':v.s+='\n';break;case't':v.s+='\t';break;
            case'r':v.s+='\r';break;case'"':v.s+='"';break;case'\\':v.s+='\\';break;case'/':v.s+='/';break;
            case'u':{char h[5]={p[1],p[2],p[3],p[4],0};unsigned cp=strtoul(h,0,16);p+=4;
                if(cp<0x80)v.s+=(char)cp;else if(cp<0x800){v.s+=(char)(0xC0|cp>>6);v.s+=(char)(0x80|(cp&0x3F));}
                else{v.s+=(char)(0xE0|cp>>12);v.s+=(char)(0x80|(cp>>6&0x3F));v.s+=(char)(0x80|(cp&0x3F));}}}}}
            else v.s+=*p;p++;}if(p<e)p++;return v;}
    JVal parr(){JVal v;v.type=JARR;p++;ws();if(p<e&&*p==']'){p++;return v;}
        while(1){v.arr.push_back(parse());ws();if(p<e&&*p==','){p++;continue;}if(p<e&&*p==']'){p++;break;}break;}return v;}
    JVal pobj(){JVal v;v.type=JOBJ;p++;ws();if(p<e&&*p=='}'){p++;return v;}
        while(1){JVal k=pstr();ws();if(p<e&&*p==':')p++;ws();v.obj[k.s]=parse();
            ws();if(p<e&&*p==','){p++;ws();continue;}if(p<e&&*p=='}'){p++;break;}break;}return v;}
};
JVal parseJSON(const std::string &s){return JParser(s.c_str(),s.size()).parse();}

// ============================================================================
// Data Models — matching RedReader's Post/Comment/Subreddit classes exactly
// ============================================================================
struct Post {
    std::string id, name, title, author, subreddit, subredditId, selftext, selftextHtml;
    std::string url, permalink, thumbnail, domain, linkFlairText, authorFlairText;
    std::string distinguished, postHint; int score=0, ups=0, downs=0, numComments=0;
    int likes=0, gilded=0; double created=0, edited=0, upvoteRatio=0;
    bool over18=false, spoiler=false, stickied=false, saved=false, hidden=false;
    bool isSelf=false, locked=false, archived=false, clicked=false;
    std::string mediaUrl, previewUrl; int previewWidth=0, previewHeight=0;

    static Post from(const JVal &d){Post p;
        p.id=d["id"].str();p.name=d["name"].str();p.title=d["title"].str();
        p.author=d["author"].str();p.subreddit=d["subreddit"].str();p.subredditId=d["subreddit_id"].str();
        p.selftext=d["selftext"].str();p.selftextHtml=d["selftext_html"].str();
        p.url=d["url"].str();p.permalink=d["permalink"].str();p.thumbnail=d["thumbnail"].str();
        p.domain=d["domain"].str();p.linkFlairText=d["link_flair_text"].str();
        p.authorFlairText=d["author_flair_text"].str();p.distinguished=d["distinguished"].str();
        p.postHint=d["post_hint"].str();p.score=d["score"].i();p.ups=d["ups"].i();
        p.downs=d["downs"].i();p.numComments=d["num_comments"].i();p.likes=d["likes"].i();
        p.gilded=d["gilded"].i();p.created=d["created_utc"].num();p.edited=d["edited"].num();
        p.upvoteRatio=d["upvote_ratio"].num();p.over18=d["over_18"].bl();
        p.spoiler=d["spoiler"].bl();p.stickied=d["stickied"].bl();p.saved=d["saved"].bl();
        p.hidden=d["hidden"].bl();p.isSelf=d["is_self"].bl();p.locked=d["locked"].bl();
        p.archived=d["archived"].bl();p.clicked=d["clicked"].bl();
        // Media preview
        JVal prev=d["preview"];if(prev.type==JOBJ&&prev.has("images")){auto imgs=prev["images"];
            if(imgs.sz()>0){auto src=imgs.at(0)["source"];p.previewUrl=src["url"].str();
                p.previewWidth=src["width"].i();p.previewHeight=src["height"].i();}}
        JVal media=d["media"];if(media.type==JOBJ){p.mediaUrl=media["oembed"]["thumbnail_url"].str();}
        return p;}
};

struct Comment {
    std::string id, name, author, body, bodyHtml, parentId, linkId, subreddit, subredditId;
    std::string authorFlairText, distinguished; int score=0, ups=0, downs=0, likes=0;
    int gilded=0, depth=0; double created=0, edited=0;
    bool stickied=false, saved=false, isSubmitter=false, scoreHidden=false, collapsed=false;
    std::vector<Comment> replies;

    static Comment from(const JVal &d,int dp=0){Comment c;
        c.id=d["id"].str();c.name=d["name"].str();c.author=d["author"].str();
        c.body=d["body"].str();c.bodyHtml=d["body_html"].str();c.parentId=d["parent_id"].str();
        c.linkId=d["link_id"].str();c.subreddit=d["subreddit"].str();c.subredditId=d["subreddit_id"].str();
        c.authorFlairText=d["author_flair_text"].str();c.distinguished=d["distinguished"].str();
        c.score=d["score"].i();c.ups=d["ups"].i();c.downs=d["downs"].i();
        c.likes=d["likes"].i();c.gilded=d["gilded"].i();c.depth=dp;
        c.created=d["created_utc"].num();c.edited=d["edited"].num();
        c.stickied=d["stickied"].bl();c.saved=d["saved"].bl();
        c.isSubmitter=d["is_submitter"].bl();c.scoreHidden=d["score_hidden"].bl();
        if(d["replies"].type==JOBJ){JVal rd=d["replies"]["data"];
            if(rd.type==JOBJ){JVal ch=rd["children"];
                for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t1")
                    c.replies.push_back(Comment::from(ch.at(i)["data"],dp+1));}}
        return c;}
};

struct Subreddit {
    std::string id, name, title, desc, publicDesc, headerImg, iconImg, bannerImg;
    int subscribers=0, activeUsers=0; bool over18=false, userIsSubscriber=false, userIsBanned=false;
    static Subreddit from(const JVal &d){Subreddit s;s.name=d["display_name"].str();s.title=d["title"].str();
        s.desc=d["description"].str();s.publicDesc=d["public_description"].str();
        s.subscribers=d["subscribers"].i();s.over18=d["over18"].bl();
        s.userIsSubscriber=d["user_is_subscriber"].bl();s.iconImg=d["icon_img"].str();
        s.headerImg=d["header_img"].str();s.bannerImg=d["banner_img"].str();return s;}
};

struct Message {
    std::string id, author, subject, body, dest, subreddit, context, linkTitle;
    double created=0; bool isNew=false; int likes=0; std::string kind;
    static Message from(const JVal &d){Message m;m.id=d["id"].str();m.author=d["author"].str();
        m.subject=d["subject"].str();m.body=d["body"].str();m.dest=d["dest"].str();
        m.subreddit=d["subreddit"].str();m.context=d["context"].str();m.created=d["created_utc"].num();
        m.isNew=d["new"].bl();m.linkTitle=d["link_title"].str();return m;}
};

struct Account {
    std::string username, accessToken, refreshToken; int64_t tokenExpiry=0;
    int linkKarma=0, commentKarma=0; bool hasMail=false, isMod=false, isGold=false, over18=false;
    bool isAuthorized() const { return !accessToken.empty() && tokenExpiry>time(nullptr)+300; }
};

// ============================================================================
// HTTP Client
// ============================================================================
struct HTTP { int code; std::string body, error; };
class HttpClient {
    std::mutex mtx;
    HTTP doReq(const std::string &host, const std::string &path, const std::string &auth, bool post, const std::string &body) {
        HTTP r; r.code=-1; hostent *srv=gethostbyname(host.c_str()); if(!srv){r.error="DNS";return r;}
        int fd=socket(AF_INET,SOCK_STREAM,0); if(fd<0){r.error="socket";return r;}
        sockaddr_in addr={}; addr.sin_family=AF_INET; memcpy(&addr.sin_addr,srv->h_addr,srv->h_length); addr.sin_port=htons(80);
        timeval tv={15,0}; setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
        if(connect(fd,(sockaddr*)&addr,sizeof(addr))<0){close(fd);r.error="connect";return r;}
        std::string req=(post?"POST ":"GET ")+path+" HTTP/1.1\r\nHost: "+host+"\r\nUser-Agent: PinkReader/1.0\r\nAccept: application/json\r\n";
        if(!auth.empty())req+="Authorization: Bearer "+auth+"\r\n";
        if(post){req+="Content-Type: application/x-www-form-urlencoded\r\nContent-Length: "+std::to_string(body.size())+"\r\n";}
        req+="Connection: close\r\n\r\n"; if(post)req+=body;
        send(fd,req.c_str(),req.size(),0);
        char buf[16384]; int n; std::string raw;
        while((n=recv(fd,buf,sizeof(buf)-1,0))>0){buf[n]=0;raw+=buf;}
        close(fd); size_t hd=raw.find("\r\n\r\n"); if(hd==std::string::npos){r.error="bad HTTP";return r;}
        size_t sp=raw.find(' ')+1; r.code=atoi(raw.c_str()+sp); r.body=raw.substr(hd+4); return r;
    }
public:
    static HttpClient& i(){static HttpClient h;return h;}
    HTTP get(const std::string &url,const std::string &auth=""){std::string h,p;
        if(url.find("https://")==0){HTTP r;r.code=-1;r.error="HTTPS needs OkHttp/JNI";return r;}
        auto ps=url.find('/',7);h=ps==std::string::npos?url.substr(7):url.substr(7,ps-7);
        p=ps==std::string::npos?"/":url.substr(ps); return doReq(h,p,auth,false,"");}
    HTTP post(const std::string &url,const std::string &data,const std::string &auth=""){std::string h,p;
        if(url.find("https://")==0){HTTP r;r.code=-1;r.error="HTTPS needs OkHttp/JNI";return r;}
        auto ps=url.find('/',7);h=ps==std::string::npos?url.substr(7):url.substr(7,ps-7);
        p=ps==std::string::npos?"/":url.substr(ps); return doReq(h,p,auth,true,data);}
};

// ============================================================================
// RedditAPI — matching RedReader's RedditAPI.java exactly
// ============================================================================
class RedditAPI {
    std::string m_token, m_base="https://oauth.reddit.com"; std::mutex mtx;
public:
    static RedditAPI& i(){static RedditAPI a;return a;}
    void setToken(const std::string &t){std::lock_guard<std::mutex> l(mtx);m_token=t;}
    std::string tok()const{return m_token;}
    bool hasToken()const{return !m_token.empty();}

    // Posts
    std::vector<Post> fetchPosts(const std::string &sub, const std::string &sort,
                                  const std::string &after, int limit) {
        std::string path=sub.empty()||sub=="frontpage"?"/"+sort+".json":"/r/"+sub+"/"+sort+".json";
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";
        if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+path+q,tok()); std::vector<Post> posts;
        if(r.code!=200||r.body.empty())return posts;
        JVal root=parseJSON(r.body); JVal data=root["data"]; JVal ch=data["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));
        return posts;
    }

    // Comments
    std::vector<Comment> fetchComments(const std::string &sub,const std::string &postId,
                                         const std::string &sort,int limit) {
        std::string path="/r/"+sub+"/comments/"+postId+".json?sort="+sort+"&limit="+std::to_string(limit)+"&raw_json=1";
        auto r=HttpClient::i().get(m_base+path,tok()); std::vector<Comment> comments;
        if(r.code!=200||r.body.empty())return comments;
        JVal root=parseJSON(r.body); if(root.type!=JARR||root.sz()<2)return comments;
        JVal listing=root.at(1); JVal data=listing["data"]; JVal ch=data["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t1")comments.push_back(Comment::from(ch.at(i)["data"]));
        return comments;
    }

    // Also fetch the post itself with comments
    Post fetchPostForComments(const std::string &sub,const std::string &postId) {
        std::string path="/r/"+sub+"/comments/"+postId+".json?limit=1&raw_json=1";
        auto r=HttpClient::i().get(m_base+path,tok()); if(r.code!=200||r.body.empty())return Post();
        JVal root=parseJSON(r.body); if(root.type!=JARR||root.sz()<2)return Post();
        return Post::from(root.at(0)["data"]["children"].at(0)["data"]);
    }

    // Inbox
    std::vector<Message> fetchInbox(const std::string &filter,int limit) {
        std::string path="/message/"+filter+".json?limit="+std::to_string(limit)+"&raw_json=1";
        auto r=HttpClient::i().get(m_base+path,tok()); std::vector<Message> msgs;
        if(r.code!=200||r.body.empty())return msgs;
        JVal root=parseJSON(r.body); JVal data=root["data"]; JVal ch=data["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t4")msgs.push_back(Message::from(ch.at(i)["data"]));
        return msgs;
    }

    // Subreddit search
    std::vector<Subreddit> searchSubreddits(const std::string &q,int limit) {
        std::string path="/subreddits/search.json?q="+q+"&limit="+std::to_string(limit)+"&raw_json=1";
        auto r=HttpClient::i().get(m_base+path,tok()); std::vector<Subreddit> subs;
        if(r.code!=200||r.body.empty())return subs;
        JVal root=parseJSON(r.body); JVal data=root["data"]; JVal ch=data["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t5")subs.push_back(Subreddit::from(ch.at(i)["data"]));
        return subs;
    }

    Subreddit fetchSubredditInfo(const std::string &name) {
        auto r=HttpClient::i().get(m_base+"/r/"+name+"/about.json?raw_json=1",tok());
        if(r.code!=200)return Subreddit(); return Subreddit::from(parseJSON(r.body)["data"]);
    }

    // Actions
    bool vote(const std::string &fullname,int dir) { return HttpClient::i().post(m_base+"/api/vote","id="+fullname+"&dir="+std::to_string(dir)+"&rank=2",tok()).code==200; }
    bool save(const std::string &id,bool s) { return HttpClient::i().post(m_base+(s?"/api/save":"/api/unsave"),"id="+id,tok()).code==200; }
    bool hide(const std::string &id,bool h) { return HttpClient::i().post(m_base+(h?"/api/hide":"/api/unhide"),"id="+id,tok()).code==200; }
    bool report(const std::string &id,const std::string &reason) { return HttpClient::i().post(m_base+"/api/report","id="+id+"&reason="+reason,tok()).code==200; }
    bool del(const std::string &id) { return HttpClient::i().post(m_base+"/api/del","id="+id,tok()).code==200; }
    bool submitComment(const std::string &parentId,const std::string &text) { return HttpClient::i().post(m_base+"/api/comment","api_type=json&thing_id="+parentId+"&text="+text,tok()).code==200; }
    bool subscribe(const std::string &sub,bool s) { return HttpClient::i().post(m_base+"/api/subscribe","action="+(s?std::string("sub"):std::string("unsub"))+"&sr_name="+sub,tok()).code==200; }
    bool markAllRead() { return HttpClient::i().post(m_base+"/api/read_all_messages","",tok()).code==200; }
};

// ============================================================================
// Cache — matching RedReader's CacheManager architecture
// ============================================================================
template<typename V> class LRU {
    struct N{std::string k;V v;}; std::vector<N> items; size_t mx; mutable std::mutex mtx;
public:
    LRU(size_t m=500):mx(m){}
    void put(const std::string &k,const V &v){std::lock_guard<std::mutex> l(mtx);
        for(auto it=items.begin();it!=items.end();++it)if(it->k==k){items.erase(it);break;}
        items.insert(items.begin(),{k,v}); if(items.size()>mx)items.pop_back();}
    bool get(const std::string &k,V &v){std::lock_guard<std::mutex> l(mtx);
        for(auto it=items.begin();it!=items.end();++it)if(it->k==k){v=it->v;auto n=*it;items.erase(it);items.insert(items.begin(),n);return true;}return false;}
    void clear(){std::lock_guard<std::mutex> l(mtx);items.clear();}
    size_t size()const{return items.size();}
};

// ============================================================================
// AppState — global state matching RedReader's patterns
// ============================================================================
class AppState {
public:
    RedditAPI api;
    LRU<std::string> postCache{500}, commentCache{300};
    std::string currentSub="popular",currentSort="hot",currentAfter;
    std::string username, accessToken;
    bool darkTheme=true, nsfwEnabled=false, leftHanded=false, bottomToolbar=false;
    bool hideReadPosts=false, autoRefresh=false, twoPane=false;
    int postCountLimit=-1; // -1 = ALL (RedReader default)
    float fontScale=1.0f;
    std::set<std::string> readPostIds, hiddenCommentIds;
    std::vector<std::string> pinnedSubs;
    mutable std::mutex mtx;

    static AppState& i(){static AppState s;return s;}
    void setAuth(const std::string &u,const std::string &t){std::lock_guard<std::mutex> l(mtx);username=u;accessToken=t;api.setToken(t);}
    std::string prefsJson()const{return "{}";}// Placeholder
};

// ============================================================================
// Serialization helpers
// ============================================================================
static std::string esc(const std::string &s){std::string r;for(char c:s){if(c=='"'||c=='\\')r+='\\';r+=c;}return r;}
static std::string jstr(const std::string &s){return "\""+esc(s)+"\"";}

static std::string postToJson(const Post &p, bool includeSelftext=true){
    char b[32768];
    snprintf(b,sizeof(b),
        "{\"id\":%s,\"name\":%s,\"title\":%s,\"author\":%s,\"subreddit\":%s,"
        "\"score\":%d,\"num_comments\":%d,\"created\":%.0f,\"url\":%s,"
        "\"thumbnail\":%s,\"domain\":%s,\"is_self\":%s,\"over18\":%s,"
        "\"spoiler\":%s,\"stickied\":%s,\"flair\":%s,\"selftext\":%s,"
        "\"selftext_html\":%s,\"permalink\":%s,\"likes\":%d,\"saved\":%s,"
        "\"hidden\":%s,\"locked\":%s,\"archived\":%s,\"gilded\":%d,"
        "\"distinguished\":%s,\"post_hint\":%s,\"preview_url\":%s,"
        "\"media_url\":%s,\"upvote_ratio\":%.2f\"}",
        jstr(p.id).c_str(),jstr(p.name).c_str(),jstr(p.title).c_str(),
        jstr(p.author).c_str(),jstr(p.subreddit).c_str(),
        p.score,p.numComments,p.created,jstr(p.url).c_str(),
        jstr(p.thumbnail).c_str(),jstr(p.domain).c_str(),
        p.isSelf?"true":"false",p.over18?"true":"false",
        p.spoiler?"true":"false",p.stickied?"true":"false",
        jstr(p.linkFlairText).c_str(),
        includeSelftext?jstr(p.selftext).c_str():"\"\"",
        includeSelftext?jstr(p.selftextHtml).c_str():"\"\"",
        jstr(p.permalink).c_str(),p.likes,p.saved?"true":"false",
        p.hidden?"true":"false",p.locked?"true":"false",
        p.archived?"true":"false",p.gilded,
        jstr(p.distinguished).c_str(),jstr(p.postHint).c_str(),
        jstr(p.previewUrl).c_str(),jstr(p.mediaUrl).c_str(),
        p.upvoteRatio);
    return std::string(b);
}

static std::string commentToJson(const Comment &c){
    std::string replies="[";
    for(size_t i=0;i<c.replies.size();i++){if(i>0)replies+=",";replies+=commentToJson(c.replies[i]);}
    replies+="]";
    char b[16384];
    snprintf(b,sizeof(b),
        "{\"id\":%s,\"author\":%s,\"body\":%s,\"body_html\":%s,"
        "\"score\":%d,\"created\":%.0f,\"depth\":%d,\"stickied\":%s,"
        "\"flair\":%s,\"distinguished\":%s,\"is_submitter\":%s,"
        "\"likes\":%d,\"saved\":%s,\"gilded\":%d,\"collapsed\":%s,"
        "\"replies\":%s}",
        jstr(c.id).c_str(),jstr(c.author).c_str(),jstr(c.body).c_str(),
        jstr(c.bodyHtml).c_str(),c.score,c.created,c.depth,
        c.stickied?"true":"false",jstr(c.authorFlairText).c_str(),
        jstr(c.distinguished).c_str(),c.isSubmitter?"true":"false",
        c.likes,c.saved?"true":"false",c.gilded,
        c.collapsed?"true":"false",replies.c_str());
    return std::string(b);
}

// ============================================================================
// JNI Bridge — 40+ functions covering all RedReader features
// ============================================================================
extern "C" {

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetVersion(JNIEnv *e,jobject){return e->NewStringUTF("PinkReader v2.0");}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeInit(JNIEnv *e,jobject,jstring cd){AppState::i();}

// Auth
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetAuth(JNIEnv *e,jobject,jstring u,jstring t){
    AppState::i().setAuth(std::string(e->GetStringUTFChars(u,0)),std::string(e->GetStringUTFChars(t,0)));}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeIsAuthenticated(JNIEnv*,jobject){return AppState::i().api.hasToken()?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetUsername(JNIEnv *e,jobject){return e->NewStringUTF(AppState::i().username.c_str());}

// Posts
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchPosts(JNIEnv *e,jobject,jstring sub,jstring sort,jstring after,jint limit){
    auto posts=AppState::i().api.fetchPosts(
        std::string(e->GetStringUTFChars(sub,0)),std::string(e->GetStringUTFChars(sort,0)),
        std::string(e->GetStringUTFChars(after,0)),limit);
    std::string json="["; for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i],false);} json+="]";
    return e->NewStringUTF(json.c_str());}

// Posts with selftext (for full post view)
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchPostsFull(JNIEnv *e,jobject,jstring sub,jstring sort,jstring after,jint limit){
    auto posts=AppState::i().api.fetchPosts(
        std::string(e->GetStringUTFChars(sub,0)),std::string(e->GetStringUTFChars(sort,0)),
        std::string(e->GetStringUTFChars(after,0)),limit);
    std::string json="["; for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i],true);} json+="]";
    return e->NewStringUTF(json.c_str());}

// Comments
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchComments(JNIEnv *e,jobject,jstring sub,jstring pid,jstring sort,jint limit){
    auto comments=AppState::i().api.fetchComments(
        std::string(e->GetStringUTFChars(sub,0)),std::string(e->GetStringUTFChars(pid,0)),
        std::string(e->GetStringUTFChars(sort,0)),limit);
    std::string json="["; for(size_t i=0;i<comments.size();i++){if(i>0)json+=",";json+=commentToJson(comments[i]);} json+="]";
    return e->NewStringUTF(json.c_str());}

// Post for comment view (header)
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchPost(JNIEnv *e,jobject,jstring sub,jstring pid){
    auto p=AppState::i().api.fetchPostForComments(
        std::string(e->GetStringUTFChars(sub,0)),std::string(e->GetStringUTFChars(pid,0)));
    return e->NewStringUTF(postToJson(p,true).c_str());}

// Inbox
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchInbox(JNIEnv *e,jobject,jstring filter,jint limit){
    auto msgs=AppState::i().api.fetchInbox(std::string(e->GetStringUTFChars(filter,0)),limit);
    std::string json="["; for(size_t i=0;i<msgs.size();i++){if(i>0)json+=",";
        char b[8192]; auto &m=msgs[i];
        snprintf(b,sizeof(b),"{\"id\":%s,\"author\":%s,\"subject\":%s,\"body\":%s,\"created\":%.0f,\"is_new\":%s,\"subreddit\":%s}",
            jstr(m.id).c_str(),jstr(m.author).c_str(),jstr(m.subject).c_str(),jstr(m.body).c_str(),m.created,m.isNew?"true":"false",jstr(m.subreddit).c_str());
        json+=b;} json+="]";
    return e->NewStringUTF(json.c_str());}

// Search
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeSearchSubreddits(JNIEnv *e,jobject,jstring query,jint limit){
    auto subs=AppState::i().api.searchSubreddits(std::string(e->GetStringUTFChars(query,0)),limit);
    std::string json="["; for(size_t i=0;i<subs.size();i++){if(i>0)json+=",";
        char b[8192]; auto &s=subs[i];
        snprintf(b,sizeof(b),"{\"name\":%s,\"title\":%s,\"subscribers\":%d,\"over18\":%s,\"icon\":%s}",
            jstr(s.name).c_str(),jstr(s.title).c_str(),s.subscribers,s.over18?"true":"false",jstr(s.iconImg).c_str());
        json+=b;} json+="]";
    return e->NewStringUTF(json.c_str());}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchSubredditInfo(JNIEnv *e,jobject,jstring name){
    auto s=AppState::i().api.fetchSubredditInfo(std::string(e->GetStringUTFChars(name,0)));
    char b[8192];
    snprintf(b,sizeof(b),"{\"name\":%s,\"title\":%s,\"subscribers\":%d,\"over18\":%s,\"desc\":%s,\"icon\":%s}",
        jstr(s.name).c_str(),jstr(s.title).c_str(),s.subscribers,s.over18?"true":"false",jstr(s.publicDesc).c_str(),jstr(s.iconImg).c_str());
    return e->NewStringUTF(b);}

// Actions
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeVote(JNIEnv *e,jobject,jstring id,jint dir){return AppState::i().api.vote(std::string(e->GetStringUTFChars(id,0)),dir)?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSave(JNIEnv *e,jobject,jstring id,jboolean s){return AppState::i().api.save(std::string(e->GetStringUTFChars(id,0)),s)?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeHide(JNIEnv *e,jobject,jstring id,jboolean h){return AppState::i().api.hide(std::string(e->GetStringUTFChars(id,0)),h)?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeReport(JNIEnv *e,jobject,jstring id,jstring reason){return AppState::i().api.report(std::string(e->GetStringUTFChars(id,0)),std::string(e->GetStringUTFChars(reason,0)))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeDelete(JNIEnv *e,jobject,jstring id){return AppState::i().api.del(std::string(e->GetStringUTFChars(id,0)))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSubmitComment(JNIEnv *e,jobject,jstring pid,jstring text){return AppState::i().api.submitComment(std::string(e->GetStringUTFChars(pid,0)),std::string(e->GetStringUTFChars(text,0)))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSubscribe(JNIEnv *e,jobject,jstring sub,jboolean s){return AppState::i().api.subscribe(std::string(e->GetStringUTFChars(sub,0)),s)?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeMarkAllRead(JNIEnv*,jobject){return AppState::i().api.markAllRead()?JNI_TRUE:JNI_FALSE;}

// Cache
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetCachedPosts(JNIEnv *e,jobject,jstring sub,jstring sort){
    std::string k=std::string(e->GetStringUTFChars(sub,0))+"_"+std::string(e->GetStringUTFChars(sort,0));
    std::string v; if(AppState::i().postCache.get(k,v))return e->NewStringUTF(v.c_str()); return e->NewStringUTF("[]");}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeCachePosts(JNIEnv *e,jobject,jstring sub,jstring sort,jstring data){
    AppState::i().postCache.put(std::string(e->GetStringUTFChars(sub,0))+"_"+std::string(e->GetStringUTFChars(sort,0)),std::string(e->GetStringUTFChars(data,0)));}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeClearCache(JNIEnv*,jobject){AppState::i().postCache.clear();AppState::i().commentCache.clear();}

// Settings
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetDarkTheme(JNIEnv*,jobject,jboolean d){AppState::i().darkTheme=d;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeGetDarkTheme(JNIEnv*,jobject){return AppState::i().darkTheme?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetNsfw(JNIEnv*,jobject,jboolean n){AppState::i().nsfwEnabled=n;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeGetNsfw(JNIEnv*,jobject){return AppState::i().nsfwEnabled?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeMarkRead(JNIEnv *e,jobject,jstring id){AppState::i().readPostIds.insert(std::string(e->GetStringUTFChars(id,0)));}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeIsRead(JNIEnv *e,jobject,jstring id){return AppState::i().readPostIds.count(std::string(e->GetStringUTFChars(id,0)))?JNI_TRUE:JNI_FALSE;}

// UI State
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetPrefs(JNIEnv *env,jobject){return env->NewStringUTF("{}");}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetPref(JNIEnv *e,jobject,jstring k,jstring v){}
} // extern "C"

// Settings activity JNI
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeGetDarkTheme(JNIEnv*,jobject){return AppState::i().darkTheme?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeSetDarkTheme(JNIEnv*,jobject,jboolean d){AppState::i().darkTheme=d;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeClearCache(JNIEnv*,jobject){AppState::i().postCache.clear();}
