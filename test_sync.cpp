#include <QTest>
#include "error.cpp"
#include "test_sync.h"

Test_Sync::Test_Sync(QObject *parent) :
    QObject(parent)
{

}


void Test_Sync::testReadUserActions()
{

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



    QString actions[4] = { "", "2 3 1 -2 -3 -1", " \n   \n ", "2 3 i1 -2 _3 ,+1" };
    QList<int> actions_expected[4] = { {}, {2, 3, 1, -2, -3, -1}, {}, {} };
    set<Error> errors_expected[4] = { {}, {}, {expected1}, {expected2} };


    for(int tsts = 0; tsts < 4; tsts++)
    {
        QList<int> actionAfterAction;
        set<Error> errors;
        readUserActions (actions[tsts], actionAfterAction, errors);

        if(errors_expected[tsts].empty())
        {
            QCOMPARE(actionAfterAction, actions_expected[tsts]);
        }

        QCOMPARE(errors, errors_expected[tsts]);

    }

}


void Test_Sync::testReadUserAccordancesEmptyString()
{
    QString accords;
    QMap<int, QString> accordModeAction;
    set<Error> errors;

    readUserAccordances (accords, accordModeAction, errors);

    QCOMPARE(accordModeAction.isEmpty(), true);
    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testReadUserAccordancesOnlyCorrect()
{
    QString accords = "A 0\nB 2\nD 3";
    QMap<int, QString> accordModeAction;
    set<Error> errors;

    readUserAccordances (accords, accordModeAction, errors);

    QMap<int, QString> expected;
    expected[0] = 'A';
    expected[2] = 'B';
    expected[3] = 'D';

    QCOMPARE(accordModeAction, expected);
    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testReadUserAccordancesBreakMissing()
{
    QString accords = "A 0\nB 2 D 3";
    QMap<int, QString> accordModeAction;
    set<Error> errors;

    readUserAccordances (accords, accordModeAction, errors);

    QMap<int, QString> expected;
    expected[0] = 'A';
    expected[2] = 'B';

    QCOMPARE(accordModeAction, expected);
    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testReadUserAccordancesElementMissing()
{
    QString actions = "A 0\nB\n 1\nD 3";
    QMap<int, QString> accordModeAction;
    set<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    Error one, two;
    one.type = Error::noActionForMode;
    one.stringElement = "B";
    one.stringNumber = 2;
    two.type = Error::noModeForAction;
    two.stringElement = "1";
    two.stringNumber = 3;
    set<Error> expected = {one, two};

    QCOMPARE(errors, expected);
}

void Test_Sync::testReadUserAccordancesHasIncorrect()
{
    QString actions = "A 0\nB 2\n-C 1\nD i3";
    QMap<int, QString> accordModeAction;
    set<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    Error one, two;
    one.type = Error::noModeForAction;
    one.stringElement = "1";
    one.stringNumber = 3;
    two.type = Error::noActionForMode;
    two.stringElement = "D";
    two.stringNumber = 4;
    set<Error> expected = {one, two};

    QCOMPARE(errors, expected);
}

void Test_Sync::testReadUserAccordancesBothIncorrect()
{
    QString actions = "C 0\nB 2\n-A i1\n+_D 0.3";
    QMap<int, QString> accordModeAction;
    set<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    Error one, two;
    one.type = Error::noActionForMode;
    one.stringElement = "-A";
    one.stringNumber = 3;
    two.type = Error::noActionForMode;
    two.stringElement = "+_D";
    two.stringNumber = 4;
    set<Error> expected = {one, two};

    QCOMPARE(errors, expected);
}

void Test_Sync::testReadUserAccordancesBreakDoubling()
{
    QString actions = "A 0\n\nD 3";
    QMap<int, QString> accordModeAction;
    set<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    QMap<int, QString> expected;
    expected[0] = 'A';
    expected[3] = 'D';

    QCOMPARE(accordModeAction, expected);
    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testReadUserAccordancesOneInMany()
{
    QString actions = "Z 0\nA 1\nB 2\nD 3\nE 2\nF 2";
    QMap<int, QString> accordModeAction;
    set<Error> errors;

    readUserAccordances (actions, accordModeAction, errors);

    Error one;
    one.type = Error::moreThanOneModeAccordance;
    one.moreThanOneModeAction = '2';
    one.moreThanOneModeAccordanceString = {3, 5, 6};
    one.moreThanOneModeAccordanceContent = {"B", "E", "F"};
    set<Error> expected = {one};

    QCOMPARE(errors, expected);
}



void Test_Sync::testCheckAccordancesOnlyCorrect()
{
    QList<int> actionAfterAction = {1, 2, -1};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'C';
    set<Error> errors;

    checkAccordances(accordModeAction, actionAfterAction, errors);

    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testCheckAccordancesNoModeZero()
{
    QList<int> actionAfterAction = {1, 2, -1};
    QMap<int, QString> accordModeAction;
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'C';
    accordModeAction[3] = 'D';
    set<Error> errors;

    checkAccordances(accordModeAction, actionAfterAction, errors);

    Error one;
    one.type = Error::noStartingMode;
    set<Error> expected = {one};

    QCOMPARE(errors, expected);
}

void Test_Sync::testCheckAccordancesNoAccordance()
{
    QList<int> actionAfterAction = {1, 2, 3};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'C';
    set<Error> errors;

    checkAccordances(accordModeAction, actionAfterAction, errors);

    Error one;
    one.type = Error::noAccordance;
    one.positionElement = "3";
    one.positionNumber = 3;
    set<Error> expected = {one};

    QCOMPARE(errors, expected);
}

void Test_Sync::testCheckAccordancesNoAccordances()
{
    QList<int> actionAfterAction = {1, 2, 3};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    set<Error> errors;

    checkAccordances(accordModeAction, actionAfterAction, errors);

    Error one, two;
    one.type = Error::noAccordance;
    one.positionElement = "3";
    one.positionNumber = 3;
    two.type = Error::noAccordance;
    two.positionElement = "2";
    two.positionNumber = 2;
    set<Error> expected = {one, two};

    QCOMPARE(errors, expected);
}



void Test_Sync::testGenerateActionModeLists_EndNoStart()
{
    QList<int> actionAfterAction = {-1, 2, -2, -2};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'C';
    set<Error> errors;
    QList<QString> modeAfterMode;

    generateActionModeLists (actionAfterAction, accordModeAction, modeAfterMode, errors);

    Error one, two;
    one.type = Error::endWithNoStart;
    one.positionElement = "1";
    one.positionNumber = 1;
    two.type = Error::endWithNoStart;
    two.positionElement = "2";
    two.positionNumber = 4;
    set<Error> expected = {one, two};

    QCOMPARE(errors, expected);
}

void Test_Sync::testGenerateActionModeLists_NoOverlap_ReturnToStart()
{
    QList<int> actionAfterAction = {1, -1, 2, -2};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'C';
    set<Error> errors;
    QList<QString> modeAfterMode;

    generateActionModeLists (actionAfterAction, accordModeAction, modeAfterMode, errors);

    QList<QString> expected = {"A", "B", "A", "C", "A"};

    QCOMPARE(modeAfterMode, expected);
    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testGenerateActionModeLists_HasOverlap_ReturnToStart_Reversed()
{
    QList<int> actionAfterAction = {1, 2, -2, -1};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'C';
    set<Error> errors;
    QList<QString> modeAfterMode;

    generateActionModeLists (actionAfterAction, accordModeAction, modeAfterMode, errors);

    QList<QString> expected = {"A", "B", "C", "B", "A"};

    QCOMPARE(modeAfterMode, expected);
    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testGenerateActionModeLists_HasOverlap_ReturnToStart_Straight()
{
    QList<int> actionAfterAction = {1, 2, 3, -1, -2, -3};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'C';
    accordModeAction[3] = 'D';
    set<Error> errors;
    QList<QString> modeAfterMode;

    generateActionModeLists (actionAfterAction, accordModeAction, modeAfterMode, errors);

    QList<QString> expected = {"A", "B", "C", "D", "D", "D", "A"};

    QCOMPARE(modeAfterMode, expected);
    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testGenerateActionModeLists_HasOverlap_NoReturn_Random()
{
    QList<int> actionAfterAction = {1, 3, -3, 2, -1};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'C';
    accordModeAction[3] = 'D';
    set<Error> errors;
    QList<QString> modeAfterMode;

    generateActionModeLists (actionAfterAction, accordModeAction, modeAfterMode, errors);

    QList<QString> expected = {"A", "B", "D", "B", "C", "C"};

    QCOMPARE(modeAfterMode, expected);
    QCOMPARE(errors.empty(), true);
}

void Test_Sync::testGenerateActionModeLists_HasOverlap_OneModeManyActions()
{
    QList<int> actionAfterAction = {1, 3, 2, -1, -2, -3};
    QMap<int, QString> accordModeAction;
    accordModeAction[0] = 'A';
    accordModeAction[1] = 'B';
    accordModeAction[2] = 'B';
    accordModeAction[3] = 'C';
    set<Error> errors;
    QList<QString> modeAfterMode;

    generateActionModeLists (actionAfterAction, accordModeAction, modeAfterMode, errors);

    QList<QString> expected = {"A", "B", "C", "B", "B", "C", "A"};

    QCOMPARE(modeAfterMode, expected);
    QCOMPARE(errors.empty(), true);
}
