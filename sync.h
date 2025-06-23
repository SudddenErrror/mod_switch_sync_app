#ifndef SYNC_H
#define SYNC_H

#include "error.h"
#include "error.cpp"
#include <QObject>
#include <QSet>
#include <QList>

int error_count = 0;


bool ifIntegerInRange (QString element, int min, int max);


void readUserActions (QString actions, QList<int> &actionAfterAction, QSet<Error> &errors)
{
    if (actions.contains("\n"))
    {
        Error a;
        a.type = Error::moreThanOneString;
        errors.insert(a);
    }
    else
    {
        char delimiter = ' ';

        // Строка ввода разбивается по пробелам
        QStringList elements = actions.split(delimiter);

        int position = 1;

        for(const QString& element : elements)
        {
            if (ifIntegerInRange(element, -2147483647, 2147483647))
            {
                actionAfterAction.append(element.toInt());
            }
            else
            {
                Error a;
                a.type = Error::notInteger;
                a.positionElement = element;
                a.positionNumber = position;
                errors.insert(a);
            }
            position++;
        }

    }
}

void readUserAccordances (QString accords, QMap<int, QString> &accordModeAction, QSet<Error> &errors);

void checkAccordances (const QMap<int, QString> &accordModeAction, const QList<int> &actionAfterAction, QSet<Error> &errors);

void generateActionModeLists (const QList<int> &actionAfterAction, const QMap<int, QString> &accordModeAction, QList<int> &modeAfterMode, QSet<Error> &errors);

#endif // SYNC_H
