#ifndef CHATROOM_H
#define CHATROOM_H
#include "chatmessage.h"
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
#include<user.h>
class ChatRoom : public QWidget {
    Q_OBJECT

public:
    explicit ChatRoom(User* user1, User* user2,User* current_User, QWidget *parent = nullptr);
    void addMessage(const QString& content, User* sender);//添加信息
    void loadHistory();//加载历史信息（也就是储存在本地的信息）
    bool hasUser(User* user) const ;//聊天室中是否包含某个用户
    int getUnreadCount(User* user) const;//获取指定用户的未读信息数量

protected:
    void showEvent(QShowEvent* event) override;//在调用show和raise时，会自动调用这个函数

private slots:
    void sendMessage();//发送信息，按下发送按钮，或者回车，会自动调用这个函数
    void displayMessage(const ChatMessage& message);
    void saveHistory();

private:
    User* user1;
    User* user2;
    User* currentUser;
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

    void markAsRead(User* user); //标记指定用户的消息为已读，一般传参传currentuser即可
    void markAsUnread(User* user);//标记指定用户的消息为未读
    friend void User::receiveNewChat(User* p, ChatRoom* q);

};

#endif // CHATROOM_H
/*使用方法

// 创建聊天室
ChatRoom* chatWindow = new ChatRoom(currentUser, targetUser);
chatWindow->show();

//应该在user中调用
*/
