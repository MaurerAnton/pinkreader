// PinkReader Native Engine v4 — Pure JSON processor
// Kotlin handles all HTTPS via OkHttp. C++ handles all JSON parsing + business logic.
// C++20, Android NDK 27

#include <jni.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <mutex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <sstream>

// ============================================================================
// JSON Parser
// ============================================================================
enum JType { JNULL, JBOOL, JNUM, JSTR, JARR, JOBJ };
struct JVal {
    JType type=JNULL; bool b=false; double n=0; std::string s;
    std::vector<JVal> arr; std::map<std::string,JVal> obj;
    bool has(const std::string &k) const { return type==JOBJ&&obj.count(k); }
    JVal operator[](const std::string &k) const { auto i=obj.find(k); return i!=obj.end()?i->second:JVal(); }
    std::string str() const { return s; } double num() const { return n; } int i() const { return (int)n; }
    int64_t i64() const { return (int64_t)n; } bool bl() const { return b; }
    int sz() const { return type==JARR?(int)arr.size():type==JOBJ?(int)obj.size():0; }
    JVal at(int i) const { return (type==JARR&&i>=0&&i<sz())?arr[i]:JVal(); }
};
struct JParser {
    const char *p,*e; JParser(const char *s,int l):p(s),e(s+l){}
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
            case'u':{if(e-p<5)break;char h[5]={p[1],p[2],p[3],p[4],0};unsigned cp=strtoul(h,0,16);p+=4;
                if(cp<0x80)v.s+=(char)cp;else if(cp<0x800){v.s+=(char)(0xC0|cp>>6);v.s+=(char)(0x80|(cp&0x3F));}
                else if(cp<=0xFFFF){v.s+=(char)(0xE0|cp>>12);v.s+=(char)(0x80|(cp>>6&0x3F));v.s+=(char)(0x80|(cp&0x3F));}}}}}
            else v.s+=*p;p++;}if(p<e)p++;return v;}
    JVal parr(){JVal v;v.type=JARR;p++;ws();if(p<e&&*p==']'){p++;return v;}
        while(1){v.arr.push_back(parse());ws();if(p<e&&*p==','){p++;continue;}if(p<e&&*p==']'){p++;break;}break;}return v;}
    JVal pobj(){JVal v;v.type=JOBJ;p++;ws();if(p<e&&*p=='}'){p++;return v;}
        while(1){JVal k=pstr();ws();if(p<e&&*p==':')p++;ws();v.obj[k.s]=parse();
            ws();if(p<e&&*p==','){p++;ws();continue;}if(p<e&&*p=='}'){p++;break;}break;}return v;}
};
JVal parseJSON(const std::string &s){if(s.empty())return JVal();return JParser(s.c_str(),s.size()).parse();}

// ============================================================================
// Data Models
// ============================================================================
struct Post { std::string id,name,title,author,subreddit,subredditId,selftext,selftextHtml,url,permalink,thumbnail,domain,linkFlairText,authorFlairText,distinguished,postHint; int score,ups,downs,numComments,likes,gilded; double created,edited,upvoteRatio; bool over18,spoiler,stickied,saved,hidden,isSelf,locked,archived;
    static Post from(const JVal &d){Post p;p.id=d["id"].str();p.name=d["name"].str();p.title=d["title"].str();p.author=d["author"].str();p.subreddit=d["subreddit"].str();p.selftext=d["selftext"].str();p.url=d["url"].str();p.permalink=d["permalink"].str();p.thumbnail=d["thumbnail"].str();p.domain=d["domain"].str();p.linkFlairText=d["link_flair_text"].str();p.authorFlairText=d["author_flair_text"].str();p.distinguished=d["distinguished"].str();p.postHint=d["post_hint"].str();p.score=d["score"].i();p.numComments=d["num_comments"].i();p.likes=d["likes"].i();p.gilded=d["gilded"].i();p.created=d["created_utc"].num();p.over18=d["over_18"].bl();p.spoiler=d["spoiler"].bl();p.stickied=d["stickied"].bl();p.saved=d["saved"].bl();p.hidden=d["hidden"].bl();p.isSelf=d["is_self"].bl();p.locked=d["locked"].bl();p.archived=d["archived"].bl();p.upvoteRatio=d["upvote_ratio"].num();return p;}};

struct Comment { std::string id,author,body,bodyHtml,parentId,subreddit,authorFlairText,distinguished; int score,likes,gilded,depth; double created; bool stickied,isSubmitter,collapsed; std::vector<Comment> replies;
    static Comment from(const JVal &d,int dp=0){Comment c;c.id=d["id"].str();c.author=d["author"].str();c.body=d["body"].str();c.bodyHtml=d["body_html"].str();c.parentId=d["parent_id"].str();c.subreddit=d["subreddit"].str();c.authorFlairText=d["author_flair_text"].str();c.distinguished=d["distinguished"].str();c.score=d["score"].i();c.gilded=d["gilded"].i();c.depth=dp;c.created=d["created_utc"].num();c.stickied=d["stickied"].bl();c.isSubmitter=d["is_submitter"].bl();if(d["replies"].type==JOBJ){JVal rd=d["replies"]["data"];if(rd.type==JOBJ){JVal ch=rd["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t1")c.replies.push_back(Comment::from(ch.at(i)["data"],dp+1));}}return c;}};

struct Subreddit { std::string name,title,desc,iconImg; int subscribers; bool over18,userIsSubscriber;
    static Subreddit from(const JVal &d){Subreddit s;s.name=d["display_name"].str();s.title=d["title"].str();s.desc=d["description"].str();s.subscribers=d["subscribers"].i();s.over18=d["over18"].bl();s.userIsSubscriber=d["user_is_subscriber"].bl();s.iconImg=d["icon_img"].str();return s;}};

struct Message { std::string id,author,subject,body,subreddit,dest; double created; bool isNew;
    static Message from(const JVal &d){Message m;m.id=d["id"].str();m.author=d["author"].str();m.subject=d["subject"].str();m.body=d["body"].str();m.subreddit=d["subreddit"].str();m.dest=d["dest"].str();m.created=d["created_utc"].num();m.isNew=d["new"].bl();return m;}};

// ============================================================================
// Cache
// ============================================================================
template<typename V> class LRU { struct N{std::string k;V v;}; std::vector<N> items; size_t mx; mutable std::mutex mtx;
public:
    LRU(size_t m=500):mx(m){}
    void put(const std::string &k,const V &v){std::lock_guard<std::mutex> l(mtx);for(auto it=items.begin();it!=items.end();++it)if(it->k==k){items.erase(it);break;}items.insert(items.begin(),{k,v});if(items.size()>mx)items.pop_back();}
    bool get(const std::string &k,V &v){std::lock_guard<std::mutex> l(mtx);for(auto it=items.begin();it!=items.end();++it)if(it->k==k){v=it->v;auto n=*it;items.erase(it);items.insert(items.begin(),n);return true;}return false;}
    void clear(){std::lock_guard<std::mutex> l(mtx);items.clear();}
};

class AppState {
public:
    LRU<std::string> postCache{500}, commentCache{300};
    std::string username,accessToken,currentSub="popular",currentSort="hot";
    bool darkTheme=true,nsfwEnabled=false; std::set<std::string> readPostIds;
    mutable std::mutex mtx;
    static AppState& i(){static AppState s;return s;}
};

// ============================================================================
// Serialization
// ============================================================================
static std::string esc(const std::string &s){std::string r;r.reserve(s.size()+32);for(char c:s){if(c=='\"'||c=='\\\\')r+='\\\\';r+=c;}return r;}
static std::string jstr(const std::string &s){if(s.size()>32000)return "\"<string too long>\"";return "\""+esc(s)+"\"";}

static std::string postToJson(const Post &p,bool full=false){
    std::string r="{\"id\":"+jstr(p.id)+",\"name\":"+jstr(p.name)+
        ",\"title\":"+jstr(p.title)+",\"author\":"+jstr(p.author)+
        ",\"subreddit\":"+jstr(p.subreddit)+",\"score\":"+std::to_string(p.score)+
        ",\"num_comments\":"+std::to_string(p.numComments)+
        ",\"created\":"+std::to_string((int64_t)p.created)+
        ",\"url\":"+jstr(p.url)+",\"thumbnail\":"+jstr(p.thumbnail)+
        ",\"domain\":"+jstr(p.domain)+",\"is_self\":"+(p.isSelf?"true":"false")+
        ",\"over18\":"+(p.over18?"true":"false")+
        ",\"spoiler\":"+(p.spoiler?"true":"false")+
        ",\"stickied\":"+(p.stickied?"true":"false")+
        ",\"flair\":"+jstr(p.linkFlairText)+
        ",\"selftext\":"+(full?jstr(p.selftext):"\"\"")+
        ",\"permalink\":"+jstr(p.permalink)+
        ",\"likes\":"+std::to_string(p.likes)+
        ",\"saved\":"+(p.saved?"true":"false")+
        ",\"hidden\":"+(p.hidden?"true":"false")+
        ",\"locked\":"+(p.locked?"true":"false")+
        ",\"archived\":"+(p.archived?"true":"false")+
        ",\"gilded\":"+std::to_string(p.gilded)+
        ",\"distinguished\":"+jstr(p.distinguished)+
        ",\"post_hint\":"+jstr(p.postHint)+
        ",\"upvote_ratio\":"+std::to_string(p.upvoteRatio)+"}";
    return r;}

static std::string commentToJson(const Comment &c,int depth=0){
    if(depth>200)return "{}";  // Fix: recursion depth cap
    std::string replies="[";for(size_t i=0;i<c.replies.size();i++){if(i>0)replies+=",";replies+=commentToJson(c.replies[i],depth+1);}replies+="]";
    return "{\"id\":"+jstr(c.id)+",\"author\":"+jstr(c.author)+",\"body\":"+jstr(c.body)+
        ",\"score\":"+std::to_string(c.score)+",\"created\":"+std::to_string((int64_t)c.created)+
        ",\"depth\":"+std::to_string(c.depth)+",\"stickied\":"+(c.stickied?"true":"false")+
        ",\"flair\":"+jstr(c.authorFlairText)+",\"distinguished\":"+jstr(c.distinguished)+
        ",\"is_submitter\":"+(c.isSubmitter?"true":"false")+",\"likes\":"+std::to_string(c.likes)+
        ",\"gilded\":"+std::to_string(c.gilded)+",\"collapsed\":"+(c.collapsed?"true":"false")+
        ",\"replies\":"+replies+"}";}

static std::string msgToJson(const Message &m){
    return "{\"id\":"+jstr(m.id)+",\"author\":"+jstr(m.author)+",\"subject\":"+jstr(m.subject)+
        ",\"body\":"+jstr(m.body)+",\"created\":"+std::to_string((int64_t)m.created)+
        ",\"is_new\":"+(m.isNew?"true":"false")+",\"subreddit\":"+jstr(m.subreddit)+"}";}
static std::string subToJson(const Subreddit &s){
    return "{\"name\":"+jstr(s.name)+",\"title\":"+jstr(s.title)+
        ",\"subscribers\":"+std::to_string(s.subscribers)+
        ",\"over18\":"+(s.over18?"true":"false")+
        ",\"icon\":"+jstr(s.iconImg)+",\"desc\":"+jstr(s.desc)+"}";}

// Helper
static std::string js(JNIEnv *e,jstring s){if(!s)return"";const char *c=e->GetStringUTFChars(s,0);std::string r(c);e->ReleaseStringUTFChars(s,c);return r;}
static jstring tj(JNIEnv *e,const std::string &s){return e->NewStringUTF(s.c_str());}

// ============================================================================
// JSON Parsing Functions (pure — no network)
// ============================================================================

// Parse Reddit listing response: {"kind":"Listing","data":{"children":[...]}}
static std::vector<Post> parseListing(const std::string &json){
    std::vector<Post> posts; JVal root=parseJSON(json); JVal ch=root["data"]["children"];
    for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"])); return posts;}

static std::vector<Post> parseUserListing(const std::string &json){
    std::vector<Post> posts; JVal root=parseJSON(json); JVal ch=root["data"]["children"];
    for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"])); return posts;}

static std::vector<Comment> parseCommentListing(const std::string &json){
    std::vector<Comment> cs; JVal root=parseJSON(json); if(root.type!=JARR||root.sz()<2)return cs;
    JVal ch=root.at(1)["data"]["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t1")cs.push_back(Comment::from(ch.at(i)["data"]));return cs;}

static Post parsePostFromComments(const std::string &json){
    JVal root=parseJSON(json); if(root.type!=JARR||root.sz()<2)return Post();
    return Post::from(root.at(0)["data"]["children"].at(0)["data"]);}

static std::vector<Message> parseInbox(const std::string &json){
    std::vector<Message> msgs; JVal root=parseJSON(json); JVal ch=root["data"]["children"];
    for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t4")msgs.push_back(Message::from(ch.at(i)["data"]));return msgs;}

static std::vector<Subreddit> parseSubredditSearch(const std::string &json){
    std::vector<Subreddit> subs; JVal root=parseJSON(json); JVal ch=root["data"]["children"];
    for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t5")subs.push_back(Subreddit::from(ch.at(i)["data"]));return subs;}

static std::vector<Subreddit> parseSubredditList(const std::string &json){
    std::vector<Subreddit> subs; JVal root=parseJSON(json); JVal ch=root["data"]["children"];
    for(int i=0;i<ch.sz();i++)subs.push_back(Subreddit::from(ch.at(i)["data"]));return subs;}

static Subreddit parseSubredditInfo(const std::string &json){return Subreddit::from(parseJSON(json)["data"]);}

// ============================================================================
// URL Builders — construct Reddit API URLs (called by Kotlin for HTTPS)
// ============================================================================
extern "C" {
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeBuildPostsUrl(JNIEnv *e,jobject,jstring sub,jstring sort,jstring after,jint limit){
    std::string s=js(e,sub); if(s=="frontpage"||s=="")s="";
    std::string so=js(e,sort); if(so.empty())so="hot";  // Fix: null/empty sort guard
    std::string path=s.empty()?"/"+so+".json":"/r/"+s+"/"+so+".json";
    std::string q="?limit="+std::to_string(limit)+"&raw_json=1"; std::string af=js(e,after);
    if(!af.empty()&&af!="null")q+="&after="+af;
    return tj(e,"https://oauth.reddit.com"+path+q);}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeBuildCommentsUrl(JNIEnv *e,jobject,jstring sub,jstring pid,jstring sort,jint limit){
    return tj(e,"https://oauth.reddit.com/r/"+js(e,sub)+"/comments/"+js(e,pid)+".json?sort="+js(e,sort)+"&limit="+std::to_string(limit)+"&raw_json=1");}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeBuildSearchUrl(JNIEnv *e,jobject,jstring sub,jstring query,jstring sort,jstring after,jint limit){
    std::string af=js(e,after); std::string q="?q="+js(e,query)+"&sort="+js(e,sort)+"&limit="+std::to_string(limit)+"&restrict_sr=on&raw_json=1";
    if(!af.empty())q+="&after="+af; return tj(e,"https://oauth.reddit.com/r/"+js(e,sub)+"/search.json"+q);}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeBuildInboxUrl(JNIEnv *e,jobject,jstring filter,jint limit){
    return tj(e,"https://oauth.reddit.com/message/"+js(e,filter)+".json?limit="+std::to_string(limit)+"&raw_json=1");}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeBuildSubredditSearchUrl(JNIEnv *e,jobject,jstring query,jint limit){
    return tj(e,"https://oauth.reddit.com/subreddits/search.json?q="+js(e,query)+"&limit="+std::to_string(limit)+"&raw_json=1");}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeBuildActionUrl(JNIEnv *e,jobject,jstring action,jstring id){
    return tj(e,"https://oauth.reddit.com/api/"+js(e,action));}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeBuildActionBody(JNIEnv *e,jobject,jstring id,jstring extra){
    std::string body="id="+js(e,id); std::string ex=js(e,extra); if(!ex.empty())body+="&"+ex; return tj(e,body);}

// ============================================================================
// Parse Functions — take raw JSON, return structured JSON
// ============================================================================
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeParsePostListing(JNIEnv *e,jobject,jstring json){
    auto posts=parseListing(js(e,json)); std::string out="[";for(size_t i=0;i<posts.size();i++){if(i>0)out+=",";out+=postToJson(posts[i]);}out+="]";return tj(e,out);}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeParsePostListingFull(JNIEnv *e,jobject,jstring json){
    auto posts=parseListing(js(e,json)); std::string out="[";for(size_t i=0;i<posts.size();i++){if(i>0)out+=",";out+=postToJson(posts[i],true);}out+="]";return tj(e,out);}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeParsePost(JNIEnv *e,jobject,jstring json){return tj(e,postToJson(parsePostFromComments(js(e,json)),true));}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeParseComments(JNIEnv *e,jobject,jstring json){
    auto cs=parseCommentListing(js(e,json)); std::string out="[";for(size_t i=0;i<cs.size();i++){if(i>0)out+=",";out+=commentToJson(cs[i]);}out+="]";return tj(e,out);}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeParseInbox(JNIEnv *e,jobject,jstring json){
    auto msgs=parseInbox(js(e,json)); std::string out="[";for(size_t i=0;i<msgs.size();i++){if(i>0)out+=",";out+=msgToJson(msgs[i]);}out+="]";return tj(e,out);}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeParseSubredditSearch(JNIEnv *e,jobject,jstring json){
    auto subs=parseSubredditSearch(js(e,json)); std::string out="[";for(size_t i=0;i<subs.size();i++){if(i>0)out+=",";out+=subToJson(subs[i]);}out+="]";return tj(e,out);}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeParseSubredditInfo(JNIEnv *e,jobject,jstring json){return tj(e,subToJson(parseSubredditInfo(js(e,json))));}

JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeParseSubredditList(JNIEnv *e,jobject,jstring json){
    auto subs=parseSubredditList(js(e,json)); std::string out="[";for(size_t i=0;i<subs.size();i++){if(i>0)out+=",";out+=subToJson(subs[i]);}out+="]";return tj(e,out);}

// ============================================================================
// Cache
// ============================================================================
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetCached(JNIEnv *e,jobject,jstring key){std::string v;if(AppState::i().postCache.get(js(e,key),v))return tj(e,v);return tj(e,"");}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativePutCache(JNIEnv *e,jobject,jstring key,jstring val){AppState::i().postCache.put(js(e,key),js(e,val));}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeClearCache(JNIEnv*,jobject){AppState::i().postCache.clear();}

// ============================================================================
// Settings / State
// ============================================================================
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetVersion(JNIEnv *e,jobject){return tj(e,"PinkReader v4.0 (OkHttp + C++ parser)");}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeInit(JNIEnv *,jobject,jstring){AppState::i();}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetAuth(JNIEnv *e,jobject,jstring u,jstring t){auto &s=AppState::i();s.username=js(e,u);s.accessToken=js(e,t);}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeIsAuthenticated(JNIEnv*,jobject){return !AppState::i().accessToken.empty()?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetUsername(JNIEnv *e,jobject){return tj(e,AppState::i().username);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetToken(JNIEnv *e,jobject){return tj(e,AppState::i().accessToken);}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetDarkTheme(JNIEnv*,jobject,jboolean d){AppState::i().darkTheme=d;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeGetDarkTheme(JNIEnv*,jobject){return AppState::i().darkTheme?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetNsfw(JNIEnv*,jobject,jboolean n){AppState::i().nsfwEnabled=n;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeGetNsfw(JNIEnv*,jobject){return AppState::i().nsfwEnabled?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeMarkRead(JNIEnv *e,jobject,jstring id){AppState::i().readPostIds.insert(js(e,id));}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeIsRead(JNIEnv *e,jobject,jstring id){return AppState::i().readPostIds.count(js(e,id))?JNI_TRUE:JNI_FALSE;}

// ============================================================================
// Tests — verify JSON parsing works correctly
// ============================================================================
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeRunTests(JNIEnv *e,jobject){
    // Test 1: Basic JSON parsing
    std::string t1="{\"name\":\"test\",\"value\":42,\"flag\":true}"; JVal v1=parseJSON(t1);
    if(v1["name"].str()!="test")return JNI_FALSE; if(v1["value"].i()!=42)return JNI_FALSE; if(!v1["flag"].bl())return JNI_FALSE;

    // Test 2: Array parsing
    std::string t2="[1,2,3,4,5]"; JVal v2=parseJSON(t2);
    if(v2.sz()!=5)return JNI_FALSE; if(v2.at(0).i()!=1)return JNI_FALSE;

    // Test 3: Nested objects
    std::string t3="{\"outer\":{\"inner\":{\"deep\":\"value\"}}}"; JVal v3=parseJSON(t3);
    if(v3["outer"]["inner"]["deep"].str()!="value")return JNI_FALSE;

    // Test 4: Reddit-style listing parse
    std::string t4="{\"kind\":\"Listing\",\"data\":{\"children\":[{\"kind\":\"t3\",\"data\":{\"id\":\"abc123\",\"title\":\"Test Post\",\"author\":\"testuser\",\"subreddit\":\"test\",\"score\":100,\"num_comments\":25,\"created_utc\":1700000000,\"url\":\"https://example.com\",\"is_self\":false,\"over_18\":false,\"spoiler\":false,\"stickied\":false,\"saved\":false,\"hidden\":false,\"locked\":false,\"archived\":false,\"likes\":1,\"gilded\":0,\"distinguished\":\"\",\"upvote_ratio\":0.95,\"post_hint\":\"link\",\"selftext\":\"\",\"thumbnail\":\"default\",\"domain\":\"example.com\",\"link_flair_text\":\"\",\"author_flair_text\":\"\",\"permalink\":\"/r/test/comments/abc123/test_post/\",\"name\":\"t3_abc123\",\"subreddit_id\":\"t5_test\"}}]}}";
    auto posts=parseListing(t4);
    if(posts.size()!=1)return JNI_FALSE;
    if(posts[0].id!="abc123")return JNI_FALSE;
    if(posts[0].title!="Test Post")return JNI_FALSE;
    if(posts[0].score!=100)return JNI_FALSE;
    if(posts[0].numComments!=25)return JNI_FALSE;

    // Test 5: Empty listing
    std::string t5="{\"kind\":\"Listing\",\"data\":{\"children\":[]}}"; auto posts2=parseListing(t5);
    if(!posts2.empty())return JNI_FALSE;

    // Test 6: Comment parsing
    std::string t6="[{\"kind\":\"Listing\",\"data\":{\"children\":[{\"kind\":\"t3\",\"data\":{\"id\":\"p1\",\"title\":\"Post\"}}]}},{\"kind\":\"Listing\",\"data\":{\"children\":[{\"kind\":\"t1\",\"data\":{\"id\":\"c1\",\"author\":\"commenter\",\"body\":\"Hello\",\"score\":5,\"created_utc\":1700000000,\"replies\":\"\"}}]}}]";
    auto cs=parseCommentListing(t6);
    if(cs.size()!=1)return JNI_FALSE;
    if(cs[0].id!="c1")return JNI_FALSE;
    if(cs[0].body!="Hello")return JNI_FALSE;

    // Test 7: Unicode
    std::string t7="{\"text\":\"Hello \\u2603 world\"}"; JVal v7=parseJSON(t7);
    if(v7["text"].str().find("world")==std::string::npos)return JNI_FALSE;

    // Test 8: Subreddit parse
    std::string t8="{\"kind\":\"Listing\",\"data\":{\"children\":[{\"kind\":\"t5\",\"data\":{\"display_name\":\"programming\",\"title\":\"Programming\",\"subscribers\":5000000,\"over18\":false,\"description\":\"Code\",\"icon_img\":\"\"}}]}}";
    auto subs=parseSubredditSearch(t8);
    if(subs.size()!=1)return JNI_FALSE;
    if(subs[0].name!="programming")return JNI_FALSE;
    if(subs[0].subscribers!=5000000)return JNI_FALSE;

    // Test 9: Inbox parse
    std::string t9="{\"kind\":\"Listing\",\"data\":{\"children\":[{\"kind\":\"t4\",\"data\":{\"id\":\"m1\",\"author\":\"sender\",\"subject\":\"Hello\",\"body\":\"Message body\",\"created_utc\":1700000000,\"new\":true,\"subreddit\":\"\",\"dest\":\"me\"}}]}}";
    auto msgs=parseInbox(t9);
    if(msgs.size()!=1)return JNI_FALSE;
    if(msgs[0].author!="sender")return JNI_FALSE;

    // Test 10: Cache
    AppState::i().postCache.put("test_key","test_value");
    std::string cached; if(!AppState::i().postCache.get("test_key",cached))return JNI_FALSE;
    if(cached!="test_value")return JNI_FALSE;

    return JNI_TRUE; // All tests passed
}

// Settings JNI
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeGetDarkTheme(JNIEnv*,jobject){return AppState::i().darkTheme?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeSetDarkTheme(JNIEnv*,jobject,jboolean d){AppState::i().darkTheme=d;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeClearCache(JNIEnv*,jobject){AppState::i().postCache.clear();}
} // extern "C"
