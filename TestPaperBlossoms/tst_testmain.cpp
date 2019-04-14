#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "../PaperBlossoms/src/dataaccesslayer.h"
#include "../PaperBlossoms/src/dataaccesslayer.cpp"

class TestMain : public QObject
{
    Q_OBJECT

public:
    TestMain();
    ~TestMain();

    DataAccessLayer* dal;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_dal_qsl_getclans();

};

TestMain::TestMain()
{
        dal = new DataAccessLayer();
}

TestMain::~TestMain()
{

}

void TestMain::initTestCase()
{

}

void TestMain::cleanupTestCase()
{

}

void TestMain::test_case1()
{
        QVERIFY(true);
}

void TestMain::test_dal_qsl_getclans()
{
        QStringList clans = dal->qsl_getclans();
        QVERIFY(!clans.isEmpty());
}

QTEST_MAIN(TestMain)

#include "tst_testmain.moc"
