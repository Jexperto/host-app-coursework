#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QJsonObject>
struct FormLoadStruct;
class DataManager
{
    enum EventTypes{
        formLoad,
        clientReply
    };
public:
    template<typename T>
    void write(DataManager::EventTypes eventType, T structure, QJsonObject* json);
    void writeFormLoad(FormLoadStruct structure, QJsonObject *json) const;
private:
     DataManager();
};
#endif // DATAMANAGER_H
