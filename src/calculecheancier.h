#ifndef CALCULECHEANCIER_H
#define CALCULECHEANCIER_H

#include "pret.h"

/**
 * @brief Calcul de l'échéancier pour le pret passé en paramètre (Method Class pattern).
 */
class CalculEcheancier
{
public:
    /**
     * Construire un nouvel échéancier pour le prêt passé en paramètre.
     */
    explicit CalculEcheancier(Pret* pret);
private:
    Pret& pret;
    int echeancierId, duree;
    double capitalRestantDu, mensualiteEcheance, interets, assurance, remboursementAnticipe;
    float tauxInteret;
    QDate dateMensualite;

    /**
     * Traiter les évènements liés au prêt.
     */
    void traiterEvenementsSurPret(int& evenementIndex);
    /**
     * Traiter un évènement lié au prêt.
     */
    void traiterEvenementSurPret(Evenement event);
    /**
     * Traiter un remboursement anticipé sur le prêt.
     */
    void traiterRemboursementAnticipe(double montantRemboursement);
    /**
     * Traiter un différé total sur le prêt.
     */
    void traiterDiffereTotal(int nbReports);
    /**
     * Traiter un différé partiel sur le prêt.
     */
    void traiterDifferePartiel(int nbReports);
    /**
     * Traiter une mise à jour de la mensualité du prêt.
     */
    void majMensualite(double nouvelleMensualite);
    /**
     * Retourne true si la mensualité courante est la dernière mensualité du prêt.
     */
    bool isDerniereMensualite();
    /**
     * Calculer une mise à jour de la durée de remboursement du prêt.
     */
    int majDureeRemboursement(float mensualite, float nouvelleMensualite);
    /**
     * Calculer le montant des intérêts pour la mensualité courante.
     */
    float calculerInteretsMensualiteCourante(float tauxInteretAnnuel);
    /**
     * Calculer le montant de l'assurance pour la mensualité courante.
     */
    float calculerCoutAssuranceMensualiteCourante() ;
public:
    /**
     * Calculer l'échéancier correspondant au prêt passé en paramètre.
     */
    int compute();
};

#endif // CALCULECHEANCIER_H
