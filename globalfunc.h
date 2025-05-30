#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H
#include <itempost.h>
#include <vector>


std::vector<std::vector<ItemPost>::iterator> findPost(QString title="",
                                                      double price=-1,
                                                      QString owner="",
                                                      QDateTime afterpostTime=QDateTime::fromString("1970-01-01 00:00:00", Qt::ISODate),
                                                      std::vector<QString> flags=std::vector<QString>());
#endif // GLOBALFUNC_H
