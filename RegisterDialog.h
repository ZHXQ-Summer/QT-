#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
class User;
class RegisterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    void handleRegister();
private:
    QLineEdit *userEdit;    // 用户名输入框
    QLineEdit *pwdEdit;     // 密码输入框
    QLineEdit *confirmPasswordEdit;
    QLineEdit *emailedit;
    QPushButton *submitBtn; // 提交按钮
};

#endif // REGISTERDIALOG_H
