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
    User* user1;
    User* user2;
    std::vector<ChatMessage> messageHistory;
    void markAsRead(User* user); //标记指定用户的消息为已读，一般传参传currentuser即可
    void markAsUnread(User* user);//标记指定用户的消息为未读
    QString getLastMessageContent() const {
        return messageHistory.empty() ?
                   "暂无消息" :
                   messageHistory.back().getContent().left(20) + "...";
    }

    // 获取最后一条消息时间
    QDateTime getLastMessageTime() const {
        return messageHistory.empty() ?
                   QDateTime::currentDateTime() :
                   messageHistory.back().getSendTime();
    }
    User* getOtherUser(const User *curUser) const {
        return (curUser->getUserID() == user1->getUserID()) ? user2 : user1;
    }
signals:  // 新增信号声明
    void aboutToClose(ChatRoom* room);
protected:
    void showEvent(QShowEvent* event) override;//在调用show和raise时，会自动调用这个函数
    void closeEvent(QCloseEvent* event) override {
        emit aboutToClose(this);  // 发出关闭信号
        QWidget::closeEvent(event);  // 执行默认关闭操作
    }

private slots:
    void sendMessage();//发送信息，按下发送按钮，或者回车，会自动调用这个函数
    void displayMessage(const ChatMessage& message);
    void saveHistory();

private:
    User* currentUser;
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

    friend void User::receiveNewChat(User* p, ChatRoom* q);

};

#endif // CHATROOM_H
/*使用方法

// 创建聊天室
ChatRoom* chatWindow = new ChatRoom(currentUser, targetUser);
chatWindow->show();

//应该在user中调用
*/
