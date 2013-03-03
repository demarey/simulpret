#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include "db.h"
#include <iostream>

bool createConnection()
{
    qDebug("Connecting to DB ...");

    QSqlDatabase defaultDB = QSqlDatabase::addDatabase( DB_DRIVER );
    defaultDB.setDatabaseName( DB_DBNAME );
    defaultDB.setUserName( DB_USER );
    defaultDB.setPassword( DB_PASSWD );
    defaultDB.setHostName( DB_HOST );
    if ( ! defaultDB.open() ) {
        qWarning() << "Failed to open database: " << defaultDB.lastError().text();
        return FALSE;
    }

    qDebug("Connection to DB established!");
    return TRUE;
}

/**
 * Create the database schema
 */
bool createSchema()
{
    qDebug("Creating DB ...");

    QSqlDatabase db = QSqlDatabase::database();
    if ( ! db.open() )
    {
        qWarning() << "Failed to open database: " << db.lastError().text();
        return FALSE;
    }

    QSqlQuery q(QString::null, db);

    if ( !q.exec("CREATE TABLE IF NOT EXISTS `pret` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `libelle` VARCHAR NULL , `capitalEmprunte` INT UNSIGNED NULL , `tauxInteret` FLOAT UNSIGNED NULL , `tauxAssurance` FLOAT UNSIGNED NULL , `duree` INT UNSIGNED NULL , `mensualite` FLOAT UNSIGNED NULL, `date_deboquage` DATETIME NULL )") )
        qDebug() << "Error on 'pret' TABLE creation" << q.lastError().text();

    if ( !q.exec("CREATE TABLE IF NOT EXISTS `echeancier` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `libelle` VARCHAR NULL , `date_creation` DATETIME NULL , isSimulation BOOLEAN , `pret_id` INT NOT NULL , FOREIGN KEY (`pret_id` ) REFERENCES `pret` (`id` ) )") )
        qDebug() << "Error on 'pret' TABLE echeancier" << q.lastError().text();

    if ( !q.exec("CREATE  TABLE IF NOT EXISTS `echeance` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `date` DATETIME NULL , `capitalRestantDu` FLOAT NULL , `capitalAmorti` FLOAT NULL , `interets` FLOAT NULL , `assurance` FLOAT NULL , `mensualite` FLOAT NULL , `echeancier_id` INT NOT NULL , FOREIGN KEY (`echeancier_id` ) REFERENCES `echeancier` (`id`) )") )
        qDebug() << "Error on 'pret' TABLE echeance" << q.lastError().text();

    if ( !q.exec("CREATE  TABLE IF NOT EXISTS `evenement_type` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `libelle` VARCHAR NULL )") )
        qDebug() << "Error on 'pret' TABLE echeance" << q.lastError().text();

    if ( !q.exec("CREATE  TABLE IF NOT EXISTS `evenement` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `date` DATETIME NULL , `libelle` VARCHAR NULL , `valeur` FLOAT NULL , `isPermanent` BOOLEAN NULL , `pret_id` INT NOT NULL , `type_id` INT NOT NULL , FOREIGN KEY (`pret_id` ) REFERENCES `pret` (`id`), FOREIGN KEY (`type_id` ) REFERENCES `evenement_type` (`id`) )") )
        qDebug() << "Error on 'pret' TABLE echeance" << q.lastError().text();

    q.prepare("insert into evenement_type (libelle) values (?)");
    QVariantList events;
    events << QString::fromUtf8("Différé total")
	<< QString::fromUtf8("Différé partiel (paiement des intérêts)")
	<< QString::fromUtf8("Modification de mensualité")
	<< QString::fromUtf8("Remboursement anticipé")
	/* << QString::fromUtf8("Report de mensualité");
	 << QString::fromUtf8("Changement de taux") */;
    q.addBindValue(events);
    if (!q.execBatch())
        qDebug() << "Error while inserting date into 'evenement_type' TABLE" << q.lastError().text();;
	
    qDebug("Database schema created!");
    return TRUE;
}


/**
 *  Ajouter un pret dans la table correspondante.
 */
int ajouterPret(QString libelle, int capitalEmprunte, float tauxInteret, float tauxAssurance, int duree, float mensualite, QDate dateDebloquage) {
    QSqlQuery query(QString::null);

    query.prepare("insert into pret(libelle, capitalEmprunte, tauxInteret, tauxAssurance, duree, mensualite, date_deboquage) values (:libelle, :capitalEmprunte, :tauxInteret, :tauxAssurance, :duree, :mensualite, :dateDebloquage)");
    query.bindValue(":libelle", libelle);
    query.bindValue(":capitalEmprunte", capitalEmprunte);
    query.bindValue(":tauxInteret", tauxInteret);
    query.bindValue(":tauxAssurance", tauxAssurance);
    query.bindValue(":duree", duree);
    query.bindValue(":mensualite", mensualite);
    query.bindValue(":dateDebloquage", dateDebloquage);
    if ( !query.exec() ) {
        qDebug() << "Error while inserting data into 'pret' table:" << query.lastError().text();
    return -1;
    } else {
        return query.lastInsertId().toInt();
    }
}

/**
 * Creer un nouvel échéancier.
 */
int creerEcheancier(QString libelle, QDate dateCreation, int pretId) {
    QSqlQuery query(QString::null);

    query.prepare("insert into echeancier(libelle, date_creation, isSimulation, pret_id) values (:libelle, :date, 0, :pret_id)");
    query.bindValue(":libelle", libelle);
    query.bindValue(":date", dateCreation);
    query.bindValue(":pret_id", pretId);
    if ( !query.exec() ) {
        qDebug() << "Error while inserting data into 'echeancier' table:" << query.lastError().text();
        return -1;
    } else {
        return query.lastInsertId().toInt();
    }
}


/**
 *  Ajouter une echeance dans la table correspondante.
 */
void ajouterEcheance(int echeancierId, QDate dateEcheance, float capitalRestantDu, float mensualite,
                     float capital, float interets, float assurance) {
    QSqlQuery query(QString::null);

    query.prepare("insert into echeance(date, capitalRestantDu, capitalAmorti, interets, assurance, mensualite, echeancier_id) values (:date, :capitalRestantDu, :capitalAmorti, :interets, :assurance, :mensualite, :echeancier_id)");
    query.bindValue(":date", dateEcheance);
    query.bindValue(":capitalRestantDu", capitalRestantDu);
    query.bindValue(":capitalAmorti", capital);
    query.bindValue(":interets", interets);
    query.bindValue(":assurance", assurance);
    query.bindValue(":mensualite", mensualite);
    query.bindValue(":echeancier_id", echeancierId);
    if ( !query.exec() )
        qDebug() << "Error while inserting data into 'echeance' table:" << query.lastError().text();
}

/**
 *  Initialiser le modèle de l'échéancier en vue de l'affichage
 */
void initializeEcheancierModel(QSqlQueryModel *model, int echeancierId)
{
    QSqlQuery query;

    query.prepare("select date, capitalRestantDu, capitalAmorti, interets, assurance, mensualite from echeance where echeancier_id = :id");
    query.bindValue(":id", echeancierId);
    query.exec();
    model->setQuery(query);
}

/**
 * Ajouter un évènement au pret en cours.
 */
void ajouterEvenement(QDate date, QString libelle, float valeur, int pretId, int typeId, bool isPermanent) {
    QSqlQuery query;

    query.prepare("insert into evenement(date, libelle, valeur , isPermanent, type_id, pret_id) values (:date, :libelle, :valeur, :isPermanent, :type_id, :pret_id)");
    query.bindValue(":date", date);
    query.bindValue(":libelle", libelle);
    query.bindValue(":valeur", valeur);
    query.bindValue(":isPermanent", isPermanent);
    query.bindValue(":type_id", typeId);
    query.bindValue(":pret_id", pretId);
    if ( !query.exec() )
        qDebug() << "Error while inserting data into 'evenement' table:" << query.lastError().text();
}

/**
 * Supprimer un évènement au pret en cours.
 */
void supprimerEvenement(int eventId, int pretId) {
    QSqlQuery query;

    std::cout << "delete from evenement where id = " << eventId << " and pret_id = " << pretId << std::endl;

    query.prepare("delete from evenement where id = :event_id");// and pret_id = :pret_id)");
    query.bindValue(":event_id", eventId);
//    query.bindValue(":pret_id", pretId);
    if ( !query.exec() )
        qDebug() << "Error while deleting data from 'evenement' table:" << query.lastError().text();
}

/**
 *  Initialiser le modèle des évènements en vue de l'affichage
 */
void initializeEvenementModel(QSqlQueryModel *model, int pretId)
{
    QSqlQuery query;

    query.prepare("select date, t.libelle as type, valeur, isPermanent, e.libelle from evenement e, evenement_type t where pret_id = :id and e.type_id = t.id");
    query.bindValue(":id", pretId);
    query.exec();
    model->setQuery(query);
}

/**
 *  Initialiser le modèle des types d'évènements en vue de l'affichage
 */
void initializeTypesEvenementModel(QSqlQueryModel *model)
{
    QSqlQuery query;

    query.exec("select id, libelle from evenement_type");
    model->setQuery(query);
}

/**
 *  Récupérer la liste des événements déclarés.
 */
QList<Evenement> getEvenements(int pretId)
{
    QSqlQuery query;

    query.prepare("select date, type_id, valeur, isPermanent from evenement where pret_id = :id");
    query.bindValue(":id", pretId);
    query.exec();

    // Pour chaque evenement, creer un objet et l'ajouter a la liste
    QList<Evenement> evts;
    while (query.next()) {
        QDate date = query.value(0).toDate();
        Evenement::EvenementType type = (Evenement::EvenementType) query.value(1).toInt();
        float valeur = query.value(2).toFloat();
        bool isPermanent = query.value(3).toBool();
        Evenement evt = Evenement(date, valeur, type, isPermanent);
        evts << evt;
    }

    std::cout << "nb evenements du pret #" << pretId << " (getEvenements) = " << evts.count() << std::endl;
    return evts;
}

/**
 * Calculer la somme des mensualités versées.
 */
float getSommeMensualites(int echeancierId) {
    QSqlQuery query;

    query.prepare("select SUM(mensualite) from echeance where echeancier_id = :id");
    query.bindValue(":id", echeancierId);
    if ( !query.exec() ) {
        qDebug() << "Error in getSommeMensualites() :" << query.lastError().text();
        return -1;
    } else {
        query.next();
        return query.value(0).toFloat();
    }
}

/**
 * Calculer les intérêts payés sur l'ensemble du prêt.
 */
float getSommeInterets(int echeancierId) {
    QSqlQuery query;

    query.prepare("select SUM(interets) from echeance where echeancier_id = :id");
    query.bindValue(":id", echeancierId);
    if ( !query.exec() ) {
        qDebug() << "Error in getSommeInterets() :" << query.lastError().text();
        return -1;
    } else {
        query.next();
        return query.value(0).toFloat();
    }
}

/**
 * Calculer la somme des remboursements anticipes.
 */
float getSommeRbtAnticipes(int pretId) {
    QSqlQuery query;

    query.prepare("select SUM(valeur) from evenement where pret_id = :id and type_id = :type");
    query.bindValue(":id", pretId);
    query.bindValue(":type", Evenement::RBT_ANTICIPE);
    if ( !query.exec() ) {
        qDebug() << "Error in getSommeRbtAnticipes() :" << query.lastError().text();
        return -1;
    } else {
        query.next();
        return query.value(0).toFloat();
    }
}

/**
 * Calculer le coût de l'assurance sur l'ensemble du prêt.
 */
float getCoutAssurance(int echeancierId) {
    QSqlQuery query;

    query.prepare("select SUM(assurance) from echeance where echeancier_id = :id");
    query.bindValue(":id", echeancierId);
    if ( !query.exec() ) {
        qDebug() << "Error in getCoutAssurance() :" << query.lastError().text();
        return -1;
    } else {
        query.next();
        return query.value(0).toFloat();
    }
}
