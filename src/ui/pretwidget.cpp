#include <iostream>
#include <QtSql>

#include "../db.h"
#include "currencydelegate.h"
#include "pretwidget.h"
#include "evenementdialog.h"
#include "evenementmodel.h"
#include "ui_pretwidget.h"

PretWidget::PretWidget(Pret* pret, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PretWidget),
    pret(*pret)
{
    ui->setupUi(this);
    this->echeancierCourant = -1;

    ui->dateDebloquage->setDate( QDate::currentDate() );
    ui->tableView->setItemDelegateForColumn(1, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(2, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(3, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(4, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(5, new CurrencyDelegate());
//    ui->groupeCoutCredit->setVisible(false);

}

PretWidget::~PretWidget()
{
    delete ui;
}

void PretWidget::on_boutonAjouterEvenement_clicked() {
    std::cout << "boutonAjouterEvenement clicked!" << std::endl;

    EvenementDialog *dialog = new EvenementDialog(this->pret);
    dialog->exec();
    delete dialog;

    EvenementModel *evenementModel = new EvenementModel( this->pret.evenements );
    ui->evenementsTable->setModel( evenementModel );
    this->majEcheancier();
}

void PretWidget::on_boutonSupprimerEvenement_clicked() {
    std::cout << "boutonSupprimerEvenement clicked!" << std::endl;
//    ui->evenementsTable->model()->removeRow( ui->evenementsTable->currentIndex() );

//    QItemSelectionModel* selection =  ui->evenementsTable->selectionModel();
//    Q_ASSERT( selection != 0 );

//    if ( selection->hasSelection() )  {
//       QModelIndexList selectedItems = selection->selectedIndexes();
//        for (int i = 0; i < selectedItems.count(); i++) {
            // TODO : trouver un moyen de recuperer la valeur de l'index dans la table
//            supprimerEvenement(selectedItems.at(i).data().toInt() + 1, this->pret->getId());
//        }
//    }
    // MAJ ui
}

void PretWidget::majEcheancier() {
    this->echeancierCourant = pret.calculerEcheancier();
    QSqlQueryModel *echeancierModel = new QSqlQueryModel();
    initializeEcheancierModel(echeancierModel, this->echeancierCourant);
    // createView(QObject::tr("Simulation de pret"), echeancierModel);
    ui->tableView->setModel(echeancierModel);
}

void PretWidget::on_calculerButton_clicked() {
    float sommeInterets, coutAssurance;

    // TODO: tester valeurs MAJ dans ui et MAJ attributs pret
    pret.initialize( ui->capitalEmprunte->text().toLong(),
                      ui->tauxPret->text().toFloat() / 100.0 ,
                      ui->duree->text().toInt(),
                      ui->mensualite->text().toFloat(),
                      ui->assTaux1->text().toFloat() / 100.0,
                      ui->dateDebloquage->date());

    this->majEcheancier();

    sommeInterets = getSommeInterets(this->echeancierCourant);
    coutAssurance = getCoutAssurance(this->echeancierCourant);
    ui->labelTotalInterets->setText( QString("%1%L2").arg(QChar(8364)).arg(sommeInterets, 0, 'f', 2) );
//    ui->labelCoutInteret->setText( QString("%1%L2").arg(QChar(8364)).arg(pret->getCoutTotalCredit(this->echeancierCourant), 0, 'f', 2) );
    ui->labelCoutAssurance->setText( QString("%1%L2").arg(QChar(8364)).arg(coutAssurance, 0, 'f', 2) );
    ui->labelCoutTotal->setText( QString("%1%L2").arg(QChar(8364)).arg(sommeInterets+coutAssurance, 0, 'f', 2) );
//    ui->groupeCoutCredit->setVisible(true);
}

