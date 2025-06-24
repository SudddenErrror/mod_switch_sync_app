#ifndef SYNC_H
#define SYNC_H

#include "error.h"
#include "error.cpp"
#include <QObject>
#include <QSet>
#include <QList>
#include <QMap>

int error_count = 0;


bool ifIntegerInRange (QString element, int min, int max);

QString whatIsElement(QString element, int el_num);

bool errorDescribed (QSet<Error> &errors, int action, Error::ErrorType type);


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

void readUserAccordances (QString accords, QMap<int, QString> &accordModeAction, QSet<Error> &errors)
{
    char delimiter = '\n';

    // Строка ввода разбивается по пробелам
    QStringList elements = accords.split(delimiter);

    int string = 1;

    for(const QString& element : elements)
    {
        char delimiter_1 = ' ';
        QStringList emts = element.split(delimiter_1);
        if (!emts.isEmpty())
        {
            int pos = 1;
            QString emt_1_type, emt_2_type, emt_1, emt_2;
            for(const QString& emt : emts)
            {
                if (pos == 1)
                {
                    emt_1_type = whatIsElement(emt, pos);
                    emt_1 = emt;
                }
                if (pos == 2)
                {
                    emt_2_type = whatIsElement(emt, pos);
                    emt_2 = emt;
                }
                pos++;
            }

            if (emt_1_type == "int")
            {
                Error a;
                a.type = Error::noModeForAction;
                a.stringElement = emt_1;
                a.stringNumber = string;
                errors.insert(a);
            }
            else if (emt_1_type == "none")
            {
                if (emt_2_type == "int")
                {
                    Error a;
                    a.type = Error::noModeForAction;
                    a.stringElement = emt_2;
                    a.stringNumber = string;
                    errors.insert(a);
                }
            }
            else if (emt_2_type.isNull() || emt_2_type == "none")
            {
                Error a;
                a.type = Error::noActionForMode;
                a.stringElement = emt_1;
                a.stringNumber = string;
                errors.insert(a);
            }
            else
            {
                int i = emt_2.toInt();
                if (accordModeAction[i].isNull())
                {
                    accordModeAction[i] = emt_1;
                }
                else
                {
                    QString described = accordModeAction[i];
                    Error a;
                    a.type = Error::moreThanOneModeAccordance;
                    a.moreThanOneModeAccordanceString.append(string);
                    a.moreThanOneModeAccordanceContent.append(described);
                    a.moreThanOneModeAccordanceContent.append(emt_1);
                    a.moreThanOneModeAction = i;
                    errors.insert(a);
                }
            }
        }
    }
}

void checkAccordances (const QMap<int, QString> &accordModeAction, const QList<int> &actionAfterAction, QSet<Error> &errors)
{
    if (accordModeAction[0].isNull())
    {
        Error a;
        a.type = Error::noStartingMode;
        errors.insert(a);
    }
    for (int i = 0; i < actionAfterAction.length(); i++)
    {
        int j = actionAfterAction[i];
        if (accordModeAction[j].isNull())
        {
            Error a;
            a.type = Error::noAccordance;
            a.positionElement = j;
            a.positionNumber = i;
            errors.insert(a);
        }
    }
}

void generateActionModeLists (const QList<int> &actionAfterAction, const QMap<int, QString> &accordModeAction, QList<int> &modeAfterMode, QSet<Error> &errors)
{
    QList<int> unfinished;

    for (int i = 0; i < actionAfterAction.length(); i++)
    {
        int j = actionAfterAction[i];
        if (j > 0)
        {
            unfinished.append(j);
        }
        else if (j < 0)
        {
            int found = -1;
            for (int k = unfinished.length() - 1; k > -1; k++)
            {
                if (unfinished[k] == j)
                {
                    found = k;
                }
            }
            if (found > -1)
            {
                unfinished.removeAt(found);
            }
            else
            {
                Error a;
                a.type = Error::endWithNoStart;
                a.positionElement = j;
                a.positionNumber = i;
                errors.insert(a);
            }
        }
    }
}

#endif // SYNC_H
