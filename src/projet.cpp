#include <QDebug>
#include "projet.h"
#include "db.h"

Projet::Projet() {
}

void Projet::addPret(Pret *unPret) {
    prets.append(unPret);
    unPret->setId(prets.size());
    this->setPretCourantIdx(prets.size() - 1);
}

QList<Pret *> Projet::getPrets() {
    return prets;
}

Pret* Projet::getPretCourant() {
    if ((pretCourantIdx >= 0) && (pretCourantIdx < prets.size()))
        return prets.at(pretCourantIdx);
    else
        return NULL;
}

int Projet::setPretCourantIdx(int pretCourantIdx) {
    this->pretCourantIdx = pretCourantIdx;
}

void Projet::accept(Visitor &visitor) {
    visitor.visit(this);
    QListIterator<Pret *> iterator( this->getPrets() );
     while (iterator.hasNext())
         visitor.visit( iterator.next() );
        // iterator.next()->accept(visitor);
}
