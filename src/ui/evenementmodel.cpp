#include "evenementmodel.h"

EvenementModel::EvenementModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

EvenementModel::EvenementModel(QList<Evenement> events, QObject *parent)
: QAbstractTableModel(parent)
{
    this->events = events;
}


int EvenementModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return events.size();
}

int EvenementModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant EvenementModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= events.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        Evenement event = events.at(index.row());

        switch( index.column() ) {
            case 0:
                return event.getDate();
            case 1:
                return event.getTypeAsString();
            case 2:
                return event.getValeur();
            case 3:
                if (event.isPermanent())
                    return "Vrai";
                else
                    return "Faux";
          case 4:
                return event.getLibelle();
        }
    }
    return QVariant();
}

QVariant EvenementModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         switch (section) {
             case 0:
                 return tr("Date");
             case 1:
                 return tr("Type");
             case 2:
                 return tr("Valeur");
             case 3:
                 return tr("Permanent");
             case 4:
                 return tr("Libelle");
             default:
                 return QVariant();
         }
     }
     return QVariant();
 }

Qt::ItemFlags EvenementModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
