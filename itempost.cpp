#pragma once
#include "itempost.h"
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>
#include<user.h>
#include<globalusers.h>
ItemPost::ItemPost(const QString& title, const QString& desc,
                   double price, const User* owner,
                   const std::vector<QString>& initialFlags)
    : title(title)
    , description(desc)
    , price(price)
    , owner(owner)
    , postTime(QDateTime::currentDateTime())
    , flags(initialFlags)
{
}

QJsonObject ItemPost::toJson() const
{
    QJsonObject json;
    json["title"] = title;
    json["description"] = description;
    json["price"] = price;
    json["owner"] = owner->getUsername();  // 只保存用户名
    json["postTime"] = postTime.toString(Qt::ISODate);

    // 序列化 flags
    QJsonArray flagsArray;
    for (const auto& flag : flags) {
        flagsArray.append(flag);
    }
    json["flags"] = flagsArray;

    // 序列化图片
    QJsonArray imagesArray;
    for (const auto& image : images) {
        imagesArray.append(image);
    }
    json["images"] = imagesArray;

    return json;
}

ItemPost ItemPost::fromJson(const QJsonObject& json)
{
    QString ownerName = json["owner"].toString();
    const User* owner = nullptr;

    // 在全局 users 向量中查找用户
    for (const auto& user : users) {
        if (user.getUsername() == ownerName) {
            owner = &user;
            break;
        }
    }

    ItemPost post(
        json["title"].toString(),
        json["description"].toString(),
        json["price"].toDouble(),
        owner
        );
    post.postTime = QDateTime::fromString(json["postTime"].toString(), Qt::ISODate);

    // 反序列化 flags
    QJsonArray flagsArray = json["flags"].toArray();
    for (const auto& flag : flagsArray) {
        post.flags.push_back(flag.toString());
    }

    // 反序列化图片
    QJsonArray imagesArray = json["images"].toArray();
    for (const auto& image : imagesArray) {
        post.images.push_back(image.toString());
    }

    return post;
}

void ItemPost::removeFlag(const QString& flag)
{
    flags.erase(
        std::remove(flags.begin(), flags.end(), flag),
        flags.end()
        );
}

bool ItemPost::hasFlag(const QString& flag) const
{
    return std::find(flags.begin(), flags.end(), flag) != flags.end();
}

void ItemPost::removeImage(const QString& imagePath)
{
    images.erase(
        std::remove(images.begin(), images.end(), imagePath),
        images.end()
        );
}

void ItemPost::setMainImage(const QString& imagePath)
{
    auto it = std::find(images.begin(), images.end(), imagePath);
    if (it != images.end()) {
        // 如果找到了图片，将其移到第一张
        images.erase(it);
        images.insert(images.begin(), imagePath);
    }
}
