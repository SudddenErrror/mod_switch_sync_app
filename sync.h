#ifndef SYNC_H
#define SYNC_H

#include "error.h"
#include <set>
#include <QList>
#include <QMap>

bool ifIntegerInRange (QString element, int min, int max);

QString whatIsElement(QString element, int el_num);;

bool errorDescribed (set<Error> &errors, int action, Error::ErrorType type);


void readUserActions (QString actions, QList<int> &actionAfterAction, set<Error> &errors);

void readUserAccordances (QString accords, QMap<int, QString> &accordModeAction, set<Error> &errors);

void checkAccordances (const QMap<int, QString> &accordModeAction, const QList<int> &actionAfterAction, set<Error> &errors);

void generateActionModeLists (const QList<int> &actionAfterAction, const QMap<int, QString> &accordModeAction, QList<QString> &modeAfterMode, set<Error> &errors);


#endif // SYNC_H
