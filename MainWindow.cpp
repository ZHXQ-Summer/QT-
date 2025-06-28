#include <MainWindow.h>
#include<stdio.h>
#include "LoginDialog.h"
#include "RegisterDialog.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>
#include "ui_MainWindow.h"
#include<QMainWindow>
#include<qlabel.h>
#include<globalusers.h>
#include<EditDialog.h>
#include<CQlabel.h>
#include<QFileDialog>
#include<QStandardPaths>
#include<qmessagebox.h>
#include<qrubberband.h>
#include<qpainter.h>
#include<itempost.h>
#include<QScrollArea>
#include<global_itempost.h>
#include<qtextedit.h>
#include <QDoubleSpinBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include<QFileInfo>
#include<qstandardpaths.h>
#include<QGroupBox>
#include<globalfunc.h>
#include <QList>
#include "RatingDialog.h"

void loadUsers() {
    QFile file("users.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray jsonArray = doc.array();
        for (const auto& value : jsonArray) {
            if (value.isObject()) {
                users.push_back(User::fromJson(value.toObject()));
            }
        }
        file.close();
    }
}

void saveUsers() {
    QJsonArray jsonArray;
    for (const auto& user : users) {
        jsonArray.append(user.toJson());
    }
    QJsonDocument doc(jsonArray);

    QFile file("users.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}
void load_item() {
    QFile file("item.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray jsonArray = doc.array();
        for (const auto& value : jsonArray) {
            if (value.isObject()) {
                posts.push_back(ItemPost::fromJson(value.toObject()));
            }
        }
        file.close();
    }
}
void save_item() {
    QJsonArray jsonArray;
    for (const auto& post : posts) {
        jsonArray.append(post.toJson());
    }
    QJsonDocument doc(jsonArray);

    QFile file("item.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("开始界面");
    connect(ui->loginBtn, &QPushButton::clicked,this, &MainWindow::onLoginClicked);
    connect(ui->registerBtn, &QPushButton::clicked,this, &MainWindow::onRegisterClicked);
    connect(ui->exitBtn, &QPushButton::clicked,qApp, &QApplication::quit);
    ui->loginBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #FFFFFF;
        color: #555555;
        border-radius: 6px;
        padding: 12px 24px;
        font-size: 14px;
        font-weight: 500;
        border: 1px solid #E0E0E0;
        transition: all 0.2s ease;
    }
    QPushButton:hover {
        background-color: #F8F8F8;
        color: #333333;
        border-color: #D0D0D0;
    }
    QPushButton:pressed {
        background-color: #F0F0F0;
        border-color: #C0C0C0;
    }
    QPushButton:focus {
        outline: none;
        border: 1px solid #7ED957;
    }
)");
    ui->registerBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #FFFFFF;
        color: #555555;
        border-radius: 6px;
        padding: 12px 24px;
        font-size: 14px;
        font-weight: 500;
        border: 1px solid #E0E0E0;
        transition: all 0.2s ease;
    }
    QPushButton:hover {
        background-color: #F5F9FF;
        color: #3366CC;
        border-color: #B3D1FF;
    }
    QPushButton:pressed {
        background-color: #EBF2FF;
    }
)");
    ui->exitBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #FFFFFF;
        color: #555555;
        border-radius: 6px;
        padding: 12px 24px;
        font-size: 14px;
        font-weight: 500;
        border: 1px solid #E0E0E0;
        transition: all 0.2s ease;
    }
    QPushButton:hover {
        background-color: #FFF5F5;
        color: #CC3333;
        border-color: #FFB3B3;
    }
    QPushButton:pressed {
        background-color: #FFEBEB;
    }
)");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::load_chat(){
    for(auto user:users){
        QString filename = QString("chat_history/%1_%2.json")
        .arg(user.getUsername())
            .arg(cur_user->getUsername());
        if(QFile::exists(filename)){
            // 创建新聊天室
            User* other=new User(user);
            ChatRoom* newRoom = new ChatRoom(const_cast<User *>(cur_user), other,const_cast<User *>(cur_user));
            // 构造函数中已经处理了loadHistory和loadUnreadState
            // 连接未读消息数量变化信号
            connect(newRoom, &ChatRoom::unreadCountChanged, this, &MainWindow::updateUnreadMessageDisplay);
            activeChatRooms.emplace_back(newRoom);
            continue;
        }
        filename = QString("chat_history/%1_%2.json")
                       .arg(cur_user->getUsername())
                       .arg(user.getUsername());
        if(QFile::exists(filename)){
            // 创建新聊天室
            User* other=new User(user);
            ChatRoom* newRoom = new ChatRoom(const_cast<User *>(cur_user), other,const_cast<User *>(cur_user));
            // 构造函数中已经处理了loadHistory和loadUnreadState
            // 连接未读消息数量变化信号
            connect(newRoom, &ChatRoom::unreadCountChanged, this, &MainWindow::updateUnreadMessageDisplay);
            activeChatRooms.emplace_back(newRoom);
            continue;
        }
    }
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/w1.jpg"));
}

void MainWindow::onLoginClicked()
{
    LoginDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted) {
        QLayout *layout = this->layout();
        if (layout) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0))) {
                delete item->widget();
                delete item;
            }
        }
        log=true;
        if(log){
            load_chat();

            // 创建菜单栏
            QMenuBar* menubar = this->menuBar();
            menubar->setStyleSheet(R"(
                QMenuBar {
                    background: #FFFFFF;
                    border-bottom: 1px solid #E0E0E0;
                    padding: 5px;
                }
                QMenuBar::item {
                    padding: 8px 16px;
                    margin: 0 2px;
                    border-radius: 4px;
                    color: #333333;
                }
                QMenuBar::item:selected {
                    background: #F0F0F0;
                }
                QMenuBar::item:pressed {
                    background: #E0E0E0;
                }
            )");

            // 创建导航菜单
            QMenu* navMenu = new QMenu("导航菜单");
            navMenu->setStyleSheet(R"(
                QMenu {
                    background: white;
                    border: 1px solid #E0E0E0;
                    border-radius: 8px;
                    padding: 5px;
                }
                QMenu::item {
                    padding: 8px 20px;
                    border-radius: 4px;
                    margin: 2px 5px;
                    color: #333333;
                    font: 14px 'Microsoft YaHei';
                }
                QMenu::item:selected {
                    background: #F0F0F0;
                }
                QMenu::item:pressed {
                    background: #E0E0E0;
                }
                QMenu::separator {
                    height: 1px;
                    background: #E0E0E0;
                    margin: 5px 10px;
                }
            )");

            // 创建页面切换按钮
            QAction *page1Action = new QAction("👤 我的账号", this);
            QAction *page2Action = new QAction("🏠 主页", this);
            QAction *page3Action = new QAction("💬 私信", this);

            // 连接信号
            connect(page1Action, &QAction::triggered, [this](){
                switchPage(0);
                statusBar()->showMessage("已切换到个人主页", 2000);
            });
            connect(page2Action, &QAction::triggered, [this](){
                switchPage(1);
                statusBar()->showMessage("已切换到商品主页", 2000);
            });
            connect(page3Action, &QAction::triggered, [this](){
                switchPage(2);
                statusBar()->showMessage("已切换到私信页面", 2000);
            });

            // 添加按钮到菜单
            navMenu->addAction(page1Action);
            navMenu->addSeparator();
            navMenu->addAction(page2Action);
            navMenu->addSeparator();
            navMenu->addAction(page3Action);

            // 添加菜单到菜单栏
            menubar->addMenu(navMenu);

            // 创建状态栏
            statusBar()->setStyleSheet(R"(
                QStatusBar {
                    background: #F8F9FA;
                    color: #666666;
                    border-top: 1px solid #E0E0E0;
                    padding: 5px;
                }
            )");

            // 设置主窗口
            stackWidget = new QStackedWidget;
            setCentralWidget(stackWidget);
            createPages();

            // 设置窗口标题
            setWindowTitle("欢迎 " + cur_user->getUsername() + "!");
        }
    }
}
void MainWindow::onRegisterClicked()
{
    RegisterDialog dlg(this);
    dlg.exec();
    saveUsers();
}
void MainWindow::edit()
{
    EditDialog dlg(this);
    if(dlg.exec()==QDialog::Accepted){
        createPages();
    }

}
void MainWindow::load_user(){

};
void MainWindow::createPages() {
    if(stackWidget) {
        // 删除旧页面
        while(stackWidget->count() > 0) {
            QWidget* widget = stackWidget->widget(0);
            stackWidget->removeWidget(widget);
            delete widget;
        }}
    else{
        stackWidget = new QStackedWidget;
    }
    // 页面1（带背景色方便区分）
    page1 = mine_create();

    // 页面2
    page2 = itempost_create();

    // 页面3
    page3 =createChatListWidget();

    // 添加页面到容器
    stackWidget->addWidget(page1);
    stackWidget->addWidget(page2);
    stackWidget->addWidget(page3);
}
void MainWindow::switchPage(int index) {
    if (index >= 0 && index < stackWidget->count()) {
        stackWidget->setCurrentIndex(index);
        statusBar()->showMessage(QString("已切换到页面 %1").arg(index + 1), 2000);
    }
}
QWidget* MainWindow::mine_create() {
    QWidget *userInfoPage = new QWidget;
    userInfoPage->setStyleSheet(R"(
        QWidget {
            background: #F8F9FA;
            font-family: 'Microsoft YaHei';
        }
    )");

    // 整体垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout(userInfoPage);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(30);

    // 顶部卡片 - 基本信息
    QWidget *cardWidget = new QWidget;
    cardWidget->setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 15px;
            padding: 20px;
        }
        QWidget:hover {
            box-shadow: 0 4px 12px rgba(0,0,0,0.1);
        }
    )");

    QHBoxLayout *cardLayout = new QHBoxLayout(cardWidget);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(30);

    // 左侧：头像区域
    ClickedLabel *avatarLabel = new ClickedLabel;
    avatarLabel->setFixedSize(120, 120);
    avatarLabel->setCursor(Qt::PointingHandCursor);
    avatarLabel->setStyleSheet(R"(
        QLabel {
            background: white;
            border-radius: 60px;
            border: 3px solid #FF69B4;
            padding: 2px;
        }
        QLabel:hover {
            border-color: #FF1493;
            background: #FFF0F5;
        }
    )");
    connect(avatarLabel, &ClickedLabel::clicked, this, [this, avatarLabel]() {
        handleAvatarUpload(avatarLabel);
    });
    updateAvatarDisplay(avatarLabel);

    // 右侧：用户信息
    QWidget *infoWidget = new QWidget;
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(12);

    // 用户名和编辑按钮行
    QWidget *nameRow = new QWidget;
    QHBoxLayout *nameLayout = new QHBoxLayout(nameRow);
    nameLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *nameLabel = new QLabel(cur_user->getUsername());
    nameLabel->setStyleSheet(R"(
        font: bold 28px 'Microsoft YaHei';
        color: #333333;
    )");

    QPushButton *editProfileBtn = new QPushButton("编辑资料");
    editProfileBtn->setFixedSize(100, 36);
    editProfileBtn->setStyleSheet(R"(
        QPushButton {
            background: #FF69B4;
            color: white;
            border-radius: 18px;
            font: bold 14px 'Microsoft YaHei';
            padding: 8px 16px;
        }
        QPushButton:hover {
            background: #FF1493;
        }
        QPushButton:pressed {
            background: #FF69B4;
        }
    )");
    connect(editProfileBtn, &QPushButton::clicked, this, &MainWindow::edit);

    nameLayout->addWidget(nameLabel);
    nameLayout->addStretch();
    nameLayout->addWidget(editProfileBtn);

    // 用户ID和注册时间
    QLabel *idLabel = new QLabel(QString("ID: %1").arg(cur_user->getUserID()));
    idLabel->setStyleSheet(R"(
        font: 14px 'Microsoft YaHei';
        color: #666666;
        padding: 4px 0;
    )");

    QLabel *regTimeLabel = new QLabel(
        QString("注册时间: %1").arg(
            cur_user->getRegTime().toString("yyyy-MM-dd hh:mm:ss")
            )
        );
    regTimeLabel->setStyleSheet(R"(
        font: 14px 'Microsoft YaHei';
        color: #888888;
        padding: 4px 0;
    )");

    infoLayout->addWidget(nameRow);
    infoLayout->addWidget(idLabel);
    infoLayout->addWidget(regTimeLabel);
    infoLayout->addStretch();

    cardLayout->addWidget(avatarLabel);
    cardLayout->addWidget(infoWidget, 1);

    // 统计信息卡片
    QWidget *statsCard = new QWidget;
    statsCard->setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 15px;
            padding: 20px;
        }
    )");

    QHBoxLayout *statsLayout = new QHBoxLayout(statsCard);
    statsLayout->setContentsMargins(20, 20, 20, 20);
    statsLayout->setSpacing(30);

    // 统计项
    auto createStatItem = [](const QString& title, const QString& value) -> QWidget* {
        QWidget *item = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(item);
        layout->setSpacing(8);

        QLabel *valueLabel = new QLabel(value);
        valueLabel->setStyleSheet(R"(
            font: bold 24px 'Microsoft YaHei';
            color: #333333;
        )");
        valueLabel->setAlignment(Qt::AlignCenter);

        QLabel *titleLabel = new QLabel(title);
        titleLabel->setStyleSheet(R"(
            font: 14px 'Microsoft YaHei';
            color: #666666;
        )");
        titleLabel->setAlignment(Qt::AlignCenter);

        layout->addWidget(valueLabel);
        layout->addWidget(titleLabel);
        return item;
    };

    // 创建发布商品统计项
    QPushButton* postCountWidget = new QPushButton("发布商品");
    postCountWidget->setStyleSheet(R"(
        QPushButton {
            background: white;
            border-radius: 8px;
            padding: 24px 10px;
            border: none;
            font: bold 22px 'Microsoft YaHei';
            color: #333333;
        }
        QPushButton:hover {
            background: #F0F0F0;
        }
    )");
    postCountWidget->setMinimumHeight(80);
    connect(postCountWidget, &QPushButton::clicked, this, &MainWindow::showItemManagementDialog);
    statsLayout->addWidget(postCountWidget);

    // 新增：收藏商品统计项
    QPushButton* favoriteCountWidget = new QPushButton("收藏商品");
    favoriteCountWidget->setStyleSheet(R"(
        QPushButton {
            background: white;
            border-radius: 8px;
            padding: 24px 10px;
            border: none;
            font: bold 22px 'Microsoft YaHei';
            color: #333333;
        }
        QPushButton:hover {
            background: #F0F0F0;
        }
    )");
    favoriteCountWidget->setMinimumHeight(80);
    connect(favoriteCountWidget, &QPushButton::clicked, this, &MainWindow::showFavoritesDialog);
    statsLayout->addWidget(favoriteCountWidget);

    // 计算未读消息数量的函数
    auto calculateUnreadMessages = [this]() -> int {
        int totalUnreadMessages = 0;
        for (auto room : activeChatRooms) {
            totalUnreadMessages += room->getUnreadCount(const_cast<User *>(cur_user));
        }
        return totalUnreadMessages;
    };
    
    // 修改：将收到私信改为可点击的按钮
    QPushButton* msgCountWidget = new QPushButton(QString("收到私信\n%1").arg(calculateUnreadMessages()));
    
    // 根据未读消息数量设置不同的样式
    int totalUnreadMessages = calculateUnreadMessages();
    if (totalUnreadMessages > 0) {
        msgCountWidget->setStyleSheet(R"(
            QPushButton {
                background: white;
                border-radius: 12px;
                padding: 24px 10px;
                border: 3px solid #FF4444;
                font: bold 22px 'Microsoft YaHei';
                color: #E74C3C;
            }
            QPushButton:hover {
                background: #FFF8F8;
                border-color: #FF6666;
                color: #C0392B;
            }
            QPushButton:pressed {
                background: #FFE8E8;
                border-color: #CC0000;
            }
        )");
    } else {
        msgCountWidget->setStyleSheet(R"(
            QPushButton {
                background: white;
                border-radius: 12px;
                padding: 24px 10px;
                border: 2px solid #E8E8E8;
                font: bold 22px 'Microsoft YaHei';
                color: #333333;
            }
            QPushButton:hover {
                background: #F8F9FA;
                border-color: #D0D0D0;
            }
            QPushButton:pressed {
                background: #F0F0F0;
            }
        )");
    }
    
    msgCountWidget->setMinimumHeight(80);
    connect(msgCountWidget, &QPushButton::clicked, [this]() {
        switchPage(2); // 跳转到私信页面（第3页，索引为2）
        statusBar()->showMessage("已切换到私信页面", 2000);
    });
    statsLayout->addWidget(msgCountWidget);

    // 修改：将账户等级改为用户评级
    QWidget* ratingWidget = createStatItem("用户评级", cur_user->getRatingLevel());
    statsLayout->addWidget(ratingWidget);

    // statsLayout->addWidget(createStatItem("账户等级", "普通用户"));

    // 添加卡片到主布局
    mainLayout->addWidget(cardWidget);
    mainLayout->addWidget(statsCard);

    // 添加退出按钮
    QPushButton *exitButton = new QPushButton("安全退出");
    exitButton->setFixedSize(120, 40);
    exitButton->setStyleSheet(R"(
        QPushButton {
            background: #FF4444;
            color: white;
            border-radius: 20px;
            font: bold 14px 'Microsoft YaHei';
            padding: 8px 16px;
        }
        QPushButton:hover {
            background: #FF6666;
        }
        QPushButton:pressed {
            background: #CC0000;
        }
    )");

    connect(exitButton, &QPushButton::clicked, this, [](){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "确认退出",
                                      "确定要退出程序吗？",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qApp->quit();
        }
    });

    QWidget *exitContainer = new QWidget;
    QHBoxLayout *exitLayout = new QHBoxLayout(exitContainer);
    exitLayout->setContentsMargins(0, 20, 0, 0);
    exitLayout->addStretch();
    exitLayout->addWidget(exitButton);
    exitLayout->addStretch();

    mainLayout->addWidget(exitContainer);
    mainLayout->addStretch();

    return userInfoPage;
}
void MainWindow::handleAvatarUpload(QLabel *avatarLabel) {
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "选择头像",
                                                    QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                                                    "图片文件 (*.jpg *.jpeg *.png *.bmp)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (pixmap.isNull()) {
            QMessageBox::warning(this, "错误", "无法加载选择的图片");
            return;
        }

        // 显示裁剪对话框
        QPixmap cropped = cropImage(pixmap);
        if (!cropped.isNull()) {
            QString projectPath = QCoreApplication::applicationDirPath();
            // 创建头像目录
            QDir avatarDir(projectPath + "/user_avatars");
            if(!avatarDir.exists()) {
                if(!avatarDir.mkpath(".")) {
                    QMessageBox::critical(this, "错误", "无法创建头像目录");
                    return;
                }
            }
            // 生成唯一文件名
            QString fileName = QString("%1_%2.png")
                                   .arg(cur_user->getUserID())
                                   .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));

            QString savePath = avatarDir.absoluteFilePath(fileName);
            // 保存文件
            if(cropped.save(savePath, "PNG")) {
                qDebug() << "头像保存至:" << QDir::toNativeSeparators(savePath);
                User* a=(User*)cur_user;
                a->setAvatarPath(savePath);
                a=NULL;
                delete a;
                createPages();
            } else {
                QMessageBox::warning(this, "错误", "头像保存失败");
            }
        }
    }
}
QPixmap MainWindow::cropImage(const QPixmap &original) {
    QDialog dialog(this);
    dialog.setWindowTitle("裁剪头像");
    QVBoxLayout layout(&dialog);

    // 创建图像显示区域
    QLabel *imageLabel = new QLabel(&dialog);
    QPixmap scaled = original.scaled(400, 400, Qt::KeepAspectRatioByExpanding);
    imageLabel->setPixmap(scaled);
    imageLabel->setMouseTracking(true);  // 启用鼠标跟踪

    // 创建裁剪框
    QRubberBand *rubberBand = new QRubberBand(QRubberBand::Rectangle, imageLabel);
    QPoint origin;

    // 安装事件过滤器到imageLabel
    imageLabel->installEventFilter(this);

    // 记录对话框相关指针
    m_cropData = { &dialog, imageLabel, rubberBand, QPoint() };
    layout.addWidget(imageLabel);

    if (dialog.exec() == QDialog::Accepted) {
        // 计算原始坐标比例
        QRect selected = m_cropData.rubberBand->geometry();
        qreal scaleX = original.width() / (qreal)scaled.width();
        qreal scaleY = original.height() / (qreal)scaled.height();

        QRect sourceRect(
            selected.x() * scaleX,
            selected.y() * scaleY,
            selected.width() * scaleX,
            selected.height() * scaleY
            );
        return original.copy(sourceRect);
    }
    return QPixmap();
}
void MainWindow::updateAvatarDisplay(QLabel *avatarLabel) {
    currentAvatarPath=cur_user->getAvatarPath();
    if (!currentAvatarPath.isEmpty()) {
        QPixmap avatar(currentAvatarPath);
        if (!avatar.isNull()) {
            // 创建圆形遮罩
            QBitmap mask(avatar.size());
            mask.fill(Qt::color0);
            QPainter painter(&mask);
            painter.setBrush(Qt::color1);
            painter.drawEllipse(0, 0, avatar.width(), avatar.height());

            avatar.setMask(mask);
            avatarLabel->setPixmap(avatar.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            return;
        }
    }

    // 显示默认头像
    avatarLabel->setPixmap(QPixmap(":/default_avatar.png").scaled(100, 100));

}
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_cropData.imageLabel) {
        switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            m_cropData.origin = me->pos();
            m_cropData.rubberBand->setGeometry(QRect(m_cropData.origin, QSize()));
            m_cropData.rubberBand->show();
            return true;
        }
        case QEvent::MouseMove: {
            if (m_cropData.rubberBand->isVisible()) {
                QMouseEvent *me = static_cast<QMouseEvent*>(event);
                QPoint currentPos = me->pos();

                // 限制在图像边界内
                currentPos.setX(qBound(0, currentPos.x(), m_cropData.imageLabel->width()));
                currentPos.setY(qBound(0, currentPos.y(), m_cropData.imageLabel->height()));

                m_cropData.rubberBand->setGeometry(
                    QRect(m_cropData.origin, currentPos).normalized()
                    );
                return true;
            }
            break;
        }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(
                    this,
                    "裁剪确认",
                    "确定要这样裁剪吗？",
                    QMessageBox::Yes | QMessageBox::No
                    );
                if (reply == QMessageBox::Yes) {
                    // 关闭裁剪对话框
                    if (m_cropData.dialog) {
                        m_cropData.dialog->accept();  // 触发Accepted状态
                    }
                } else {
                    // 保持对话框打开，继续调整
                    m_cropData.rubberBand->hide();
                    m_cropData.origin = QPoint();
                }
                return true;
            }
            break;
        }
        default:
            break;
        }
    }
    // 处理聊天卡片点击事件
    else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        if (me->button() == Qt::LeftButton) {
            QWidget* widget = qobject_cast<QWidget*>(obj);
            if (widget) {
                // 检查点击的目标是否是ClickedLabel（用户名），如果是则不处理聊天室打开
                ClickedLabel* clickedLabel = qobject_cast<ClickedLabel*>(obj);
                if (clickedLabel) {
                    // 点击的是用户名标签，让ClickedLabel自己处理点击事件
                    return false;
                }
                
                // 检查是否是聊天卡片
                QVariant chatRoomVariant = widget->property("chatRoom");
                if (chatRoomVariant.isValid()) {
                    ChatRoom* room = chatRoomVariant.value<ChatRoom*>();
                    if (room) {
                        room->markAsRead(const_cast<User *>(cur_user));
                        room->show();
                        room->raise();
                        room->activateWindow();
                        return true;
                    }
                }
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
QWidget* MainWindow::itempost_create() {
    // 主容器
    QWidget* mainWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);

    /* 添加按钮工具栏 */
    QWidget* toolBar = new QWidget(mainWidget);
    QHBoxLayout* btnLayout = new QHBoxLayout(toolBar);

    // 发布帖子按钮
    QPushButton* postBtn = new QPushButton("＋ 发布新帖子", toolBar);
    postBtn->setStyleSheet(R"(
        QPushButton {
            background: #007BFF;
            color: white;
            border-radius: 15px;
            padding: 10px 20px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #0056B3;
        }
    )");
    connect(postBtn, &QPushButton::clicked, this, &MainWindow::showPostEditor);

    // 刷新按钮
    QPushButton* refreshBtn = new QPushButton("⟳ 刷新", toolBar);
    refreshBtn->setStyleSheet(R"(
        QPushButton {
            background: #28A745;
            color: white;
            border-radius: 15px;
            padding: 10px 20px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #218838;
        }
    )");
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshItemPosts);
    QLineEdit* searchInput = new QLineEdit;
    QPushButton* searchBtn = new QPushButton("🔍 搜索");

    // 样式设置
    searchInput->setStyleSheet(R"(
        QLineEdit {
            border: 2px solid #6C757D;
            border-radius: 15px;
            padding: 8px 16px;
            font-family: 'Microsoft YaHei';
            min-width: 300px;
        }
        QLineEdit:focus {
            border-color: #007BFF;
        }
    )");

    searchBtn->setStyleSheet(R"(
        QPushButton {
            background: #6C757D;
            color: white;
            border-radius: 15px;
            padding: 10px 20px;
            font: bold 14px 'Microsoft YaHei';
            margin-left: 8px;
        }
        QPushButton:hover {
            background: #5A6268;
        }
    )");

    // 实时搜索防抖处理（500ms）
    QTimer* searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);

    // 连接信号
    connect(searchInput, &QLineEdit::textChanged, [=](){
        searchTimer->start(500);
    });

    // 将搜索组件添加到工具栏布局
    btnLayout->addWidget(searchInput);
    btnLayout->addWidget(searchBtn);
    // 按钮布局
    btnLayout->addWidget(postBtn);
    btnLayout->addWidget(refreshBtn);
    btnLayout->addStretch();
    toolBar->setLayout(btnLayout);
    mainLayout->addWidget(toolBar);

    /* 添加滚动区域 */
    QScrollArea* scrollArea = new QScrollArea(mainWidget);
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);

    // 生成帖子内容
    for (const auto& post : posts) {
        QWidget* postCard = createSinglePost(post);
        contentLayout->addWidget(postCard);
        contentLayout->addSpacing(15);
    }
    contentLayout->addStretch();

    // 设置滚动区域
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background: #F5F5F5; border: none;");
    contentWidget->setStyleSheet("background: transparent;");

    mainLayout->addWidget(scrollArea);
    auto performSearch = [=](const QString& keyword) {
        // 清空原有内容
        QLayoutItem* item;
        while ((item = contentLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // 获取搜索结果
        auto results = findPost(keyword.trimmed());

        // 显示结果
        if(results.empty()) {
            QLabel* tipLabel = new QLabel("未找到相关商品");
            tipLabel->setStyleSheet("font: 16px 'Microsoft YaHei'; color: #666;");
            tipLabel->setAlignment(Qt::AlignCenter);
            contentLayout->addWidget(tipLabel);
        } else {
            for (auto post : results) {
                QWidget* card = createSinglePost(*post);
                highlightKeywords(card, keyword);
                contentLayout->addWidget(card);
                contentLayout->addSpacing(15);
            }
        }
        contentLayout->addStretch();
    };
    connect(searchBtn, &QPushButton::clicked, [=](){ performSearch(searchInput->text()); });
    connect(searchTimer, &QTimer::timeout, [=](){ performSearch(searchInput->text()); });
    return mainWidget;
}
void MainWindow::highlightKeywords(QWidget* card, const QString& keyword) {
    if(keyword.isEmpty()) return;

    // 高亮标题
    QLabel* titleLabel = card->findChild<QLabel*>("titleLabel"); // 假设标题标签有objectName
    if(titleLabel) {
        QString text = titleLabel->text();
        text.replace(keyword, QString("<b style='background:#FFF3CD;'>%1</b>").arg(keyword),
                     Qt::CaseInsensitive);
        titleLabel->setText(text);
    }

    // 高亮内容
    QLabel* contentLabel = card->findChild<QLabel*>("contentLabel");
    if(contentLabel) {
        QString text = contentLabel->text();
        text.replace(keyword, QString("<b style='background:#E7F5FF;'>%1</b>").arg(keyword),
                     Qt::CaseInsensitive);
        contentLabel->setText(text);
    }
}
QWidget* MainWindow::createSinglePost(const ItemPost& post) {
    QWidget* card = new QWidget;
    card->setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 12px;
            padding: 16px;
            margin: 8px 0;
        }
        QWidget:hover {
            background: #FCFCFC;
            box-shadow: 0 2px 12px rgba(0,0,0,0.1);
        }
    )");

    QHBoxLayout* mainLayout = new QHBoxLayout(card);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(20);

    /* 图片区域 - 自适应宽高比 */
    QLabel* imageLabel = new QLabel;
    QString mainImage = post.getImages().empty() ?
                            ":/default_item.jpg" : post.getMainImage();
    QPixmap pixmap(mainImage);
    pixmap = pixmap.scaled(160, 160, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    imageLabel->setPixmap(pixmap);
    imageLabel->setStyleSheet(R"(
        QLabel {
            border-radius: 8px;
            background: #F0F0F0;
            min-width: 160px;
            max-width: 160px;
            min-height: 160px;
            max-height: 160px;
        }
        QLabel:hover {
            background: #E8E8E8;
        }
    )");
    imageLabel->setScaledContents(true);
    mainLayout->addWidget(imageLabel);

    /* 信息区域 */
    QWidget* infoWidget = new QWidget;
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(8);

    // 标题和价格行
    QWidget* titleRow = new QWidget;
    QHBoxLayout* titleLayout = new QHBoxLayout(titleRow);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    ClickedLabel* titleLabel = new ClickedLabel(post.getTitle());
    titleLabel->setStyleSheet(R"(
        font: 14px 'Microsoft YaHei';
        color: #007BFF;
        padding-right: 8px;
        text-decoration: underline;
        qproperty-alignment: AlignVCenter;
    )");
    titleLabel->setWordWrap(true);

    QLabel* priceLabel = new QLabel(QString("¥%1").arg(post.getPrice(), 0, 'f', 2));
    priceLabel->setStyleSheet(R"(
        font: bold 20px 'Microsoft YaHei';
        color: #FF4444;
        min-width: 80px;
        qproperty-alignment: AlignRight;
    )");

    titleLayout->addWidget(titleLabel, 4);
    titleLayout->addWidget(priceLabel, 1);
    infoLayout->addWidget(titleRow);

    // 描述文本（限制3行）
    QLabel* descLabel = new QLabel(post.getDescription());
    descLabel->setStyleSheet(R"(
        font: 14px 'Microsoft YaHei';
        color: #666666;
        line-height: 1.4;
        margin-top: 4px;
    )");
    descLabel->setWordWrap(true);
    descLabel->setMaximumHeight(60); // 3行高度
    infoLayout->addWidget(descLabel);

    // 标签流式布局
    QWidget* tagWidget = new QWidget;
    QHBoxLayout* tagLayout = new QHBoxLayout(tagWidget);
    tagLayout->setContentsMargins(0, 0, 0, 0);
    tagLayout->setSpacing(6);

    for (const auto& tag : post.getFlags()) {
        QLabel* tagLabel = new QLabel(tag);
        tagLabel->setStyleSheet(R"(
            background: #F0F0F0;
            border-radius: 14px;
            padding: 4px 12px;
            color: #666666;
            font: 12px 'Microsoft YaHei';
        )");
        tagLayout->addWidget(tagLabel);
    }
    
    // 恢复：作者标签
    ClickedLabel* authorLabel = new ClickedLabel("@" + post.getOwner()->getUsername());
    authorLabel->setStyleSheet(R"(
        font: 14px 'Microsoft YaHei';
        color: #007BFF;
        padding-right: 8px;
        text-decoration: underline;
        qproperty-alignment: AlignVCenter;
    )");
    authorLabel->setCursor(Qt::PointingHandCursor);
    authorLabel->setToolTip("查看作者信息");
    
    // 新增：收藏按钮
    QPushButton* favoriteBtn = new QPushButton;
    favoriteBtn->setFixedSize(32, 32);
    favoriteBtn->setCursor(Qt::PointingHandCursor);
    
    // 根据收藏状态设置按钮样式
    if (cur_user && post.isFavoritedBy(cur_user)) {
        favoriteBtn->setText("❤️");
        favoriteBtn->setToolTip("取消收藏");
        favoriteBtn->setStyleSheet(R"(
            QPushButton {
                background: #FFE6E6;
                border: 2px solid #FF4444;
                border-radius: 16px;
                font-size: 16px;
                color: #FF4444;
            }
            QPushButton:hover {
                background: #FFCCCC;
            }
        )");
    } else {
        favoriteBtn->setText("♡");
        favoriteBtn->setToolTip("收藏商品");
        favoriteBtn->setStyleSheet(R"(
            QPushButton {
                background: #F8F9FA;
                border: 2px solid #DEE2E6;
                border-radius: 16px;
                font-size: 16px;
                color: #6C757D;
            }
            QPushButton:hover {
                background: #E9ECEF;
                border-color: #ADB5BD;
            }
        )");
    }
    
    connect(favoriteBtn, &QPushButton::clicked, [this, post]() {
        toggleFavorite(post);
    });
    
    // 连接作者标签点击信号
    connect(authorLabel, &ClickedLabel::clicked, this, [this, post]{
        showAuthorInfo(*post.getOwner());
    });
    
    tagLayout->addWidget(authorLabel);
    tagLayout->addWidget(favoriteBtn);
    tagLayout->addStretch();

    infoLayout->addWidget(tagWidget);
    infoLayout->addStretch();

    mainLayout->addWidget(infoWidget, 1);

    // 交互效果
    connect(titleLabel, &ClickedLabel::clicked, this, [this, post]{
        showDetailPage(post);
    });

    return card;
}
void MainWindow::showAuthorInfo(const User& author) {
    QDialog dialog(this);
    dialog.setWindowTitle("用户信息 - " + author.getUsername());
    dialog.setMinimumSize(400, 400);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(20, 15, 20, 15);

    // 头像区域
    QHBoxLayout* avatarLayout = new QHBoxLayout;
    QLabel* avatarLabel = new QLabel;
    QString avatarPath = author.getAvatarPath().isEmpty() ?
                             ":/default_avatar.png" : author.getAvatarPath();
    QPixmap avatarPix = QPixmap(avatarPath)
                            .scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatarLabel->setPixmap(avatarPix);
    avatarLabel->setStyleSheet(R"(
        border-radius: 32px;
        border: 2px solid #EEE;
    )");
    avatarLayout->addWidget(avatarLabel, 0, Qt::AlignHCenter);
    mainLayout->addLayout(avatarLayout);

    // 信息表格
    QFormLayout* infoLayout = new QFormLayout;
    infoLayout->setLabelAlignment(Qt::AlignRight);

    // 用户名
    QLabel* nameValue = new QLabel(author.getUsername());
    nameValue->setStyleSheet("font-weight: bold; color: #333;");
    infoLayout->addRow("用户名：", nameValue);

    // 用户ID
    QLabel* idValue = new QLabel(author.getUserID());
    idValue->setStyleSheet("color: #666;");
    infoLayout->addRow("用户ID：", idValue);

    // 注册时间
    QLabel* regTimeValue = new QLabel(author.getRegTime().toString("yyyy-MM-dd hh:mm"));
    regTimeValue->setStyleSheet("color: #666;");
    infoLayout->addRow("注册时间：", regTimeValue);

    // 新增：用户评分
    QLabel* ratingValue = new QLabel(author.getRatingLevel());
    ratingValue->setStyleSheet("font: 14px 'Microsoft YaHei'; color: #FF6B35;");
    infoLayout->addRow("用户评级：", ratingValue);

    // 新增：评分统计
    if (author.getRatingCount() > 0) {
        QLabel* ratingStatsValue = new QLabel(
            QString("平均分: %1 (%2人评价)").arg(author.getAverageRating(), 0, 'f', 1).arg(author.getRatingCount())
        );
        ratingStatsValue->setStyleSheet("color: #666;");
        infoLayout->addRow("评分统计：", ratingStatsValue);
    }

    // 认证等级
    QString authLevelStr = author.getAuthLevel() > 0 ? "认证用户" : "普通用户";
    QLabel* authValue = new QLabel(authLevelStr);
    authValue->setStyleSheet(author.getAuthLevel() > 0 ?
                                 "color: #28a745;" : "color: #666;");
    infoLayout->addRow("用户类型：", authValue);

    // 邮箱（可选显示）
    if(!author.getEmail().isEmpty()) {
        QLabel* emailValue = new QLabel(author.getEmail());
        emailValue->setStyleSheet("color: #007BFF;");
        infoLayout->addRow("联系邮箱：", emailValue);
    }

    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();

    // 按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    // 只有当前用户不是作者本人时，才显示相关按钮
    if (cur_user && cur_user->getUserID() != author.getUserID()) {
        // 发送私信按钮
        QPushButton* chatBtn = new QPushButton("发送私信");
        chatBtn->setStyleSheet(R"(
            QPushButton {
                background: #28a745;
                color: white;
                padding: 8px 16px;
                border-radius: 4px;
                font: bold 12px 'Microsoft YaHei';
            }
            QPushButton:hover {
                background: #218838;
            }
        )");

        // 新增：评分按钮
        QPushButton* rateBtn = new QPushButton("评价用户");
        rateBtn->setStyleSheet(R"(
            QPushButton {
                background: #FF6B35;
                color: white;
                padding: 8px 16px;
                border-radius: 4px;
                font: bold 12px 'Microsoft YaHei';
            }
            QPushButton:hover {
                background: #E55A2B;
            }
        )");

        // 新增：查看评价按钮
        QPushButton* viewRatingsBtn = new QPushButton("查看评价");
        viewRatingsBtn->setStyleSheet(R"(
            QPushButton {
                background: #007BFF;
                color: white;
                padding: 8px 16px;
                border-radius: 4px;
                font: bold 12px 'Microsoft YaHei';
            }
            QPushButton:hover {
                background: #0056B3;
            }
        )");

        buttonLayout->addWidget(chatBtn);
        buttonLayout->addWidget(rateBtn);
        buttonLayout->addWidget(viewRatingsBtn);

        // 连接按钮信号
        connect(chatBtn, &QPushButton::clicked, [this, &author, &dialog]{
            dialog.accept();
            createChatRoom(const_cast<User*>(&author));
        });

        connect(rateBtn, &QPushButton::clicked, [this, &author, &dialog]{
            dialog.accept();
            showRatingDialog(author);
        });

        connect(viewRatingsBtn, &QPushButton::clicked, [this, &author, &dialog]{
            dialog.accept();
            showUserRatings(author);
        });

    } else {
        // 如果是查看自己的信息，显示查看评价按钮
        QPushButton* viewRatingsBtn = new QPushButton("查看我的评价");
        viewRatingsBtn->setStyleSheet(R"(
            QPushButton {
                background: #007BFF;
                color: white;
                padding: 8px 16px;
                border-radius: 4px;
                font: bold 12px 'Microsoft YaHei';
            }
            QPushButton:hover {
                background: #0056B3;
            }
        )");

        buttonLayout->addWidget(viewRatingsBtn);

        connect(viewRatingsBtn, &QPushButton::clicked, [this, &author, &dialog]{
            dialog.accept();
            showUserRatings(author);
        });
    }

    // 关闭按钮
    QPushButton* closeBtn = new QPushButton("关闭");
    closeBtn->setStyleSheet(R"(
        QPushButton {
            background: #6C757D;
            color: white;
            padding: 8px 16px;
            border-radius: 4px;
            font: bold 12px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #5A6268;
        }
    )");

    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    mainLayout->addLayout(buttonLayout);

    dialog.exec();
}
void MainWindow::createChatRoom(User* targetUser){
    // 参数有效性检查
    if (!targetUser || !cur_user) { // currentUser需要是MainWindow的成员变量
        qWarning() << "Invalid user pointer";
        return;
    }

    // 检查是否已有聊天窗口
    for(ChatRoom* room : activeChatRooms) {
        if((room->user1->getUserID() == cur_user->getUserID() && room->user2->getUserID() == targetUser->getUserID()) ||
            (room->user2->getUserID() == cur_user->getUserID() && room->user1->getUserID() == targetUser->getUserID()))
        {
            room->show();
            return;
        }
    }

    // 创建新聊天室
    ChatRoom* newRoom = new ChatRoom(const_cast<User *>(cur_user), targetUser,const_cast<User *>(cur_user));

    // 初始化设置
    newRoom->loadHistory();
    // 连接未读消息数量变化信号
    connect(newRoom, &ChatRoom::unreadCountChanged, this, &MainWindow::updateUnreadMessageDisplay);
    activeChatRooms.emplace_back(newRoom);
    newRoom->show();
}

void MainWindow::showDetailPage(const ItemPost& post){
    QDialog *detailDialog = new QDialog(this);
    detailDialog->setWindowTitle("商品详情");
    detailDialog->setMinimumSize(800, 600);

    // 创建详情内容
    QScrollArea *scrollArea = new QScrollArea(detailDialog);
    QWidget *detailContent = createDetailDialogContent(post, detailDialog);

    // 设置滚动区域
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(detailContent);

    QVBoxLayout *dialogLayout = new QVBoxLayout(detailDialog);
    dialogLayout->addWidget(scrollArea);

    // 设置为模态对话框
    detailDialog->setWindowModality(Qt::WindowModal);
    detailDialog->exec(); // 使用exec()实现模态对话框
}

QWidget* MainWindow::createDetailDialogContent(const ItemPost& post, QDialog* parent) {
    QWidget *content = new QWidget(parent);
    QVBoxLayout *mainLayout = new QVBoxLayout(content);
    mainLayout->setContentsMargins(20, 15, 20, 15);
    mainLayout->setSpacing(15);

    // 返回按钮布局优化
    QHBoxLayout* headerLayout = new QHBoxLayout;
    QPushButton *backBtn = new QPushButton("← 返回", content);
    backBtn->setStyleSheet("font: bold 14px; padding:8px; margin-bottom:15px;");
    headerLayout->addWidget(backBtn);
    
    // 新增：如果是商品发布者，显示编辑和删除按钮
    if (cur_user && post.getOwner()->getUserID() == cur_user->getUserID()) {
        QPushButton *editBtn = new QPushButton("✏️ 编辑", content);
        QPushButton *deleteBtn = new QPushButton("🗑️ 删除", content);
        
        editBtn->setStyleSheet(R"(
            QPushButton {
                background: #28A745;
                color: white;
                border-radius: 8px;
                padding: 8px 16px;
                font: bold 12px 'Microsoft YaHei';
                margin-left: 8px;
            }
            QPushButton:hover {
                background: #218838;
            }
        )");
        
        deleteBtn->setStyleSheet(R"(
            QPushButton {
                background: #DC3545;
                color: white;
                border-radius: 8px;
                padding: 8px 16px;
                font: bold 12px 'Microsoft YaHei';
                margin-left: 8px;
            }
            QPushButton:hover {
                background: #C82333;
            }
        )");
        
        connect(editBtn, &QPushButton::clicked, [this, post, parent]() {
            parent->accept(); // 关闭详情对话框
            editItemPost(post);
        });
        
        connect(deleteBtn, &QPushButton::clicked, [this, post, parent]() {
            parent->accept(); // 关闭详情对话框
            deleteItemPost(post);
        });
        
        headerLayout->addWidget(editBtn);
        headerLayout->addWidget(deleteBtn);
    }
    
    // 新增：收藏按钮（所有用户都可以看到）
    if (cur_user) {
        QPushButton *favoriteBtn = new QPushButton;
        favoriteBtn->setFixedSize(40, 40);
        favoriteBtn->setCursor(Qt::PointingHandCursor);
        
        if (post.isFavoritedBy(cur_user)) {
            favoriteBtn->setText("❤️");
            favoriteBtn->setToolTip("取消收藏");
            favoriteBtn->setStyleSheet(R"(
                QPushButton {
                    background: #FFE6E6;
                    border: 2px solid #FF4444;
                    border-radius: 20px;
                    font-size: 18px;
                    color: #FF4444;
                    margin-left: 8px;
                }
                QPushButton:hover {
                    background: #FFCCCC;
                }
            )");
        } else {
            favoriteBtn->setText("♡");
            favoriteBtn->setToolTip("收藏商品");
            favoriteBtn->setStyleSheet(R"(
                QPushButton {
                    background: #F8F9FA;
                    border: 2px solid #DEE2E6;
                    border-radius: 20px;
                    font-size: 18px;
                    color: #6C757D;
                    margin-left: 8px;
                }
                QPushButton:hover {
                    background: #E9ECEF;
                    border-color: #ADB5BD;
                }
            )");
        }
        
        connect(favoriteBtn, &QPushButton::clicked, [this, post, parent]() {
            toggleFavorite(post);
            parent->accept(); // 关闭详情对话框
            refreshItemPosts(); // 刷新界面
        });
        
        headerLayout->addWidget(favoriteBtn);
    }
    
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // 图片展示区域（缩小尺寸）
    QLabel *imageLabel = new QLabel(content);
    QPixmap pixmap(post.getMainImage().isEmpty() ?
                       ":/default_item.jpg" : post.getMainImage());
    pixmap = pixmap.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 缩小图片尺寸
    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("margin-bottom: 20px;"); // 增加下边距

    // 信息区域重新布局
    QWidget *infoWidget = new QWidget(content);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(15, 0, 15, 0);
    infoLayout->setSpacing(12);

    // 标题与价格并排
    QWidget* titleRow = new QWidget(infoWidget);
    QHBoxLayout* titleLayout = new QHBoxLayout(titleRow);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *titleLabel = new QLabel(post.getTitle(), titleRow);
    titleLabel->setStyleSheet(R"(
        font: bold 24px 'Microsoft YaHei';
        color: #333333;
        margin-right: 20px;
    )");
    titleLabel->setWordWrap(true);

    QLabel *priceLabel = new QLabel(QString("¥%1").arg(post.getPrice(), 0, 'f', 2), titleRow);
    priceLabel->setStyleSheet(R"(
        font: 28px 'Microsoft YaHei';
        color: #FF4444;
        min-width: 120px;
        qproperty-alignment: AlignRight;
    )");

    titleLayout->addWidget(titleLabel, 3);
    titleLayout->addWidget(priceLabel, 1);

    // 描述区域优化
    QLabel *descLabel = new QLabel(post.getDescription(), infoWidget);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(R"(
        font: 16px 'Microsoft YaHei';
        color: #666666;
        line-height: 1.6;
        margin-top: 10px;
        padding-top: 15px;
        border-top: 1px solid #EEE;
    )");

    infoLayout->addWidget(titleRow);
    infoLayout->addWidget(descLabel);
    
    // 新增：作者信息
    QWidget* authorRow = new QWidget(infoWidget);
    QHBoxLayout* authorLayout = new QHBoxLayout(authorRow);
    authorLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel* authorTitleLabel = new QLabel("发布者:", authorRow);
    authorTitleLabel->setStyleSheet(R"(
        font: 14px 'Microsoft YaHei';
        color: #666666;
    )");
    
    ClickedLabel* authorNameLabel = new ClickedLabel(post.getOwner()->getUsername(), authorRow);
    authorNameLabel->setStyleSheet(R"(
        font: 14px 'Microsoft YaHei';
        color: #007BFF;
        text-decoration: underline;
        cursor: pointer;
    )");
    authorNameLabel->setCursor(Qt::PointingHandCursor);
    authorNameLabel->setToolTip("查看作者信息");
    
    connect(authorNameLabel, &ClickedLabel::clicked, [this, post]() {
        showAuthorInfo(*post.getOwner());
    });
    
    authorLayout->addWidget(authorTitleLabel);
    authorLayout->addWidget(authorNameLabel);
    authorLayout->addStretch();
    
    infoLayout->addWidget(authorRow);
    infoLayout->addStretch();

    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(infoWidget);
    mainLayout->addStretch();

    connect(backBtn, &QPushButton::clicked, parent, &QDialog::accept);
    return content;
}
void MainWindow::showPostEditor() {
    QDialog editor(this);
    editor.setWindowTitle("发布新帖子");
    editor.resize(600, 500);

    // 图片相关变量
    QStringList imagePaths;
    QString coverImagePath;

    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(&editor);

    /* 图片上传区域 */
    QHBoxLayout* imageLayout = new QHBoxLayout;

    // 图片预览标签
    QLabel* imagePreview = new QLabel;
    imagePreview->setFixedSize(100, 100);
    imagePreview->setStyleSheet("border: 2px dashed #ccc; border-radius: 8px;");
    imagePreview->setAlignment(Qt::AlignCenter);

    // 上传按钮
    QPushButton* uploadBtn = new QPushButton("上传图片");
    uploadBtn->setFixedSize(100, 30);
    uploadBtn->setStyleSheet(R"(
        QPushButton {
            background: #007BFF;
            color: white;
            border-radius: 4px;
        }
        QPushButton:hover {
            background: #0056B3;
        }
    )");

    // 布局排列
    imageLayout->addWidget(imagePreview);
    imageLayout->addWidget(uploadBtn);
    imageLayout->addStretch();

    // 图片上传功能
    connect(uploadBtn, &QPushButton::clicked, [&]{
        QString path = QFileDialog::getOpenFileName(
            &editor,
            "选择商品图片",
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            "图片文件 (*.jpg *.jpeg *.png)"
            );

        if(!path.isEmpty()) {
            // 显示缩略图
            QPixmap pixmap(path);
            pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imagePreview->setPixmap(pixmap);
            imagePaths.append(path);
            coverImagePath = path;
        }
    });

    mainLayout->addLayout(imageLayout);
    // 表单控件
    QLineEdit* titleEdit = new QLineEdit;
    QDoubleSpinBox* priceEdit = new QDoubleSpinBox;
    QLineEdit* tagsEdit = new QLineEdit;
    QTextEdit* contentEdit = new QTextEdit;
    QPushButton* submitBtn = new QPushButton("提交");

    // 配置价格输入
    priceEdit->setRange(0.0, 999999.99);
    priceEdit->setPrefix("¥ ");
    priceEdit->setDecimals(2);
    priceEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

    // 配置标签提示
    tagsEdit->setPlaceholderText("用空格分隔标签，如：电子 数码 手机");

    // 表单布局
    QFormLayout* formLayout = new QFormLayout(&editor);
    formLayout->setLabelAlignment(Qt::AlignRight);

    formLayout->addRow("商品标题：", titleEdit);
    formLayout->addRow("商品价格：", priceEdit);
    formLayout->addRow("商品标签：", tagsEdit);
    formLayout->addRow("详细描述：", contentEdit);
    formLayout->addRow(submitBtn);
    mainLayout->addLayout(formLayout);
    // 输入验证
    auto validateInput = [&]() -> bool {
        if(titleEdit->text().isEmpty()) {
            QMessageBox::warning(&editor, "提示", "请输入商品标题");
            return false;
        }
        if(priceEdit->value() <= 0) {
            QMessageBox::warning(&editor, "提示", "价格必须大于0");
            return false;
        }
        return true;
    };
    connect(submitBtn, &QPushButton::clicked, [&]{
        if(!validateInput()) return;
        // 处理标签输入（新增逻辑）
        QString tagsInput = tagsEdit->text();
        std::vector<QString> parsedTags;
        if(!tagsInput.isEmpty()) {
            // 支持逗号和中文顿号分割
            qWarning()<<tagsInput;
            QStringList rawTags = tagsInput.split(" ");

            foreach(const QString& rawTag, rawTags) {
                QString processedTag = rawTag.trimmed()
                .replace(" ", "")       // 移除内部空格
                    .replace("#", "")       // 过滤特殊字符
                    .left(15);              // 长度限制

                // 有效性检查 + 去重
                if(!processedTag.isEmpty()) {
                    parsedTags.push_back(processedTag);
                }
            }
        }

        // 空值处理优化
        if(parsedTags.empty()) {
            parsedTags.push_back("未分类");
        }

        // 限制最大标签数
        if(parsedTags.size() > 5) {
            parsedTags.resize(5);
        }
        ItemPost newPost(titleEdit->text(),contentEdit->toPlainText(),priceEdit->value(),cur_user,parsedTags);
        for(const auto& path : imagePaths) {
            newPost.addImage(path);
        }
        // 添加默认封面图
        posts.push_back(newPost);
        editor.accept();
        refreshItemPosts();
    });

    // 样式设置
    editor.setStyleSheet(R"(
        QDialog {
            background: #F5F5F5;
        }
        QLineEdit, QTextEdit, QDoubleSpinBox {
            padding: 8px;
            border: 1px solid #DDD;
            border-radius: 4px;
            font: 14px 'Microsoft YaHei';
        }
        QTextEdit {
            min-height: 100px;
        }
    )");
    editor.exec();
}

void MainWindow::refreshItemPosts() {
    // 重新生成界面
    save_item();
    createPages();
    switchPage(1);
}
QWidget* MainWindow::createChatRoomCard(ChatRoom* room) {
    // 卡片容器
    QWidget* card = new QWidget(this);
    card->setMinimumHeight(80);

    // 主布局保持水平布局
    QHBoxLayout* mainLayout = new QHBoxLayout(card);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(16);

    /* 头像区域 */
    QLabel* avatarLabel = new QLabel;
    User* otherUser = room->getOtherUser(cur_user); // 获取对方用户
    QPixmap avatar(otherUser->getAvatarPath().isEmpty() ?
                       ":/default_item.jpg" : otherUser->getAvatarPath());
    avatar = avatar.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setStyleSheet(R"(
        QLabel {
            border-radius: 26px;
            border: 2px solid #E8E8E8;
            background: #F8F9FA;
        }
    )");
    mainLayout->addWidget(avatarLabel);

    /* 右侧信息区域 */
    QWidget* rightArea = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(6);

    // 顶部行（用户名 + 时间）
    QWidget* topRow = new QWidget;
    QHBoxLayout* topLayout = new QHBoxLayout(topRow);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(8);

    // 用户名标签 - 改为可点击的ClickedLabel
    ClickedLabel* nameLabel = new ClickedLabel(otherUser->getUsername());
    nameLabel->setStyleSheet(R"(
        font: bold 15px 'Microsoft YaHei';
        color: #007BFF;
        text-decoration: underline;
        cursor: pointer;
    )");
    nameLabel->setCursor(Qt::PointingHandCursor);
    nameLabel->setToolTip("点击查看用户信息");
    
    // 连接用户名点击事件
    connect(nameLabel, &ClickedLabel::clicked, this, [this, otherUser]() {
        showAuthorInfo(*otherUser);
    });
    
    topLayout->addWidget(nameLabel);

    // 时间标签（右对齐）
    QWidget* timeSpacer = new QWidget;
    timeSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    topLayout->addWidget(timeSpacer);

    QLabel* timeLabel = new QLabel(room->getLastMessageTime().toString("hh:mm"));
    timeLabel->setStyleSheet(R"(
        font: 12px 'Microsoft YaHei';
        color: #95A5A6;
        padding-right: 4px;
    )");
    topLayout->addWidget(timeLabel);

    rightLayout->addWidget(topRow);

    // 消息内容
    QLabel* msgLabel = new QLabel(room->getLastMessageContent());
    msgLabel->setStyleSheet(R"(
        font: 13px 'Microsoft YaHei';
        color: #7F8C8D;
        qproperty-elideMode: ElideRight;
        padding-top: 2px;
    )");
    msgLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    rightLayout->addWidget(msgLabel);

    mainLayout->addWidget(rightArea, 1);

    /* 未读消息标记和样式 */
    int unread = room->getUnreadCount(const_cast<User *>(cur_user));
    if(unread > 0) {
        // 未读消息的简洁视觉反馈
        QLabel* badge = new QLabel(QString::number(unread));
        badge->setStyleSheet(R"(
            background: #FF4444;
            color: white;
            min-width: 22px;
            max-width: 22px;
            min-height: 22px;
            max-height: 22px;
            border-radius: 11px;
            font: bold 11px 'Microsoft YaHei';
            qproperty-alignment: AlignCenter;
            border: 2px solid white;
        )");

        // 使用覆盖布局定位未读标记
        QHBoxLayout* badgeLayout = new QHBoxLayout(card);
        badgeLayout->setContentsMargins(0, 6, 6, 0);
        badgeLayout->addWidget(badge, 0, Qt::AlignTop | Qt::AlignRight);
        
        // 未读消息的简洁卡片样式
        card->setStyleSheet(R"(
            QWidget {
                background: white;
                border-radius: 12px;
                padding: 12px;
                border: 2px solid #FF4444;
            }
            QWidget:hover {
                background: #FFF8F8;
                border-color: #FF6666;
            }
        )");
    } else {
        // 已读消息的简洁样式
        card->setStyleSheet(R"(
            QWidget {
                background: white;
                border-radius: 12px;
                padding: 12px;
                border: 1px solid #F0F0F0;
            }
            QWidget:hover {
                background: #FAFAFA;
                border-color: #E0E0E0;
            }
        )");
    }

    // 设置鼠标手势
    card->setCursor(Qt::PointingHandCursor);

    // 整个卡片都可以点击 - 使用事件过滤器
    card->installEventFilter(this);
    
    // 存储聊天室指针到卡片对象中，以便事件过滤器使用
    card->setProperty("chatRoom", QVariant::fromValue(room));

    return card;
}

// 创建聊天室列表容器
QWidget* MainWindow::createChatListWidget() {
    QWidget* container = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(container);

    // ===== 新增：顶部工具栏 =====
    QWidget* toolBar = new QWidget(container);
    QHBoxLayout* toolLayout = new QHBoxLayout(toolBar);
    toolLayout->setContentsMargins(0, 0, 0, 0);
    toolLayout->setSpacing(8);

    // 新增：未读消息统计 - 实时计算
    auto calculateTotalUnread = [this]() -> int {
        int totalUnread = 0;
        for (auto room : activeChatRooms) {
            totalUnread += room->getUnreadCount(const_cast<User *>(cur_user));
        }
        return totalUnread;
    };
    
    QLabel* unreadLabel = new QLabel(QString("未读消息: %1").arg(calculateTotalUnread()));
    unreadLabel->setStyleSheet(R"(
        font: bold 16px 'Microsoft YaHei';
        color: #E74C3C;
        padding: 10px 18px;
        background: white;
        border-radius: 10px;
        border: 2px solid #FF4444;
    )");
    toolLayout->addWidget(unreadLabel);

    QPushButton* refreshBtn = new QPushButton("⟳ 刷新", toolBar);
    refreshBtn->setStyleSheet(R"(
        QPushButton {
            background: #28A745;
            color: white;
            border-radius: 15px;
            padding: 8px 20px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #218838;
        }
    )");
    connect(refreshBtn, &QPushButton::clicked, this, [this](){createPages();switchPage(2);});
    toolLayout->addWidget(refreshBtn);
    toolLayout->addStretch();
    toolBar->setLayout(toolLayout);
    mainLayout->addWidget(toolBar);
    // ===== 工具栏结束 =====

    /* 滚动区域 */
    QScrollArea* scrollArea = new QScrollArea;
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);

    // 添加间距
    contentLayout->setContentsMargins(16, 16, 16, 16);
    contentLayout->setSpacing(12);
    // 动态生成聊天卡片
    for(auto room: activeChatRooms) {
        contentLayout->addWidget(createChatRoomCard(room));
    }
    contentLayout->addStretch();

    // 设置滚动区域
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    mainLayout->addWidget(scrollArea);
    return container;
}

void MainWindow::showMyPosts() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("我发布的商品");
    dialog->setMinimumSize(800, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);

    // 标题栏
    QWidget* headerWidget = new QWidget;
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);

    QLabel* titleLabel = new QLabel("我发布的商品");
    titleLabel->setStyleSheet(R"(
        font: bold 20px 'Microsoft YaHei';
        color: #333333;
    )");

    QPushButton* closeBtn = new QPushButton("关闭");
    closeBtn->setStyleSheet(R"(
        QPushButton {
            background: #FF4444;
            color: white;
            border-radius: 15px;
            padding: 8px 16px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #FF6666;
        }
    )");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeBtn);

    mainLayout->addWidget(headerWidget);

    // 滚动区域
    QScrollArea* scrollArea = new QScrollArea;
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(15);

    // 获取并显示用户发布的商品
    bool hasPosts = false;
    for(const auto& post : posts) {
        if(post.getOwner()->getUserID() == cur_user->getUserID()) {
            hasPosts = true;
            
            // 创建商品卡片
            QWidget* postCard = new QWidget;
            postCard->setStyleSheet(R"(
                QWidget {
                    background: white;
                    border-radius: 12px;
                    padding: 16px;
                    margin: 8px 0;
                }
                QWidget:hover {
                    background: #FCFCFC;
                    box-shadow: 0 2px 12px rgba(0,0,0,0.1);
                }
            )");

            QHBoxLayout* cardLayout = new QHBoxLayout(postCard);
            cardLayout->setContentsMargins(0, 0, 0, 0);
            cardLayout->setSpacing(20);

            // 商品图片
            QLabel* imageLabel = new QLabel;
            QString mainImage = post.getImages().empty() ?
                                ":/default_item.jpg" : post.getMainImage();
            QPixmap pixmap(mainImage);
            pixmap = pixmap.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            imageLabel->setPixmap(pixmap);
            imageLabel->setStyleSheet(R"(
                QLabel {
                    border-radius: 8px;
                    background: #F0F0F0;
                    min-width: 120px;
                    max-width: 120px;
                    min-height: 120px;
                    max-height: 120px;
                }
            )");
            imageLabel->setScaledContents(true);
            cardLayout->addWidget(imageLabel);

            // 商品信息
            QWidget* infoWidget = new QWidget;
            QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
            infoLayout->setContentsMargins(0, 0, 0, 0);
            infoLayout->setSpacing(8);

            QLabel* titleLabel = new QLabel(post.getTitle());
            titleLabel->setStyleSheet(R"(
                font: bold 16px 'Microsoft YaHei';
                color: #333333;
            )");

            QLabel* priceLabel = new QLabel(QString("¥%1").arg(post.getPrice(), 0, 'f', 2));
            priceLabel->setStyleSheet(R"(
                font: 18px 'Microsoft YaHei';
                color: #FF4444;
            )");

            QLabel* timeLabel = new QLabel(QString("发布时间: %1").arg(
                post.getPostTime().toString("yyyy-MM-dd hh:mm:ss")));
            timeLabel->setStyleSheet(R"(
                font: 12px 'Microsoft YaHei';
                color: #666666;
            )");

            infoLayout->addWidget(titleLabel);
            infoLayout->addWidget(priceLabel);
            infoLayout->addWidget(timeLabel);
            infoLayout->addStretch();

            cardLayout->addWidget(infoWidget, 1);

            // 操作按钮
            QWidget* buttonWidget = new QWidget;
            QVBoxLayout* buttonLayout = new QVBoxLayout(buttonWidget);
            buttonLayout->setContentsMargins(0, 0, 0, 0);
            buttonLayout->setSpacing(8);

            QPushButton* editBtn = new QPushButton("✏️ 编辑");
            QPushButton* deleteBtn = new QPushButton("🗑️ 删除");

            editBtn->setStyleSheet(R"(
                QPushButton {
                    background: #28A745;
                    color: white;
                    border-radius: 8px;
                    padding: 8px 16px;
                    font: bold 12px 'Microsoft YaHei';
                    min-width: 80px;
                }
                QPushButton:hover {
                    background: #218838;
                }
            )");

            deleteBtn->setStyleSheet(R"(
                QPushButton {
                    background: #DC3545;
                    color: white;
                    border-radius: 8px;
                    padding: 8px 16px;
                    font: bold 12px 'Microsoft YaHei';
                    min-width: 80px;
                }
                QPushButton:hover {
                    background: #C82333;
                }
            )");

            connect(editBtn, &QPushButton::clicked, [this, post, dialog]() {
                dialog->accept();
                editItemPost(post);
            });

            connect(deleteBtn, &QPushButton::clicked, [this, post, dialog]() {
                dialog->accept();
                deleteItemPost(post);
            });

            buttonLayout->addWidget(editBtn);
            buttonLayout->addWidget(deleteBtn);
            buttonLayout->addStretch();

            cardLayout->addWidget(buttonWidget);

            contentLayout->addWidget(postCard);
        }
    }

    if(!hasPosts) {
        QLabel* emptyLabel = new QLabel("暂无发布的商品");
        emptyLabel->setStyleSheet(R"(
            font: 16px 'Microsoft YaHei';
            color: #666666;
            padding: 20px;
        )");
        emptyLabel->setAlignment(Qt::AlignCenter);
        contentLayout->addWidget(emptyLabel);
    }

    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    mainLayout->addWidget(scrollArea);

    // 连接关闭按钮
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->exec();
}

void MainWindow::editItemPost(const ItemPost& post) {
    // 权限验证
    if (!cur_user || post.getOwner()->getUserID() != cur_user->getUserID()) {
        QMessageBox::warning(this, "权限不足", "您只能编辑自己发布的商品！");
        return;
    }

    QDialog editor(this);
    editor.setWindowTitle("编辑商品信息");
    editor.resize(600, 500);

    // 图片相关变量 - 修复类型转换
    QStringList imagePaths;
    const std::vector<QString>& images = post.getImages();
    for (const auto& image : images) {
        imagePaths.append(image);
    }
    QString coverImagePath = post.getMainImage();

    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(&editor);

    /* 图片上传区域 */
    QHBoxLayout* imageLayout = new QHBoxLayout;

    // 图片预览标签
    QLabel* imagePreview = new QLabel;
    imagePreview->setFixedSize(100, 100);
    imagePreview->setStyleSheet("border: 2px dashed #ccc; border-radius: 8px;");
    imagePreview->setAlignment(Qt::AlignCenter);

    // 显示当前图片
    if (!coverImagePath.isEmpty()) {
        QPixmap pixmap(coverImagePath);
        pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imagePreview->setPixmap(pixmap);
    }

    // 上传按钮
    QPushButton* uploadBtn = new QPushButton("更换图片");
    uploadBtn->setFixedSize(100, 30);
    uploadBtn->setStyleSheet(R"(
        QPushButton {
            background: #007BFF;
            color: white;
            border-radius: 4px;
        }
        QPushButton:hover {
            background: #0056B3;
        }
    )");

    // 布局排列
    imageLayout->addWidget(imagePreview);
    imageLayout->addWidget(uploadBtn);
    imageLayout->addStretch();

    // 图片上传功能
    connect(uploadBtn, &QPushButton::clicked, [&]{
        QString path = QFileDialog::getOpenFileName(
            &editor,
            "选择商品图片",
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            "图片文件 (*.jpg *.jpeg *.png)"
            );

        if(!path.isEmpty()) {
            // 显示缩略图
            QPixmap pixmap(path);
            pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imagePreview->setPixmap(pixmap);
            imagePaths.clear();
            imagePaths.append(path);
            coverImagePath = path;
        }
    });

    mainLayout->addLayout(imageLayout);

    // 表单控件 - 预填充当前数据
    QLineEdit* titleEdit = new QLineEdit(post.getTitle());
    QDoubleSpinBox* priceEdit = new QDoubleSpinBox;
    priceEdit->setValue(post.getPrice());
    QLineEdit* tagsEdit = new QLineEdit;
    QTextEdit* contentEdit = new QTextEdit(post.getDescription());
    QPushButton* submitBtn = new QPushButton("保存修改");

    // 配置价格输入
    priceEdit->setRange(0.0, 999999.99);
    priceEdit->setPrefix("¥ ");
    priceEdit->setDecimals(2);
    priceEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

    // 处理标签显示
    QString tagsText;
    for (const auto& tag : post.getFlags()) {
        if (!tagsText.isEmpty()) tagsText += " ";
        tagsText += tag;
    }
    tagsEdit->setText(tagsText);
    tagsEdit->setPlaceholderText("用空格分隔标签，如：电子 数码 手机");

    // 表单布局
    QFormLayout* formLayout = new QFormLayout(&editor);
    formLayout->setLabelAlignment(Qt::AlignRight);

    formLayout->addRow("商品标题：", titleEdit);
    formLayout->addRow("商品价格：", priceEdit);
    formLayout->addRow("商品标签：", tagsEdit);
    formLayout->addRow("详细描述：", contentEdit);
    formLayout->addRow(submitBtn);
    mainLayout->addLayout(formLayout);

    // 输入验证
    auto validateInput = [&]() -> bool {
        if(titleEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&editor, "提示", "请输入商品标题");
            return false;
        }
        if(priceEdit->value() <= 0) {
            QMessageBox::warning(&editor, "提示", "价格必须大于0");
            return false;
        }
        return true;
    };

    connect(submitBtn, &QPushButton::clicked, [&]{
        if(!validateInput()) return;

        // 处理标签输入
        QString tagsInput = tagsEdit->text();
        std::vector<QString> parsedTags;
        if(!tagsInput.isEmpty()) {
            QStringList rawTags = tagsInput.split(" ");
            foreach(const QString& rawTag, rawTags) {
                QString processedTag = rawTag.trimmed()
                    .replace(" ", "")
                    .replace("#", "")
                    .left(15);

                if(!processedTag.isEmpty()) {
                    parsedTags.push_back(processedTag);
                }
            }
        }

        if(parsedTags.empty()) {
            parsedTags.push_back("未分类");
        }

        if(parsedTags.size() > 5) {
            parsedTags.resize(5);
        }

        // 查找并更新商品
        for (auto& itemPost : posts) {
            if (itemPost.getTitle() == post.getTitle() && 
                itemPost.getOwner()->getUserID() == post.getOwner()->getUserID() &&
                itemPost.getPostTime() == post.getPostTime()) {
                
                // 更新商品信息
                itemPost = ItemPost(titleEdit->text().trimmed(), 
                                   contentEdit->toPlainText(), 
                                   priceEdit->value(), 
                                   cur_user, 
                                   parsedTags);
                
                // 更新图片 - 修复类型转换
                for(const auto& path : imagePaths) {
                    itemPost.addImage(path);
                }
                
                QMessageBox::information(&editor, "成功", "商品信息已更新！");
                editor.accept();
                save_item(); // 保存数据到文件
                refreshItemPosts();
                return;
            }
        }
        
        QMessageBox::warning(&editor, "错误", "未找到要编辑的商品！");
    });

    // 样式设置
    editor.setStyleSheet(R"(
        QDialog {
            background: #F5F5F5;
        }
        QLineEdit, QTextEdit, QDoubleSpinBox {
            padding: 8px;
            border: 1px solid #DDD;
            border-radius: 4px;
            font: 14px 'Microsoft YaHei';
        }
        QTextEdit {
            min-height: 100px;
        }
    )");
    
    editor.exec();
}

void MainWindow::deleteItemPost(const ItemPost& post) {
    // 权限验证
    if (!cur_user || post.getOwner()->getUserID() != cur_user->getUserID()) {
        QMessageBox::warning(this, "权限不足", "您只能删除自己发布的商品！");
        return;
    }

    // 确认删除对话框
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "确认删除", 
        QString("确定要删除商品「%1」吗？\n此操作不可撤销！").arg(post.getTitle()),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        // 查找并删除商品
        auto it = posts.begin();
        while (it != posts.end()) {
            if (it->getTitle() == post.getTitle() && 
                it->getOwner()->getUserID() == post.getOwner()->getUserID() &&
                it->getPostTime() == post.getPostTime()) {
                
                // 删除相关的图片文件（可选）
                for (const auto& imagePath : it->getImages()) {
                    QFile file(imagePath);
                    if (file.exists()) {
                        // 可以选择删除图片文件，这里暂时注释掉
                        // file.remove();
                    }
                }
                
                // 从向量中删除
                posts.erase(it);
                
                QMessageBox::information(this, "删除成功", "商品已成功删除！");
                save_item(); // 保存数据到文件
                refreshItemPosts();
                return;
            }
            ++it;
        }
        
        QMessageBox::warning(this, "错误", "未找到要删除的商品！");
    }
}

void MainWindow::showItemManagementDialog() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("我的商品管理");
    dialog->setMinimumSize(900, 700);

    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);

    // 标题栏
    QWidget* headerWidget = new QWidget;
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);

    QLabel* titleLabel = new QLabel("我的商品管理");
    titleLabel->setStyleSheet(R"(
        font: bold 20px 'Microsoft YaHei';
        color: #333333;
    )");

    QPushButton* closeBtn = new QPushButton("关闭");
    closeBtn->setStyleSheet(R"(
        QPushButton {
            background: #6C757D;
            color: white;
            border-radius: 15px;
            padding: 8px 16px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #5A6268;
        }
    )");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeBtn);

    mainLayout->addWidget(headerWidget);

    // 滚动区域
    QScrollArea* scrollArea = new QScrollArea;
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(15);

    // 获取并显示用户发布的商品
    bool hasPosts = false;
    for(const auto& post : posts) {
        if(post.getOwner()->getUserID() == cur_user->getUserID()) {
            hasPosts = true;
            
            // 创建商品卡片
            QWidget* postCard = new QWidget;
            postCard->setStyleSheet(R"(
                QWidget {
                    background: white;
                    border-radius: 12px;
                    padding: 16px;
                    margin: 8px 0;
                }
                QWidget:hover {
                    background: #FCFCFC;
                    box-shadow: 0 2px 12px rgba(0,0,0,0.1);
                }
            )");

            QHBoxLayout* cardLayout = new QHBoxLayout(postCard);
            cardLayout->setContentsMargins(0, 0, 0, 0);
            cardLayout->setSpacing(20);

            // 商品图片
            QLabel* imageLabel = new QLabel;
            QString mainImage = post.getImages().empty() ?
                                ":/default_item.jpg" : post.getMainImage();
            QPixmap pixmap(mainImage);
            pixmap = pixmap.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            imageLabel->setPixmap(pixmap);
            imageLabel->setStyleSheet(R"(
                QLabel {
                    border-radius: 8px;
                    background: #F0F0F0;
                    min-width: 120px;
                    max-width: 120px;
                    min-height: 120px;
                    max-height: 120px;
                }
            )");
            imageLabel->setScaledContents(true);
            cardLayout->addWidget(imageLabel);

            // 商品信息
            QWidget* infoWidget = new QWidget;
            QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
            infoLayout->setContentsMargins(0, 0, 0, 0);
            infoLayout->setSpacing(8);

            QLabel* titleLabel = new QLabel(post.getTitle());
            titleLabel->setStyleSheet(R"(
                font: bold 16px 'Microsoft YaHei';
                color: #333333;
            )");

            QLabel* priceLabel = new QLabel(QString("¥%1").arg(post.getPrice(), 0, 'f', 2));
            priceLabel->setStyleSheet(R"(
                font: 18px 'Microsoft YaHei';
                color: #FF4444;
            )");

            QLabel* timeLabel = new QLabel(QString("发布时间: %1").arg(
                post.getPostTime().toString("yyyy-MM-dd hh:mm:ss")));
            timeLabel->setStyleSheet(R"(
                font: 12px 'Microsoft YaHei';
                color: #666666;
            )");

            infoLayout->addWidget(titleLabel);
            infoLayout->addWidget(priceLabel);
            infoLayout->addWidget(timeLabel);
            infoLayout->addStretch();

            cardLayout->addWidget(infoWidget, 1);

            // 操作按钮
            QWidget* buttonWidget = new QWidget;
            QVBoxLayout* buttonLayout = new QVBoxLayout(buttonWidget);
            buttonLayout->setContentsMargins(0, 0, 0, 0);
            buttonLayout->setSpacing(8);

            QPushButton* editBtn = new QPushButton("✏️ 编辑");
            QPushButton* deleteBtn = new QPushButton("🗑️ 删除");

            editBtn->setStyleSheet(R"(
                QPushButton {
                    background: #28A745;
                    color: white;
                    border-radius: 8px;
                    padding: 8px 16px;
                    font: bold 12px 'Microsoft YaHei';
                    min-width: 80px;
                }
                QPushButton:hover {
                    background: #218838;
                }
            )");

            deleteBtn->setStyleSheet(R"(
                QPushButton {
                    background: #DC3545;
                    color: white;
                    border-radius: 8px;
                    padding: 8px 16px;
                    font: bold 12px 'Microsoft YaHei';
                    min-width: 80px;
                }
                QPushButton:hover {
                    background: #C82333;
                }
            )");

            connect(editBtn, &QPushButton::clicked, [this, post, dialog]() {
                dialog->accept();
                editItemPost(post);
            });

            connect(deleteBtn, &QPushButton::clicked, [this, post, dialog]() {
                dialog->accept();
                deleteItemPost(post);
            });

            buttonLayout->addWidget(editBtn);
            buttonLayout->addWidget(deleteBtn);
            buttonLayout->addStretch();

            cardLayout->addWidget(buttonWidget);

            contentLayout->addWidget(postCard);
        }
    }

    if(!hasPosts) {
        QLabel* emptyLabel = new QLabel("暂无发布的商品");
        emptyLabel->setStyleSheet(R"(
            font: 16px 'Microsoft YaHei';
            color: #666666;
            padding: 20px;
        )");
        emptyLabel->setAlignment(Qt::AlignCenter);
        contentLayout->addWidget(emptyLabel);
    }

    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    mainLayout->addWidget(scrollArea);

    // 连接关闭按钮
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->exec();
}

void MainWindow::toggleFavorite(const ItemPost& post) {
    if (!cur_user) {
        QMessageBox::warning(this, "提示", "请先登录！");
        return;
    }

    // 查找商品并切换收藏状态
    for (auto& itemPost : posts) {
        if (itemPost.getTitle() == post.getTitle() && 
            itemPost.getOwner()->getUserID() == post.getOwner()->getUserID() &&
            itemPost.getPostTime() == post.getPostTime()) {
            
            if (itemPost.isFavoritedBy(cur_user)) {
                // 取消收藏
                itemPost.removeFromFavorites(cur_user);
                QMessageBox::information(this, "收藏", "已取消收藏！");
            } else {
                // 添加收藏
                itemPost.addToFavorites(cur_user);
                QMessageBox::information(this, "收藏", "已添加到收藏！");
            }
            
            // 保存数据并刷新界面
            save_item();
            refreshItemPosts();
            return;
        }
    }
    
    QMessageBox::warning(this, "错误", "未找到商品！");
}

void MainWindow::showFavoritesDialog() {
    if (!cur_user) {
        QMessageBox::warning(this, "提示", "请先登录！");
        return;
    }

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("我的收藏");
    dialog->setMinimumSize(900, 700);

    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);

    // 标题栏
    QWidget* headerWidget = new QWidget;
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);

    QLabel* titleLabel = new QLabel("我的收藏");
    titleLabel->setStyleSheet(R"(
        font: bold 20px 'Microsoft YaHei';
        color: #333333;
    )");

    QPushButton* closeBtn = new QPushButton("关闭");
    closeBtn->setStyleSheet(R"(
        QPushButton {
            background: #6C757D;
            color: white;
            border-radius: 15px;
            padding: 8px 16px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #5A6268;
        }
    )");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeBtn);

    mainLayout->addWidget(headerWidget);

    // 滚动区域
    QScrollArea* scrollArea = new QScrollArea;
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(15);

    // 获取并显示用户收藏的商品
    bool hasFavorites = false;
    for(const auto& post : posts) {
        if(post.isFavoritedBy(cur_user)) {
            hasFavorites = true;
            
            // 创建商品卡片
            QWidget* postCard = new QWidget;
            postCard->setStyleSheet(R"(
                QWidget {
                    background: white;
                    border-radius: 12px;
                    padding: 16px;
                    margin: 8px 0;
                }
                QWidget:hover {
                    background: #FCFCFC;
                    box-shadow: 0 2px 12px rgba(0,0,0,0.1);
                }
            )");

            QHBoxLayout* cardLayout = new QHBoxLayout(postCard);
            cardLayout->setContentsMargins(0, 0, 0, 0);
            cardLayout->setSpacing(20);

            // 商品图片
            QLabel* imageLabel = new QLabel;
            QString mainImage = post.getImages().empty() ?
                                ":/default_item.jpg" : post.getMainImage();
            QPixmap pixmap(mainImage);
            pixmap = pixmap.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            imageLabel->setPixmap(pixmap);
            imageLabel->setStyleSheet(R"(
                QLabel {
                    border-radius: 8px;
                    background: #F0F0F0;
                    min-width: 120px;
                    max-width: 120px;
                    min-height: 120px;
                    max-height: 120px;
                }
            )");
            imageLabel->setScaledContents(true);
            cardLayout->addWidget(imageLabel);

            // 商品信息
            QWidget* infoWidget = new QWidget;
            QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
            infoLayout->setContentsMargins(0, 0, 0, 0);
            infoLayout->setSpacing(8);

            QLabel* titleLabel = new QLabel(post.getTitle());
            titleLabel->setStyleSheet(R"(
                font: bold 16px 'Microsoft YaHei';
                color: #333333;
            )");

            QLabel* priceLabel = new QLabel(QString("¥%1").arg(post.getPrice(), 0, 'f', 2));
            priceLabel->setStyleSheet(R"(
                font: 18px 'Microsoft YaHei';
                color: #FF4444;
            )");

            QLabel* ownerLabel = new QLabel(QString("发布者: %1").arg(post.getOwner()->getUsername()));
            ownerLabel->setStyleSheet(R"(
                font: 12px 'Microsoft YaHei';
                color: #666666;
            )");

            infoLayout->addWidget(titleLabel);
            infoLayout->addWidget(priceLabel);
            infoLayout->addWidget(ownerLabel);
            infoLayout->addStretch();

            cardLayout->addWidget(infoWidget, 1);

            // 操作按钮
            QWidget* buttonWidget = new QWidget;
            QVBoxLayout* buttonLayout = new QVBoxLayout(buttonWidget);
            buttonLayout->setContentsMargins(0, 0, 0, 0);
            buttonLayout->setSpacing(8);

            QPushButton* viewBtn = new QPushButton("👁️ 查看");
            viewBtn->setStyleSheet(R"(
                QPushButton {
                    background: #007BFF;
                    color: white;
                    border-radius: 8px;
                    padding: 8px 16px;
                    font: bold 12px 'Microsoft YaHei';
                    min-width: 80px;
                }
                QPushButton:hover {
                    background: #0056B3;
                }
            )");
            
            QPushButton* unfavoriteBtn = new QPushButton;
            unfavoriteBtn->setFixedSize(32, 32);
            unfavoriteBtn->setCursor(Qt::PointingHandCursor);
            unfavoriteBtn->setText("❤️");
            unfavoriteBtn->setToolTip("取消收藏");
            unfavoriteBtn->setStyleSheet(R"(
                QPushButton {
                    background: #FFE6E6;
                    border: 2px solid #FF4444;
                    border-radius: 16px;
                    font-size: 16px;
                    color: #FF4444;
                }
                QPushButton:hover {
                    background: #FFCCCC;
                }
            )");

            connect(viewBtn, &QPushButton::clicked, [this, post, dialog]() {
                dialog->accept();
                showDetailPage(post);
            });

            connect(unfavoriteBtn, &QPushButton::clicked, [this, post, dialog]() {
                dialog->accept();
                toggleFavorite(post);
            });

            buttonLayout->addWidget(viewBtn);
            buttonLayout->addWidget(unfavoriteBtn);
            buttonLayout->addStretch();

            cardLayout->addWidget(buttonWidget);

            contentLayout->addWidget(postCard);
        }
    }

    if(!hasFavorites) {
        QLabel* emptyLabel = new QLabel("暂无收藏的商品");
        emptyLabel->setStyleSheet(R"(
            font: 16px 'Microsoft YaHei';
            color: #666666;
            padding: 20px;
        )");
        emptyLabel->setAlignment(Qt::AlignCenter);
        contentLayout->addWidget(emptyLabel);
    }

    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    mainLayout->addWidget(scrollArea);

    // 连接关闭按钮
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->exec();
}

// 新增：更新未读消息显示的函数
void MainWindow::updateUnreadMessageDisplay() {
    // 计算当前未读消息总数
    int totalUnread = 0;
    qDebug() << "更新未读消息显示，聊天室数量:" << activeChatRooms.size();
    
    for (auto room : activeChatRooms) {
        int roomUnread = room->getUnreadCount(const_cast<User *>(cur_user));
        totalUnread += roomUnread;
        qDebug() << "聊天室未读数量:" << roomUnread;
    }
    
    qDebug() << "总未读消息数量:" << totalUnread;
    
    // 如果当前在个人主页，更新未读消息按钮
    if (stackWidget && stackWidget->currentIndex() == 0) {
        // 查找未读消息按钮并更新
        QWidget* page1Widget = stackWidget->widget(0);
        if (page1Widget) {
            // 查找统计卡片中的未读消息按钮
            QList<QPushButton*> buttons = page1Widget->findChildren<QPushButton*>();
            for (QPushButton* btn : buttons) {
                if (btn->text().contains("收到私信")) {
                    btn->setText(QString("收到私信\n%1").arg(totalUnread));
                    qDebug() << "更新个人主页未读消息按钮:" << totalUnread;
                    
                    // 根据未读消息数量更新样式
                    if (totalUnread > 0) {
                        btn->setStyleSheet(R"(
                            QPushButton {
                                background: white;
                                border-radius: 12px;
                                padding: 24px 10px;
                                border: 3px solid #FF4444;
                                font: bold 22px 'Microsoft YaHei';
                                color: #E74C3C;
                            }
                            QPushButton:hover {
                                background: #FFF8F8;
                                border-color: #FF6666;
                                color: #C0392B;
                            }
                            QPushButton:pressed {
                                background: #FFE8E8;
                                border-color: #CC0000;
                            }
                        )");
                    } else {
                        btn->setStyleSheet(R"(
                            QPushButton {
                                background: white;
                                border-radius: 12px;
                                padding: 24px 10px;
                                border: 2px solid #E8E8E8;
                                font: bold 22px 'Microsoft YaHei';
                                color: #333333;
                            }
                            QPushButton:hover {
                                background: #F8F9FA;
                                border-color: #D0D0D0;
                            }
                            QPushButton:pressed {
                                background: #F0F0F0;
                            }
                        )");
                    }
                    break;
                }
            }
        }
    }
    
    // 如果当前在私信页面，更新未读消息标签
    if (stackWidget && stackWidget->currentIndex() == 2) {
        QWidget* page3Widget = stackWidget->widget(2);
        if (page3Widget) {
            // 查找未读消息标签并更新
            QList<QLabel*> labels = page3Widget->findChildren<QLabel*>();
            for (QLabel* label : labels) {
                if (label->text().contains("未读消息")) {
                    label->setText(QString("未读消息: %1").arg(totalUnread));
                    qDebug() << "更新私信页面未读消息标签:" << totalUnread;
                    break;
                }
            }
        }
    }
}

// 新增：评分系统方法实现
void MainWindow::showRatingDialog(const User& targetUser)
{
    if (!cur_user) {
        QMessageBox::warning(this, "提示", "请先登录！");
        return;
    }

    // 检查是否已经评价过
    if (targetUser.hasRatedBy(cur_user->getUserID())) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, 
            "确认重新评价", 
            QString("您已经评价过 %1，是否要重新评价？").arg(targetUser.getUsername()),
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    RatingDialog dialog(targetUser.getUsername(), this);
    if (dialog.exec() == QDialog::Accepted) {
        int rating = dialog.getRating();
        QString comment = dialog.getComment();
        
        // 查找目标用户并添加评分
        for (auto& user : users) {
            if (user.getUserID() == targetUser.getUserID()) {
                user.addRating(cur_user->getUserID(), rating, comment);
                break;
            }
        }
        
        // 保存用户数据
        saveUsers();
        
        QMessageBox::information(this, "评价成功", 
            QString("您已成功为 %1 评分 %2 星！").arg(targetUser.getUsername()).arg(rating));
    }
}

void MainWindow::showUserRatings(const User& user)
{
    QDialog dialog(this);
    dialog.setWindowTitle(QString("%1 的用户评价").arg(user.getUsername()));
    dialog.setMinimumSize(600, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(20, 15, 20, 15);

    // 评分统计信息
    QWidget* statsWidget = new QWidget;
    QHBoxLayout* statsLayout = new QHBoxLayout(statsWidget);
    statsLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* avgRatingLabel = new QLabel(QString("平均评分: %1").arg(user.getAverageRating(), 0, 'f', 1));
    avgRatingLabel->setStyleSheet(R"(
        font: bold 16px 'Microsoft YaHei';
        color: #FF6B35;
        padding: 10px;
        background: #FFF8F0;
        border-radius: 8px;
    )");

    QLabel* ratingLevelLabel = new QLabel(user.getRatingLevel());
    ratingLevelLabel->setStyleSheet(R"(
        font: bold 16px 'Microsoft YaHei';
        color: #FF6B35;
        padding: 10px;
        background: #FFF8F0;
        border-radius: 8px;
    )");

    QLabel* countLabel = new QLabel(QString("评价人数: %1").arg(user.getRatingCount()));
    countLabel->setStyleSheet(R"(
        font: bold 16px 'Microsoft YaHei';
        color: #666666;
        padding: 10px;
        background: #F8F9FA;
        border-radius: 8px;
    )");

    statsLayout->addWidget(avgRatingLabel);
    statsLayout->addWidget(ratingLevelLabel);
    statsLayout->addWidget(countLabel);
    statsLayout->addStretch();

    mainLayout->addWidget(statsWidget);

    // 评价列表
    QScrollArea* scrollArea = new QScrollArea;
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(10);

    const QMap<QString, int>& ratings = user.getRatings();
    const QMap<QString, QString>& comments = user.getComments();

    if (ratings.isEmpty()) {
        QLabel* emptyLabel = new QLabel("暂无用户评价");
        emptyLabel->setStyleSheet(R"(
            font: 16px 'Microsoft YaHei';
            color: #666666;
            padding: 20px;
        )");
        emptyLabel->setAlignment(Qt::AlignCenter);
        contentLayout->addWidget(emptyLabel);
    } else {
        for (auto it = ratings.begin(); it != ratings.end(); ++it) {
            QString raterID = it.key();
            int rating = it.value();
            QString comment = comments.value(raterID, "");

            // 查找评价者用户名
            QString raterName = "未知用户";
            for (const auto& u : users) {
                if (u.getUserID() == raterID) {
                    raterName = u.getUsername();
                    break;
                }
            }

            // 创建评价卡片
            QWidget* ratingCard = new QWidget;
            ratingCard->setStyleSheet(R"(
                QWidget {
                    background: white;
                    border: 1px solid #E0E0E0;
                    border-radius: 8px;
                    padding: 15px;
                }
                QWidget:hover {
                    background: #F8F9FA;
                }
            )");

            QVBoxLayout* cardLayout = new QVBoxLayout(ratingCard);
            cardLayout->setContentsMargins(0, 0, 0, 0);
            cardLayout->setSpacing(8);

            // 评价者信息和评分
            QHBoxLayout* headerLayout = new QHBoxLayout;
            
            QLabel* raterLabel = new QLabel(QString("评价者: %1").arg(raterName));
            raterLabel->setStyleSheet("font: bold 14px 'Microsoft YaHei'; color: #333333;");
            
            QString stars;
            for (int i = 0; i < rating; ++i) {
                stars += "⭐";
            }
            QLabel* starsLabel = new QLabel(stars);
            starsLabel->setStyleSheet("font: 16px; color: #FF6B35;");

            headerLayout->addWidget(raterLabel);
            headerLayout->addStretch();
            headerLayout->addWidget(starsLabel);

            cardLayout->addLayout(headerLayout);

            // 评价内容
            if (!comment.isEmpty()) {
                QLabel* commentLabel = new QLabel(comment);
                commentLabel->setStyleSheet(R"(
                    font: 14px 'Microsoft YaHei';
                    color: #666666;
                    padding: 8px;
                    background: #F8F9FA;
                    border-radius: 4px;
                )");
                commentLabel->setWordWrap(true);
                cardLayout->addWidget(commentLabel);
            }

            contentLayout->addWidget(ratingCard);
        }
    }

    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    mainLayout->addWidget(scrollArea);

    // 关闭按钮
    QPushButton* closeBtn = new QPushButton("关闭");
    closeBtn->setStyleSheet(R"(
        QPushButton {
            background: #6C757D;
            color: white;
            padding: 8px 20px;
            border-radius: 6px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #5A6268;
        }
    )");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    mainLayout->addLayout(buttonLayout);

    dialog.exec();
}
