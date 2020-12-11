#ifndef QUESTIONFORMLIST_H
#define QUESTIONFORMLIST_H
#include "structures.h"
#include <QObject>
#include <QVector>
//struct QuestionFormItem{
//    QString question;
//    int numOfRows;
//    int numOfCols;
//    int timer;
//    bool timerRunning;
//    QStringList answers;
//};
class QuestionFormList : public QObject
{
    Q_OBJECT
public:
    explicit QuestionFormList(QObject *parent = nullptr);
    QVector<QuestionFormItem> items() const;
    bool setItemAt(int index, const QuestionFormItem &status);
    void appendItem(const QuestionFormItem &status);
    void removeItem(int index);
    QuestionFormItem getItem(int index) const;

signals:
    void beginInsertRow(int row);
    void beginRemoveRow(int row);
    void insertedRow();
    void removedRow();

private:
    QVector<QuestionFormItem> mItems;

};

#endif // QUESTIONFORMLIST_H
