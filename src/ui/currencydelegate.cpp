#include "currencydelegate.h"

CurrencyDelegate::CurrencyDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void CurrencyDelegate::paint ( QPainter *painter,
                               const QStyleOptionViewItem & option,
                               const QModelIndex & index ) const {
    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

    float currency = index.model()->data(index, Qt::DisplayRole).toFloat();

    drawDisplay(painter, myOption, myOption.rect, QString("%1%L2").arg(QChar(8364)).arg(currency, 0, 'f', 2));
    drawFocus(painter, option, myOption.rect);
}
