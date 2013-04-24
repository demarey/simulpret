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
    /** positionner le prêt courant */
    int setPretCourantIdx(int pretCourantIdx);
    /** Récupérer le prêt courant */
    Pret* getPretCourant();
    /** Pattern visiteur. */
    void accept(Visitor &visitor);
private:
    int pretCourantIdx;
};

#endif // PROJET_H
