#ifndef FORMOBJECT_H
#define FORMOBJECT_H
#include <QObject>
#include "structures.h"
class QFile;
class FormList : public QObject
{
    Q_OBJECT
public:
    explicit FormList(QObject *parent = nullptr);
    void setObjectsList(const QVector<QuestionFormItem>& objects);
    void write(QJsonArray&json) const;
    void read(const QJsonArray& json);
    FormList(const QVector<QuestionFormItem>& list, QObject *parent = nullptr);
    QVector<QuestionFormItem> objectsList() const;
private:
    QVector<QuestionFormItem> mObjectsList;
};

#endif // FORMOBJECT_H
