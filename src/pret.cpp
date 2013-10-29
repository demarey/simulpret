#include "db.h"
#include "pret.h"
#include <QDebug>
#include <QStringBuilder>
#include <QString>
#include <iostream>

Pret::Pret() {}

Pret::Pret(long capitalEmprunte, float tauxInteret, int duree, float tauxAssurance, QDate dateDebloquage)
{
    this->capitalEmprunte = capitalEmprunte;
    this->tauxInteret = tauxInteret;
    this->duree = duree;
    this->mensualite = 0;
    this->tauxAssurance = tauxAssurance;
    this->dateDebloquage = dateDebloquage;
    qDebug() << "creation pret avec duree" << capitalEmprunte << tauxInteret << tauxAssurance << mensualite << duree << dateDebloquage.toString(Qt::ISODate);
}

Pret::Pret(long capitalEmprunte, float tauxInteret, float mensualiteHorsAssurance, float tauxAssurance, QDate dateDebloquage)
{
    this->capitalEmprunte = capitalEmprunte;
    this->tauxInteret = tauxInteret;
    this->tauxAssurance = tauxAssurance;
    this->mensualite = mensualiteHorsAssurance;
    this->duree = 0;
    this->dateDebloquage = dateDebloquage;
    qDebug() << "creation pret avec mensualite" << capitalEmprunte << tauxInteret << tauxAssurance << mensualite << duree << dateDebloquage.toString(Qt::ISODate);
}

void Pret::initialize(long capitalEmprunte, float tauxInteret, int duree, float mensualiteHorsAssurance, float tauxAssurance, QDate dateDebloquage) {
    qDebug() << "initialisation pret: " << capitalEmprunte << tauxInteret << duree << tauxAssurance << mensualiteHorsAssurance << dateDebloquage.toString(Qt::ISODate);
    this->capitalEmprunte = capitalEmprunte;
   this->tauxInteret = tauxInteret;
    this->duree = duree;
    this->tauxAssurance = tauxAssurance;
    this->mensualite = mensualiteHorsAssurance;
    this->dateDebloquage = dateDebloquage;
}

void Pret::ajouterEvenement(Evenement *evt) {
    evenements.append(*evt);
}

int Pret::calculerDureeRemboursement(long capitalEmprunte, float tauxInteret, float mensualite, float tauxAssurance) {
    float n = (log((12*mensualite) / (12*mensualite-tauxInteret*capitalEmprunte))) / log(1+tauxInteret/12);
    return ceil(n);
}

float Pret::calculerMensualite(long capitalEmprunte, float tauxInteret, int duree, float tauxAssurance) {
    return (capitalEmprunte * tauxInteret/12 ) / (1 - pow(1 + tauxInteret/12, -1 * duree));
}

int Pret::getId() {
    return this->id;
}

void Pret::setId(int id) {
    this->id = id;
}

float Pret::getSommeRbtAnticipes() {
    float somme = 0.0;

    QListIterator<Evenement> iterator(this->evenements);
     while (iterator.hasNext()) {
         Evenement evt = iterator.next();
         if (evt.getType() == Evenement::RBT_ANTICIPE)
            somme += evt.getValeur();
     }
     return somme;
}

float Pret::getCoutTotalCredit(int echeancierId) {
    return getSommeMensualites(echeancierId) + getSommeRbtAnticipes() - this->capitalEmprunte;
}

long Pret::getCapitalEmprunte() { return capitalEmprunte; }
float Pret::getTauxInteret() { return tauxInteret; }
float Pret::getTauxAssurance() { return tauxAssurance; }
QDate Pret::getDateDebloquage() { return dateDebloquage; }

/**
 * @brief Récupérer la durée totale du prêt en mois.
 * @return la durée du prêt spécifiée par l'utilisateur, sinon la durée calculéee à partir des autres données.
 */
int Pret::getDuree() {
    if (duree == 0)
        return calculerDureeRemboursement(capitalEmprunte, tauxInteret, mensualite, tauxAssurance);
    else
        return this->duree;
}

/**
 * @brief Récupérer la mensualité (hors assurance) du prêt courant.
 * @return la mensualité désirée si fournie par l'utilisateur ou calculée à partir des autres données.
 */
float Pret::getMensualite() {
    if (mensualite == 0)
        return calculerMensualite(capitalEmprunte, tauxInteret, duree, tauxAssurance);
    else
        return mensualite;
}

QString Pret::toString() {
    QString mensualite = "mensualite = ";
    QString value ; value.setNum(this->getMensualite());
    QString duree = " duree = ";
    QString value2 ; value2.setNum(this->getDuree());
    QString result = mensualite % value % duree % value2;
    return result;
}

void Pret::accept(Visitor &visitor) {
    visitor.visit(this);
/*    QListIterator<Pret *> iterator( this->evenements );
     while (iterator.hasNext())
         visitor.visit( iterator.next() );*/
}


