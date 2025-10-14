#ifndef ONLY_MAP_H
#define ONLY_MAP_H

#include <QObject>
#include <QMap>

using namespace std;

class only_map
{
public:
    only_map();
    QMap<int, QString> map;
};

//Q_DECLARE_METATYPE(only_map)

#endif // ONLY_MAP_H
