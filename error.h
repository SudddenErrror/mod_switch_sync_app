#ifndef ERROR_H
#define ERROR_H

#include <QObject>

using namespace std;

class Error
{
public:
    Error();
    enum ErrorType {moreThanOneString, notInteger, noModeForAction, noActionForMode, noStartingMode, noAccordance,
                    endWithNoStart, moreThanOneModeAccordance, inFileNotExist, outFileCreateFail};
    ErrorType type = moreThanOneString;
    QString stringElement = "";
    int  stringNumber = 0;
    QString positionElement = "";
    int positionNumber = 0;
    QList<int>  moreThanOneModeAccordanceString = {};
    QList<QString> moreThanOneModeAccordanceContent = {};
    int  moreThanOneModeAction = 0;
    QString errorFilePass = "";

    bool operator< (const Error& e) const
    {
        return(! (type == e.type && stringNumber == e.stringNumber && positionNumber == e.positionNumber) );
    }

    bool operator== (const Error& e) const
    {
        return( (type == e.type && stringNumber == e.stringNumber && positionNumber == e.positionNumber) );
    }

    QString generateErrorMessage()
    {
        QString toReturn;

        if (Error::type == moreThanOneString)
        {
            toReturn.append("Данные о последовательности действий пользователя записываются в одной строке.");
        }

        if (Error::type == notInteger)
        {
            toReturn.append("Элемент \"");
            toReturn.append(positionElement);
            toReturn.append("\" (позиция: ");
            toReturn.append(positionNumber);
            toReturn.append(") не является целым числом.");
        }

        if (Error::type == noModeForAction)
        {
            toReturn.append("Действию \"");
            toReturn.append(stringElement);
            toReturn.append("\" (стр: ");
            toReturn.append(stringNumber);
            toReturn.append(") не соответствует режим работы программы.");
        }

        if (Error::type == noActionForMode)
        {
            toReturn.append("Режиму \"");
            toReturn.append(stringElement);
            toReturn.append("\" (стр: ");
            toReturn.append(stringNumber);
            toReturn.append(") не соответствует действие пользователя.");
        }

        if (Error::type == noStartingMode)
        {
            toReturn.append("Отсутствует начальный режим. Одному из режимов должно соответствовать действие пользователя 0.");
        }

        if (Error::type == noAccordance)
        {
            toReturn.append("Пользователь не имеет заданного режима для действия");
            toReturn.append(positionElement);
            toReturn.append("\" (позиция: ");
            toReturn.append(positionNumber);
            toReturn.append(").");
        }

        if (Error::type == endWithNoStart)
        {
            toReturn.append("Действие \"");
            toReturn.append(positionElement);
            toReturn.append("\" (позиция: ");
            toReturn.append(positionNumber);
            toReturn.append(") завершается в тот момент, когда оно ещё не было начато.");
        }

        if (Error::type == moreThanOneModeAccordance)
        {
            toReturn.append("Действию пользователя \"");
            toReturn.append(moreThanOneModeAction);
            toReturn.append("\" соответствует два режима: \"");
            toReturn.append(moreThanOneModeAccordanceContent[0]);
            toReturn.append("\" (стр: ");
            toReturn.append(moreThanOneModeAccordanceString[0]);
            toReturn.append(") и \"");
            toReturn.append(moreThanOneModeAccordanceContent[1]);
            toReturn.append("\" (стр: ");
            toReturn.append(moreThanOneModeAccordanceString[1]);
            toReturn.append(").");
        }

        if (Error::type == inFileNotExist)
        {
            toReturn.append("Неверно указан файл с входными данными. Возможно, файл не существует.");
        }

        if (Error::type == outFileCreateFail)
        {
            toReturn.append("Неверно указан файл для выходных данных. Возможно указанного расположения не существует или нет прав на запись.");
        }

        return toReturn;
    }

};

#endif // ERROR_H
