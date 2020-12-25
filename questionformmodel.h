#ifndef QUESTIONFORMMODEL_H
#define QUESTIONFORMMODEL_H

#include <QAbstractListModel>

class QuestionFormList;
class QuestionFormModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QuestionFormList *list READ list WRITE setList)

public:
    explicit QuestionFormModel(QObject *parent = nullptr);
    enum{
        questionRole = Qt::UserRole,
        numOfRowsRole,
        numOfColsRole,
        timerRole,
        timerRunningRole,
        answersRole
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

   // bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
     virtual QHash<int,QByteArray> roleNames()const override;
    QuestionFormList *list() const;
    void setList(QuestionFormList *formList);

private:
    QuestionFormList* mFormList;
};

#endif // QUESTIONFORMMODEL_H
