#include <QTest>
#include "error.cpp"
#include "only_map.cpp"
#include "test_sync.h"

Test_Sync::Test_Sync(QObject *parent) :
    QObject(parent)
{

}


void Test_Sync::testGenerateActionModeLists_data()
{

    qRegisterMetaType<Error>();
    qRegisterMetaType<only_map>();

    QTest::addColumn<QList<int>>("actions_list");
    QTest::addColumn<only_map>("accordances_map");
    QTest::addColumn<QList<QString>>("modes_list");
    QTest::addColumn<set<Error>>("errors_expected");

    Error one, two, three;

    one.type = Error::endWithNoStart;
    one.positionElement = "1";
    one.positionNumber = 1;
    two.type = Error::endWithNoStart;
    two.positionElement = "2";
    two.positionNumber = 4;
    set<Error> expected1 = {one, two};

    set<Error> set_empty;

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

    only_map map1, map2, map3, map4, map5, map6;
    map1.map = accordModeAction1;
    map2.map = accordModeAction2;
    map3.map = accordModeAction3;
    map4.map = accordModeAction4;
    map5.map = accordModeAction5;
    map6.map = accordModeAction6;

    QList<int> actionAfterAction1 = {-1, 2, -2, -2};
    QList<int> actionAfterAction2 = {1, -1, 2, -2};
    QList<int> actionAfterAction3 = {1, 2, -2, -1};
    QList<int> actionAfterAction4 = {1, 2, 3, -1, -2, -3};
    QList<int> actionAfterAction5 = {1, 3, -3, 2, -1};
    QList<int> actionAfterAction6 = {1, 3, 2, -1, -2, -3};

    QList<QString> modes_expected_1 = {};
    QList<QString> modes_expected_2 = {"A", "B", "A", "C", "A"};
    QList<QString> modes_expected_3 = {"A", "B", "C", "B", "A"};
    QList<QString> modes_expected_4 = {"A", "B", "C", "D", "D", "D", "A"};
    QList<QString> modes_expected_5 = {"A", "B", "D", "B", "C", "C"};
    QList<QString> modes_expected_6 = {"A", "B", "C", "B", "B", "C", "A"};


    QTest::newRow("AMList_EndNoStart") << actionAfterAction1 << map1 << modes_expected_1 << expected1;
    QTest::newRow("AMList_") << actionAfterAction2 << map2 << modes_expected_2 << set_empty;
    QTest::newRow("AMList_") << actionAfterAction3 << map3 << modes_expected_3 << set_empty;
    QTest::newRow("AMList_") << actionAfterAction4 << map4 << modes_expected_4 << set_empty;
    QTest::newRow("AMList_") << actionAfterAction5 << map5 << modes_expected_5 << set_empty;
    QTest::newRow("AMList_") << actionAfterAction6 << map6 << modes_expected_6 << set_empty;

}


void Test_Sync::testGenerateActionModeLists()
{

    qRegisterMetaType<Error>();
    qRegisterMetaType<only_map>();

    QFETCH(QList<int>, actions_list);
    QFETCH(only_map, accordances_map);
    QFETCH(QList<QString>, modes_list);
    QFETCH(set<Error>, errors_expected);

    set<Error> errors;
    QList<QString> modeAfterMode;
    generateActionModeLists(actions_list, accordances_map.map, modeAfterMode, errors);

    if(errors_expected.empty())
    {
        QCOMPARE(modeAfterMode, modes_list);
    }

    QCOMPARE(errors, errors_expected);

}
