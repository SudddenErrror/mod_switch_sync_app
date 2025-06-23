#ifndef TEST_SYNC_H
#define TEST_SYNC_H

#include "error.h"
#include <QObject>

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

};

#endif // TEST_SYNC_H
