#ifndef CHATROOM_H
#define CHATROOM_H

#include "chatmessage.h"
#include "user.h"
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QFileSystemWatcher>
#include <vector>

class ChatRoom : public QWidget {
    Q_OBJECT

public:
    explicit ChatRoom(User* user1, User* user2, User* currentUser, QWidget *parent = nullptr);
    void addMessage(const QString& content, User* sender);//添加信息
    void loadHistory();//加载历史信息（也就是储存在本地的信息）
    bool hasUser(User* user) const ;//聊天室中是否包含某个用户
    int getUnreadCount(User* user) const;//获取指定用户的未读信息数量
    void markAsRead(User* user); //标记指定用户的消息为已读
    void markAsUnread(User* user);//标记指定用户的消息为未读
    void showEvent(QShowEvent* event) override;  // 使用showEvent替代show

private slots:
    void sendMessage();
    void displayMessage(const ChatMessage& message);
    void saveHistory();

private:
    User* user1;
    User* user2;
    User* currentUser;  // 当前登录用户
    std::vector<ChatMessage> messageHistory;
    QString historyFilePath;
    int unreadCount1 = 0;  // user1的未读消息数
    int unreadCount2 = 0;  // user2的未读消息数

    // UI组件
    QTextEdit* messageDisplay;
    QLineEdit* messageInput;
    QPushButton* sendButton;

    void setupUI();
    QString getCurrentTime() const;
    QString getHistoryFilePath() const;
    void updateTitle();  // 更新窗口标题，显示未读消息数
};

#endif // CHATROOM_H
/*使用方法

// 创建聊天室
ChatRoom* chatWindow = new ChatRoom(currentUser, targetUser);
chatWindow->show();

//应该在user中调用
*/
