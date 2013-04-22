#ifndef PROJET_H
#define PROJET_H

#include <QList>
#include "pret.h"
#include "visitor.h"

// forward declaration to avoid recursion loop on included files
class Visitor;
class Pret;

class Projet
{
    QList<Pret *> prets;
public:
    Projet();

    /** Ajouter un pret au projet */
    void addPret(Pret *unPret);
    /** Récupérer la liste des prêts relatifs au projet. */
    QList<Pret *> getPrets();
    /** Pattern visiteur. */
    void accept(Visitor &visitor);
};

#endif // PROJET_H
