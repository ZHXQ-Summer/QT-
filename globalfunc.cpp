#include "global_itempost.h"
#include "user.h"
#include <vector>
#include <algorithm>


auto findPost(QString title="",
              double price=-1,
              QString owner="",
              QDateTime afterpostTime=QDateTime::fromString("1970-01-01 00:00:00", Qt::ISODate),
              std::vector<QString> flags=std::vector<QString>()) {

    std::vector<std::vector<ItemPost>::iterator> result;

    // 遍历所有帖子
    for (auto it = posts.begin(); it != posts.end(); ++it) {
        bool match = true;

        // 检查标题
        if (!title.isEmpty() && !it->getTitle().contains(title, Qt::CaseInsensitive)) {
            match = false;
        }

        // 检查价格
        if (price != -1 && it->getPrice() != price) {
            match = false;
        }

        // 检查发布者
        if (!owner.isEmpty() && it->getOwner()->getUsername() != owner) {
            match = false;
        }

        // 检查发布时间
        if (it->getPostTime() < afterpostTime) {
            match = false;
        }

        // 检查标签
        if (!flags.empty()) {
            bool hasAllFlags = true;
                    hasAllFlags = false;
            if (!hasAllFlags) {
                match = false;
            }
        }

        // 如果所有条件都匹配，将迭代器添加到结果中
        if (match) {
            result.push_back(it);
        }
    }

    return result;
}
/*！！！返回类型是vector<vector<ItemPost>::iterator>
！！！传入的flags是vector<QString>
title匹配逻辑是包含关系
price匹配逻辑是等于
owner匹配逻辑是等于
postTime匹配逻辑是大于等于
flags匹配逻辑是包含关系
缺省的情况就不会进行检查
*/
