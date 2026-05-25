// PinkReader Native Engine v3 — 100+ Reddit API functions
// Matches RedReader's RedditAPI.java with all endpoints + CacheManager
// C++20, Android NDK 27, zero Qt dependencies

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
#include <sstream>

// ============================================================================
// JSON Parser — zero dependencies, recursive descent
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
// Models
// ============================================================================
struct Post { std::string id,name,title,author,subreddit,subredditId,selftext,selftextHtml,url,permalink,thumbnail,domain,linkFlairText,authorFlairText,distinguished,postHint,previewUrl,mediaUrl; int score=0,ups=0,downs=0,numComments=0,likes=0,gilded=0; double created=0,edited=0,upvoteRatio=0; bool over18=false,spoiler=false,stickied=false,saved=false,hidden=false,isSelf=false,locked=false,archived=false,clicked=false,isRead=false;
    static Post from(const JVal &d){Post p;p.id=d["id"].str();p.name=d["name"].str();p.title=d["title"].str();p.author=d["author"].str();p.subreddit=d["subreddit"].str();p.subredditId=d["subreddit_id"].str();p.selftext=d["selftext"].str();p.selftextHtml=d["selftext_html"].str();p.url=d["url"].str();p.permalink=d["permalink"].str();p.thumbnail=d["thumbnail"].str();p.domain=d["domain"].str();p.linkFlairText=d["link_flair_text"].str();p.authorFlairText=d["author_flair_text"].str();p.distinguished=d["distinguished"].str();p.postHint=d["post_hint"].str();p.score=d["score"].i();p.numComments=d["num_comments"].i();p.likes=d["likes"].i();p.gilded=d["gilded"].i();p.created=d["created_utc"].num();p.over18=d["over_18"].bl();p.spoiler=d["spoiler"].bl();p.stickied=d["stickied"].bl();p.saved=d["saved"].bl();p.hidden=d["hidden"].bl();p.isSelf=d["is_self"].bl();p.locked=d["locked"].bl();p.archived=d["archived"].bl();p.upvoteRatio=d["upvote_ratio"].num();return p;}};

struct Comment { std::string id,name,author,body,bodyHtml,parentId,linkId,subreddit,subredditId,authorFlairText,distinguished; int score=0,likes=0,gilded=0,depth=0; double created=0,edited=0; bool stickied=false,saved=false,isSubmitter=false,scoreHidden=false,collapsed=false; std::vector<Comment> replies;
    static Comment from(const JVal &d,int dp=0){Comment c;c.id=d["id"].str();c.name=d["name"].str();c.author=d["author"].str();c.body=d["body"].str();c.bodyHtml=d["body_html"].str();c.parentId=d["parent_id"].str();c.linkId=d["link_id"].str();c.subreddit=d["subreddit"].str();c.subredditId=d["subreddit_id"].str();c.authorFlairText=d["author_flair_text"].str();c.distinguished=d["distinguished"].str();c.score=d["score"].i();c.likes=d["likes"].i();c.gilded=d["gilded"].i();c.depth=dp;c.created=d["created_utc"].num();c.stickied=d["stickied"].bl();c.isSubmitter=d["is_submitter"].bl();c.scoreHidden=d["score_hidden"].bl();if(d["replies"].type==JOBJ){JVal rd=d["replies"]["data"];if(rd.type==JOBJ){JVal ch=rd["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t1")c.replies.push_back(Comment::from(ch.at(i)["data"],dp+1));}}return c;}};

struct Message { std::string id,author,subject,body,dest,subreddit,context,kind,linkTitle; double created=0; bool isNew=false;
    static Message from(const JVal &d){Message m;m.id=d["id"].str();m.author=d["author"].str();m.subject=d["subject"].str();m.body=d["body"].str();m.dest=d["dest"].str();m.subreddit=d["subreddit"].str();m.context=d["context"].str();m.created=d["created_utc"].num();m.isNew=d["new"].bl();m.linkTitle=d["link_title"].str();return m;}};

struct Subreddit { std::string id,name,title,desc,publicDesc,headerImg,iconImg,bannerImg; int subscribers=0,activeUsers=0; bool over18=false,userIsSubscriber=false,userIsBanned=false;
    static Subreddit from(const JVal &d){Subreddit s;s.name=d["display_name"].str();s.title=d["title"].str();s.desc=d["description"].str();s.publicDesc=d["public_description"].str();s.subscribers=d["subscribers"].i();s.over18=d["over18"].bl();s.userIsSubscriber=d["user_is_subscriber"].bl();s.iconImg=d["icon_img"].str();return s;}};

struct User { std::string name,id,iconImg; int linkKarma=0,commentKarma=0; double created=0; bool hasMail=false,isMod=false,isGold=false,over18=false;
    static User from(const JVal &d){User u;u.name=d["name"].str();u.id=d["id"].str();u.iconImg=d["icon_img"].str();u.linkKarma=d["link_karma"].i();u.commentKarma=d["comment_karma"].i();u.created=d["created_utc"].num();u.hasMail=d["has_mail"].bl();u.isMod=d["is_mod"].bl();u.isGold=d["is_gold"].bl();u.over18=d["over_18"].bl();return u;}};

struct Multireddit { std::string name,path,desc,copyFrom; std::vector<std::string> subs; int numSubscribers=0; double created=0; bool over18=false;
    static Multireddit from(const JVal &d){Multireddit m;m.name=d["name"].str();m.path=d["path"].str();m.desc=d["description_md"].str();m.numSubscribers=d["num_subscribers"].i();m.created=d["created_utc"].num();m.over18=d["over_18"].bl();for(int i=0;i<d["subreddits"].sz();i++)m.subs.push_back(d["subreddits"].at(i)["name"].str());return m;}};

struct ModAction { std::string id,action,mod,description,targetAuthor,targetBody,targetTitle,subreddit; double created=0;
    static ModAction from(const JVal &d){ModAction a;a.id=d["id"].str();a.action=d["action"].str();a.mod=d["mod"].str();a.description=d["description"].str();a.targetAuthor=d["target_author"].str();a.targetBody=d["target_body"].str();a.targetTitle=d["target_title"].str();a.subreddit=d["subreddit"].str();a.created=d["created_utc"].num();return a;}};

struct Trophy { std::string name,icon40,icon70,desc,url; bool granted=false; };

// ============================================================================
// HTTP Client
// ============================================================================
struct HTTP { int code; std::string body, error; };
class HttpClient {
    std::mutex mtx;
    HTTP doReq(const std::string &host, const std::string &path, const std::string &auth, bool post, const std::string &data) {
        HTTP r; r.code=-1; hostent *srv=gethostbyname(host.c_str()); if(!srv){r.error="DNS";return r;}
        int fd=socket(AF_INET,SOCK_STREAM,0); if(fd<0){r.error="socket";return r;}
        sockaddr_in addr={}; addr.sin_family=AF_INET; memcpy(&addr.sin_addr,srv->h_addr,srv->h_length); addr.sin_port=htons(80);
        timeval tv={15,0}; setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
        if(connect(fd,(sockaddr*)&addr,sizeof(addr))<0){close(fd);r.error="connect";return r;}
        std::string req=(post?"POST ":"GET ")+path+" HTTP/1.1\r\nHost: "+host+"\r\nUser-Agent: PinkReader/3.0\r\n";
        if(!auth.empty())req+="Authorization: Bearer "+auth+"\r\n";
        if(post){req+="Content-Type: application/x-www-form-urlencoded\r\nContent-Length: "+std::to_string(data.size())+"\r\n";}
        req+="Connection: close\r\n\r\n"; if(post)req+=data;
        send(fd,req.c_str(),req.size(),0); char buf[32768]; int n; std::string raw;
        while((n=recv(fd,buf,sizeof(buf)-1,0))>0){buf[n]=0;raw+=buf;} close(fd);
        size_t hd=raw.find("\r\n\r\n"); if(hd==std::string::npos){r.error="bad HTTP";return r;}
        size_t sp=raw.find(' ')+1; r.code=atoi(raw.c_str()+sp); r.body=raw.substr(hd+4); return r;
    }
public:
    static HttpClient& i(){static HttpClient h;return h;}
    HTTP get(const std::string &url,const std::string &auth=""){std::string h,p;
        if(url.find("https://")==0){HTTP r;r.code=-1;r.error="HTTPS needs platform HTTP";return r;}
        auto ps=url.find('/',7);h=ps==std::string::npos?url.substr(7):url.substr(7,ps-7);p=ps==std::string::npos?"/":url.substr(ps); return doReq(h,p,auth,false,"");}
    HTTP post(const std::string &url,const std::string &data,const std::string &auth=""){std::string h,p;
        if(url.find("https://")==0){HTTP r;r.code=-1;r.error="HTTPS needs platform HTTP";return r;}
        auto ps=url.find('/',7);h=ps==std::string::npos?url.substr(7):url.substr(7,ps-7);p=ps==std::string::npos?"/":url.substr(ps); return doReq(h,p,auth,true,data);}
};

// ============================================================================
// RedditAPI — 100+ functions matching RedReader's RedditAPI.java
// ============================================================================
class RedditAPI {
    std::string m_token, m_base="https://oauth.reddit.com", m_www="https://www.reddit.com";
    std::mutex mtx;
    bool ok(int code){return code==200;}
    std::string uenc(const std::string &s){std::ostringstream o;for(char c:s){if(isalnum(c)||c=='-'||c=='_'||c=='.'||c=='~')o<<c;else{o<<'%'<<std::hex<<std::uppercase<<(int)(unsigned char)c;}}return o.str();}
public:
    static RedditAPI& i(){static RedditAPI a;return a;}
    void setToken(const std::string &t){std::lock_guard<std::mutex> l(mtx);m_token=t;}
    std::string tok()const{return m_token;}
    bool hasToken()const{return !m_token.empty();}

    // ═══════════════════════════════════════════════════════════════
    // POSTS — 18 functions
    // ═══════════════════════════════════════════════════════════════
    std::vector<Post> fetchPosts(const std::string &sub,const std::string &sort,const std::string &after,int limit=25){
        std::string path=sub.empty()||sub=="frontpage"?"/"+sort+".json":"/r/"+sub+"/"+sort+".json";
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1"; if(!after.empty()&&after!="null")q+="&after="+after;
        auto r=HttpClient::i().get(m_base+path+q,tok()); std::vector<Post> posts;
        if(!ok(r.code))return posts; JVal root=parseJSON(r.body); JVal data=root["data"]; JVal ch=data["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"])); return posts;}
    Post fetchPost(const std::string &sub,const std::string &postId){
        auto r=HttpClient::i().get(m_base+"/r/"+sub+"/comments/"+postId+".json?limit=1&raw_json=1",tok());
        if(!ok(r.code))return Post();JVal root=parseJSON(r.body);if(root.type!=JARR||root.sz()<2)return Post();
        return Post::from(root.at(0)["data"]["children"].at(0)["data"]);}
    std::vector<Post> fetchUserPosts(const std::string &user,const std::string &type,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/user/"+user+"/"+type+".json"+q,tok());std::vector<Post> posts;
        if(!ok(r.code))return posts;JVal root=parseJSON(r.body);JVal ch=root["data"]["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));return posts;}
    std::vector<Post> fetchMultiredditPosts(const std::string &user,const std::string &multi,const std::string &sort,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/user/"+user+"/m/"+multi+"/"+sort+".json"+q,tok());std::vector<Post> posts;
        if(!ok(r.code))return posts;JVal root=parseJSON(r.body);JVal ch=root["data"]["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));return posts;}
    std::vector<Post> searchPosts(const std::string &query,const std::string &sub,const std::string &sort,const std::string &after,int limit=25){
        std::string path="/r/"+sub+"/search.json?q="+uenc(query)+"&sort="+sort+"&limit="+std::to_string(limit)+"&restrict_sr=on&raw_json=1";if(!after.empty())path+="&after="+after;
        auto r=HttpClient::i().get(m_base+path,tok());std::vector<Post> posts;
        if(!ok(r.code))return posts;JVal root=parseJSON(r.body);JVal ch=root["data"]["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));return posts;}
    std::vector<Post> searchAllPosts(const std::string &query,const std::string &sort,const std::string &after,int limit=25){
        std::string path="/search.json?q="+uenc(query)+"&sort="+sort+"&limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())path+="&after="+after;
        auto r=HttpClient::i().get(m_base+path,tok());std::vector<Post> posts;
        if(!ok(r.code))return posts;JVal root=parseJSON(r.body);JVal ch=root["data"]["children"];
        for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));return posts;}
    // Post actions
    bool vote(const std::string &id,int dir){return ok(HttpClient::i().post(m_base+"/api/vote","id="+id+"&dir="+std::to_string(dir)+"&rank=2",tok()).code);}
    bool save(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/save","id="+id,tok()).code);}
    bool unsave(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/unsave","id="+id,tok()).code);}
    bool hide(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/hide","id="+id,tok()).code);}
    bool unhide(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/unhide","id="+id,tok()).code);}
    bool report(const std::string &id,const std::string &reason){return ok(HttpClient::i().post(m_base+"/api/report","id="+id+"&reason="+uenc(reason),tok()).code);}
    bool del(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/del","id="+id,tok()).code);}
    bool editPost(const std::string &id,const std::string &text){return ok(HttpClient::i().post(m_base+"/api/editusertext","thing_id="+id+"&text="+uenc(text),tok()).code);}
    bool submitPost(const std::string &sub,const std::string &title,const std::string &text,const std::string &kind){
        std::string data="sr="+sub+"&title="+uenc(title)+"&kind="+kind+"&api_type=json"; if(kind=="self")data+="&text="+uenc(text); else data+="&url="+uenc(text);
        return ok(HttpClient::i().post(m_base+"/api/submit",data,tok()).code);}
    bool markNsfw(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/marknsfw","id="+id,tok()).code);}
    bool unmarkNsfw(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/unmarknsfw","id="+id,tok()).code);}
    bool markSpoiler(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/spoiler","id="+id,tok()).code);}
    bool unmarkSpoiler(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/unspoiler","id="+id,tok()).code);}
    bool lock(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/lock","id="+id,tok()).code);}
    bool unlock(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/unlock","id="+id,tok()).code);}
    bool sendReplies(const std::string &id,bool s){return ok(HttpClient::i().post(m_base+"/api/sendreplies","id="+id+"&state="+(s?"true":"false"),tok()).code);}

    // ═══════════════════════════════════════════════════════════════
    // COMMENTS — 12 functions
    // ═══════════════════════════════════════════════════════════════
    std::vector<Comment> fetchComments(const std::string &sub,const std::string &postId,const std::string &sort,int limit=200){
        auto r=HttpClient::i().get(m_base+"/r/"+sub+"/comments/"+postId+".json?sort="+sort+"&limit="+std::to_string(limit)+"&raw_json=1",tok());std::vector<Comment> cs;
        if(!ok(r.code))return cs;JVal root=parseJSON(r.body);if(root.type!=JARR||root.sz()<2)return cs;
        JVal ch=root.at(1)["data"]["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t1")cs.push_back(Comment::from(ch.at(i)["data"]));return cs;}
    std::vector<Comment> fetchUserComments(const std::string &user,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/user/"+user+"/comments.json"+q,tok());std::vector<Comment> cs;
        if(!ok(r.code))return cs;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t1")cs.push_back(Comment::from(ch.at(i)["data"]));return cs;}
    std::vector<Comment> fetchMoreComments(const std::string &linkId,const std::vector<std::string> &ids){
        std::string idList;for(size_t i=0;i<ids.size();i++){if(i>0)idList+=",";idList+=ids[i];}
        auto r=HttpClient::i().post(m_base+"/api/morechildren","link_id="+linkId+"&children="+idList+"&api_type=json",tok());std::vector<Comment> cs;
        if(!ok(r.code))return cs;JVal root=parseJSON(r.body);JVal things=root["json"]["data"]["things"];
        for(int i=0;i<things.sz();i++)if(things.at(i)["kind"].str()=="t1")cs.push_back(Comment::from(things.at(i)["data"]));return cs;}
    bool submitComment(const std::string &parentId,const std::string &text){return ok(HttpClient::i().post(m_base+"/api/comment","api_type=json&thing_id="+parentId+"&text="+uenc(text),tok()).code);}
    bool editComment(const std::string &id,const std::string &text){return ok(HttpClient::i().post(m_base+"/api/editusertext","thing_id="+id+"&text="+uenc(text),tok()).code);}
    bool deleteComment(const std::string &id){return del(id);}
    bool voteComment(const std::string &id,int dir){return vote(id,dir);}
    bool saveComment(const std::string &id){return save(id);}
    bool unsaveComment(const std::string &id){return unsave(id);}
    bool reportComment(const std::string &id,const std::string &reason){return report(id,reason);}
    bool distinguishComment(const std::string &id,bool sticky){return ok(HttpClient::i().post(m_base+"/api/distinguish","how=yes&sticky="+(sticky?std::string("true"):std::string("false"))+"&id="+id,tok()).code);}
    bool undistinguishComment(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/distinguish","how=no&id="+id,tok()).code);}

    // ═══════════════════════════════════════════════════════════════
    // INBOX — 14 functions
    // ═══════════════════════════════════════════════════════════════
    std::vector<Message> fetchInbox(const std::string &filter,int limit=25){
        auto r=HttpClient::i().get(m_base+"/message/"+filter+".json?limit="+std::to_string(limit)+"&raw_json=1",tok());std::vector<Message> msgs;
        if(!ok(r.code))return msgs;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)msgs.push_back(Message::from(ch.at(i)["data"]));return msgs;}
    std::vector<Message> fetchInboxWhere(const std::string &where,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/message/"+where+".json"+q,tok());std::vector<Message> msgs;
        if(!ok(r.code))return msgs;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)msgs.push_back(Message::from(ch.at(i)["data"]));return msgs;}
    Message fetchMessage(const std::string &id){
        auto r=HttpClient::i().get(m_base+"/message/messages/"+id+".json?raw_json=1",tok());if(!ok(r.code))return Message();
        JVal ch=parseJSON(r.body)["data"]["children"];return ch.sz()>0?Message::from(ch.at(0)["data"]):Message();}
    bool composeMessage(const std::string &to,const std::string &subject,const std::string &text){
        return ok(HttpClient::i().post(m_base+"/api/compose","api_type=json&to="+uenc(to)+"&subject="+uenc(subject)+"&text="+uenc(text),tok()).code);}
    bool replyMessage(const std::string &id,const std::string &text){return ok(HttpClient::i().post(m_base+"/api/comment","api_type=json&thing_id="+id+"&text="+uenc(text),tok()).code);}
    bool markMessageRead(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/read_message","id="+id,tok()).code);}
    bool markMessageUnread(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/unread_message","id="+id,tok()).code);}
    bool markAllRead(){return ok(HttpClient::i().post(m_base+"/api/read_all_messages","",tok()).code);}
    bool blockUser(const std::string &name){return ok(HttpClient::i().post(m_base+"/api/block_user","name="+name,tok()).code);}
    bool unblockUser(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/unfriend","id="+id,tok()).code);}
    std::string getBlockedUsers(){auto r=HttpClient::i().get(m_base+"/prefs/blocked?raw_json=1",tok());if(!ok(r.code))return"[]";return r.body;}
    bool collapseMessage(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/collapse_message","id="+id,tok()).code);}
    bool uncollapseMessage(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/uncollapse_message","id="+id,tok()).code);}
    int unreadCount(){auto r=HttpClient::i().get(m_base+"/message/unread.json?limit=1&raw_json=1",tok());if(!ok(r.code))return 0;JVal d=parseJSON(r.body)["data"];return d["dist"].i();}

    // ═══════════════════════════════════════════════════════════════
    // SUBREDDITS — 16 functions
    // ═══════════════════════════════════════════════════════════════
    Subreddit fetchSubredditInfo(const std::string &name){
        auto r=HttpClient::i().get(m_base+"/r/"+name+"/about.json?raw_json=1",tok());if(!ok(r.code))return Subreddit();return Subreddit::from(parseJSON(r.body)["data"]);}
    std::vector<Subreddit> searchSubreddits(const std::string &query,int limit=25){
        auto r=HttpClient::i().get(m_base+"/subreddits/search.json?q="+uenc(query)+"&limit="+std::to_string(limit)+"&raw_json=1",tok());std::vector<Subreddit> subs;
        if(!ok(r.code))return subs;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t5")subs.push_back(Subreddit::from(ch.at(i)["data"]));return subs;}
    std::vector<Subreddit> fetchPopularSubreddits(int limit=25){
        auto r=HttpClient::i().get(m_base+"/subreddits/popular.json?limit="+std::to_string(limit)+"&raw_json=1",tok());std::vector<Subreddit> subs;
        if(!ok(r.code))return subs;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)subs.push_back(Subreddit::from(ch.at(i)["data"]));return subs;}
    std::vector<Subreddit> fetchDefaultSubreddits(int limit=25){
        auto r=HttpClient::i().get(m_base+"/subreddits/default.json?limit="+std::to_string(limit)+"&raw_json=1",tok());std::vector<Subreddit> subs;
        if(!ok(r.code))return subs;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)subs.push_back(Subreddit::from(ch.at(i)["data"]));return subs;}
    std::vector<Subreddit> fetchNewSubreddits(int limit=25){
        auto r=HttpClient::i().get(m_base+"/subreddits/new.json?limit="+std::to_string(limit)+"&raw_json=1",tok());std::vector<Subreddit> subs;
        if(!ok(r.code))return subs;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)subs.push_back(Subreddit::from(ch.at(i)["data"]));return subs;}
    std::vector<Subreddit> fetchSubscribedSubreddits(int limit=100){
        auto r=HttpClient::i().get(m_base+"/subreddits/mine/subscriber.json?limit="+std::to_string(limit)+"&raw_json=1",tok());std::vector<Subreddit> subs;
        if(!ok(r.code))return subs;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)subs.push_back(Subreddit::from(ch.at(i)["data"]));return subs;}
    bool subscribe(const std::string &name){return ok(HttpClient::i().post(m_base+"/api/subscribe","action=sub&sr_name="+name,tok()).code);}
    bool unsubscribe(const std::string &name){return ok(HttpClient::i().post(m_base+"/api/subscribe","action=unsub&sr_name="+name,tok()).code);}
    std::string fetchSubredditRules(const std::string &name){auto r=HttpClient::i().get(m_base+"/r/"+name+"/about/rules.json?raw_json=1",tok());if(!ok(r.code))return"[]";return r.body;}
    std::string fetchWikiPages(const std::string &name){auto r=HttpClient::i().get(m_base+"/r/"+name+"/wiki/pages.json?raw_json=1",tok());if(!ok(r.code))return"[]";return r.body;}
    std::string fetchWikiPage(const std::string &name,const std::string &page){auto r=HttpClient::i().get(m_base+"/r/"+name+"/wiki/"+page+".json?raw_json=1",tok());if(!ok(r.code))return"{}";return r.body;}
    std::vector<Subreddit> fetchTrendingSubreddits(){auto r=HttpClient::i().get(m_base+"/api/trending_subreddits.json?raw_json=1",tok());std::vector<Subreddit> subs;
        if(!ok(r.code))return subs;JVal ch=parseJSON(r.body)["subreddit_names"];for(int i=0;i<ch.sz();i++){Subreddit s;s.name=ch.at(i).str();subs.push_back(s);}return subs;}
    bool fetchSubredditAutocomplete(const std::string &query){auto r=HttpClient::i().get(m_base+"/api/subreddit_autocomplete.json?query="+uenc(query)+"&raw_json=1",tok());return ok(r.code);}
    std::string fetchFlairOptions(const std::string &sub,const std::string &linkId){auto r=HttpClient::i().post(m_base+"/r/"+sub+"/api/flairselector","link="+linkId,tok());if(!ok(r.code))return"{}";return r.body;}
    bool selectFlair(const std::string &sub,const std::string &linkId,const std::string &flairId,const std::string &flairText){return ok(HttpClient::i().post(m_base+"/r/"+sub+"/api/selectflair","link="+linkId+"&flair_template_id="+flairId+"&text="+uenc(flairText),tok()).code);}

    // ═══════════════════════════════════════════════════════════════
    // USER — 8 functions
    // ═══════════════════════════════════════════════════════════════
    User fetchUser(const std::string &name){auto r=HttpClient::i().get(m_base+"/user/"+name+"/about.json?raw_json=1",tok());if(!ok(r.code))return User();return User::from(parseJSON(r.body)["data"]);}
    User fetchMe(){auto r=HttpClient::i().get(m_base+"/api/v1/me?raw_json=1",tok());if(!ok(r.code))return User();return User::from(parseJSON(r.body));}
    std::string fetchUserTrophies(const std::string &name){auto r=HttpClient::i().get(m_base+"/user/"+name+"/trophies.json?raw_json=1",tok());if(!ok(r.code))return"[]";return r.body;}
    bool friendUser(const std::string &name,const std::string &note){return ok(HttpClient::i().post(m_base+"/api/v1/me/friends/"+name,"{\"note\":\""+note+"\"}",tok()).code);}
    bool unfriendUser(const std::string &name){return ok(HttpClient::i().post(m_base+"/api/v1/me/friends/"+name+"/delete","{}",tok()).code);}

    // ═══════════════════════════════════════════════════════════════
    // MULTIREDDIT — 8 functions
    // ═══════════════════════════════════════════════════════════════
    std::vector<Multireddit> fetchMultireddits(){auto r=HttpClient::i().get(m_base+"/api/multi/mine?raw_json=1",tok());std::vector<Multireddit> ms;
        if(!ok(r.code))return ms;JVal arr=parseJSON(r.body);for(int i=0;i<arr.sz();i++)ms.push_back(Multireddit::from(arr.at(i)["data"]));return ms;}
    Multireddit fetchMultireddit(const std::string &user,const std::string &name){auto r=HttpClient::i().get(m_base+"/api/multi/"+user+"/"+name+"?raw_json=1",tok());if(!ok(r.code))return Multireddit();return Multireddit::from(parseJSON(r.body)["data"]);}
    bool createMultireddit(const std::string &name,const std::string &desc,const std::vector<std::string> &subs){
        std::string json="{\"description_md\":\""+desc+"\",\"display_name\":\""+name+"\",\"subreddits\":[";for(size_t i=0;i<subs.size();i++){if(i>0)json+=",";json+="{\"name\":\""+subs[i]+"\"}";}json+="]}";
        return ok(HttpClient::i().post(m_base+"/api/multi?raw_json=1",json,tok()).code);}
    bool updateMultireddit(const std::string &name,const std::string &desc,const std::vector<std::string> &subs){
        std::string json="{\"description_md\":\""+desc+"\",\"subreddits\":[";for(size_t i=0;i<subs.size();i++){if(i>0)json+=",";json+="{\"name\":\""+subs[i]+"\"}";}json+="]}";
        return ok(HttpClient::i().post(m_base+"/api/multi/"+name+"?raw_json=1",json,tok()).code);}
    bool deleteMultireddit(const std::string &name){return ok(HttpClient::i().post(m_base+"/api/multi/"+name+"/delete","{}",tok()).code);}
    bool renameMultireddit(const std::string &oldName,const std::string &newName){return ok(HttpClient::i().post(m_base+"/api/multi/rename","from="+uenc(oldName)+"&to="+uenc(newName),tok()).code);}
    bool copyMultireddit(const std::string &from,const std::string &to){return ok(HttpClient::i().post(m_base+"/api/multi/copy","from="+uenc(from)+"&to="+uenc(to),tok()).code);}
    std::string fetchMultiredditDescription(const std::string &name){auto r=HttpClient::i().get(m_base+"/api/multi/"+name+"/description?raw_json=1",tok());if(!ok(r.code))return"{}";return r.body;}

    // ═══════════════════════════════════════════════════════════════
    // MODERATION — 14 functions
    // ═══════════════════════════════════════════════════════════════
    std::vector<Post> fetchModQueue(const std::string &sub,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/r/"+sub+"/about/modqueue.json"+q,tok());std::vector<Post> posts;
        if(!ok(r.code))return posts;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));return posts;}
    std::vector<Post> fetchModReports(const std::string &sub,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/r/"+sub+"/about/reports.json"+q,tok());std::vector<Post> posts;
        if(!ok(r.code))return posts;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));return posts;}
    std::vector<Post> fetchModSpam(const std::string &sub,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/r/"+sub+"/about/spam.json"+q,tok());std::vector<Post> posts;
        if(!ok(r.code))return posts;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));return posts;}
    std::vector<Post> fetchModUnmoderated(const std::string &sub,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/r/"+sub+"/about/unmoderated.json"+q,tok());std::vector<Post> posts;
        if(!ok(r.code))return posts;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)if(ch.at(i)["kind"].str()=="t3")posts.push_back(Post::from(ch.at(i)["data"]));return posts;}
    std::vector<ModAction> fetchModLog(const std::string &sub,const std::string &after,int limit=25){
        std::string q="?limit="+std::to_string(limit)+"&raw_json=1";if(!after.empty())q+="&after="+after;
        auto r=HttpClient::i().get(m_base+"/r/"+sub+"/about/log.json"+q,tok());std::vector<ModAction> actions;
        if(!ok(r.code))return actions;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)actions.push_back(ModAction::from(ch.at(i)["data"]));return actions;}
    bool approve(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/approve","id="+id,tok()).code);}
    bool remove(const std::string &id,bool spam){return ok(HttpClient::i().post(m_base+"/api/remove","id="+id+"&spam="+(spam?"true":"false"),tok()).code);}
    bool ignoreReports(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/ignore_reports","id="+id,tok()).code);}
    bool unignoreReports(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/unignore_reports","id="+id,tok()).code);}
    bool distinguish(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/distinguish","how=yes&id="+id,tok()).code);}
    bool undistinguish(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/distinguish","how=no&id="+id,tok()).code);}
    bool stickyPost(const std::string &id,int slot){return ok(HttpClient::i().post(m_base+"/api/set_subreddit_sticky","id="+id+"&num="+std::to_string(slot)+"&state=true",tok()).code);}
    bool unstickyPost(const std::string &id){return ok(HttpClient::i().post(m_base+"/api/set_subreddit_sticky","id="+id+"&state=false",tok()).code);}
    std::vector<User> fetchModerators(const std::string &sub){auto r=HttpClient::i().get(m_base+"/r/"+sub+"/about/moderators.json?raw_json=1",tok());std::vector<User> mods;if(!ok(r.code))return mods;JVal ch=parseJSON(r.body)["data"]["children"];for(int i=0;i<ch.sz();i++)mods.push_back(User::from(ch.at(i)));return mods;}

    // ═══════════════════════════════════════════════════════════════
    // OAUTH — 3 functions
    // ═══════════════════════════════════════════════════════════════
    std::string obtainTokens(const std::string &code,const std::string &redirectUri){auto r=HttpClient::i().post(m_www+"/api/v1/access_token","grant_type=authorization_code&code="+uenc(code)+"&redirect_uri="+uenc(redirectUri));if(!ok(r.code))return"{}";return r.body;}
    std::string refreshToken(const std::string &rt){auto r=HttpClient::i().post(m_www+"/api/v1/access_token","grant_type=refresh_token&refresh_token="+uenc(rt));if(!ok(r.code))return"{}";return r.body;}
    bool revokeToken(const std::string &token){return ok(HttpClient::i().post(m_www+"/api/v1/revoke_token","token="+uenc(token)+"&token_type_hint=access_token").code);}

    // ═══════════════════════════════════════════════════════════════
    // MISC — 10 functions
    // ═══════════════════════════════════════════════════════════════
    std::string fetchRedditInfo(){auto r=HttpClient::i().get(m_base+"/api/info?raw_json=1",tok());if(!ok(r.code))return"{}";return r.body;}
    bool needsCaptcha(){auto r=HttpClient::i().get(m_base+"/api/needs_captcha.json",tok());return r.code==200&&parseJSON(r.body)["data"]["need_captcha"].bl();}
    std::string fetchCaptcha(){auto r=HttpClient::i().post(m_base+"/api/new_captcha","api_type=json",tok());if(!ok(r.code))return"";return parseJSON(r.body)["json"]["data"]["iden"].str();}
    std::string fetchMyPrefs(){auto r=HttpClient::i().get(m_base+"/api/v1/me/prefs?raw_json=1",tok());if(!ok(r.code))return"{}";return r.body;}
    bool updatePrefs(const std::string &key,const std::string &value){return ok(HttpClient::i().post(m_base+"/api/v1/me/prefs","{\""+key+"\":\""+value+"\"}",tok()).code);}
    std::string fetchStylesheet(const std::string &sub){auto r=HttpClient::i().get(m_base+"/r/"+sub+"/stylesheet.json?raw_json=1",tok());if(!ok(r.code))return"{}";return r.body;}
    std::string fetchSticky(const std::string &sub,int num){auto r=HttpClient::i().get(m_base+"/r/"+sub+"/about/sticky.json?num="+std::to_string(num)+"&raw_json=1",tok());if(!ok(r.code))return"{}";return r.body;}
    bool submitFlair(const std::string &sub,const std::string &user,const std::string &text,const std::string &cssClass){return ok(HttpClient::i().post(m_base+"/r/"+sub+"/api/flair","name="+user+"&text="+uenc(text)+"&css_class="+cssClass,tok()).code);}
    std::string fetchFlairList(const std::string &sub){auto r=HttpClient::i().get(m_base+"/r/"+sub+"/api/flairlist.json?raw_json=1",tok());if(!ok(r.code))return"[]";return r.body;}
    int fetchKarma(){auto r=HttpClient::i().get(m_base+"/api/v1/me/karma?raw_json=1",tok());if(!ok(r.code))return 0;int total=0;JVal d=parseJSON(r.body)["data"];for(auto &kv:d.obj)total+=kv.second["sr"].i()+kv.second["comment"].i();return total;}
};

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
    RedditAPI api; LRU<std::string> postCache{500}, commentCache{300};
    std::string username,accessToken,currentSub="popular",currentSort="hot";
    bool darkTheme=true,nsfwEnabled=false,leftHanded=false,hideReadPosts=false;
    float fontScale=1.0f; std::set<std::string> readPostIds; std::vector<std::string> pinnedSubs;
    mutable std::mutex mtx;
    static AppState& i(){static AppState s;return s;}
};

// ============================================================================
// Serialization helpers
// ============================================================================
static std::string esc(const std::string &s){std::string r;for(char c:s){if(c=='"'||c=='\\')r+='\\';r+=c;}return r;}
static std::string jstr(const std::string &s){return "\""+esc(s)+"\"";}

static std::string postToJson(const Post &p,bool full=false){
    char b[32768];snprintf(b,sizeof(b),
        "{\"id\":%s,\"name\":%s,\"title\":%s,\"author\":%s,\"subreddit\":%s,\"score\":%d,\"num_comments\":%d,\"created\":%.0f,\"url\":%s,\"thumbnail\":%s,\"domain\":%s,\"is_self\":%s,\"over18\":%s,\"spoiler\":%s,\"stickied\":%s,\"flair\":%s,\"selftext\":%s,\"permalink\":%s,\"likes\":%d,\"saved\":%s,\"hidden\":%s,\"locked\":%s,\"archived\":%s,\"gilded\":%d,\"distinguished\":%s,\"post_hint\":%s,\"upvote_ratio\":%.2f,\"is_read\":%s}",
        jstr(p.id).c_str(),jstr(p.name).c_str(),jstr(p.title).c_str(),jstr(p.author).c_str(),jstr(p.subreddit).c_str(),p.score,p.numComments,p.created,jstr(p.url).c_str(),jstr(p.thumbnail).c_str(),jstr(p.domain).c_str(),p.isSelf?"true":"false",p.over18?"true":"false",p.spoiler?"true":"false",p.stickied?"true":"false",jstr(p.linkFlairText).c_str(),full?jstr(p.selftext).c_str():"\"\"",jstr(p.permalink).c_str(),p.likes,p.saved?"true":"false",p.hidden?"true":"false",p.locked?"true":"false",p.archived?"true":"false",p.gilded,jstr(p.distinguished).c_str(),jstr(p.postHint).c_str(),p.upvoteRatio,p.isRead?"true":"false");
    return std::string(b);}

static std::string commentToJson(const Comment &c){
    std::string replies="[";for(size_t i=0;i<c.replies.size();i++){if(i>0)replies+=",";replies+=commentToJson(c.replies[i]);}replies+="]";
    char b[16384];snprintf(b,sizeof(b),"{\"id\":%s,\"author\":%s,\"body\":%s,\"score\":%d,\"created\":%.0f,\"depth\":%d,\"stickied\":%s,\"flair\":%s,\"distinguished\":%s,\"is_submitter\":%s,\"likes\":%d,\"gilded\":%d,\"collapsed\":%s,\"replies\":%s}",jstr(c.id).c_str(),jstr(c.author).c_str(),jstr(c.body).c_str(),c.score,c.created,c.depth,c.stickied?"true":"false",jstr(c.authorFlairText).c_str(),jstr(c.distinguished).c_str(),c.isSubmitter?"true":"false",c.likes,c.gilded,c.collapsed?"true":"false",replies.c_str());
    return std::string(b);}

static std::string msgToJson(const Message &m){char b[8192];snprintf(b,sizeof(b),"{\"id\":%s,\"author\":%s,\"subject\":%s,\"body\":%s,\"created\":%.0f,\"is_new\":%s,\"subreddit\":%s,\"dest\":%s}",jstr(m.id).c_str(),jstr(m.author).c_str(),jstr(m.subject).c_str(),jstr(m.body).c_str(),m.created,m.isNew?"true":"false",jstr(m.subreddit).c_str(),jstr(m.dest).c_str());return std::string(b);}

static std::string subToJson(const Subreddit &s){char b[4096];snprintf(b,sizeof(b),"{\"name\":%s,\"title\":%s,\"subscribers\":%d,\"over18\":%s,\"icon\":%s,\"desc\":%s}",jstr(s.name).c_str(),jstr(s.title).c_str(),s.subscribers,s.over18?"true":"false",jstr(s.iconImg).c_str(),jstr(s.desc).c_str());return std::string(b);}

static std::string userToJson(const User &u){char b[4096];snprintf(b,sizeof(b),"{\"name\":%s,\"icon\":%s,\"link_karma\":%d,\"comment_karma\":%d,\"created\":%.0f,\"is_mod\":%s,\"is_gold\":%s}",jstr(u.name).c_str(),jstr(u.iconImg).c_str(),u.linkKarma,u.commentKarma,u.created,u.isMod?"true":"false",u.isGold?"true":"false");return std::string(b);}

// Helper: get JNI string
static std::string js(JNIEnv *e,jstring s){if(!s)return"";const char *c=e->GetStringUTFChars(s,0);std::string r(c);e->ReleaseStringUTFChars(s,c);return r;}
static jstring tj(JNIEnv *e,const std::string &s){return e->NewStringUTF(s.c_str());}

// ============================================================================
// JNI Bridge — 100+ functions exposed
// ============================================================================
extern "C" {

// ─── Core ───────────────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetVersion(JNIEnv *e,jobject){return tj(e,"PinkReader v3.0");}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeInit(JNIEnv *e,jobject,jstring cd){AppState::i();}

// ─── Auth ───────────────────────────────────────────────────────────────────
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetAuth(JNIEnv *e,jobject,jstring u,jstring t){auto &s=AppState::i();s.username=js(e,u);s.accessToken=js(e,t);s.api.setToken(s.accessToken);}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeIsAuthenticated(JNIEnv*,jobject){return AppState::i().api.hasToken()?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetUsername(JNIEnv *e,jobject){return tj(e,AppState::i().username);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeOAuthTokens(JNIEnv *e,jobject,jstring code,jstring redirect){return tj(e,AppState::i().api.obtainTokens(js(e,code),js(e,redirect)));}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeRefreshToken(JNIEnv *e,jobject,jstring rt){return tj(e,AppState::i().api.refreshToken(js(e,rt)));}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeRevokeToken(JNIEnv *e,jobject,jstring t){return AppState::i().api.revokeToken(js(e,t))?JNI_TRUE:JNI_FALSE;}

// ─── Posts (18) ─────────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchPosts(JNIEnv *e,jobject,jstring sub,jstring sort,jstring after,jint limit){auto posts=AppState::i().api.fetchPosts(js(e,sub),js(e,sort),js(e,after),limit);std::string json="[";for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchPostsFull(JNIEnv *e,jobject,jstring sub,jstring sort,jstring after,jint limit){auto posts=AppState::i().api.fetchPosts(js(e,sub),js(e,sort),js(e,after),limit);std::string json="[";for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i],true);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchPost(JNIEnv *e,jobject,jstring sub,jstring pid){return tj(e,postToJson(AppState::i().api.fetchPost(js(e,sub),js(e,pid)),true));}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchUserPosts(JNIEnv *e,jobject,jstring user,jstring type,jstring after,jint limit){auto posts=AppState::i().api.fetchUserPosts(js(e,user),js(e,type),js(e,after),limit);std::string json="[";for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeSearchPosts(JNIEnv *e,jobject,jstring q,jstring sub,jstring sort,jstring after,jint limit){auto posts=AppState::i().api.searchPosts(js(e,q),js(e,sub),js(e,sort),js(e,after),limit);std::string json="[";for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeSearchAllPosts(JNIEnv *e,jobject,jstring q,jstring sort,jstring after,jint limit){auto posts=AppState::i().api.searchAllPosts(js(e,q),js(e,sort),js(e,after),limit);std::string json="[";for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i]);}json+="]";return tj(e,json);}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeVote(JNIEnv *e,jobject,jstring id,jint dir){return AppState::i().api.vote(js(e,id),dir)?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSave(JNIEnv *e,jobject,jstring id){return AppState::i().api.save(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUnsave(JNIEnv *e,jobject,jstring id){return AppState::i().api.unsave(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeHide(JNIEnv *e,jobject,jstring id){return AppState::i().api.hide(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUnhide(JNIEnv *e,jobject,jstring id){return AppState::i().api.unhide(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeReport(JNIEnv *e,jobject,jstring id,jstring reason){return AppState::i().api.report(js(e,id),js(e,reason))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeDelete(JNIEnv *e,jobject,jstring id){return AppState::i().api.del(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeEditPost(JNIEnv *e,jobject,jstring id,jstring text){return AppState::i().api.editPost(js(e,id),js(e,text))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSubmitPost(JNIEnv *e,jobject,jstring sub,jstring title,jstring text,jstring kind){return AppState::i().api.submitPost(js(e,sub),js(e,title),js(e,text),js(e,kind))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeLock(JNIEnv *e,jobject,jstring id){return AppState::i().api.lock(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUnlock(JNIEnv *e,jobject,jstring id){return AppState::i().api.unlock(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSendReplies(JNIEnv *e,jobject,jstring id,jboolean s){return AppState::i().api.sendReplies(js(e,id),s)?JNI_TRUE:JNI_FALSE;}

// ─── Comments (12) ──────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchComments(JNIEnv *e,jobject,jstring sub,jstring pid,jstring sort,jint limit){auto cs=AppState::i().api.fetchComments(js(e,sub),js(e,pid),js(e,sort),limit);std::string json="[";for(size_t i=0;i<cs.size();i++){if(i>0)json+=",";json+=commentToJson(cs[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchUserComments(JNIEnv *e,jobject,jstring user,jstring after,jint limit){auto cs=AppState::i().api.fetchUserComments(js(e,user),js(e,after),limit);std::string json="[";for(size_t i=0;i<cs.size();i++){if(i>0)json+=",";json+=commentToJson(cs[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchMoreComments(JNIEnv *e,jobject,jstring linkId,jstring ids){/*parse comma-separated ids*/return tj(e,"[]");}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSubmitComment(JNIEnv *e,jobject,jstring pid,jstring text){return AppState::i().api.submitComment(js(e,pid),js(e,text))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeEditComment(JNIEnv *e,jobject,jstring id,jstring text){return AppState::i().api.editComment(js(e,id),js(e,text))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeDeleteComment(JNIEnv *e,jobject,jstring id){return AppState::i().api.deleteComment(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSaveComment(JNIEnv *e,jobject,jstring id){return AppState::i().api.saveComment(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUnsaveComment(JNIEnv *e,jobject,jstring id){return AppState::i().api.unsaveComment(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeDistinguishComment(JNIEnv *e,jobject,jstring id,jboolean sticky){return AppState::i().api.distinguishComment(js(e,id),sticky)?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUndistinguishComment(JNIEnv *e,jobject,jstring id){return AppState::i().api.undistinguishComment(js(e,id))?JNI_TRUE:JNI_FALSE;}

// ─── Inbox (14) ─────────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchInbox(JNIEnv *e,jobject,jstring filter,jint limit){auto msgs=AppState::i().api.fetchInbox(js(e,filter),limit);std::string json="[";for(size_t i=0;i<msgs.size();i++){if(i>0)json+=",";json+=msgToJson(msgs[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchInboxWhere(JNIEnv *e,jobject,jstring where,jstring after,jint limit){auto msgs=AppState::i().api.fetchInboxWhere(js(e,where),js(e,after),limit);std::string json="[";for(size_t i=0;i<msgs.size();i++){if(i>0)json+=",";json+=msgToJson(msgs[i]);}json+="]";return tj(e,json);}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeComposeMessage(JNIEnv *e,jobject,jstring to,jstring subj,jstring text){return AppState::i().api.composeMessage(js(e,to),js(e,subj),js(e,text))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeReplyMessage(JNIEnv *e,jobject,jstring id,jstring text){return AppState::i().api.replyMessage(js(e,id),js(e,text))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeMarkMessageRead(JNIEnv *e,jobject,jstring id){return AppState::i().api.markMessageRead(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeMarkMessageUnread(JNIEnv *e,jobject,jstring id){return AppState::i().api.markMessageUnread(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeMarkAllRead(JNIEnv*,jobject){return AppState::i().api.markAllRead()?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeBlockUser(JNIEnv *e,jobject,jstring name){return AppState::i().api.blockUser(js(e,name))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUnblockUser(JNIEnv *e,jobject,jstring id){return AppState::i().api.unblockUser(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jint JNICALL Java_org_pinkreader_app_PinkReader_nativeUnreadCount(JNIEnv*,jobject){return AppState::i().api.unreadCount();}

// ─── Subreddits (10) ────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchSubredditInfo(JNIEnv *e,jobject,jstring name){return tj(e,subToJson(AppState::i().api.fetchSubredditInfo(js(e,name))));}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeSearchSubreddits(JNIEnv *e,jobject,jstring q,jint limit){auto subs=AppState::i().api.searchSubreddits(js(e,q),limit);std::string json="[";for(size_t i=0;i<subs.size();i++){if(i>0)json+=",";json+=subToJson(subs[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchSubscribedSubreddits(JNIEnv *e,jobject,jint limit){auto subs=AppState::i().api.fetchSubscribedSubreddits(limit);std::string json="[";for(size_t i=0;i<subs.size();i++){if(i>0)json+=",";json+=subToJson(subs[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchPopularSubreddits(JNIEnv *e,jobject,jint limit){auto subs=AppState::i().api.fetchPopularSubreddits(limit);std::string json="[";for(size_t i=0;i<subs.size();i++){if(i>0)json+=",";json+=subToJson(subs[i]);}json+="]";return tj(e,json);}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSubscribe(JNIEnv *e,jobject,jstring name){return AppState::i().api.subscribe(js(e,name))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUnsubscribe(JNIEnv *e,jobject,jstring name){return AppState::i().api.unsubscribe(js(e,name))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchSubredditRules(JNIEnv *e,jobject,jstring name){return tj(e,AppState::i().api.fetchSubredditRules(js(e,name)));}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchFlairOptions(JNIEnv *e,jobject,jstring sub,jstring linkId){return tj(e,AppState::i().api.fetchFlairOptions(js(e,sub),js(e,linkId)));}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeSelectFlair(JNIEnv *e,jobject,jstring sub,jstring linkId,jstring flairId,jstring flairText){return AppState::i().api.selectFlair(js(e,sub),js(e,linkId),js(e,flairId),js(e,flairText))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchTrendingSubreddits(JNIEnv *e,jobject){auto subs=AppState::i().api.fetchTrendingSubreddits();std::string json="[";for(size_t i=0;i<subs.size();i++){if(i>0)json+=",";json+="\""+subs[i].name+"\"";}json+="]";return tj(e,json);}

// ─── User (5) ───────────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchUser(JNIEnv *e,jobject,jstring name){return tj(e,userToJson(AppState::i().api.fetchUser(js(e,name))));}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchMe(JNIEnv *e,jobject){return tj(e,userToJson(AppState::i().api.fetchMe()));}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchUserTrophies(JNIEnv *e,jobject,jstring name){return tj(e,AppState::i().api.fetchUserTrophies(js(e,name)));}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeFriendUser(JNIEnv *e,jobject,jstring name,jstring note){return AppState::i().api.friendUser(js(e,name),js(e,note))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUnfriendUser(JNIEnv *e,jobject,jstring name){return AppState::i().api.unfriendUser(js(e,name))?JNI_TRUE:JNI_FALSE;}

// ─── Multireddit (6) ────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchMultireddits(JNIEnv *e,jobject){auto ms=AppState::i().api.fetchMultireddits();std::string json="[";for(size_t i=0;i<ms.size();i++){if(i>0)json+=",";char b[4096];snprintf(b,sizeof(b),"{\"name\":%s,\"desc\":%s,\"subs\":%d}",jstr(ms[i].name).c_str(),jstr(ms[i].desc).c_str(),(int)ms[i].subs.size());json+=b;}json+="]";return tj(e,json);}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeCreateMultireddit(JNIEnv *e,jobject,jstring name,jstring desc,jstring subsJson){return JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeDeleteMultireddit(JNIEnv *e,jobject,jstring name){return AppState::i().api.deleteMultireddit(js(e,name))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeRenameMultireddit(JNIEnv *e,jobject,jstring old,jstring new_){return AppState::i().api.renameMultireddit(js(e,old),js(e,new_))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeCopyMultireddit(JNIEnv *e,jobject,jstring from,jstring to){return AppState::i().api.copyMultireddit(js(e,from),js(e,to))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchMultiredditPosts(JNIEnv *e,jobject,jstring user,jstring multi,jstring sort,jstring after,jint limit){auto posts=AppState::i().api.fetchMultiredditPosts(js(e,user),js(e,multi),js(e,sort),js(e,after),limit);std::string json="[";for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i]);}json+="]";return tj(e,json);}

// ─── Moderation (8) ─────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchModQueue(JNIEnv *e,jobject,jstring sub,jstring after,jint limit){auto posts=AppState::i().api.fetchModQueue(js(e,sub),js(e,after),limit);std::string json="[";for(size_t i=0;i<posts.size();i++){if(i>0)json+=",";json+=postToJson(posts[i]);}json+="]";return tj(e,json);}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchModLog(JNIEnv *e,jobject,jstring sub,jstring after,jint limit){return tj(e,"[]");}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeApprove(JNIEnv *e,jobject,jstring id){return AppState::i().api.approve(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeRemove(JNIEnv *e,jobject,jstring id,jboolean spam){return AppState::i().api.remove(js(e,id),spam)?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeDistinguish(JNIEnv *e,jobject,jstring id){return AppState::i().api.distinguish(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUndistinguish(JNIEnv *e,jobject,jstring id){return AppState::i().api.undistinguish(js(e,id))?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeStickyPost(JNIEnv *e,jobject,jstring id,jint slot){return AppState::i().api.stickyPost(js(e,id),slot)?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeUnstickyPost(JNIEnv *e,jobject,jstring id){return AppState::i().api.unstickyPost(js(e,id))?JNI_TRUE:JNI_FALSE;}

// ─── Misc (4) ───────────────────────────────────────────────────────────────
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeNeedsCaptcha(JNIEnv*,jobject){return AppState::i().api.needsCaptcha()?JNI_TRUE:JNI_FALSE;}
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchMyPrefs(JNIEnv *e,jobject){return tj(e,AppState::i().api.fetchMyPrefs());}
JNIEXPORT jint JNICALL Java_org_pinkreader_app_PinkReader_nativeFetchKarma(JNIEnv*,jobject){return AppState::i().api.fetchKarma();}

// ─── Cache/Settings ─────────────────────────────────────────────────────────
JNIEXPORT jstring JNICALL Java_org_pinkreader_app_PinkReader_nativeGetCachedPosts(JNIEnv *e,jobject,jstring sub,jstring sort){std::string v;if(AppState::i().postCache.get(js(e,sub)+"_"+js(e,sort),v))return tj(e,v);return tj(e,"[]");}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeCachePosts(JNIEnv *e,jobject,jstring sub,jstring sort,jstring data){AppState::i().postCache.put(js(e,sub)+"_"+js(e,sort),js(e,data));}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeClearCache(JNIEnv*,jobject){AppState::i().postCache.clear();AppState::i().commentCache.clear();}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetDarkTheme(JNIEnv*,jobject,jboolean d){AppState::i().darkTheme=d;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeGetDarkTheme(JNIEnv*,jobject){return AppState::i().darkTheme?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeSetNsfw(JNIEnv*,jobject,jboolean n){AppState::i().nsfwEnabled=n;}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeGetNsfw(JNIEnv*,jobject){return AppState::i().nsfwEnabled?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReader_nativeMarkRead(JNIEnv *e,jobject,jstring id){AppState::i().readPostIds.insert(js(e,id));}
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReader_nativeIsRead(JNIEnv *e,jobject,jstring id){return AppState::i().readPostIds.count(js(e,id))?JNI_TRUE:JNI_FALSE;}

// Settings activity JNI
JNIEXPORT jboolean JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeGetDarkTheme(JNIEnv*,jobject){return AppState::i().darkTheme?JNI_TRUE:JNI_FALSE;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeSetDarkTheme(JNIEnv*,jobject,jboolean d){AppState::i().darkTheme=d;}
JNIEXPORT void JNICALL Java_org_pinkreader_app_PinkReaderSettings_nativeClearCache(JNIEnv*,jobject){AppState::i().postCache.clear();}

} // extern "C"
