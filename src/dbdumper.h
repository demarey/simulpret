#ifndef DBDUMPER_H
#define DBDUMPER_H

#include "visitor.h"

class DBdumper : public Visitor
{
public:
    DBdumper();
public:
    virtual void visit(Projet* projet);
    virtual void visit(Pret* pret);
    virtual void visit(Evenement* evt);
};

#endif // DBDUMPER_H
