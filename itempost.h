#ifndef ITEMPOST_H
#define ITEMPOST_H
#include <QDateTime>
#include<qstring.h>
class User;
class ItemPost {
public:
    ItemPost(const QString& title, const QString& desc, double price, const User* owner,
             const std::vector<QString>& initialFlags = std::vector<QString>());
    //构造函数，要求传入标题，描述，价格，发布者，标签（有缺省值空）

    //存储与读取方法
    QJsonObject toJson() const;
    static ItemPost fromJson(const QJsonObject& json);

    // Getter方法
    QString getTitle() const{return title;};
    //返回标题
    QString getDescription() const { return description; }
    //返回描述
    double getPrice() const { return price; }
    //返回价格
    const User* getOwner() const { return owner; }
    //发布者
    QDateTime getPostTime() const { return postTime; }
    //返回发布时间
    const std::vector<QString>& getFlags() const { return flags; }
    //返回标签
    bool hasFlag(const QString& flag) const;
    //判断是否拥有某个标签
    const std::vector<QString>& getImages() const { return images; }
    //返回所有图片路径
    QString getMainImage() const { return images.empty() ? QString() : images[0]; }
    //返回主图片路径（第一张图片）

    // Setter方法
    void addFlag(const QString& flag) { flags.push_back(flag); }
    //添加标签
    void removeFlag(const QString& flag);
    //移除标签
    void clearFlags() { flags.clear(); }
    //清空标签

    //与图片有关的方法
    void addImage(const QString& imagePath) { images.push_back(imagePath); }
    //添加图片
    void removeImage(const QString& imagePath);
    //移除图片
    void clearImages() { images.clear(); }
    //清空图片
    void setMainImage(const QString& imagePath);
    //设置主图片（将图片移到第一张）

    // 新增：收藏相关功能
    void addToFavorites(const User* user);
    void removeFromFavorites(const User* user);
    bool isFavoritedBy(const User* user) const;
    int getFavoriteCount() const { return favoritedBy.size(); }
    const std::vector<QString>& getFavoritedBy() const { return favoritedBy; }

private:
    QString title;
    QString description;
    double price;
    const User* owner;
    QDateTime postTime;
    std::vector<QString> flags;
    std::vector<QString> images;  // 存储图片路径
    
    // 新增：收藏功能相关
    std::vector<QString> favoritedBy;  // 收藏此商品的用户ID列表
};
#endif // ITEMPOST_H
