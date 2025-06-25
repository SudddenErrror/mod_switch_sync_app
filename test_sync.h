#ifndef TEST_SYNC_H
#define TEST_SYNC_H

#include "sync.h"

class Test_Sync : public QObject
{
    Q_OBJECT
public:
    explicit Test_Sync(QObject *parent = 0);

private slots:

    void testReadUserActionsEmptyString();
    void testReadUserActionsOnlyInts();
    void testReadUserActionsHasBreaks();
    void testReadUserActionsHasNonInts();

    void testReadUserAccordancesEmptyString();
    void testReadUserAccordancesOnlyCorrect();
    void testReadUserAccordancesBreakMissing();
    void testReadUserAccordancesElementMissing();
    void testReadUserAccordancesHasIncorrect();
    void testReadUserAccordancesBothIncorrect();
    void testReadUserAccordancesBreakDoubling();
    void testReadUserAccordancesOneInMany();

    void testCheckAccordancesOnlyCorrect();
    void testCheckAccordancesNoModeZero();
    void testCheckAccordancesNoAccordance();
    void testCheckAccordancesNoAccordances();

    void testGenerateActionModeLists_EndNoStart();
    void testGenerateActionModeLists_NoOverlap_ReturnToStart();
    void testGenerateActionModeLists_HasOverlap_ReturnToStart_Reversed();
    void testGenerateActionModeLists_HasOverlap_ReturnToStart_Straight();
    void testGenerateActionModeLists_HasOverlap_NoReturn_Random();
    void testGenerateActionModeLists_HasOverlap_OneModeManyActions();

};

#endif // TEST_SYNC_H
