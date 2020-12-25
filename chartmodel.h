#ifndef CHARTMODEL_H
#define CHARTMODEL_H

#include <QAbstractListModel>

class ChartModel : public QAbstractListModel
{
    Q_OBJECT
     Q_PROPERTY(QList<ResultUnit> items READ getItems)

public:
    struct ResultUnit{
        QString name;
        double result;
    };
    explicit ChartModel(QObject *parent = nullptr);
    enum{
        nameRole = Qt::UserRole,
        resultRole,
        countRole
    };
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    void append(QString name, double result);
    void clear();
    virtual QHash<int,QByteArray> roleNames()const override;

    QList<ResultUnit> getItems();

private:
    QList<ResultUnit> items;
};

#endif // CHARTMODEL_H
