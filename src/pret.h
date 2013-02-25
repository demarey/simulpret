#ifndef PRET_H
#define PRET_H

#include <qstring.h>
#include <qdatetime.h>

class Pret
{
    int id;
    long capitalEmprunte;
    int duree; // duree du pret en mois
    float mensualite;
    float tauxInteret;
    float tauxAssurance;
    QDate dateDebloquage;
public:
    Pret();
    Pret(long capitalEmprunte, float tauxInteret, int duree, float tauxAssurance, QDate dateDebloquage);
    Pret(long capitalEmprunte, float tauxInteret, float mensualite, float tauxAssurance, QDate dateDebloquage);

    /**
     * Mettre a jour les données du prêt en fonction des paramètres.
     */
    void majPret(long capitalEmprunte, float tauxInteret, int duree, float tauxAssurance, QDate dateDebloquage);

    /**
     * Mettre a jour les données du prêt en fonction des paramètres.
     */
    void majPret(long capitalEmprunte, float tauxInteret, float mensualite, float tauxAssurance, QDate dateDebloquage);

    /**
     * Calculer le coût mensuel de l'assurance.
     */
    float getCoutMensuelAssurance();

    /**
     * Calculer le coût mensuel des intérêts.
     */
    float calculerInteretsMensuels(double capitalRestantDu, float tauxInteret);

    /**
     * Calculer la durée de remboursement à payer en fonction des paramètres donnés (dont mensualité).
     */
    int calculerDureeRemboursement(long capitalEmprunte, float tauxInteret, float mensualite, float tauxAssurance);

    /**
     * Calculer la durée de remboursement à payer en fonction des paramètres du prêt et de la mensualité donnée.
     */
    int calculerDureeRemboursement(float mensualite);

    /**
     * Calculer la mensualité à payer en fonction des paramètres donnés (dont durée).
     */
    float calculerMensualite(long capitalEmprunte, float tauxInteret, int duree, float tauxAssurance);

    /**
     * Calculer l'échéancier et l'enregistrer dans la BD.
     */
    int calculerEcheancier();

    /**
     * Calculer le coût total du crédit (inclue l'assurance).
     */
    float getCoutTotalCredit(int echeancierId);

    /**
     * Récupérer l'identifiant du prêt.
     */
    int getId();

    /**
     * Afficher les informations principales du prêt.
     */
    QString toString();
};

#endif // PRET_H
