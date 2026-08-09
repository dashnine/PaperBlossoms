#include <QtTest>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

// add necessary includes here
#include "../PaperBlossoms/src/dependency/dependencybuilder.h"
#include "../PaperBlossoms/src/dependency/dependencybuilder.cpp"
#include "../PaperBlossoms/src/dependency/databasedependency.h"
#include "../PaperBlossoms/src/dependency/databasedependency.cpp"
#include "../PaperBlossoms/src/repository/dataaccesslayer.h"
#include "../PaperBlossoms/src/repository/dataaccesslayer.cpp"
#include "../PaperBlossoms/src/repository/upbringingsrepository.h"
#include "../PaperBlossoms/src/repository/upbringingsrepository.cpp"
#include "../PaperBlossoms/src/repository/titlesrepository.h"
#include "../PaperBlossoms/src/repository/titlesrepository.cpp"
#include "../PaperBlossoms/src/repository/techniquesrepository.h"
#include "../PaperBlossoms/src/repository/techniquesrepository.cpp"
#include "../PaperBlossoms/src/repository/skillsrepository.h"
#include "../PaperBlossoms/src/repository/skillsrepository.cpp"
#include "../PaperBlossoms/src/repository/schoolsrepository.h"
#include "../PaperBlossoms/src/repository/schoolsrepository.cpp"
#include "../PaperBlossoms/src/repository/ringsrepository.h"
#include "../PaperBlossoms/src/repository/ringsrepository.cpp"
#include "../PaperBlossoms/src/repository/regionsrepository.h"
#include "../PaperBlossoms/src/repository/regionsrepository.cpp"
#include "../PaperBlossoms/src/repository/inventoryrepository.h"
#include "../PaperBlossoms/src/repository/inventoryrepository.cpp"
#include "../PaperBlossoms/src/repository/hertiagesrepository.h"
#include "../PaperBlossoms/src/repository/hertiagesrepository.cpp"
#include "../PaperBlossoms/src/repository/clansrepository.h"
#include "../PaperBlossoms/src/repository/clansrepository.cpp"
#include "../PaperBlossoms/src/repository/bondsrepository.h"
#include "../PaperBlossoms/src/repository/bondsrepository.cpp"
#include "../PaperBlossoms/src/repository/advantagesrepository.h"
#include "../PaperBlossoms/src/repository/advantagesrepository.cpp"
#include "../PaperBlossoms/src/repository/abstractrepository.h"
#include "../PaperBlossoms/src/repository/abstractrepository.cpp"
#include "../PaperBlossoms/src/repository/familiesrepository.h"
#include "../PaperBlossoms/src/repository/familiesrepository.cpp"

class TestMain : public QObject {
    Q_OBJECT

public:
    TestMain();

    ~TestMain();

    DatabaseDependency *deps;

private
    slots:
            void initTestCase();

    void cleanupTestCase();

    void test_case1();

    void test_dal_importCSV();

    void test_dal_qsl_getclans();

    void test_dal_qsl_getfamilies();

    void test_dal_qsl_getfamilyrings();

    void test_dal_qs_getclandesc();

    void test_dal_qs_getfamilydesc();

    void test_dal_qsl_getschools();

    void test_dal_qs_getschooldesc();

    void test_dal_qsl_getschoolskills();

    void test_dal_i_getschoolskillcount();
};

TestMain::TestMain() {

    QString targetpath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (QDir(targetpath).exists()) {
        QMessageBox msgBox;
        msgBox.setText("TESTING");
        msgBox.setInformativeText("Do you want to remove local data for testing?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        const int ret = msgBox.exec();

        if (ret == QMessageBox::Yes) {
            QDir target = targetpath;
            target.removeRecursively();
        }
    }

    deps = (new DependencyBuilder())->build("en");

}

TestMain::~TestMain() {

}

void TestMain::initTestCase() {
}

void TestMain::cleanupTestCase() {

}

void TestMain::test_case1() {
    QVERIFY(true);
}

void TestMain::test_dal_importCSV() {

    bool success = true;
    foreach(QString
    tablename, deps->dal->user_tables){
        success &= deps->dal->importCSV(":/exports/export_sample", tablename);
    }
    QVERIFY(success);
}

void TestMain::test_dal_qsl_getclans() {
    QStringList clans = deps->clansRepository->qsl_getclans();
    QVERIFY(!clans.isEmpty());
}

void TestMain::test_dal_qsl_getfamilies() {
    QStringList clans = deps->clansRepository->qsl_getclans();
    foreach(QString clan, clans){
        QStringList families = deps->familiesRepository->qsl_getfamilies(clan);
        QVERIFY(!families.empty());
    }
}

void TestMain::test_dal_qsl_getfamilyrings() {
    QStringList clans = deps->clansRepository->qsl_getclans();
    foreach(QString clan, clans){
        QStringList families = deps->familiesRepository->qsl_getfamilies(clan);
        foreach(QString
        family, families){
            QStringList rings = deps->familiesRepository->qsl_getfamilyrings(family);
            QVERIFY(!rings.empty());
        }
    }

}

void TestMain::test_dal_qs_getclandesc() {
    //have already imported a database that includes a crane desc
    QString desc = deps->clansRepository->qs_getclandesc("Crane");
    QVERIFY(!desc.isEmpty());

}

void TestMain::test_dal_qs_getfamilydesc() {
    //have already imported a database that includes a crane desc
    QString desc = deps->familiesRepository->qs_getfamilydesc("Kakita");
    QVERIFY(!desc.isEmpty());
}

void TestMain::test_dal_qsl_getschools() {
    QStringList schools = deps->schoolsRepository->qsl_getschools("Crane");
    QStringList schools2 = deps->schoolsRepository->qsl_getschools("Crane", true);
    int count1 = schools.size();
    int count2 = schools2.size();
    QVERIFY2(count1 > 0, "No schools found for Crane");
    QVERIFY2(count1 < count2, "Extra schools not added for 'allschools=true");
}

void TestMain::test_dal_qs_getschooldesc() {
    QString desc = deps->schoolsRepository->qs_getschooldesc("Kakita Duelist School");
    QVERIFY(!desc.isEmpty());
}

void TestMain::test_dal_qsl_getschoolskills() {
    QStringList schools = deps->schoolsRepository->qsl_getschools("Crane", true); //all schools
    foreach(QString
    school, schools){
        QStringList skills = deps->schoolsRepository->qsl_getschoolskills(school);
        QString errmessage = QString("Error: no skills in ") + school;
        QVERIFY2(skills.count() > 0, errmessage.toLatin1());
    }
}

void TestMain::test_dal_i_getschoolskillcount() {
    QStringList schools = deps->schoolsRepository->qsl_getschools("Crane", true); //all schools
    foreach(QString
    school, schools){
        int count = deps->schoolsRepository->i_getschoolskillcount(school);
        QString errmessage = QString("Error: bad skill count in ") + school;
        QVERIFY2(count > 0, errmessage.toLatin1());
    }
}


QStringList qsl_getschoolskills(const QString school);

int i_getschoolskillcount(const QString school);


QTEST_MAIN(TestMain)

#include "tst_testmain.moc"
