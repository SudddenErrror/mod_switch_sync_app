#include <QCoreApplication>
#include <iostream>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include "error.h"
#include "sync.h"

using namespace std;

int main(int argc, char *argv[])
{
    set<Error> errors;

    QCoreApplication a(argc, argv);

    // Получаем список всех переданных аргументов
    QStringList args = QCoreApplication::arguments();

    string input;
    cout << " ";
    cin >> input;

    if (args.size() < 4) {
            std::cerr << "Ошибка! Необходимо передать путь к файлам." << std::endl;
            return 1;
    }

    // Извлекаем имена файлов
    QString inputFileName1 = args.at(1); // Первый файл для чтения
    QString inputFileName2 = args.at(2); // Второй файл для чтения
    QString outputFileName = args.at(3); // Файл для записи

    // Читаем содержимое двух входных файлов
    QString dataFromFirstFile;
    QString dataFromSecondFile;

    QFile firstInputFile(inputFileName1);
    QFile secondInputFile(inputFileName2);
    QFile outputFile(outputFileName);

    if (firstInputFile.open(QIODevice::ReadOnly | QIODevice::Text) && secondInputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {

            QTextStream streamIn1(&firstInputFile);
            dataFromFirstFile = streamIn1.readAll(); // Чтение всего содержимого
            QString contentStr1(dataFromFirstFile.constData());
            dataFromFirstFile = contentStr1.replace("\n", "\\n").replace("\r", "");
            firstInputFile.close();

            QTextStream streamIn2(&secondInputFile);
            dataFromSecondFile = streamIn2.readAll(); // Чтение всего содержимого
            QString contentStr2(dataFromSecondFile.constData());
            dataFromFirstFile = contentStr2.replace("\n", "\\n").replace("\r", "");
            secondInputFile.close();

        }

        else
        {

            Error a;
            a.type = Error::outFileCreateFail;
            errors.insert(a);

        }

    }

    else

    {

        Error a;
        a.type = Error::inFileNotExist;
        errors.insert(a);

    }

    return a.exec();

    return 0;
}
