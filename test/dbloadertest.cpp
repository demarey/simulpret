#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtTest/QtTest>
#include "dbloadertest.h"
#include "../src/dbdumper.h"
#include "../src/pret.h"
#include "../src/db.h"

void DBloaderTest::initTestCase() {
    qDebug("called before everything else");

    QSqlDatabase memoryDB = QSqlDatabase::addDatabase( DB_DRIVER, IN_MEMORY_DB_NAME );
    memoryDB.setDatabaseName(":memory:");
    if ( ! memoryDB.open() ) {
        qWarning() << "Failed to open database: " << memoryDB.lastError().text();
        return;
    }
    setDefaultDB(":memory:");
    createSchema();

    Pret *pret;
    DBdumper dumper;

    pret = new Pret(120000L, 0.045f, 1000.0f, 0.0035f, QDate(2013,04,14));
    pret->setId(1);
    pret->accept( dumper );
    pret = new Pret(70000L, 0.04f, 120, 0.0021f, QDate(2013,04,16));
    pret->setId(2);
    pret->accept( dumper );
}

void DBloaderTest::testChargementPret() {
    QList<Pret *> prets = getPrets();
    QCOMPARE(prets.size(), 2);

    Pret *pret = prets.first();
    QCOMPARE(pret->getCapitalEmprunte(), 120000L);
    QCOMPARE(pret->getTauxInteret(), 0.045f);
    QCOMPARE(pret->getTauxAssurance(), 0.0035f);
    QCOMPARE(pret->getMensualite(), 1000.0f);
    QCOMPARE(pret->getDateDebloquage(), QDate(2013,04,14));
    QCOMPARE(pret->getId(), 1);

    pret = prets.at(1);
    QCOMPARE(pret->getCapitalEmprunte(), 70000L);
    QCOMPARE(pret->getTauxInteret(), 0.04f);
    QCOMPARE(pret->getTauxAssurance(), 0.0021f);
    QCOMPARE(pret->getDuree(), 120);
    QCOMPARE(pret->getDateDebloquage(), QDate(2013,04,16));
    QCOMPARE(pret->getId(), 2);
}

void DBloaderTest::cleanupTestCase() {
    qDebug("called after myFirstTest and mySecondTest");
    closeDB();
}

QTEST_MAIN(DBloaderTest)
#include "dbloadertest.moc"
