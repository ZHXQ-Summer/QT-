#include "chatroom.h"
#include <QScrollBar>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QFileSystemWatcher>
#include <QTimer>

ChatRoom::ChatRoom(User* user1, User* user2, User* currentUser, QWidget *parent)
    : QWidget(parent), user1(user1), user2(user2), currentUser(currentUser)
{
    setupUI();
    historyFilePath = getHistoryFilePath();
    loadHistory();
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
    
    // 设置窗口属性
    updateTitle();
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
        markAsUnread(sender == user1 ? user2 : user1);
    }
    
    saveHistory();
}

void ChatRoom::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    markAsRead(currentUser);  // 显示窗口时清除当前用户的未读消息
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
    for (const auto& message : messageHistory) {
        displayMessage(message);
    }
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
    
    return filename;
}

QString ChatRoom::getCurrentTime() const
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

void ChatRoom::markAsRead(User* user) { 
        if (user == user1) unreadCount1 = 0;
        else if (user == user2) unreadCount2 = 0;
        updateTitle();
    }//标记指定用户的消息为已读

void ChatRoom::markAsUnread(User* user) { 
        if (user == user1) unreadCount1++;
        else if (user == user2) unreadCount2++;
        updateTitle();
    }//标记指定用户的消息为未读

int ChatRoom::getUnreadCount(User* user) const { 
        return user == user1 ? unreadCount1 : unreadCount2;
    }//获取指定用户的未读信息数量

bool ChatRoom::hasUser(User* user) const { return user == user1 || user == user2; }