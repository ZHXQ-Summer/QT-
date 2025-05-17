#include "user.h"
#include <QCryptographicHash>
#include <QJsonArray>
#include <QSystemTrayIcon>
#include <QApplication>

User::User(const QString& username, const QString& password)
    : username(username)
{
    // 对密码进行SHA256哈希处理
    QByteArray passwordData = password.toUtf8();
    this->password = QString(QCryptographicHash::hash(passwordData, 
                                                    QCryptographicHash::Sha256).toHex());
}

QJsonObject User::toJson() const
{
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    
    // 序列化发布的商品
    QJsonArray postsArray;
    for (const auto& post : postPointer) {
        if (post) {
            postsArray.append(post->toJson());
        }
    }
    json["posts"] = postsArray;
    
    // 序列化收藏的商品
    QJsonArray favoritesArray;
    for (const auto& favorite : myFavourite) {
        if (favorite) {
            favoritesArray.append(favorite->toJson());
        }
    }
    json["favorites"] = favoritesArray;
    
    return json;
}

User User::fromJson(const QJsonObject& json)
{
    User user(json["username"].toString(), "");
    user.password = json["password"].toString();
    
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
    
    return user;
}

QString User::getUsername() const
{
    return username;
}

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