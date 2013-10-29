#include "calculecheancier.h"
#include "db.h"
#include <QDate>
#include <iostream>

CalculEcheancier::CalculEcheancier(Pret* pret) :
    pret(*pret)
{
    tauxInteret = pret->tauxInteret;
    duree = pret->getDuree();
}

void CalculEcheancier::traiterEvenementsSurPret(int& evenementIndex) {
    if (evenementIndex < pret.evenements.size()) {
        Evenement evenementATraiter = pret.evenements.at(evenementIndex);
        if (dateMensualite >= evenementATraiter.getDate()) {
            traiterEvenementSurPret( evenementATraiter );
            evenementIndex++;
        }
    }
}

void CalculEcheancier::traiterEvenementSurPret(Evenement event) {
    std::cout << "evenement a traiter : " << event.getType() << " " << event.getDate().toString().toStdString() << std::endl;
    switch(event.getType()) {
    case Evenement::DIFFERE_TOTAL :
        this->traiterDiffereTotal( event.getValeur() );
        break;
    case Evenement::DIFFERE_PARTIEL :
        this->traiterDifferePartiel( event.getValeur() );
        break;
    case Evenement::MAJ_MENSUALITE :
        this->majMensualite( event.getValeur() );
        break;
    case Evenement::MAJ_TAUX :
        std::cout << "maj taux" << std::endl;
        break;
    case Evenement::RBT_ANTICIPE :
        this->traiterRemboursementAnticipe( event.getValeur() );
        break;
    case Evenement::REPORT_MENSUALITE : // utile? utiliser report partiel/total?
        std::cout << "report mensualite" << std::endl;
        break;
    }
}

void CalculEcheancier::traiterRemboursementAnticipe(double montantRemboursement) {
    std::cout << "rbt anticipe" << std::endl;
    remboursementAnticipe = montantRemboursement;
}

void CalculEcheancier::traiterDiffereTotal(int nbReports) {
    std::cout << "differe total" << std::endl;
    for ( ; nbReports > 0; nbReports--) {
        // capital restant du est augmenté des frais intercalaires générés par le différé total
        capitalRestantDu += interets;
        // mensualite = assurance
        ajouterEcheance(echeancierId, dateMensualite, capitalRestantDu, assurance, 0, interets, assurance);
        dateMensualite = dateMensualite.addMonths(1);
    }
}

void CalculEcheancier::traiterDifferePartiel(int nbReports) {
    std::cout << "differe partiel" << std::endl;
    for ( ; nbReports > 0; nbReports--) {
        // mensualite = cout des frais intercalaires + assurance
        ajouterEcheance(echeancierId, dateMensualite, capitalRestantDu, interets + assurance, 0, interets, assurance);
        dateMensualite = dateMensualite.addMonths(1);
    }
}

void CalculEcheancier::majMensualite(double nouvelleMensualite) {
    std::cout << "maj mensualite" << std::endl;
    mensualiteEcheance = nouvelleMensualite - calculerCoutAssuranceMensualiteCourante(); // BUG? on retire vraiment coût assurance?
    duree = majDureeRemboursement(pret.getMensualite(), mensualiteEcheance);
}

bool CalculEcheancier::isDerniereMensualite() {
    return mensualiteEcheance > (capitalRestantDu + interets);
}

int CalculEcheancier::majDureeRemboursement(float mensualite, float nouvelleMensualite) {
    float n = (log((12*mensualite) / (12*nouvelleMensualite-tauxInteret*pret.capitalEmprunte))) / log(1+tauxInteret/12);
    return ceil(n);
}

float CalculEcheancier::calculerInteretsMensualiteCourante(float tauxInteretAnnuel) {
    return capitalRestantDu * tauxInteretAnnuel / 12;
}

float CalculEcheancier::calculerCoutAssuranceMensualiteCourante() {
    // http://www.emprunt-et-credit.com/news/112/105/calcul-assurance-sur-capital-restant-du.htm
    return pret.capitalEmprunte * pret.tauxAssurance / 12;
}

int CalculEcheancier::compute() {
    double capitalRembourse;
    int evenementIndex = 0;

    capitalRestantDu = pret.capitalEmprunte;
    mensualiteEcheance = pret.getMensualite();
    dateMensualite = pret.dateDebloquage;
    echeancierId = creerEcheancier("Echéancier prêt", QDate::currentDate(), pret.id);
    interets = 0;

    std::cout << "Echeancier id : " << echeancierId << "\n";
    if (echeancierId != -1) {
        std::cout << "evts size = " << pret.evenements.count() << std::endl;

        // Calculer les échéances
        while (capitalRestantDu > 0) {
            assurance = calculerCoutAssuranceMensualiteCourante();
            interets = calculerInteretsMensualiteCourante(pret.tauxInteret);

            traiterEvenementsSurPret(evenementIndex);

            if ( isDerniereMensualite() ) {
                capitalRembourse = capitalRestantDu;
                mensualiteEcheance = capitalRestantDu + interets;
            } else {
                capitalRembourse = mensualiteEcheance - interets + remboursementAnticipe;
            }

            capitalRestantDu -= capitalRembourse;
            ajouterEcheance(echeancierId, dateMensualite, capitalRestantDu, mensualiteEcheance + assurance, capitalRembourse, interets, assurance);
            remboursementAnticipe = 0;
            dateMensualite = dateMensualite.addMonths(1);
        }
    }
//    std::cout.flush();
    return echeancierId;
}
