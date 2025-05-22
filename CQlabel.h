#ifndef CQLABEL_H
#define CQLABEL_H


#include <QLabel>
#include <QWidget>
#include <Qt>
#include <QMouseEvent>
#include <QDebug>
class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget* parent = Q_NULLPTR) : QLabel(parent) {}
    ClickedLabel(QString a,QWidget* parent = Q_NULLPTR) : QLabel(a,parent) {}

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent* ev)
    {
        if (ev != nullptr && ev->button() == Qt::LeftButton)
        {
            emit clicked();	//emit signal
        }
    }
    void enterEvent(QEvent*)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    void leaveEvent(QEvent*)
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }

};


#endif // CQLABEL_H
