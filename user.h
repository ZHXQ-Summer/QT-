#ifndef USER_H
#define USER_H
#include <QString>
#include<qjsonobject.h>
#include"itempost.h"
#include<vector>
class ChatRoom;
class User {
public:
    User(const QString& username,
         const QString& password,
         const QString& email,
         int authLevel = 0);

    // 序列化为JSON对象
    QJsonObject toJson() const;
    static User fromJson(const QJsonObject& json);
    // Getter方法
    QString getUsername() const;
    QString getUserID() const;
    QDateTime getRegTime() const;
    QString getEmail() const;
    void selectUsername(QString a) ;
    void selectpwd(QString a) ;
    void selectEmail(QString a) ;
    int getAuthLevel() const;
    QString getAvatarPath() const;
    void setAvatarPath(const QString &path);
    bool checkPassword(const QString& inputPw) const;//登陆时检查密码

    //行为方法
    void releasePost(ItemPost* p);//发布商品
    void deletePostr(ItemPost* p);//下架商品
    void chat(User* p);//发起聊天，this是当前用户
    void receiveNewChat(User* p,ChatRoom* q);//接收到新聊天
    void openHistoryChat(ChatRoom* p);//打开历史聊天
    void addFvourite(ItemPost* p);//添加收藏

private:
    QString avatarPath;
    QString username;
    QString password;
    QString userID;
    QDateTime regTime;
    QString email;
    int authLevel;
    std::vector<ItemPost*> postPointer;//指向发布的商品
    std::vector<ChatRoom*> historyChat;//历史聊天记录
    std::vector<ItemPost*> myFavourite;//收藏


};
#endif // USER_H
