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


void Test_Sync::testReadUserAccordances()
{

    Error one, two, three;

    one.type = Error::noActionForMode;
    one.stringElement = "B";
    one.stringNumber = 2;
    two.type = Error::noModeForAction;
    two.stringElement = "1";
    two.stringNumber = 3;
    set<Error> expected4 = {one, two};

    one.type = Error::noModeForAction;
    one.stringElement = "1";
    one.stringNumber = 3;
    two.type = Error::noActionForMode;
    two.stringElement = "D";
    two.stringNumber = 4;
    set<Error> expected5 = {one, two};

    one.type = Error::noActionForMode;
    one.stringElement = "-A";
    one.stringNumber = 3;
    two.type = Error::noActionForMode;
    two.stringElement = "+_D";
    two.stringNumber = 4;
    set<Error> expected6 = {one, two};

    one.type = Error::moreThanOneModeAccordance;
    one.moreThanOneModeAction = '2';
    one.moreThanOneModeAccordanceString = {3, 5, 6};
    one.moreThanOneModeAccordanceContent = {"B", "E", "F"};
    set<Error> expected8 = {one};






    QMap<int, QString> map_expected1;
        map_expected1[0] = 'A';
        map_expected1[2] = 'B';
        map_expected1[3] = 'D';

    QMap<int, QString> map_expected2;
        map_expected1[0] = 'A';
        map_expected1[2] = 'B';


    QMap<int, QString> map_expected3;
        map_expected1[0] = 'A';
        map_expected1[3] = 'D';



    QString accords[8] = { "", "A 0\nB 2\nD 3", "A 0\nB 2 D 3", "A 0\nB\n 1\nD 3", "A 0\nB 2\n-C 1\nD i3", "C 0\nB 2\n-A i1\n+_D 0.3", "A 0\n\nD 3", "Z 0\nA 1\nB 2\nD 3\nE 2\nF 2" };
    QMap<int, QString> accords_expected[8] = { {}, {map_expected1}, {map_expected2}, {}, {}, {}, {map_expected3}, {} };
    set<Error> errors_expected[8] = { {}, {}, {}, {expected4}, {expected5}, {expected6}, {}, {expected8} };


    for(int tsts = 0; tsts < 8; tsts++)
    {
        QMap<int, QString> accordModeAction;
        set<Error> errors;
        readUserAccordances (accords[tsts], accordModeAction, errors);

        if(errors_expected[tsts].empty())
        {
            QCOMPARE(accordModeAction, accords_expected[tsts]);
        }

        QCOMPARE(errors, errors_expected[tsts]);

    }

}

void Test_Sync::testCheckAccordances()
{

    Error one, two, three;

    one.type = Error::noStartingMode;
    set<Error> expected2 = {one};

    one.type = Error::noAccordance;
    one.positionElement = "3";
    one.positionNumber = 3;
    set<Error> expected3 = {one};

    one.type = Error::noAccordance;
    one.positionElement = "3";
    one.positionNumber = 3;
    two.type = Error::noAccordance;
    two.positionElement = "2";
    two.positionNumber = 2;
    set<Error> expected4 = {one, two};



    QMap<int, QString> map_expected1;
    map_expected1[0] = 'A';
    map_expected1[1] = 'B';
    map_expected1[2] = 'C';

    QMap<int, QString> map_expected2;
    map_expected2[1] = 'B';
    map_expected2[2] = 'C';
    map_expected2[3] = 'D';

    QMap<int, QString> map_expected3;
    map_expected3[0] = 'A';
    map_expected3[1] = 'B';
    map_expected3[2] = 'C';

    QMap<int, QString> map_expected4;
    map_expected4[0] = 'A';
    map_expected4[1] = 'B';


    QList<int> actions[4] = { {1, 2, -1}, {1, 2, -1}, {1, 2, 3}, {1, 2, 3} };
    QMap<int, QString> accords[4] = { {map_expected1}, {map_expected2}, {map_expected3}, {map_expected4} };
    set<Error> errors_expected[4] = { {}, {expected2}, {expected3}, {expected4} };


    for(int tsts = 0; tsts < 4; tsts++)
    {

        set<Error> errors;
        checkAccordances(accords[tsts], actions[tsts], errors);

        QCOMPARE(errors, errors_expected[tsts]);

    }

}


void Test_Sync::testGenerateActionModeLists()
{

    Error one, two, three;

    one.type = Error::endWithNoStart;
    one.positionElement = "1";
    one.positionNumber = 1;
    two.type = Error::endWithNoStart;
    two.positionElement = "2";
    two.positionNumber = 4;
    set<Error> expected1 = {one, two};

    QMap<int, QString> accordModeAction1;
    accordModeAction1[0] = 'A';
    accordModeAction1[1] = 'B';
    accordModeAction1[2] = 'C';

    QMap<int, QString> accordModeAction2;
    accordModeAction2[0] = 'A';
    accordModeAction2[1] = 'B';
    accordModeAction2[2] = 'C';

    QMap<int, QString> accordModeAction3;
    accordModeAction3[0] = 'A';
    accordModeAction3[1] = 'B';
    accordModeAction3[2] = 'C';

    QMap<int, QString> accordModeAction4;
    accordModeAction4[0] = 'A';
    accordModeAction4[1] = 'B';
    accordModeAction4[2] = 'C';
    accordModeAction4[3] = 'D';

    QMap<int, QString> accordModeAction5;
    accordModeAction5[0] = 'A';
    accordModeAction5[1] = 'B';
    accordModeAction5[2] = 'C';
    accordModeAction5[3] = 'D';

    QMap<int, QString> accordModeAction6;
    accordModeAction6[0] = 'A';
    accordModeAction6[1] = 'B';
    accordModeAction6[2] = 'C';
    accordModeAction6[3] = 'D';


    QList<int> actionAfterAction[6] = { {-1, 2, -2, -2}, {1, -1, 2, -2}, {1, 2, -2, -1}, {1, 2, 3, -1, -2, -3}, {1, 3, -3, 2, -1}, {1, 3, 2, -1, -2, -3} };
    QList<QString> expected[6] = { {}, {"A", "B", "A", "C", "A"}, {"A", "B", "C", "B", "A"}, {"A", "B", "C", "D", "D", "D", "A"}, {"A", "B", "D", "B", "C", "C"}, {"A", "B", "C", "B", "B", "C", "A"} };
    QMap<int, QString> accordModeAction[6] = { accordModeAction1, accordModeAction2, accordModeAction3, accordModeAction4, accordModeAction5, accordModeAction6 };
    set<Error> errors_expected[6] = { expected1, {}, {}, {}, {}, {} };


    for(int tsts = 0; tsts < 6; tsts++)
    {

        set<Error> errors;
        QList<QString> modeAfterMode;
        generateActionModeLists(actionAfterAction[tsts], accordModeAction[tsts], modeAfterMode, errors);

        if(errors_expected[tsts].empty())
        {
            QCOMPARE(modeAfterMode, expected[tsts]);
        }

        QCOMPARE(errors, errors_expected[tsts]);

    }



}



