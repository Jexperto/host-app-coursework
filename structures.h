#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QObject>
#include <QJsonObject>

struct QuestionFormItem{
    QString question;
    int numOfRows;
    int numOfCols;
    int timer;
    bool timerRunning;
    QStringList answers;
};


struct FormLoadStruct{
    QString mFormName;
   //QString mUser;
    QJsonObject mDataByID;
    QJsonObject mDataByClass;
};
#endif // STRUCTURES_H
