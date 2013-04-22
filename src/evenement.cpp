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

QDate Evenement::getDate() {
    return date;
}

float Evenement::getValeur() {
    return valeur;
}

Evenement::EvenementType Evenement::getType() {
    return type;
}

bool Evenement::isPermanent() {
    return permanent;
}

void Evenement::persist() {
}
