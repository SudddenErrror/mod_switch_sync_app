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

};

#endif // ERROR_H
