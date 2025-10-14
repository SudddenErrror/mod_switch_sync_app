#include <QTest>
#include "error.cpp"
#include "only_map.cpp"
#include "test_sync.h"

Test_Sync::Test_Sync(QObject *parent) :
    QObject(parent)
{

}


void Test_Sync::testCheckAccordances_data()
{

    qRegisterMetaType<Error>();
    qRegisterMetaType<set<Error>>();
    qRegisterMetaType<only_map>();

    QTest::addColumn<QList<int>>("actions_list");
    QTest::addColumn<only_map>("accordances_map");
    QTest::addColumn<set<Error>>("errors_expected");

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

    set<Error> set_empty;

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

    only_map map1, map2, map3, map4;
    map1.map = map_expected1;
    map2.map = map_expected2;
    map3.map = map_expected3;
    map4.map = map_expected4;

    QList<int> actions_expected1 = {1, 2, -1};
    QList<int> actions_expected2 = {1, 2, -1};
    QList<int> actions_expected3 = {1, 2, 3};
    QList<int> actions_expected4 = {1, 2, 3};


    QTest::newRow("accordance_only_correct") << actions_expected1 << map1 << set_empty;
    QTest::newRow("accordance_no_mode_zero") << actions_expected2 << map2 << expected2;
    QTest::newRow("accordance_one_missing") << actions_expected3 << map3 << expected3;
    QTest::newRow("accordance_many_missing") << actions_expected4 << map4 << expected4;

}


void Test_Sync::testCheckAccordances()
{

    qRegisterMetaType<Error>();
    qRegisterMetaType<set<Error>>();
    qRegisterMetaType<only_map>();

    QFETCH(QList<int>, actions_list);
    QFETCH(only_map, accordances_map);
    QFETCH(set<Error>, errors_expected);

    set<Error> errors;
    checkAccordances(accordances_map.map, actions_list, errors);

    QVERIFY2(errors == errors_expected, "errors checking failed");

}


