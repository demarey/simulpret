#ifndef DB_H
#define DB_H

#define DB_DRIVER     "QSQLITE"
#define DB_HOST       "localhost"
#define IN_MEMORY_DB_NAME  "memory"

#include <QDate>
#include <QString>
#include <QSqlQuery>
#include <QtSql>
#include "evenement.h"
#include "pret.h"

/**
 * Créer une nouvelle base avec le schéma associé.
 */
bool newDB(QString dbName);
/**
 * Fermer la connection à la BD.
 */
void closeDB();
/**
 *  Positionner la base par défaut.
 */
bool setDefaultDB(QString dbName);
/**
 *  Créer le schéma pour la base par défaut.
 */
bool createSchema();
/**
 * Créer une base de donnees en memoire.
 */
bool createInMemoryDB();

/**
 *  Ajouter un pret dans la table correspondante.
 */
void ajouterPret(int id, QString libelle, int capitalEmprunte, float tauxInteret, float tauxAssurance, int duree, float mensualite, QDate dateDebloquage);

/**
 *  Récupérer les prets depuis la BD.
 */
QList<Pret *> getPrets();

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

#endif // DB_H
