#ifndef EDITDIALOG_H
#define EDITDIALOG_H
#include<QDialog>
#include<QLineEdit>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QRegularExpressionValidator>

class EditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditDialog(
                        QWidget *parent = nullptr);

    // 获取编辑后的数据
    QString getUsername() const;
    QString getEmail() const;
    QString getNewPassword() const;

private slots:
    void validateInput();

private:
    void setupUI();
    void setupValidators();

    QLineEdit *usernameEdit;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QDialogButtonBox *buttonBox;
};
#endif // EDITDIALOG_H
