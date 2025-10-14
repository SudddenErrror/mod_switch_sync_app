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

int main(int argc, char *argv[])
{
    set<Error> errors;
    QList<int> actionAfterAction;
    QMap<int, QString> accordModeAction;
    QList<QString> modeAfterMode;

    //QCoreApplication a(argc, argv);

    // Получаем список всех переданных аргументов
    //QStringList args = QCoreApplication::arguments();

    /*string input;
    cout << " ";
    cin >> input;*/

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
            QString contentStr1(dataFromFirstFile.constData());
            dataFromFirstFile = contentStr1.replace("\r", "");
            firstInputFile.close();

            readUserActions(dataFromFirstFile, actionAfterAction, errors);

            QTextStream streamIn2(&secondInputFile);
            dataFromSecondFile = streamIn2.readAll(); // Чтение всего содержимого
            QString contentStr2(dataFromSecondFile.constData());
            dataFromSecondFile = contentStr2.replace("\r", "");
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

                secondRow.append(accordModeAction[0]);

                for(int i = 0; i < actionAfterAction.length(); i++)
                {

                    firstRow.append("      ");
                    firstRow.append(QString::number(actionAfterAction[i]));

                    secondRow.append("      ");
                    secondRow.append(modeAfterMode[i+1]);

                    // Используем поток QTextStream для удобной записи
                    QTextStream out(&outputFile);

                    out.setCodec("Windows-1251");

                    // Записываем первую строку
                    out << firstRow << endl;

                    // Записываем вторую строку
                    out << secondRow << endl;

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
        QFile outputFile_err("outError.txt");

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

    // Закрываем файл
    outputFile.close();

    //return a.exec();

    return 0;
}
