#ifndef TEST_SYNC_H
#define TEST_SYNC_H

#include <QtTest/QtTest>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "sync.h"

class Test_Sync : public QObject
{
    Q_OBJECT
public:
    explicit Test_Sync(QObject *parent = 0);

private slots:

    void testReadUserActions_data();
    void testReadUserActions();

    /*void testReadUserActionsEmptyString();
    void testReadUserActionsOnlyInts();
    void testReadUserActionsHasBreaks();
    void testReadUserActionsHasNonInts();*/

    void testReadUserAccordances_data();
    void testReadUserAccordances();

   /* void testReadUserAccordancesEmptyString();
    void testReadUserAccordancesOnlyCorrect();
    void testReadUserAccordancesBreakMissing();
    void testReadUserAccordancesElementMissing();
    void testReadUserAccordancesHasIncorrect();
    void testReadUserAccordancesBothIncorrect();
    void testReadUserAccordancesBreakDoubling();
    void testReadUserAccordancesOneInMany();*/

    void testCheckAccordances_data();
    void testCheckAccordances();

    /*void testCheckAccordancesOnlyCorrect();;
    void testCheckAccordancesNoModeZero();
    void testCheckAccordancesNoAccordance();
    void testCheckAccordancesNoAccordances();*/

    void testGenerateActionModeLists_data();
    void testGenerateActionModeLists();

    /*void testGenerateActionModeLists_EndNoStart();
    void testGenerateActionModeLists_NoOverlap_ReturnToStart();
    void testGenerateActionModeLists_HasOverlap_ReturnToStart_Reversed();
    void testGenerateActionModeLists_HasOverlap_ReturnToStart_Straight();
    void testGenerateActionModeLists_HasOverlap_NoReturn_Random();
    void testGenerateActionModeLists_HasOverlap_OneModeManyActions();*/

};

#endif // TEST_SYNC_H
