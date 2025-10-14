#include "sync.h"

bool ifIntegerInRange (QString element, int min, int max){
    bool ok;
    int num = element.toInt(&ok);

    if(!ok)
        return false;

    if(num < min || num > max)
        return false;

    return true;
}

QString whatIsElement(QString element, int el_num)
{
    if (element == "") return "none";

    if (el_num == 1)
    {

        bool ok;
        int num = element.toInt(&ok);

        if(ok)
            return "int";

        for (int i = 0; i < element.length(); i++)
        {
            if (element[i] != "_" && !element[i].isLetter())
            {
                return "none";
            }
        }
        return "string";
    }

    else if (el_num == 2)
    {
        bool ok;
        int num = element.toInt(&ok);

        if(ok)
            return "int";
    }

    return "none";
}

bool errorDescribed (set<Error> &errors, int action, Error::ErrorType type)
{
    foreach(const Error& er, errors)
    {
        if (er.moreThanOneModeAction == action && er.type == type)
        {
            return true;
        }
    }
    return false;
}

void readUserActions (QString actions, QList<int> &actionAfterAction, set<Error> &errors)
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

        if(elements[0] != "")
        {
            int position = 0;

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
}

void readUserAccordances (QString accords, QMap<int, QString> &accordModeAction, set<Error> &errors)
{
    char delimiter = '\n';

    // Строка ввода разбивается по переносам
    QStringList elements = accords.split(delimiter);

    int string = 1;

    for(const QString& element : elements)
    {
        char delimiter_1 = ' ';
        QStringList emts = element.split(delimiter_1);

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
                a.stringElement = QString(emt_1);
                a.stringNumber = string;
                errors.insert(a);
            }
            else if (emt_1_type == "none")
            {
                if (emt_2_type == "int")
                {
                    Error a;
                    a.type = Error::noModeForAction;
                    a.stringElement = QString(emt_2);
                    a.stringNumber = string;
                    errors.insert(a);
                }
            }
            else if (emt_2_type.isNull() || emt_2_type == "none")
            {
                Error a;
                a.type = Error::noActionForMode;
                a.stringElement = QString(emt_1);
                a.stringNumber = string;
                errors.insert(a);
            }
            else
            {
                int i = abs(emt_2.toInt());
                if (accordModeAction[i].isNull())
                {
                    accordModeAction[i] = emt_1;
                }
                else
                {
                    QString described = accordModeAction[i];
                    Error a;
                    a.type = Error::moreThanOneModeAccordance;
                    a.moreThanOneModeAccordanceContent.append(described);
                    a.moreThanOneModeAccordanceString.append(string);
                    a.moreThanOneModeAccordanceContent.append(QString(emt_1));
                    a.moreThanOneModeAccordanceString.append(string);
                    a.moreThanOneModeAction = i;
                    errors.insert(a);
                }
            }
        }

        string++;
    }
}

void checkAccordances (const QMap<int, QString> &accordModeAction, const QList<int> &actionAfterAction, set<Error> &errors)
{
    if (accordModeAction[0].isNull())
    {
        Error a;
        a.type = Error::noStartingMode;
        errors.insert(a);
    }
    for (int i = 0; i < actionAfterAction.length(); i++)
    {
        int jabs = abs(actionAfterAction[i]);
        if (accordModeAction[jabs].isNull())
        {
            Error a;
            a.type = Error::noAccordance;
            a.positionElement = QString::number(jabs);
            a.positionNumber = i;
            errors.insert(a);
        }
    }
}

void generateActionModeLists (const QList<int> &actionAfterAction, const QMap<int, QString> &accordModeAction, QList<QString> &modeAfterMode, set<Error> &errors)
{
    QString start = accordModeAction[0];
    modeAfterMode.append(start);

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
            j *= -1;
            for (int k = unfinished.length() - 1; k > -1; k--)
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
                a.positionElement = QString::number(j);
                a.positionNumber = i;
                errors.insert(a);
            }
        }
        int key;
        if (unfinished.isEmpty())
            key = 0;
        else key = unfinished.last();
        QString current_mode = accordModeAction[key];
        modeAfterMode.append(current_mode);
    }
}
