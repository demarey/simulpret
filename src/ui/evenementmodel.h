#ifndef EVENEMENTMODEL_H
#define EVENEMENTMODEL_H

#include <QAbstractTableModel>
#include "../evenement.h"

class EvenementModel : public QAbstractTableModel
{
    Q_OBJECT

public:
     EvenementModel(QObject *parent=0);
     EvenementModel(QList<Evenement> events, QObject *parent=0);

     int rowCount(const QModelIndex &parent) const;
     int columnCount(const QModelIndex &parent) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;

private:
     QList<Evenement> events;
};

#endif // EVENEMENTMODEL_H
