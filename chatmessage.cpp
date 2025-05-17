 #include "chatmessage.h"
#include <QJsonDocument>
#include <QJsonArray>

ChatMessage::ChatMessage(const QString& sender, const QString& receiver,
                         const QString& content)
    : sender(sender), receiver(receiver), content(content)
{
    sendTime = QDateTime::currentDateTime();
}

QJsonObject ChatMessage::toJson() const
{
    QJsonObject json;
    json["sender"] = sender;
    json["receiver"] = receiver;
    json["content"] = content;
    json["sendTime"] = sendTime.toString(Qt::ISODate);
    return json;
}

ChatMessage ChatMessage::fromJson(const QJsonObject& json)
{
    ChatMessage message(
        json["sender"].toString(),
        json["receiver"].toString(),
        json["content"].toString()
    );
    message.sendTime = QDateTime::fromString(json["sendTime"].toString(), Qt::ISODate);
    return message;
}

bool ChatMessage::saveMessages(const QString& filename, const std::vector<ChatMessage>& messages)
{
    QJsonArray jsonArray;
    for (const auto& message : messages) {
        jsonArray.append(message.toJson());
    }

    QJsonDocument doc(jsonArray);
    QFile file(filename);
    
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson());
    return true;
}

std::vector<ChatMessage> ChatMessage::loadMessages(const QString& filename)
{
    std::vector<ChatMessage> messages;
    QFile file(filename);
    
    if (!file.open(QIODevice::ReadOnly)) {
        return messages;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isArray()) {
        QJsonArray jsonArray = doc.array();
        for (const auto& value : jsonArray) {
            if (value.isObject()) {
                messages.push_back(ChatMessage::fromJson(value.toObject()));
            }
        }
    }

    return messages;
}