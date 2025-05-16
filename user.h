#ifndef USER_H
#define USER_H
#include <QString>
#include<qjsonobject.h>
#include"itempost.h"
#include"chatroom.h"
#include<vector>
class User {
public:
    User(const QString& username, const QString& password);//创建

    // 序列化为JSON对象
    QJsonObject toJson() const;
    static User fromJson(const QJsonObject& json);

    // Getter方法
    QString getUsername() const;
    bool checkPassword(const QString& inputPw) const;

    //行为方法
    void releasePost(ItemPost* p);//发布商品
    void deletePostr(ItemPost* p);//下架商品
    void chat(chatroom* p);//加入聊天
    void addFvourite(ItemPost* p);//添加收藏
private:
    QString username;
    QString password;
    std::vector<ItemPost*> postPointer;//指向发布的商品
    std::vector<chatroom*> historyChat;//历史聊天记录
    std::vector<ItemPost*> myFavourite;//收藏


};
#endif // USER_H
