#ifndef DBLOADERTEST_H
#define DBLOADERTEST_H

class DBloaderTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testChargementPret();
    void cleanupTestCase();
};


#endif // DBLOADERTEST_H
