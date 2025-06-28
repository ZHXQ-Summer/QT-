#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include<vector>
#include<qstring.h>
#include<qstackedwidget.h>
#include<QLabel>
#include<QRubberBand>
#include<itempost.h>
#include<chatroom.h>
class User;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(QString username,QWidget *parent=nullptr);
    ~MainWindow();
    void load_user();
    void createPages();  // 创建所有页面
    void switchPage(int index);
    QWidget* mine_create();
    QWidget* itempost_create();
    void paintEvent(QPaintEvent * event);
    void showAuthorInfo(const User& author);
    void createChatRoom(User* targetuser);
    void load_chat();
    QWidget* createChatRoomCard(ChatRoom* room);
    QWidget* createChatListWidget();
    void highlightKeywords(QWidget* card, const QString& keyword);
    // 新增：评分系统相关方法
    void showRatingDialog(const User& targetUser);
    void showUserRatings(const User& user);
private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void edit();
    void handleAvatarUpload(QLabel *avatarLabel);
    void showDetailPage(const ItemPost& post);// 显示单个商品的详情页
    void showPostEditor();
    void refreshItemPosts();
    void showMyPosts();
    // 新增：商品管理功能
    void editItemPost(const ItemPost& post);
    void deleteItemPost(const ItemPost& post);
    void showItemManagementDialog();
    // 新增：收藏功能
    void toggleFavorite(const ItemPost& post);
    void showFavoritesDialog();
    // 新增：更新未读消息显示
    void updateUnreadMessageDisplay();
private:
    std::vector<ChatRoom*> activeChatRooms;
    void loadItemPosts(); // 用于重新加载数据
    QWidget* createItemPostList(const std::vector<ItemPost>& posts); // 创建商品列表视图
    QWidget* createSinglePost(const ItemPost& post);    // 创建单个商品卡片
    QWidget* createDetailDialogContent(const ItemPost& post, QDialog* parent);    // 创建商品详情页
    struct CropData {
        QDialog* dialog;
        QLabel* imageLabel;
        QRubberBand* rubberBand;
        QPoint origin;
    } m_cropData;
    void updateAvatarDisplay(QLabel *avatarLabel);
    bool eventFilter(QObject *obj, QEvent *event);
    QPixmap cropImage(const QPixmap &original);
    QString currentAvatarPath;
    QPushButton *loginBtn;     // 登录按钮
    QPushButton *registerBtn;  // 注册按钮
    QPushButton *exitBtn;      // 退出按钮
    Ui::MainWindow *ui;
    bool log=false;
    QStackedWidget *stackWidget; // 页面容器
    QWidget *page1;              // 页面1
    QWidget *page2;              // 页面2
    QWidget *page3;              // 页面3
};
#endif // MAINWINDOW_H

