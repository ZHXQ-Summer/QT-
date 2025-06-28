#include "RatingDialog.h"
#include <QMessageBox>
#include <QApplication>

RatingDialog::RatingDialog(const QString& targetUsername, QWidget *parent)
    : QDialog(parent), targetUsername(targetUsername)
{
    setWindowTitle("用户评价 - " + targetUsername);
    setFixedSize(400, 350);
    setModal(true);
    
    setupUI();
    setupConnections();
}

void RatingDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    // 标题
    QLabel *titleLabel = new QLabel(QString("为 %1 评分").arg(targetUsername));
    titleLabel->setStyleSheet(R"(
        font: bold 18px 'Microsoft YaHei';
        color: #333333;
        padding: 10px;
    )");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // 评分区域
    QGroupBox *ratingGroup = new QGroupBox("评分");
    ratingGroup->setStyleSheet(R"(
        QGroupBox {
            font: bold 14px 'Microsoft YaHei';
            color: #333333;
            border: 2px solid #E0E0E0;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
    )");

    QVBoxLayout *ratingLayout = new QVBoxLayout(ratingGroup);

    // 评分滑块
    QHBoxLayout *sliderLayout = new QHBoxLayout;
    QLabel *sliderLabel = new QLabel("评分:");
    sliderLabel->setStyleSheet("font: 14px 'Microsoft YaHei';");
    
    ratingSlider = new QSlider(Qt::Horizontal);
    ratingSlider->setRange(1, 5);
    ratingSlider->setValue(5);
    ratingSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            border: 1px solid #999999;
            height: 8px;
            background: #F0F0F0;
            border-radius: 4px;
        }
        QSlider::handle:horizontal {
            background: #007BFF;
            border: 1px solid #5c5c5c;
            width: 18px;
            margin: -2px 0;
            border-radius: 9px;
        }
        QSlider::sub-page:horizontal {
            background: #007BFF;
            border-radius: 4px;
        }
    )");

    ratingSpinBox = new QSpinBox;
    ratingSpinBox->setRange(1, 5);
    ratingSpinBox->setValue(5);
    ratingSpinBox->setStyleSheet(R"(
        QSpinBox {
            padding: 5px;
            border: 1px solid #CCCCCC;
            border-radius: 4px;
            font: 14px 'Microsoft YaHei';
        }
    )");

    sliderLayout->addWidget(sliderLabel);
    sliderLayout->addWidget(ratingSlider);
    sliderLayout->addWidget(ratingSpinBox);
    ratingLayout->addLayout(sliderLayout);

    // 评分显示标签
    ratingDisplayLabel = new QLabel("⭐⭐⭐⭐⭐ 优秀");
    ratingDisplayLabel->setStyleSheet(R"(
        font: 16px 'Microsoft YaHei';
        color: #FF6B35;
        padding: 10px;
        background: #FFF8F0;
        border-radius: 6px;
        margin: 5px;
    )");
    ratingDisplayLabel->setAlignment(Qt::AlignCenter);
    ratingLayout->addWidget(ratingDisplayLabel);

    mainLayout->addWidget(ratingGroup);

    // 评价内容区域
    QGroupBox *commentGroup = new QGroupBox("评价内容");
    commentGroup->setStyleSheet(R"(
        QGroupBox {
            font: bold 14px 'Microsoft YaHei';
            color: #333333;
            border: 2px solid #E0E0E0;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
    )");

    QVBoxLayout *commentLayout = new QVBoxLayout(commentGroup);

    commentEdit = new QTextEdit;
    commentEdit->setPlaceholderText("请输入您的评价内容（可选）...");
    commentEdit->setMaximumHeight(100);
    commentEdit->setStyleSheet(R"(
        QTextEdit {
            border: 1px solid #CCCCCC;
            border-radius: 4px;
            padding: 8px;
            font: 14px 'Microsoft YaHei';
        }
    )");
    commentLayout->addWidget(commentEdit);

    mainLayout->addWidget(commentGroup);

    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    cancelBtn = new QPushButton("取消");
    cancelBtn->setStyleSheet(R"(
        QPushButton {
            background: #6C757D;
            color: white;
            border-radius: 6px;
            padding: 8px 20px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #5A6268;
        }
    )");

    submitBtn = new QPushButton("提交评价");
    submitBtn->setStyleSheet(R"(
        QPushButton {
            background: #28A745;
            color: white;
            border-radius: 6px;
            padding: 8px 20px;
            font: bold 14px 'Microsoft YaHei';
        }
        QPushButton:hover {
            background: #218838;
        }
    )");

    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(submitBtn);
    mainLayout->addLayout(buttonLayout);
}

void RatingDialog::setupConnections()
{
    // 连接滑块和数字输入框
    connect(ratingSlider, &QSlider::valueChanged, ratingSpinBox, &QSpinBox::setValue);
    connect(ratingSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), ratingSlider, &QSlider::setValue);
    
    // 连接评分变化信号
    connect(ratingSlider, &QSlider::valueChanged, this, &RatingDialog::updateRatingDisplay);
    connect(ratingSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &RatingDialog::updateRatingDisplay);
    
    // 连接按钮信号
    connect(submitBtn, &QPushButton::clicked, this, &RatingDialog::validateInput);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void RatingDialog::updateRatingDisplay()
{
    int rating = ratingSlider->value();
    QString displayText;
    
    switch (rating) {
        case 5:
            displayText = "⭐⭐⭐⭐⭐ 优秀";
            break;
        case 4:
            displayText = "⭐⭐⭐⭐ 良好";
            break;
        case 3:
            displayText = "⭐⭐⭐ 一般";
            break;
        case 2:
            displayText = "⭐⭐ 较差";
            break;
        case 1:
            displayText = "⭐ 很差";
            break;
        default:
            displayText = "暂无评分";
            break;
    }
    
    ratingDisplayLabel->setText(displayText);
}

void RatingDialog::validateInput()
{
    int rating = getRating();
    if (rating < 1 || rating > 5) {
        QMessageBox::warning(this, "输入错误", "评分必须在1-5之间！");
        return;
    }
    
    accept();
}

int RatingDialog::getRating() const
{
    return ratingSpinBox->value();
}

QString RatingDialog::getComment() const
{
    return commentEdit->toPlainText().trimmed();
} 