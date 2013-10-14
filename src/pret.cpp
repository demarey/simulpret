#include "pret.h"
#include "db.h"
#include <QListIterator>
#include <QStringBuilder>
#include <QString>
#include <math.h>
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

float Pret::getCoutMensuelAssurance() {
    return this->capitalEmprunte * this->tauxAssurance / 12;
}

int Pret::calculerDureeRemboursement(long capitalEmprunte, float tauxInteret, float mensualite, float tauxAssurance) {
    float n = (log((12*mensualite) / (12*mensualite-tauxInteret*capitalEmprunte))) / log(1+tauxInteret/12);
    return ceil(n);
}

int Pret::majDureeRemboursement(float mensualite, float nouvelleMensualite) {
    float n = (log((12*mensualite) / (12*nouvelleMensualite-this->tauxInteret*this->capitalEmprunte))) / log(1+this->tauxInteret/12);
    return ceil(n);
}

float Pret::calculerMensualite(long capitalEmprunte, float tauxInteret, int duree, float tauxAssurance) {
    return (capitalEmprunte * tauxInteret/12 ) / (1 - pow(1 + tauxInteret/12, -1 * duree));
}

float Pret::calculerInteretsMensuels(double capitalRestantDu, float tauxInteret) {
    return capitalRestantDu * tauxInteret / 12;
}

int Pret::getId() {
    return this->id;
}

void Pret::setId(int id) {
    this->id = id;
}

int Pret::calculerEcheancier() {
    double capitalRestantDu = this->capitalEmprunte;
    double assurance = getCoutMensuelAssurance();
    double capitalRembourse,
           interets = 0,
           mensualiteEcheance = this->getMensualite() - assurance;
    QDate dateMensualite = this->dateDebloquage;
    bool evenementTraite = true;

    // Créer l'échéancier
    int echeancierId = creerEcheancier("Echéancier prêt", QDate::currentDate(), this->id);

    std::cout << "Echeancier id : " << echeancierId << "\n";
    if (echeancierId != -1) {
        // Récupérer les événements relatifs au prêt
        QListIterator<Evenement> evtsIterator(this->evenements);
        std::cout << "evts size = " << this->evenements.count() << std::endl;
        Evenement event;

        // Calculer les échéances
        while (capitalRestantDu > 0) {
            if ( !evenementTraite || (evenementTraite && evtsIterator.hasNext()) ) {
                // Si evenement traité, passer au suivant
                if ( evenementTraite ) {
                    event = evtsIterator.next();
                    evenementTraite = false;
                }
                // Si la date de mensualité est antérieure à la date de l'évènement, rien à faire : on attend l'échéance suivante.
                if (dateMensualite >= event.getDate()) {
                    std::cout << "evenement a traiter : " << event.getType() << " " << event.getDate().toString().toStdString() << std::endl;
                    switch(event.getType()) {
                    case Evenement::DIFFERE_TOTAL :
                        std::cout << "differe total" << std::endl;
                        for (int nbReports = event.getValeur(); nbReports > 0; nbReports--) {
                            // capital restant du est augmenté des frais intercalaires générés par le différé total
                            capitalRestantDu += calculerInteretsMensuels(capitalRestantDu, this->tauxInteret);
                            // mensualite = assurance
                            ajouterEcheance(echeancierId, dateMensualite, capitalRestantDu, assurance, 0, interets, assurance);
                            dateMensualite = dateMensualite.addMonths(1);
                        }
                        break;
                    case Evenement::DIFFERE_PARTIEL :
                        std::cout << "differe partiel" << std::endl;
                        interets = calculerInteretsMensuels(capitalRestantDu, this->tauxInteret);
                        for (int nbReports = event.getValeur(); nbReports > 0; nbReports--) {
                            // mensualite = cout des frais intercalaires + assurance
                            ajouterEcheance(echeancierId, dateMensualite, capitalRestantDu, interets + assurance, 0, interets, assurance);
                            dateMensualite = dateMensualite.addMonths(1);
                        }
                        break;
                    case Evenement::MAJ_MENSUALITE :
                        std::cout << "maj mensualite" << std::endl;
                        mensualiteEcheance = event.getValeur() - getCoutMensuelAssurance();
                        this->duree = majDureeRemboursement(this->getMensualite(), mensualiteEcheance);
                        break;
                    case Evenement::MAJ_TAUX :
                        std::cout << "maj taux" << std::endl;
                        break;
                    case Evenement::RBT_ANTICIPE :
                        std::cout << "rbt anticipe" << std::endl;
                        capitalRestantDu -= event.getValeur();
                        break;
                    case Evenement::REPORT_MENSUALITE : // utile? utiliser report partiel/total?
                        std::cout << "report mensualite" << std::endl;
                        break;
                    }
                    evenementTraite = true;
                }
            }

            interets = calculerInteretsMensuels(capitalRestantDu, this->tauxInteret);
            if ( (capitalRestantDu+interets) >= mensualiteEcheance) {
                capitalRembourse = mensualiteEcheance - interets;
            } else {
                capitalRembourse = capitalRestantDu;
                mensualiteEcheance = capitalRestantDu+interets;
            }
            capitalRestantDu -= capitalRembourse;
            ajouterEcheance(echeancierId, dateMensualite, capitalRestantDu, mensualiteEcheance + assurance, capitalRembourse, interets, assurance);
            dateMensualite = dateMensualite.addMonths(1);
        }
    }
//    std::cout.flush();
    return echeancierId;
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


