#include <QDebug>
#include "projet.h"
#include "db.h"

Projet::Projet() {
}

void Projet::addPret(Pret *unPret) {
    prets.append(unPret);
    unPret->setId( prets.size() );
}

QList<Pret *> Projet::getPrets() {
    return prets;
}

void Projet::accept(Visitor &visitor) {
    visitor.visit(this);
    QListIterator<Pret *> iterator( this->getPrets() );
     while (iterator.hasNext())
         visitor.visit( iterator.next() );
        // iterator.next()->accept(visitor);
}
