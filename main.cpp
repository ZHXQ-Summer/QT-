#include "mainwindow.h"
#include <QApplication>
#include "globalusers.h"
#include "user.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include<global_itempost.h>
#include<QFileInfo>
#include<qstandardpaths.h>
void loadUsers();
void saveUsers();
void load_item();
void save_item();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    loadUsers();
    load_item();
    MainWindow w;
    w.show();

    int result = a.exec();
    saveUsers();
    delete cur_user;  // 移至此位置
    cur_user = nullptr;
    return result;
}
