#include "EditDialog.h"
#include "globalusers.h"
#include <QMessageBox>

EditDialog::EditDialog(QWidget *parent){
    QString username=cur_user->getUsername();
    QString email=cur_user->getEmail();
    usernameEdit = new QLineEdit;
    emailEdit = new QLineEdit;
    usernameEdit->setText(username);
    emailEdit->setText(email);
    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("留空则不修改");
    passwordEdit->setEchoMode(QLineEdit::Password);

    confirmPasswordEdit = new QLineEdit;
    confirmPasswordEdit->setPlaceholderText("确认新密码");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    // 按钮组
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &EditDialog::validateInput);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &EditDialog::reject);

    // 布局
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("用户名:", usernameEdit);
    formLayout->addRow("电子邮箱:", emailEdit);
    formLayout->addRow("新密码:", passwordEdit);
    formLayout->addRow("确认密码:", confirmPasswordEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    setWindowTitle("编辑个人信息");
    setMinimumWidth(350);

    };
void EditDialog::validateInput()
{
    QString error;

    // 检查用户名
    if(usernameEdit->text().trimmed().isEmpty()){
        error = "用户名不能为空";
    }
    // 检查密码一致性
    else if(!passwordEdit->text().isEmpty() &&
             (passwordEdit->text() != confirmPasswordEdit->text())){
        error = "两次输入的密码不一致";
    }
    if(!error.isEmpty()){
        QMessageBox::warning(this, "输入错误", error);
        return;
    }
    else{
        User* a;
        a=(User*)cur_user;
        a->selectUsername(this->getUsername());
        a->selectEmail(getEmail());
        if(!passwordEdit->text().isEmpty()){
            a->selectpwd(getNewPassword());
        }
        a=NULL;
    }
    accept(); // 关闭对话框
}
QString EditDialog::getUsername() const {
    return usernameEdit->text().trimmed();
}

QString EditDialog::getEmail() const {
    return emailEdit->text().trimmed();
}

QString EditDialog::getNewPassword() const {
    return passwordEdit->text();
}
