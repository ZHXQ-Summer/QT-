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
#include<QDoubleSpinBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include<QFileInfo>
#include<qstandardpaths.h>
#include<QGroupBox>
#include<globalfunc.h>
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
            // 初始化设置
            newRoom->loadHistory();
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
            // 初始化设置
            newRoom->loadHistory();
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
            QMenuBar* menubar=this->menuBar();
            QMenu* menu1 = new QMenu("当前页面");
            menubar->addMenu(menu1);
            stackWidget = new QStackedWidget;
            setCentralWidget(stackWidget);  // 设为中央部件
            createPages();  // 创建所有页面
            QAction *page1Action = new QAction("我的账号", this);
            QAction *page2Action = new QAction("主页", this);
            QAction *page3Action = new QAction("私信", this);
            connect(page1Action, &QAction::triggered, [this](){ switchPage(0); });
            connect(page2Action, &QAction::triggered, [this](){ switchPage(1); });
            connect(page3Action, &QAction::triggered, [this](){ switchPage(2); });
            menu1->addAction(page1Action);
            menu1->addAction(page2Action);
            menu1->addAction(page3Action);
            setWindowTitle("欢迎"+cur_user->getUsername()+"!");
    }
}
}
void MainWindow::onRegisterClicked()
{
    RegisterDialog dlg(this);
    dlg.exec();
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
QWidget* MainWindow::mine_create() {//主页创建
    QWidget *userInfoPage = new QWidget;

    // 整体垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout(userInfoPage);
    mainLayout->setContentsMargins(30, 20, 30, 20); // 设置边距
    mainLayout->setSpacing(25); // 设置间距


    QWidget *basicInfoWidget = new QWidget;
    QHBoxLayout *basicLayout = new QHBoxLayout(basicInfoWidget);
    basicLayout->setSpacing(20);

    // 左侧：头像区域（示例使用占位符）
    ClickedLabel *avatarLabel = new ClickedLabel;
    avatarLabel->setFixedSize(100, 100);
    avatarLabel->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setCursor(Qt::PointingHandCursor); // 设置手型光标
    connect(avatarLabel, &ClickedLabel::clicked, this, [this, avatarLabel]() {
        handleAvatarUpload(avatarLabel);
    });
    // 初始化头像显示
    updateAvatarDisplay(avatarLabel); // 新增头像更新函数

    // 设置样式和点击事件
    avatarLabel->setStyleSheet(
        "background: #FFFFFF;"
        "border-radius: 50px;"
        "border: 2px solid #FF69B4;"
        "QLabel:hover { border-color: #FF1493; }" // 悬停效果
        );


    // 右侧：基本信息
    QWidget *infoWidget = new QWidget;
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setSpacing(8);

    // 用户名（突出显示）
    QLabel *nameLabel = new QLabel(cur_user->getUsername());
    nameLabel->setStyleSheet(
        "font: bold 24px 'Microsoft YaHei';"
        "color: #FF1493;"
        );

    // 用户ID
    QLabel *idLabel = new QLabel(QString("ID: %1").arg(cur_user->getUserID()));
    idLabel->setStyleSheet("font: 14px; color: #666;");

    // 注册时间
    QLabel *regTimeLabel = new QLabel(
        QString("注册时间: %1").arg(
            cur_user->getRegTime().toString("yyyy-MM-dd hh:mm:ss")
            )
        );
    regTimeLabel->setStyleSheet("font: 14px; color: #888;");

    // 组装基本信息
    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(idLabel);
    infoLayout->addWidget(regTimeLabel);
    infoLayout->addStretch();
    //编辑
    QWidget *actionWidget = new QWidget;
    QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(0, 10, 0, 0);

    QPushButton *editProfileBtn = new QPushButton("编辑资料");
    editProfileBtn->setFixedSize(100, 30);
    editProfileBtn->setStyleSheet(
        "QPushButton {"
        "  background: #FF69B4;"
        "  color: white;"
        "  border-radius: 5px;"
        "}"
        "QPushButton:hover { background: #FF1493; }"
        );
    actionLayout->addWidget(editProfileBtn);
    actionLayout->addStretch();
    connect(editProfileBtn, &QPushButton::clicked,this, &MainWindow::edit);
    // ================= 修改布局结构 =================
    basicLayout->addWidget(avatarLabel);
    basicLayout->addWidget(infoWidget);
    basicLayout->addWidget(actionWidget);

    // ================= 详细信息区域 =================
    QWidget *detailWidget = new QWidget;
    QFormLayout *formLayout = new QFormLayout(detailWidget);
    formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->setContentsMargins(10, 10, 10, 10);
    formLayout->setSpacing(15);

    // 邮箱信息
    QLabel *emailTitle = new QLabel("电子邮箱:");
    QLabel *emailContent = new QLabel(cur_user->getEmail());
    emailContent->setStyleSheet("color: #333;");
    formLayout->addRow(emailTitle, emailContent);

    // 权限等级
    QString authLevel = "";
    switch(cur_user->getAuthLevel()) {
    case 0: authLevel = "普通用户"; break;
    case 1: authLevel = "VIP用户"; break;
    case 2: authLevel = "管理员"; break;
    }
    QLabel *authTitle = new QLabel("权限等级:");
    QLabel *authContent = new QLabel(authLevel);
    authContent->setStyleSheet("color: #FF4500; font-weight: bold;");
    formLayout->addRow(authTitle, authContent);

    // 其他信息
    QLabel *statusTitle = new QLabel("账户状态:");
    QLabel *statusContent = new QLabel("正常使用中");
    statusContent->setStyleSheet("color: #32CD32;");
    formLayout->addRow(statusTitle, statusContent);

    // ================= 组装最终布局 =================
    mainLayout->addWidget(basicInfoWidget);

    // 添加分隔线
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #EEE;");
    mainLayout->addWidget(line);

    mainLayout->addWidget(detailWidget);
    mainLayout->addStretch(); // 底部留空

    // 添加退出按钮
    QPushButton *exitButton = new QPushButton("安全退出");
    exitButton->setFixedSize(120, 35);
    exitButton->setStyleSheet(
        "QPushButton {"
        "  background: #FF4444;"
        "  color: white;"
        "  border-radius: 8px;"
        "  font: bold 14px 'Microsoft YaHei';"
        "}"
        "QPushButton:hover {"
        "  background: #FF6666;"
        "}"
        "QPushButton:pressed {"
        "  background: #CC0000;"
        "}");
    connect(exitButton, &QPushButton::clicked, this, [](){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "确认退出",
                                      "确定要退出程序吗？",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qApp->quit();
        }
    });

    // 将退出按钮放入水平布局容器保持居中
    QWidget *exitContainer = new QWidget;
    QHBoxLayout *exitLayout = new QHBoxLayout(exitContainer);
    exitLayout->setContentsMargins(0, 20, 0, 20);
    exitLayout->addStretch();
    exitLayout->addWidget(exitButton);
    exitLayout->addStretch();

    mainLayout->addWidget(exitContainer);
    // 设置页面样式
    userInfoPage->setStyleSheet(
        "background: #FFF5EE;"  // 浅粉色背景
        "font-family: 'Microsoft YaHei';"
        );

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

    // 在原有titleLayout中添加作者标签
    titleLayout->addWidget(authorLabel);
    titleLayout->addWidget(titleLabel, 4);
    titleLayout->addWidget(priceLabel, 1);

    // 连接作者标签点击信号
    connect(authorLabel, &ClickedLabel::clicked, this, [this, post]{
        showAuthorInfo(*post.getOwner());
    });
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
    dialog.setMinimumSize(320, 280);

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

    QPushButton* chatBtn = new QPushButton("发送私信");
    chatBtn->setStyleSheet(R"(
        QPushButton {
            background: #28a745;
            color: white;
            padding: 8px 16px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background: #218838;
        }
    )");

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok);
    buttons->addButton(chatBtn, QDialogButtonBox::ActionRole);
    connect(chatBtn, &QPushButton::clicked, [this, &author, &dialog]{
        dialog.accept();
        createChatRoom(const_cast<User*>(&author)); // 实际项目中应避免const_cast
    });

    mainLayout->addWidget(buttons);
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
    card->setStyleSheet(R"(
    QWidget {
        background: white;
        border-radius: 8px;
        padding: 12px;
    }
    QWidget:hover {
        background: #F8F9FA;
    }
)");
    card->setMinimumHeight(80);

    // 主布局保持水平布局
    QHBoxLayout* mainLayout = new QHBoxLayout(card);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(12);

    /* 头像区域（保持原样） */
    QLabel* avatarLabel = new QLabel;
    User* otherUser = room->getOtherUser(cur_user); // 获取对方用户
    QPixmap avatar(otherUser->getAvatarPath().isEmpty() ?
                       ":/default_item.jpg" : otherUser->getAvatarPath());
    avatar = avatar.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setStyleSheet(R"(
        QLabel {
            border-radius: 24px;
            border: 1px solid #EEE;
        }
    )");
    mainLayout->addWidget(avatarLabel);

    /* 右侧信息区域 */
    QWidget* rightArea = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightArea);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(4);

    // 顶部行（用户名 + 时间）
    QWidget* topRow = new QWidget;
    QHBoxLayout* topLayout = new QHBoxLayout(topRow);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(8);

    // 用户名标签（保持原样）
    ClickedLabel* nameLabel = new ClickedLabel(otherUser->getUsername());
    nameLabel->setStyleSheet(R"(
        font: bold 14px 'Microsoft YaHei';
        color: #333;
    )");
    topLayout->addWidget(nameLabel);

    // 时间标签（新增弹性空间使其右对齐）
    QWidget* timeSpacer = new QWidget;
    timeSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    topLayout->addWidget(timeSpacer);

    QLabel* timeLabel = new QLabel(room->getLastMessageTime().toString("hh:mm"));
    timeLabel->setStyleSheet(R"(
    font: 12px 'Microsoft YaHei';
    color: #999;
    padding-right: 4px;
)");
    topLayout->addWidget(timeLabel);

    rightLayout->addWidget(topRow);

    // 消息内容（保持原样但调整对齐）
    QLabel* msgLabel = new QLabel(room->getLastMessageContent());
    msgLabel->setStyleSheet(R"(
    font: 13px 'Microsoft YaHei';
    color: #666;
    qproperty-elideMode: ElideRight;
    padding-top: 2px;
)");
    msgLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    rightLayout->addWidget(msgLabel);

    mainLayout->addWidget(rightArea, 1);

    /* 未读标记调整到右上角 */
    if(int unread = room->getUnreadCount(const_cast<User *>(cur_user))) {
        QLabel* badge = new QLabel(QString::number(unread));
        // 保持原有样式，仅修改布局
        badge->setStyleSheet(R"(
        background: #FF4444;
        color: white;
        min-width: 20px;
        max-width: 20px;
        min-height: 20px;
        max-height: 20px;
        border-radius: 10px;
        font: bold 10px 'Microsoft YaHei';
        qproperty-alignment: AlignCenter;
    )");

        // 使用覆盖布局定位
        QHBoxLayout* badgeLayout = new QHBoxLayout(card);
        badgeLayout->setContentsMargins(0, 4, 4, 0);
        badgeLayout->addWidget(badge, 0, Qt::AlignTop | Qt::AlignRight);
    }

    // 点击事件处理
    connect(nameLabel, &ClickedLabel::clicked, [this, room]{
        room->markAsRead(const_cast<User *>(cur_user));
        room->show();
    });

    // 设置鼠标手势
    card->setCursor(Qt::PointingHandCursor);

    return card;
}

// 创建聊天室列表容器
QWidget* MainWindow::createChatListWidget() {
    QWidget* container = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(container);

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
