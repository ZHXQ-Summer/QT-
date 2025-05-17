#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H
#include<Qstring>
#include <QDateTime>
#include <QJsonObject>
#include <QFile>
#include <QDir>

class ChatMessage {
public:
    ChatMessage(const QString& sender, const QString& receiver,
                const QString& content);

    // 序列化方法
    QJsonObject toJson() const;
    static ChatMessage fromJson(const QJsonObject& json);

    // Getter方法
    QString getSender() const { return sender; }
    QString getReceiver() const { return receiver; }
    QString getContent() const { return content; }
    QDateTime getSendTime() const { return sendTime; }

    // 静态方法：保存和加载消息历史
    static bool saveMessages(const QString& filename, const std::vector<ChatMessage>& messages);
    static std::vector<ChatMessage> loadMessages(const QString& filename);

private:
    QString sender;
    QString receiver;
    QString content;
    QDateTime sendTime;
};
#endif // CHATMESSAGE_H
