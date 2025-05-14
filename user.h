#ifndef USER_H
#define USER_H
#include <QString>
#include<qjsonobject.h>
class User {
public:
    User(const QString& username, const QString& hashedPw);

    // 序列化为JSON对象
    QJsonObject toJson() const;
    static User fromJson(const QJsonObject& json);

    // Getter方法
    QString getUsername() const;
    bool checkPassword(const QString& inputPw) const;

private:
    QString username;
    QString hashedPassword; // SHA256哈希值
};
#endif // USER_H
