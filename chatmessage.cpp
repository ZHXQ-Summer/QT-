#pragma once
#include <QDateTime>

class ChatMessage {
public:
    ChatMessage(const QString& sender, const QString& receiver,
                const QString& content);

    QJsonObject toJson() const;
    static ChatMessage fromJson(const QJsonObject& json);

private:
    QString sender;
    QString receiver;
    QString content;
    QDateTime sendTime;
};
