#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <qdatetime.h>
#include <QString>

class Evenement
{
public:
    enum EvenementType {DIFFERE_TOTAL = 1, DIFFERE_PARTIEL = 2, MAJ_MENSUALITE = 3, RBT_ANTICIPE = 4, REPORT_MENSUALITE = 5, MAJ_TAUX = 6};

    Evenement();
    Evenement(QString libelle, QDate date, float valeur, EvenementType type, bool isPermanent);

    QString getLibelle() const;
    QDate getDate() const;
    float getValeur() const;
    EvenementType getType() const;
    QString getTypeAsString() const;
    bool isPermanent() const;
    void persist();

private:
    QDate date;
    float valeur;
    QString libelle;
    EvenementType type;
    bool permanent;
};

#endif // EVENEMENT_H
