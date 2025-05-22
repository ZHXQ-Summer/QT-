#include "LoginDialog.h"
#include "globalusers.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("用户登录");
    resize(250, 150);

    userEdit = new QLineEdit(this);
    pwdEdit = new QLineEdit(this);
    pwdEdit->setEchoMode(QLineEdit::Password);
    submitBtn = new QPushButton("登录", this);

    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow("账号：", userEdit);
    formLayout->addRow("密码：", pwdEdit);
    formLayout->addRow(submitBtn);

    connect(submitBtn, &QPushButton::clicked, this, [this]() {
        QString username = userEdit->text();
        QString password = pwdEdit->text();

        for (const auto& user : users) {
            if (user.getUsername() == username && user.checkPassword(password)) {
                QMessageBox::information(this, "登录成功", "欢迎登录");
                username=user.getUsername();
                cur_user=&user;
                accept();
                return;
            }
        }

        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    });
}
