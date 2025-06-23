#include <QTest>
#include "test_sync.h"
#include "sync.h"

Test_Sync::Test_Sync(QObject *parent) :
    QObject(parent)
{

}


void Test_Sync::testReadUserActionsEmptyString()
{
    QString actions;
    QList<int> actionAfterAction;
    QSet<Error> errors;

    readUserActions (actions, actionAfterAction, errors);

    QCOMPARE(actionAfterAction.isEmpty(), true);
    QCOMPARE(errors.isEmpty(), true);
}

void Test_Sync::testReadUserActionsOnlyInts()
{
    QString actions = "2 3 1 -2 -3 -1";
    QList<int> actionAfterAction;
    QSet<Error> errors;


    readUserActions (actions, actionAfterAction, errors);

    QList<int> expected = {2, 3, 1, -2, -3, -1};

    QCOMPARE(actionAfterAction, expected);
    QCOMPARE(errors.isEmpty(), true);
}

void Test_Sync::testReadUserActionsHasBreaks()
{
    QString actions = " \n   \n ";
    QList<int> actionAfterAction;
    QSet<Error> errors;

    readUserActions (actions, actionAfterAction, errors);

    Error one;
    one.type = Error::moreThanOneString;
    QList<Error> expected = {one};

    QCOMPARE(errors, expected);
}

void Test_Sync::testReadUserActionsHasNonInts()
{
    QString actions = "2 3 i1 -2 _3 ,+1";
    QList<int> actionAfterAction;
    QSet<Error> errors;

    readUserActions (actions, actionAfterAction, errors);

    Error one, two, three;
    one.type = Error::notInteger;
    one.positionElement = "i1";
    one.positionNumber = 3;
    two.type = Error::notInteger;
    two.positionElement = "_3";
    two.positionNumber = 5;
    three.type = Error::notInteger;
    three.positionElement = ",+1";
    three.positionNumber = 6;
    QList<Error> expected = {one, two, three};

    QCOMPARE(errors, expected);
}
