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

QString Evenement::getLibelle() {
    return libelle;
}

QDate Evenement::getDate() {
    return date;
}

float Evenement::getValeur() {
    return valeur;
}

Evenement::EvenementType Evenement::getType() {
    return type;
}

QString Evenement::getTypeAsString() {
    switch(type) {
        case Evenement::DIFFERE_TOTAL : return QString::fromUtf8("Différé total");
        case Evenement::DIFFERE_PARTIEL : return QString::fromUtf8("Différé partiel");
        case Evenement::MAJ_MENSUALITE : return QString::fromUtf8("maj mensualité");
        case Evenement::RBT_ANTICIPE : return QString::fromUtf8("remboursement anticipé");
        case Evenement::REPORT_MENSUALITE : return QString::fromUtf8("report de mensualité");
        case Evenement::MAJ_TAUX : return QString::fromUtf8("maj taux");
    }
}

bool Evenement::isPermanent() {
    return permanent;
}

void Evenement::persist() {
}
