#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include "structures.h"
class DataManager
{
public:
    template<class T>
    static void write(const T* structure, QJsonObject* json);
    template<class T>
    static void read(T* structure, const QJsonObject* json);

private:
     DataManager();

};
#endif // DATAMANAGER_H
