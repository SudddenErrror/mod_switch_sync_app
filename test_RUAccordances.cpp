#include <QTest>
#include "error.cpp"
#include "only_map.cpp"
#include "test_sync.h"

Test_Sync::Test_Sync(QObject *parent) :
    QObject(parent)
{

}


void Test_Sync::testReadUserAccordances_data()
{
    qRegisterMetaType<Error>();
    qRegisterMetaType<only_map>();

    QTest::addColumn<QString>("accordances_string");
    QTest::addColumn<only_map>("maps_expected");
    QTest::addColumn<set<Error>>("errors_expected");

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
    one.moreThanOneModeAccordanceString = {3, 5};
    one.moreThanOneModeAccordanceContent = {"B", "E"};
    two.type = Error::moreThanOneModeAccordance;
    two.moreThanOneModeAction = '2';
    two.moreThanOneModeAccordanceString = {3, 6};
    two.moreThanOneModeAccordanceContent = {"B", "F"};
    set<Error> expected8 = {one, two};


    QMap<int, QString> map_expected2;
        map_expected2[0] = 'A';
        map_expected2[2] = 'B';
        map_expected2[3] = 'D';

    QMap<int, QString> map_expected3;
        map_expected3[0] = 'A';
        map_expected3[2] = 'B';

    QMap<int, QString> map_expected7;
        map_expected7[0] = 'A';
        map_expected7[3] = 'D';

    QMap<int, QString> map_empty;

    QSet<Error> set_empty;

    only_map map1, map2, map3, map4, map5, map6, map7, map8;
    map1.map = map_empty;
    map2.map = map_expected2;
    map3.map = map_expected3;
    map4.map = map_empty;
    map5.map = map_empty;
    map6.map = map_empty;
    map7.map = map_expected7;
    map8.map = map_empty;

    QTest::newRow("string_empty") << "" << map1 << set_empty;
    QTest::newRow("string_only_correct") << "2 3 1 -2 -3 -1" << map2 << set_empty;
    QTest::newRow("string_newline_missing") << "A 0\nB 2 D 3" << map3 << set_empty;
    QTest::newRow("string_element_missing") << "A 0\nB\n 1\nD 3" << map4 << expected4;
    QTest::newRow("string_has_incorrect") << "A 0\nB 2\n-C 1\nD i3" << map5 << expected5;
    QTest::newRow("string_both_incorrect") << "C 0\nB 2\n-A i1\n+_D 0.3" << map6 << expected6;
    QTest::newRow("string_newlines_paired") << "A 0\n\nD 3" << map7 << set_empty;
    QTest::newRow("string_one_in_many") << "Z 0\nA 1\nB 2\nD 3\nE 2\nF 2" << map8 << expected8;

}

void Test_Sync::testReadUserAccordances()
{
    qRegisterMetaType<Error>();
    qRegisterMetaType<only_map>();

    QFETCH(QString, accordances_string);
    QFETCH(only_map, maps_expected);
    QFETCH(set<Error>, errors_expected);

    QMap<int, QString> accordModeAction;
    set<Error> errors;
    readUserAccordances (accordances_string, accordModeAction, errors);

    if(errors_expected.empty())
    {
        QVERIFY2(accordModeAction == maps_expected.map, "accordModeAction checking failed");
    }

    QVERIFY2(errors == errors_expected, "errors checking failed");
}
