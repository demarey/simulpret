#include "pret.h"
#include "db.h"
#include <QStringBuilder>
#include <QString>
#include <math.h>
#include <iostream>

Pret::Pret(long capitalEmprunte, float tauxInteret, int duree, float tauxAssurance, QDate dateDebloquage)
{
    majPret(capitalEmprunte, tauxInteret, duree, tauxAssurance, dateDebloquage);
    // Enregistrement dans la BD
    this->id = ajouterPret("Pret", capitalEmprunte, tauxInteret, tauxAssurance, duree, this->mensualite, dateDebloquage);
}

Pret::Pret(long capitalEmprunte, float tauxInteret, float mensualite, float tauxAssurance, QDate dateDebloquage)
{
    majPret(capitalEmprunte, tauxInteret, mensualite, tauxAssurance, dateDebloquage);
    // Enregistrement dans la BD
    this->id = ajouterPret("Pret", capitalEmprunte, tauxInteret, tauxAssurance, this->duree, mensualite, dateDebloquage);
}

void Pret::majPret(long capitalEmprunte, float tauxInteret, int duree, float tauxAssurance, QDate dateDebloquage) {
    this->capitalEmprunte = capitalEmprunte;
    this->tauxInteret = tauxInteret;
    this->duree = duree;
    this->tauxAssurance = tauxAssurance;
    this->dateDebloquage = dateDebloquage;
    // calculer la mensualite
    this->mensualite = calculerMensualite(capitalEmprunte, tauxInteret, duree, tauxAssurance);
    // TODO : MAJ BD
}

void Pret::majPret(long capitalEmprunte, float tauxInteret, float mensualite, float tauxAssurance, QDate dateDebloquage) {
    this->capitalEmprunte = capitalEmprunte;
    this->tauxInteret = tauxInteret;
    this->tauxAssurance = tauxAssurance;
    this->mensualite = mensualite - getCoutMensuelAssurance();
    this->dateDebloquage = dateDebloquage;
    this->duree = calculerDureeRemboursement(capitalEmprunte, tauxInteret, mensualite, tauxAssurance);
    // TODO : MAJ BD
}

float Pret::getCoutMensuelAssurance() {
    return this->capitalEmprunte * this->tauxAssurance / 12;
}

int Pret::calculerDureeRemboursement(long capitalEmprunte, float tauxInteret, float mensualite, float tauxAssurance) {
    float n = (log((12*mensualite) / (12*mensualite-tauxInteret*capitalEmprunte))) / log(1+tauxInteret/12);
    return ceil(n);
}

int Pret::calculerDureeRemboursement(float mensualite) {
    float n = (log((12*this->mensualite) / (12*mensualite-this->tauxInteret*this->capitalEmprunte))) / log(1+this->tauxInteret/12);
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

int Pret::calculerEcheancier() {
    double capitalRestantDu = this->capitalEmprunte;
    double capitalRembourse,
           interets = 0,
           mensualiteEcheance = this->mensualite;
    double assurance = getCoutMensuelAssurance();
    QDate dateMensualite = this->dateDebloquage;
    bool evenementTraite = true;

    // Créer l'échéancier
    int echeancierId = creerEcheancier("Echéancier prêt", QDate::currentDate(), 1);

    std::cout << "Echeancier id : " << echeancierId << "\n";
    if (echeancierId != -1) {
        // Récupérer les événements relatifs au prêt
        QList<Evenement> evts = getEvenements(this->id);
        QListIterator<Evenement> evtsIterator(evts);
        std::cout << "evts size = " << evts.count() << std::endl;
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
                        this->duree = calculerDureeRemboursement(mensualiteEcheance);
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

/**
 * Calculer le coût total du crédit (inclue l'assurance).
 */
float Pret::getCoutTotalCredit(int echeancierId) {
    return getSommeMensualites(echeancierId) + getSommeRbtAnticipes(this->getId()) - this->capitalEmprunte;
}

QString Pret::toString() {
    QString mensualite = "mensualite = ";
    QString value ; value.setNum(this->mensualite);
    QString duree = " duree = ";
    QString value2 ; value2.setNum(this->duree);
    QString result = mensualite % value % duree % value2;
    return result;
}
