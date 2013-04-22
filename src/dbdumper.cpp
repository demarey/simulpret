#include "visitor.h"
#include "db.h"
#include "dbdumper.h"

DBdumper::DBdumper() { }

void DBdumper::visit(Projet* projet) {
    // ajouterProjet();
}

void DBdumper::visit(Pret* pret) {
    ajouterPret(pret->getId(), "Pret", pret->getCapitalEmprunte(), pret->getTauxInteret(), pret->getTauxAssurance(), pret->duree, pret->mensualite, pret->getDateDebloquage());
}

void DBdumper::visit(Evenement* evt)
{
    evt;
}
