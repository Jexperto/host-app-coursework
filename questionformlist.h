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
    bool setItemAt(int index, const QuestionFormItem &item);
    void appendItem(QuestionFormItem item);
    void removeItem(int index);
    int convertItemToEventDataJson(int index, QJsonObject& data);
    QuestionFormItem getItem(int index) const;
    void clear();   
    int size();
    QuestionFormList(int shuffleSeed, QObject *parent);
    int getShuffleSeed() const;
    void setShuffleSeed(int value);

signals:
    void beginInsertRow(int row);
    void beginRemoveRow(int row);
    void insertedRow();
    void removedRow();

private:
    QVector<QuestionFormItem> mItems;
    int shuffleSeed;
};

#endif // QUESTIONFORMLIST_H
