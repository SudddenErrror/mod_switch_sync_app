#include "sync.h"

/*!
    \brief Реализация функций, объявленных в файле sync.h
*/
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

        if (elements.last() == "")
            elements.removeLast();

        if(elements[0] != "")
        {
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
}

void readUserAccordances (QString accords, QMap<int, QString> &accordModeAction, set<Error> &errors)
{
    char delimiter = '\n';

    // Строка ввода разбивается по переносам на одинарные строки
    QStringList elements = accords.split(delimiter);

    int string = 1;

    // Для каждой строки
    for(const QString& element : elements)
    {
        char delimiter_1 = ' ';

        // Строка разбивается по переносам на отдельные элементы
        QStringList emts = element.split(delimiter_1);

        {
            int pos = 1;

            //ищем первые два элемента
            QString emt_1_type = "none", emt_2_type = "none", emt_1, emt_2;

            //обход всех элементов
            for(const QString& emt : emts)
            {
                // если есть первый элемент, определяем его тип
                if (pos == 1)
                {
                    emt_1_type = whatIsElement(emt, pos);
                    emt_1 = emt;
                }

                // если есть второй элемент, определяем его тип
                if (pos == 2)
                {
                    emt_2_type = whatIsElement(emt, pos);
                    emt_2 = emt;
                }

                pos++;
            }

            // если первый элемент является действием
            if (emt_1_type == "int")
            {
                // ошибка: нет режима для действия
                Error a;
                a.type = Error::noModeForAction;
                a.stringElement = QString(emt_1);
                a.stringNumber = string;
                errors.insert(a);
            }

            // если первый элемент не является ни действием, ни режимом
            else if (emt_1_type == "none")
            {
                // если второй элемент является действием
                if (emt_2_type == "int")
                {
                    // ошибка: нет режима для действия
                    Error a;
                    a.type = Error::noModeForAction;
                    a.stringElement = QString(emt_2);
                    a.stringNumber = string;
                    errors.insert(a);
                }

                // если второй элемент не является действием и первый элемент не пустой
                else if (emt_1 != "")
                {
                    // ошибка: нет действия для режима
                    Error a;
                    a.type = Error::noActionForMode;
                    a.stringElement = QString(emt_1);
                    a.stringNumber = string;
                    errors.insert(a);
                }
            }

            // если первый элемент является режимом, а второй элемент не является действием
            else if (emt_2_type.isNull() || emt_2_type == "none")
            {
                // ошибка: нет действия для режима
                Error a;
                a.type = Error::noActionForMode;
                a.stringElement = QString(emt_1);
                a.stringNumber = string;
                errors.insert(a);
            }

            // если первый элемент является режимом, а второй элемент является действием
            else
            {
                // проверяем, занесено ли данное действие в словарь
                int action = abs(emt_2.toInt());

                // если ещё не занесено, заносим вместе с режимом
                if (accordModeAction[action].isNull())
                {
                    accordModeAction[action] = emt_1;
                }

                // если уже занесено, описываем ошибку
                else
                {
                    // проверяем, описана ли уже данная ошибка
                    bool currentActionNotDescribed = true;

                    // перебираем список ошибок
                    for (auto i = errors.begin(), end = errors.end(); i != end; ++i)
                    {
                        Error error = *i;
                        // если ошибка уже описана, отмечаем это
                        if(error.type == Error::moreThanOneModeAccordance && error.moreThanOneModeAction == action)
                        {
                            currentActionNotDescribed = false;
                        }
                    }

                    // если ошибка ещё не описана
                    if(currentActionNotDescribed)
                    {
                        // новая ошибка
                        Error a;

                        // тип - более одного соответствия
                        a.type = Error::moreThanOneModeAccordance;
                        a.moreThanOneModeAction = action;

                        int error_string = 0;

                        // Для каждой строки
                        for (const QString& emt : elements)
                        {
                            // номер строки
                            error_string++;

                            // делим строку на элементы
                            QStringList emts = emt.split(' ');

                            // если в строке хотя бы два элемента и второй элемент является искомым действием
                            if(emts.length() > 1 && emts[0] != "" && emts[1].toInt() == action)
                            {
                                // добавляем режим и номер страницы в описание ошибки
                                a.moreThanOneModeAccordanceContent.append(emts[0]);
                                a.moreThanOneModeAccordanceString.append(error_string);
                            }
                        }

                        // добавляем ошибку в список ошибок
                        errors.insert(a);
                    }
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
            a.positionNumber = i+1;
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
                a.positionNumber = i+1;
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
