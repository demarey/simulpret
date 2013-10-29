#include "evenement.h"

Evenement::Evenement() {}

Evenement::Evenement(QString libelle, QDate date, float valeur, EvenementType type, bool isPermanent)
{
    this->libelle = libelle;
    this->date = date;
    this->valeur = valeur;
    this->type = type;
    this->permanent = isPermanent;
}

QString Evenement::getLibelle() const {
    return libelle;
}

QDate Evenement::getDate() const {
    return date;
}

float Evenement::getValeur() const {
    return valeur;
}

Evenement::EvenementType Evenement::getType() const {
    return type;
}

QString Evenement::getTypeAsString() const {
    switch(type) {
        case Evenement::DIFFERE_TOTAL : return QString::fromUtf8("Différé total");
        case Evenement::DIFFERE_PARTIEL : return QString::fromUtf8("Différé partiel (paiement des intérêts)");
        case Evenement::MAJ_MENSUALITE : return QString::fromUtf8("Modification de mensualité");
        case Evenement::RBT_ANTICIPE : return QString::fromUtf8("Remboursement anticipé");
        case Evenement::REPORT_MENSUALITE : return QString::fromUtf8("Report de mensualité");
        case Evenement::MAJ_TAUX : return QString::fromUtf8("Changement de taux");
    }
}

bool Evenement::isPermanent() const {
    return permanent;
}

void Evenement::persist() {
}
