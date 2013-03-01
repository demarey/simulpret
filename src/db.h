#ifndef DB_H
#define DB_H

#define DB_DRIVER     "QSQLITE"
#define DB_DBNAME     ":memory:"
#define DB_USER       "chris"
#define DB_PASSWD     "chris"
#define DB_HOST       "localhost"

#include <QString>
#include <QDate>
#include <QtSql>
#include "evenement.h"

bool createConnection();
bool createSchema();

/**
 *  Ajouter un pret dans la table correspondante.
 */
int ajouterPret(QString libelle, int capitalEmprunte, float tauxInteret, float tauxAssurance, int duree, float mensualite, QDate dateDebloquage);

/**
 * Creer un nouvel échéancier.
 */
int creerEcheancier(QString libelle, QDate dateCreation, int pretId);

/**
 *  Ajouter une echeance dans la table correspondante.
 */
void ajouterEcheance(int id, QDate dateEcheance, float capitalRestantDu, float mensualite,
                     float capital, float interets, float assurance);

/**
 *  Initialiser le modèle de l'échéancier en vue de l'affichage
 */
void initializeEcheancierModel(QSqlQueryModel *model, int echeancierId);

/**
 * Ajouter un évènement au pret en cours.
 */
void ajouterEvenement(QDate date, QString libelle, float valeur, int pretId, int typeId, bool isPermanent = true);

/**
 * Supprimer un évènement au pret en cours.
 */
void supprimerEvenement(int eventId, int pretId);

/**
 *  Initialiser le modèle des évènements en vue de l'affichage
 */
void initializeEvenementModel(QSqlQueryModel *model, int echeancierId);

/**
 *  Initialiser le modèle des types d'évènements en vue de l'affichage
 */
void initializeTypesEvenementModel(QSqlQueryModel *model);

/**
 *  Récupérer la liste des événements déclarés.
 */
QList<Evenement> getEvenements(int pretId);

/**
 * Calculer les intérêts payés sur l'ensemble du prêt.
 */
float getSommeInterets(int echeancierId);

/**
 * Calculer le coût de l'assurance sur l'ensemble du prêt.
 */
float getCoutAssurance(int echeancierId);

/**
 * Calculer la somme des mensualités versées.
 */
float getSommeMensualites(int echeancierId);

/**
 * Calculer la somme des remboursements anticipes.
 */
float getSommeRbtAnticipes(int echeancierId);

#endif // DB_H
