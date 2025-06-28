#ifndef RATINGDIALOG_H
#define RATINGDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSlider>

class RatingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RatingDialog(const QString& targetUsername, QWidget *parent = nullptr);
    
    int getRating() const;
    QString getComment() const;

private slots:
    void updateRatingDisplay();
    void validateInput();

private:
    void setupUI();
    void setupConnections();

    QString targetUsername;
    QSpinBox *ratingSpinBox;
    QSlider *ratingSlider;
    QLabel *ratingDisplayLabel;
    QTextEdit *commentEdit;
    QPushButton *submitBtn;
    QPushButton *cancelBtn;
};

#endif // RATINGDIALOG_H 