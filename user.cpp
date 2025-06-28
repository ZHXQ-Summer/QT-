#include "user.h"
#include <QCryptographicHash>
#include <QJsonArray>
#include <QSystemTrayIcon>
#include <QApplication>
#include<chatroom.h>
#include"itempost.h"
#include<qjsondocument.h>
User::User(const QString& username,
           const QString& password,
           const QString& email,
           int authLevel)
    : username(username),
    email(email),
    authLevel(authLevel)
{
    // 对密码进行SHA256哈希处理
    QByteArray passwordData = password.toUtf8();
    this->password = QString(QCryptographicHash::hash(passwordData, 
                                                    QCryptographicHash::Sha256).toHex());
    userID = QUuid::createUuid().toString(QUuid::WithoutBraces);
    regTime = QDateTime::currentDateTime();
}

QJsonObject User::toJson() const
{
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    json["userID"] = userID;
    json["regTime"] = regTime.toString(Qt::ISODate);
    json["email"] = email;
    json["authLevel"] = authLevel;
    json["avatarpath"]=avatarPath;
    // 序列化发布的商品
    QJsonArray postsArray;
    for (const auto& post : postPointer) {
        if (post) {
            postsArray.append(post->ItemPost::toJson());
        }
    }
    json["posts"] = postsArray;
    
    // 序列化收藏的商品
    QJsonArray favoritesArray;
    for (const auto& favorite : myFavourite) {
        if (favorite) {
            favoritesArray.append(favorite->ItemPost::toJson());
        }
    }
    json["favorites"] = favoritesArray;
    
    // 新增：序列化评分数据
    QJsonObject ratingsObj;
    for (auto it = ratings.begin(); it != ratings.end(); ++it) {
        ratingsObj[it.key()] = it.value();
    }
    json["ratings"] = ratingsObj;
    
    QJsonObject commentsObj;
    for (auto it = comments.begin(); it != comments.end(); ++it) {
        commentsObj[it.key()] = it.value();
    }
    json["comments"] = commentsObj;
    
    return json;
}

User User::fromJson(const QJsonObject& json)
{
    User user(
        json["username"].toString(),
        "",  // 密码从加密字段读取
        json["email"].toString(),
        json["authLevel"].toInt()
        );

    // 设置特殊字段
    user.userID = json["userID"].toString();
    user.regTime = QDateTime::fromString(json["regTime"].toString(), Qt::ISODate);
    user.password = json["password"].toString();
    user.avatarPath=json["avatarpath"].toString();
    
    // 反序列化发布的商品
    QJsonArray postsArray = json["posts"].toArray();
    for (const auto& postJson : postsArray) {
        if (postJson.isObject()) {
            ItemPost* post = new ItemPost(ItemPost::fromJson(postJson.toObject()));
            user.postPointer.push_back(post);
        }
    }
    
    // 反序列化收藏的商品
    QJsonArray favoritesArray = json["favorites"].toArray();
    for (const auto& favoriteJson : favoritesArray) {
        if (favoriteJson.isObject()) {
            ItemPost* favorite = new ItemPost(ItemPost::fromJson(favoriteJson.toObject()));
            user.myFavourite.push_back(favorite);
        }
    }
    
    // 新增：反序列化评分数据
    QJsonObject ratingsObj = json["ratings"].toObject();
    for (auto it = ratingsObj.begin(); it != ratingsObj.end(); ++it) {
        user.ratings[it.key()] = it.value().toInt();
    }
    
    QJsonObject commentsObj = json["comments"].toObject();
    for (auto it = commentsObj.begin(); it != commentsObj.end(); ++it) {
        user.comments[it.key()] = it.value().toString();
    }
    
    return user;
}

QString User::getUsername() const { return username; }
QString User::getUserID() const { return userID; }
QDateTime User::getRegTime() const { return regTime; }
QString User::getEmail() const { return email; }
QString User::getAvatarPath() const {
    return avatarPath;
}

void User::setAvatarPath(const QString &path) {
    avatarPath = path;
}
void User::selectUsername(QString a){username=a;} ;
void User::selectpwd(QString a){    QByteArray passwordData = a.toUtf8();
    this->password = QString(QCryptographicHash::hash(passwordData,
                                                      QCryptographicHash::Sha256).toHex());} ;
void User::selectEmail(QString a){email=a;} ;
int User::getAuthLevel() const { return authLevel; }

bool User::checkPassword(const QString& inputPw) const
{
    QByteArray inputData = inputPw.toUtf8();
    QString hashedInput = QString(QCryptographicHash::hash(inputData, 
                                                         QCryptographicHash::Sha256).toHex());
    return hashedInput == password;
}

void User::releasePost(ItemPost* p)
{
    if (p) {
        postPointer.push_back(p);
    }
}

void User::deletePostr(ItemPost* p)
{
    if (p) {
        auto it = std::find(postPointer.begin(), postPointer.end(), p);
        if (it != postPointer.end()) {
            postPointer.erase(it);
            delete p;
        }
    }
}

void User::chat(User* p)
{
    if (p) {
        // 检查是否已有聊天室
        for (ChatRoom* room : historyChat) {
            if (room->hasUser(p)) {
                room->show();
                return;
            }
        }
        
        // 创建新的聊天室
        ChatRoom* newChat = new ChatRoom(this, p, this);
        historyChat.push_back(newChat);
        newChat->show();
        
        // 通知对方有新聊天
        p->receiveNewChat(this, newChat);
    }
}

void User::addFvourite(ItemPost* p)
{
    if (p) {
        // 检查是否已经收藏
        auto it = std::find(myFavourite.begin(), myFavourite.end(), p);
        if (it == myFavourite.end()) {
            myFavourite.push_back(p);
        }
    }
}

void User::receiveNewChat(User* p, ChatRoom* q)
{
    if (p && q) {
        // 检查是否已有聊天室
        for (ChatRoom* room : historyChat) {
            if (room->hasUser(p)) {
                room->show();
                return;
            }
        }
        
        // 将聊天室添加到历史记录
        historyChat.push_back(q);
        q->markAsUnread(this);  // 标记当前用户为未读
        q->show();
    }
}

void User::openHistoryChat(ChatRoom* p)
{
    if (p) {
        // 检查聊天室是否在历史记录中
        auto it = std::find(historyChat.begin(), historyChat.end(), p);
        if (it != historyChat.end()) {
            // 显示聊天窗口
            p->show();
            // 将窗口置于前台
            p->activateWindow();
            p->raise();
        }
    }
}

int User::getpostcount() const
{
    return postPointer.size();
}

// 新增：评分系统方法实现
void User::addRating(const QString& raterID, int rating, const QString& comment)
{
    // 限制评分范围在1-5之间
    rating = qBound(1, rating, 5);
    
    // 更新或添加评分
    ratings[raterID] = rating;
    comments[raterID] = comment;
}

double User::getAverageRating() const
{
    if (ratings.isEmpty()) {
        return 0.0;
    }
    
    int totalRating = 0;
    for (auto it = ratings.begin(); it != ratings.end(); ++it) {
        totalRating += it.value();
    }
    
    return static_cast<double>(totalRating) / ratings.size();
}

int User::getRatingCount() const
{
    return ratings.size();
}

QString User::getRatingLevel() const
{
    double avgRating = getAverageRating();
    
    if (avgRating >= 4.5) return "⭐⭐⭐⭐⭐ 优秀";
    else if (avgRating >= 4.0) return "⭐⭐⭐⭐ 良好";
    else if (avgRating >= 3.0) return "⭐⭐⭐ 一般";
    else if (avgRating >= 2.0) return "⭐⭐ 较差";
    else if (avgRating >= 1.0) return "⭐ 很差";
    else return "暂无评分";
}

bool User::hasRatedBy(const QString& raterID) const
{
    return ratings.contains(raterID);
}
