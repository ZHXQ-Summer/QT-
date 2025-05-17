#ifndef ITEMPOST_H
#define ITEMPOST_H
#include <QDateTime>

class ItemPost {
public:
    ItemPost(const QString& title, const QString& desc,
             double price, const QString& owner);

    QJsonObject toJson() const;
    static ItemPost fromJson(const QJsonObject& json);

    // Getter方法
    QString getTitle() const;
    QString getCategory() const;

private:
    QString title;
    QString description;
    double price;
    QString owner;
    QDateTime postTime;
    bool isSold = false;
};
#endif // ITEMPOST_H