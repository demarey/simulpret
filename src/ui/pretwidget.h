#ifndef PRETWIDGET_H
#define PRETWIDGET_H

#include "../pret.h"
#include <QWidget>

namespace Ui {
    class PretWidget;
}

class PretWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PretWidget(Pret* pret, QWidget *parent = 0);
    ~PretWidget();
    
protected:
    void majEcheancier();

private slots:
    void on_boutonAjouterEvenement_clicked();
    void on_boutonSupprimerEvenement_clicked();
    void on_calculerButton_clicked();

private:
    Ui::PretWidget *ui;
    Pret& pret;
    int echeancierCourant;
};

#endif // PRETWIDGET_H
