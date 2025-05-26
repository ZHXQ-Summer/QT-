#include <MainWindow.h>
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
    page3 = new QWidget;
    page3->setStyleSheet("background: #F5F5DC;");
    QLabel *label3 = new QLabel("这是页面3", page3);
    label3->setAlignment(Qt::AlignCenter);

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

    return mainWidget;
}

QWidget* MainWindow::createSinglePost(const ItemPost& post) {
    // 创建卡片容器
    QWidget* card = new QWidget;
    card->setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 12px;
            padding: 20px;
        }
        QWidget:hover {
            background: #F8F8F8;
        }
    )");

    // 主布局
    QHBoxLayout* layout = new QHBoxLayout(card);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(20);

    /* 图片区域 */
    QLabel* imageLabel = new QLabel;
    QString mainImage = post.getMainImage().isEmpty() ?
                            ":/default_item.png" : post.getMainImage();
    QPixmap pixmap(mainImage);
    pixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(pixmap);
    imageLabel->setFixedSize(200, 200);
    imageLabel->setStyleSheet("border-radius: 8px;");
    layout->addWidget(imageLabel);

    /* 信息区域 */
    QWidget* infoWidget = new QWidget;
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);

    // 标题和价格
    ClickedLabel* titleLabel = new ClickedLabel(
        QString("<a href='detail' style='text-decoration:none; color:#333;'>%1</a>")
            .arg(post.getTitle())
        );
    titleLabel->setTextFormat(Qt::RichText);
    titleLabel->setCursor(Qt::PointingHandCursor);
    connect(titleLabel, &ClickedLabel::clicked, this, [this, post]{
        showDetailPage(post);
    });

    QLabel* priceLabel = new QLabel(QString("¥%1").arg(post.getPrice(), 0, 'f', 2));
    priceLabel->setStyleSheet(R"(
        font: bold 20px 'Microsoft YaHei';
        color: #FF4444;
        margin-top: 8px;
    )");

    // 标签流式布局
    QWidget* tagWidget = new QWidget;
    QHBoxLayout* tagLayout = new QHBoxLayout(tagWidget);
    tagLayout->setContentsMargins(0, 10, 0, 0);
    tagLayout->setSpacing(8);

    for (const auto& tag : post.getFlags()) {
        QLabel* tagLabel = new QLabel(tag);
        tagLabel->setStyleSheet(R"(
            background: #E8E8E8;
            border-radius: 4px;
            padding: 4px 8px;
            color: #666666;
            font: 12px 'Microsoft YaHei';
        )");
        tagLayout->addWidget(tagLabel);
    }
    tagLayout->addStretch();

    // 组装信息区域
    QLabel* descLabel = new QLabel(post.getDescription());
    descLabel->setStyleSheet(R"(
    font: 14px 'Microsoft YaHei';
    color: #666666;
    margin-top: 8px;
    margin-bottom: 12px;
)");
    descLabel->setWordWrap(true);  // 启用自动换行
    descLabel->setFixedWidth(400); // 限制宽度确保换行效果

    // 在infoLayout中的价格标签后添加
    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(priceLabel);
    infoLayout->addWidget(descLabel);  // 新增描述
    infoLayout->addWidget(tagWidget);
    infoLayout->addStretch();

    layout->addWidget(infoWidget, 1);  // 设置伸展因子为1

    return card;
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

    // 返回按钮（改为关闭弹窗）
    QPushButton *backBtn = new QPushButton("← 返回", content);
    backBtn->setStyleSheet("font: bold 14px; padding:8px;");
    connect(backBtn, &QPushButton::clicked, parent, &QDialog::accept);

    // 图片展示区域
    QLabel *imageLabel = new QLabel(content);
    QPixmap pixmap(post.getMainImage().isEmpty() ?
                       ":/default_item.png" : post.getMainImage());
    pixmap = pixmap.scaled(600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);

    // 信息区域
    QWidget *infoWidget = new QWidget(content);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);

    QLabel *titleLabel = new QLabel(post.getTitle(), infoWidget);
    titleLabel->setStyleSheet(R"(
        font: bold 24px 'Microsoft YaHei';
        color: #333333;
        margin: 15px 0;
    )");

    QLabel *priceLabel = new QLabel(QString("价格: ¥%1").arg(post.getPrice(), 0, 'f', 2), infoWidget);
    priceLabel->setStyleSheet(R"(
        font: 20px 'Microsoft YaHei';
        color: #FF4444;
        margin: 10px 0;
    )");

    QLabel *descLabel = new QLabel(post.getDescription(), infoWidget);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(R"(
        font: 16px 'Microsoft YaHei';
        color: #666666;
        line-height: 1.5;
        margin: 10px 0;
    )");

    // 组装布局
    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(priceLabel);
    infoLayout->addWidget(descLabel);
    infoLayout->addStretch();

    mainLayout->addWidget(backBtn);
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(infoWidget);
    mainLayout->addStretch();

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
    tagsEdit->setPlaceholderText("用逗号分隔标签，如：电子, 数码, 手机");

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

        ItemPost newPost(titleEdit->text(),contentEdit->toPlainText(),priceEdit->value(),cur_user);
        for(const auto& path : imagePaths) {
            newPost.addImage(path);
        }
        // 添加默认封面图
        if(newPost.getImages().empty()) {
            newPost.addImage(":/default_item.png");
        }
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
