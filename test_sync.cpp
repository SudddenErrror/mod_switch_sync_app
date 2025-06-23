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


void Test_Sync::testReadUserAccordancesEmptyString()
{
    QString accords;
    QMap<int, QString> accordModeAction;
    QSet<Error> errors;

    readUserAccordances (accords, accordModeAction, errors);

    QCOMPARE(accordModeAction.isEmpty(), true);
    QCOMPARE(errors.isEmpty(), true);
}

void Test_Sync::testReadUserAccordancesOnlyCorrect()
{
    QString accords = "A 0\nB 2\nD 3";
    QMap<int, QString> accordModeAction;
    QSet<Error> errors;

    readUserAccordances (accords, accordModeAction, errors);

    QMap<int, QString> expected;
    expected[0] = 'A';
    expected[2] = 'B';
    expected[3] = 'D';

    QCOMPARE(accordModeAction, expected);
    QCOMPARE(errors.isEmpty(), true);
}

void Test_Sync::testReadUserAccordancesBreakMissing()
{
    QString accords = "A 0\nB 2 D 3";
    QMap<int, QString> accordModeAction;
    QSet<Error> errors;

    readUserAccordances (accords, accordModeAction, errors);

    QMap<int, QString> expected;
    expected[0] = 'A';
    expected[2] = 'B';

    QCOMPARE(accordModeAction, expected);
    QCOMPARE(errors.isEmpty(), true);
}

void Test_Sync::testReadUserAccordancesElementMissing()
{
    QString actions = "A 0\nB\n 1\nD 3";
    QMap<int, QString> accordModeAction;
    QSet<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    Error one, two;
    one.type = Error::noActionForMode;
    one.stringElement = "B";
    one.stringNumber = 2;
    two.type = Error::noModeForAction;
    two.stringElement = "1";
    two.stringNumber = 3;
    QList<Error> expected = {one, two};

    QCOMPARE(errors, expected);
}

void Test_Sync::testReadUserAccordancesHasIncorrect()
{
    QString actions = "A 0\nB 2\n-C 1\nD i3";
    QMap<int, QString> accordModeAction;
    QSet<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    Error one, two;
    one.type = Error::noModeForAction;
    one.stringElement = "1";
    one.stringNumber = 3;
    two.type = Error::noActionForMode;
    two.stringElement = "D";
    two.stringNumber = 4;
    QList<Error> expected = {one, two};

    QCOMPARE(errors, expected);
}

void Test_Sync::testReadUserAccordancesBothIncorrect()
{
    QString actions = "C 0\nB 2\n-A i1\n+_D 0.3";
    QMap<int, QString> accordModeAction;
    QSet<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    Error one, two;
    one.type = Error::noActionForMode;
    one.stringElement = "-A";
    one.stringNumber = 3;
    two.type = Error::noActionForMode;
    two.stringElement = "+_D";
    two.stringNumber = 4;
    QList<Error> expected = {one, two};

    QCOMPARE(errors, expected);
}

void Test_Sync::testReadUserAccordancesBreakDoubling()
{
    QString actions = "A 0\n\nD 3";
    QMap<int, QString> accordModeAction;
    QSet<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    QMap<int, QString> expected;
    expected[0] = 'A';
    expected[3] = 'D';

    QCOMPARE(accordModeAction, expected);
    QCOMPARE(errors.isEmpty(), true);
}

void Test_Sync::testReadUserAccordancesOneInMany()
{
    QString actions = "Z 0\nA 1\nB 2\nD 3\nE 2\nF 2";
    QMap<int, QString> accordModeAction;
    QSet<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    Error one;
    one.type = Error::moreThanOneModeAccordance;
    one.moreThanOneModeAction = '2';
    one.moreThanOneModeAccordanceString = {3, 5, 6};
    one.moreThanOneModeAccordanceContent = {"B", "E", "F"};
    QList<Error> expected = {one};

    QCOMPARE(errors, expected);
}
