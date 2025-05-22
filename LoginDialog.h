#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    void handlelogin();
    QString username;
private:
    QLineEdit *userEdit;    // 用户名输入框
    QLineEdit *pwdEdit;     // 密码输入框
    QPushButton *submitBtn; // 提交按钮
};

#endif // LOGINDIALOG_H
