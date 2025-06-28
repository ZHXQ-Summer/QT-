#include "chatroom.h"
#include <QScrollBar>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include<user.h>
#include <QDebug>

ChatRoom::ChatRoom(User* user1, User* user2, User* currentUser, QWidget *parent)
    : QWidget(parent), user1(user1), user2(user2), currentUser(currentUser)
{
    qDebug() << "创建聊天室:" << user1->getUsername() << "与" << user2->getUsername() 
             << "当前用户:" << currentUser->getUsername();
    
    setupUI();
    historyFilePath = getHistoryFilePath();
    unreadStateFilePath = getUnreadStateFilePath();
    
    qDebug() << "历史文件路径:" << historyFilePath;
    qDebug() << "未读状态文件路径:" << unreadStateFilePath;
    
    loadUnreadState(); // 先加载未读状态
    loadHistory(); // 再加载历史消息
    
    qDebug() << "聊天室创建完成，未读数量 - user1:" << unreadCount1 << "user2:" << unreadCount2;
}

void ChatRoom::setupUI()
{
    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // 创建消息显示区域
    messageDisplay = new QTextEdit(this);
    messageDisplay->setReadOnly(true);
    mainLayout->addWidget(messageDisplay);
    
    // 创建输入区域
    QHBoxLayout* inputLayout = new QHBoxLayout();
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("发送", this);
    
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);
    mainLayout->addLayout(inputLayout);
    
    // 连接信号和槽
    connect(sendButton, &QPushButton::clicked, this, &ChatRoom::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatRoom::sendMessage);
    
    // 设置窗口属性（不在这里调用updateTitle，等未读状态加载后再调用）
    setWindowTitle("聊天室");
    resize(400, 600);
}

void ChatRoom::addMessage(const QString& content, User* sender)
{
    ChatMessage message(sender->getUsername(), 
                       (sender == user1 ? user2 : user1)->getUsername(),
                       content);
    messageHistory.push_back(message);
    displayMessage(message);
    
    // 如果消息不是当前用户发送的，标记接收方为未读
    if (sender != currentUser) {
        User* receiver = (sender == user1 ? user2 : user1);
        if (receiver == user1) {
            unreadCount1++;
        } else if (receiver == user2) {
            unreadCount2++;
        }
        updateTitle();
        saveUnreadState(); // 保存未读状态
        emit unreadCountChanged(); // 发出未读消息数量变化信号
    }
    
    saveHistory();
}

void ChatRoom::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    // 只有当窗口真正被激活时才标记为已读
    // 这里暂时注释掉，让用户手动点击时才标记为已读
    // markAsRead(currentUser);
}

void ChatRoom::updateTitle()
{
    QString title = QString("聊天室 - %1 与 %2").arg(user1->getUsername()).arg(user2->getUsername());
    int currentUnread = getUnreadCount(currentUser);
    if (currentUnread > 0) {
        title += QString(" (%1条未读)").arg(currentUnread);
    }
    setWindowTitle(title);
}

void ChatRoom::sendMessage()
{
    QString content = messageInput->text().trimmed();
    if (!content.isEmpty()) {
        addMessage(content, currentUser);
        messageInput->clear();
    }
}

void ChatRoom::displayMessage(const ChatMessage& message)
{
    QString style = message.getSender() == currentUser->getUsername() ? 
                   "background-color: #e3f2fd; margin: 5px; padding: 5px; border-radius: 5px;" :
                   "background-color: #f5f5f5; margin: 5px; padding: 5px; border-radius: 5px;";
    
    QString displayText = QString("<div style='%1'>"
                                "<b>%2</b> (%3):<br>%4</div>")
        .arg(style)
        .arg(message.getSender())
        .arg(message.getSendTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg(message.getContent());
    
    messageDisplay->append(displayText);
    
    QScrollBar* scrollbar = messageDisplay->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}

void ChatRoom::loadHistory()
{
    messageHistory = ChatMessage::loadMessages(historyFilePath);
    messageDisplay->clear();
    
    // 只显示消息，不重新计算未读数量
    for (const auto& message : messageHistory) {
        displayMessage(message);
    }
    
    // 更新窗口标题（此时未读状态已经加载）
    updateTitle();
}

void ChatRoom::saveHistory()
{
    if (!ChatMessage::saveMessages(historyFilePath, messageHistory)) {
        QMessageBox::warning(this, "警告", "保存聊天记录失败！");
    }
}

QString ChatRoom::getHistoryFilePath() const
{
    // 创建聊天记录目录
    QDir dir("chat_history");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // 生成文件名：user1_user2.json
    QString filename = QString("chat_history/%1_%2.json")
        .arg(user1->getUsername())
        .arg(user2->getUsername());
    if(QFile::exists(filename)){
        return filename;
    }
    else{
        QString filename = QString("chat_history/%1_%2.json")
        .arg(user2->getUsername())
            .arg(user1->getUsername());
        return filename;}
}

QString ChatRoom::getCurrentTime() const
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

QString ChatRoom::getUnreadStateFilePath() const
{
    // 创建未读状态目录
    QDir dir("chat_unread_state");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // 生成文件名：user1_user2_unread.json
    QString filename = QString("chat_unread_state/%1_%2_unread.json")
        .arg(user1->getUsername())
        .arg(user2->getUsername());
    if(QFile::exists(filename)){
        return filename;
    }
    else{
        QString filename = QString("chat_unread_state/%1_%2_unread.json")
        .arg(user2->getUsername())
            .arg(user1->getUsername());
        return filename;}
}

void ChatRoom::loadUnreadState()
{
    QFile file(unreadStateFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            QJsonObject json = doc.object();
            unreadCount1 = json["unreadCount1"].toInt(0);
            unreadCount2 = json["unreadCount2"].toInt(0);
            qDebug() << "加载未读状态:" << unreadStateFilePath 
                     << "user1:" << unreadCount1 << "user2:" << unreadCount2;
        }
        file.close();
    } else {
        qDebug() << "未找到未读状态文件:" << unreadStateFilePath;
    }
    updateTitle();
}

void ChatRoom::saveUnreadState()
{
    QJsonObject json;
    json["unreadCount1"] = unreadCount1;
    json["unreadCount2"] = unreadCount2;
    
    QJsonDocument doc(json);
    QFile file(unreadStateFilePath);
    
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "保存未读状态:" << unreadStateFilePath 
                 << "user1:" << unreadCount1 << "user2:" << unreadCount2;
    } else {
        qDebug() << "保存未读状态失败:" << unreadStateFilePath;
    }
}

void ChatRoom::markAsRead(User* user) { 
        if (user == user1) unreadCount1 = 0;
        else if (user == user2) unreadCount2 = 0;
        updateTitle();
        saveUnreadState(); // 保存未读状态
        emit unreadCountChanged(); // 发出未读消息数量变化信号
    }//标记指定用户的消息为已读

void ChatRoom::markAsUnread(User* user) { 
        if (user == user1) unreadCount1++;
        else if (user == user2) unreadCount2++;
        updateTitle();
        saveUnreadState(); // 保存未读状态
        emit unreadCountChanged(); // 发出未读消息数量变化信号
    }//标记指定用户的消息为未读

int ChatRoom::getUnreadCount(User* user) const { 
        int count = user == user1 ? unreadCount1 : unreadCount2;
        qDebug() << "获取未读数量:" << user->getUsername() << "count:" << count;
        return count;
    }//获取指定用户的未读信息数量

bool ChatRoom::hasUser(User* user) const { return user->getUserID() == user1->getUserID() || user->getUserID() == user2->getUserID(); }
