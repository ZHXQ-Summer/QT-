#include "RegisterDialog.h"
#include "globalusers.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("用户注册");
    resize(250, 150);

    userEdit = new QLineEdit(this);
    pwdEdit = new QLineEdit(this);
    pwdEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit = new QLineEdit(this);
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    emailedit=new QLineEdit(this);
    submitBtn = new QPushButton("注册", this);

    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("账号：", userEdit);
    formLayout->addRow("密码：", pwdEdit);
    formLayout->addRow("确认密码：", confirmPasswordEdit);
    formLayout->addRow("邮箱：", emailedit);
    formLayout->addRow(submitBtn);

    connect(submitBtn, &QPushButton::clicked, this, [this]() {
        QString username = userEdit->text();
        QString password = pwdEdit->text();
        QString confirmPassword = confirmPasswordEdit->text();
        QString email = emailedit->text();
        if (password != confirmPassword) {
            QMessageBox::warning(this, "注册失败", "两次输入的密码不一致");
            return;
        }

        for (const auto& user : users) {
            if (user.getUsername() == username) {
                QMessageBox::warning(this, "注册失败", "该用户名已被使用");
                return;
            }
        }

        users.emplace_back(username, password,email);
        QMessageBox::information(this, "注册成功", "注册成功，请登录");
        accept();
    });
}
