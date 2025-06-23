#ifndef SYNC_H
#define SYNC_H

#include "error.h"
#include "error.cpp"
#include <QObject>
#include <QSet>

int error_count = 0;

void readUserActions (QString actions, QList<int> &actionAfterAction, QSet<Error> &errors)
{
    if (actions.contains("\n"))
    {
        Error one;
        one.type = Error::moreThanOneString;
        errors.insert(one);
    }
}

void readUserAccordances (QString accords, QMap<int, QString> &accordModeAction, QSet<Error> &errors);

void checkAccordances (const QMap<int, QString> &accordModeAction, const QList<int> &actionAfterAction, QSet<Error> &errors);

void generateActionModeLists (const QList<int> &actionAfterAction, const QMap<int, QString> &accordModeAction, QList<int> &modeAfterMode, QSet<Error> &errors);

#endif // SYNC_H
