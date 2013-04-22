class DBdloder {
public:
    DBloader();
public:
    void loadProjet(Projet* projet) {
        // TODO: charger infos relatives au projet
        QSqlQuery q(QString::null, db);

        if ( !q.exec("SELECT `id`, `libelle`, `capitalEmprunte`, `tauxInteret`, `tauxAssurance`, `duree`, `mensualite`, `date_deboquage`) FROM `pret` SORT BY `id` ASC") )
            qDebug() << "Error on 'pret' selection" << q.lastError().text();

        QListIterator<Pret *> iterator(projet->prets);
         while (iterator.hasNext())
             visitPret( iterator.next() );
    }

    void loadPret(Pret* pret) {
        ajouterPret(pret->id, "Pret", pret->capitalEmprunte, pret->tauxInteret, pret->tauxAssurance, pret->duree, pret->mensualite, pret->dateDebloquage);

        QListIterator<Evenement *> iterator(this->evenements);
         while (iterator.hasNext()) {
            visitEvenement( iterator.next() );
         }
    }

    virtual void loadEvenement(Evenement* evt)
    {
        evt;
    }
};
