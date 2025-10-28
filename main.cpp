#include <QCoreApplication>
#include <iostream>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QTextCodec>
#include "error.h"
#include "sync.h"

#include <QTextCodec>
#include <locale>

using namespace std;

/*!
    \brief Главная исполняемая функция
*/
int main(int argc, char *argv[])
{
    set<Error> errors;
    QList<int> actionAfterAction;
    QMap<int, QString> accordModeAction;
    QList<QString> modeAfterMode;

    if (argc < 4) {
            std::cerr << "Ошибка! Необходимо передать путь к файлам." << std::endl;
            return 1;
    }


    // Извлекаем имена файлов
    QString inputFileName1 = argv[1]; // Первый файл для чтения
    QString inputFileName2 = argv[2]; // Второй файл для чтения
    QString outputFileName = argv[3]; // Файл для записи

    // Читаем содержимое двух входных файлов
    QString dataFromFirstFile;
    QString dataFromSecondFile;

    QFile firstInputFile(inputFileName1);
    QFile secondInputFile(inputFileName2);
    QFile outputFile(outputFileName);



    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {

        if (firstInputFile.open(QIODevice::ReadOnly | QIODevice::Text) && secondInputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {

            QTextStream streamIn1(&firstInputFile);
            dataFromFirstFile = streamIn1.readAll(); // Чтение всего содержимого
            firstInputFile.close();

            readUserActions(dataFromFirstFile, actionAfterAction, errors);

            QTextStream streamIn2(&secondInputFile);
            dataFromSecondFile = streamIn2.readAll(); // Чтение всего содержимого
            secondInputFile.close();

            readUserAccordances(dataFromSecondFile, accordModeAction, errors);

        }

        else
        {

            Error a;
            a.type = Error::inFileNotExist;
            errors.insert(a);

        }

    }

    else

    {

        Error a;
        a.type = Error::outFileCreateFail;
        errors.insert(a);

    }

    if(errors.empty())
    {

        checkAccordances(accordModeAction, actionAfterAction, errors);

        if(errors.empty())
        {

            generateActionModeLists(actionAfterAction, accordModeAction, modeAfterMode, errors);

            if(errors.empty())
            {

                QString firstRow;
                QString secondRow;

                secondRow.append(modeAfterMode[0]);

                for(int i = 0; i < actionAfterAction.length(); i++)
                {
                    int string_length = 0;

                    for(int j = 0; j < modeAfterMode[i].length(); j++)
                    {
                        string_length++;
                        QString characterInLowerCase = modeAfterMode[i][j].toLower();
                        if (modeAfterMode[i][j] != characterInLowerCase)
                            string_length++;
                        if (characterInLowerCase != "i" && characterInLowerCase != "j")
                            string_length++;
                        if (characterInLowerCase == "w")
                            string_length++;
                    }

                    while (string_length > 0)
                    {
                        firstRow.append("\t");
                        secondRow.append("\t");
                        string_length -= 5;
                    }
                    firstRow.append(QString::number(actionAfterAction[i]));

                    int integer_length = actionAfterAction[i] < 0;
                    int number_abs = abs(actionAfterAction[i]);
                    do {
                        number_abs /= 10;
                        integer_length++;
                    } while (number_abs > 0);

                    while (integer_length > 0)
                    {
                        integer_length -= 4;
                        secondRow.append("\t");
                    }

                    firstRow.append("\t");

                    secondRow.append(modeAfterMode[i+1]);

                }

                // Используем поток QTextStream для удобной записи
                QTextStream out(&outputFile);

                out.setCodec("Windows-1251");

                // Записываем первую строку
                out << firstRow << endl;

                // Записываем вторую строку
                out << secondRow << endl;

            }

            else
            {

                    QTextStream out(&outputFile);

                    out.setCodec("Windows-1251");

                    foreach(const Error& er, errors)
                    {
                        QString toOut = er.generateErrorMessage();
                        out << toOut << endl;
                    }
            }

        }

        else
        {

                QTextStream out(&outputFile);

                out.setCodec("Windows-1251");

                foreach(const Error& er, errors)
                {
                    QString toOut = er.generateErrorMessage();
                    out << toOut << endl;
                }
        }

    }

    else
    {
        bool fileNotOpenedError = false;

        for (auto i = errors.begin(), end = errors.end(); i != end; ++i)
        {
            Error error = *i;
            if(error.type == Error::inFileNotExist || error.type == Error::outFileCreateFail)
            {
                fileNotOpenedError = true;
            }
        }

        if (fileNotOpenedError)
        {

            QFile outputFile_err("failedToOpenFile.txt");

            if (outputFile_err.open(QIODevice::WriteOnly | QIODevice::Text))
            {

                QTextStream out(&outputFile_err);

                //setlocale(LC_ALL, "Russian");

                foreach(const Error& er, errors)
                {
                    QString toOut = er.generateErrorMessage();
                    out << toOut << endl;
                }

            }

        }

        else
        {
            QTextStream out(&outputFile);

            out.setCodec("Windows-1251");

            foreach(const Error& er, errors)
            {
                QString toOut = er.generateErrorMessage();
                out << toOut << endl;
            }
        }
    }

    // Закрываем файл
    outputFile.close();

    return 0;
}
