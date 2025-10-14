#include <QTest>
#include "error.cpp"
#include "test_sync.h"

Test_Sync::Test_Sync(QObject *parent) :
    QObject(parent)
{

}


void Test_Sync::testReadUserActions_data()
{

    qRegisterMetaType<Error>();
    qRegisterMetaType<set<Error>>();

    QTest::addColumn<QString>("actions_string");
    QTest::addColumn<QList<int>>("actions_expected");
    QTest::addColumn<set<Error>>("errors_expected");

    QList<int> list_empty = {};
    set<Error> set_empty = {};

    QList<int> list_only_ints = {2, 3, 1, -2, -3, -1};

    Error one, two, three;

    one.type = Error::moreThanOneString;
    set<Error> expected1 = {one};

    one.type = Error::notInteger;
    one.positionElement = "i1";
    one.positionNumber = 3;
    two.type = Error::notInteger;
    two.positionElement = "_3";
    two.positionNumber = 5;
    three.type = Error::notInteger;
    three.positionElement = ",+1";
    three.positionNumber = 6;
    set<Error> expected2 = {one, two, three};

    QTest::newRow("string_empty") << "" << list_empty << set_empty;
    QTest::newRow("string_only_ints") << "2 3 1 -2 -3 -1" << list_only_ints << set_empty;
    QTest::newRow("string_has_breaks") << " \n   \n " << list_empty << expected1;
    QTest::newRow("string_has_non_ints") << "2 3 i1 -2 _3 ,+1" << list_empty << expected2;

}

void Test_Sync::testReadUserActions()
{
    qRegisterMetaType<Error>();
    qRegisterMetaType<set<Error>>();

    QFETCH(QString, actions_string);
    QFETCH(QList<int>, actions_expected);
    QFETCH(set<Error>, errors_expected);

    QList<int> actionAfterAction;
    set<Error> errors;
    readUserActions (actions_string, actionAfterAction, errors);

    if(errors_expected.empty())
    {
        QVERIFY2(actionAfterAction == actions_expected, "actionAfterAction checking failed");
    }

    QVERIFY2(errors == errors_expected, "errors checking failed");
}












