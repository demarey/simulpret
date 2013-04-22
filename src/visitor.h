#ifndef SIMULPRETVISITOR_H
#define SIMULPRETVISITOR_H

#include "pret.h"
#include "projet.h"
#include "evenement.h"

// forward declaration to avoid recursion loop on included files
class Projet;
class Pret;
class Evenement;

class Visitor
{
  public:
    virtual void visit(Projet*) = 0;
    virtual void visit(Pret*) = 0;
    virtual void visit(Evenement*) = 0;
};

#endif // SIMULPRETVISITOR_H
