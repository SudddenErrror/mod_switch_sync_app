#ifndef ERROR_H
#define ERROR_H

#include <QObject>

using namespace std;

/*!
    \brief Класс для фиксирования ошибок

    Данный класс предназначен для фиксирования ошибок, происходящих в ходе работы программы, и генерации сообщений о зафиксированных ошибках в дальнейшем
*/
class Error
{
public:
    Error();
    /*!
        \brief Перечисление типов ошибок
    */
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

    QString generateErrorMessage() const
    {
        if (type == moreThanOneString)
        {
            return("User's actions must be written in a single string");
        }

        if (type == notInteger)
        {
            return("Element \"" + positionElement + "\" (position: " + QString::number(positionNumber) + ") is not a whole number.");
        }

        if (type == noModeForAction)
        {
            return("No mode for action \"" + stringElement + "\" (str: " + QString::number(stringNumber) + ").");
        }

        if (type == noActionForMode)
        {
            return("No action for mode \"" + stringElement + "\" (str: " + QString::number(stringNumber) + ").");
        }

        if (type == noStartingMode)
        {
            return("No starting mode. One of the modes must correspond to action 0.");
        }

        if (type == noAccordance)
        {
            return("User has no given mode for action \"" + QString(positionElement) + "\" (position: "
            + QString::number(positionNumber) + ").");
        }

        if (type == endWithNoStart)
        {
            return("Action \"" + positionElement + "\" (position: " + QString::number(positionNumber) + ") ends without having been started.");
        }

        if (type == moreThanOneModeAccordance)
        {
            return ("Mode \"" + moreThanOneModeAccordanceContent[0] + "\" is initial for action \"" + QString::number(moreThanOneModeAction) + "\". You can not correspond the said action with \""
            + moreThanOneModeAccordanceContent[1] + "\" (str: " + QString::number(moreThanOneModeAccordanceString[1]) + ").");
        }

        if (type == inFileNotExist)
        {
            return ("The input data file is specified incorrectly. Perhaps, the file does not exist.");
        }

        if (type == outFileCreateFail)
        {
            return ("The input data file is specified incorrectly. Perhaps, the specified location does not exist or there are no write permissions.");
        }

        return "";
    }

};

//Q_DECLARE_METATYPE(Error)
//Q_DECLARE_METATYPE(QSet<Error>)

#endif // ERROR_H
