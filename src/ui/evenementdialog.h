#ifndef EVENEMENTDIALOG_H
#define EVENEMENTDIALOG_H

#include <QDialog>
#include "../pret.h"

namespace Ui {
class EvenementDialog;
}

class EvenementDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EvenementDialog(Pret& pret, QWidget *parent = 0);
    ~EvenementDialog();
    
private:
    Ui::EvenementDialog *ui;
    Pret& pret; // pret courant auquel se rapporte l'évènement à ajouter
private slots:
    void accept();
};

#endif // EVENEMENTDIALOG_H
