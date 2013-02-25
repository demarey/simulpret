#ifndef CURRENCYDELEGATE_H
#define CURRENCYDELEGATE_H

#include <QItemDelegate>

class CurrencyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CurrencyDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    
public slots:
    
};

#endif // CURRENCYDELEGATE_H
