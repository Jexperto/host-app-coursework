#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QObject>
struct QuestionFormItem{
    QString question;
    int numOfRows;
    int numOfCols;
    int timer;
    bool timerRunning;
    QStringList answers;
};
#endif // STRUCTURES_H
