#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../dbdumper.h"
#include "../pret.h"
#include "../projet.h"
#include "../db.h"
#include "currencydelegate.h"
#include "evenementdialog.h"
#include "evenementmodel.h"
#include <QtSql>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dateDebloquage->setDate( QDate::currentDate() );

    ui->tableView->setItemDelegateForColumn(1, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(2, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(3, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(4, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(5, new CurrencyDelegate());

    ui->groupeCoutCredit->setVisible(false);

    this->projet = new Projet();
    this->echeancierCourant = -1;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Ask to save before exiting.
 *
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
 /*   if (QMessageBox::Yes == QMessageBox::question(this, "Quitter", "Voulez-vous sauvegarder avant de quitter?", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) ) {
        QString filename = QFileDialog::getSaveFileName(this, "Sauvegarder le projet", ".smp", ".smp");
        //save(filename);
        // use savepoints? http://www.sqlite.org/lang_savepoint.html
    } */
    closeDB();
    event->accept();
}


void createView(const QString &title, QSqlQueryModel *model);
void initializeModel(QSqlQueryModel *model);

/**
 * @brief slot executed when the Nouveau_projet menu is triggered.
 */
void MainWindow::on_actionNouveau_projet_triggered() {
    this->projet = new Projet();
}

/**
 * @brief slot executed when the Sauvegarder menu is triggered.
 */
void MainWindow::on_actionSauvegarder_triggered() {
    QString filename = QFileDialog::getSaveFileName(this, "Sauvergarder le projet", ".smp", ".smp");
    newDB(filename);
    DBdumper dbdumper;
    this->projet->accept( dbdumper );
}

void MainWindow::majEcheancier() {
    this->echeancierCourant = this->projet->getPretCourant()->calculerEcheancier();
    QSqlQueryModel *echeancierModel = new QSqlQueryModel();
    initializeEcheancierModel(echeancierModel, this->echeancierCourant);
    //    createView(QObject::tr("Simulation de pret"), plainModel);
    ui->tableView->setModel(echeancierModel);
}

void MainWindow::on_simulationButton_clicked() {
    Pret *pret;

    std::cout << "simulation button clicked!" << std::endl;
    std::cout << "capital emprunte =" << ui->capitalEmprunte->text().toLong() << std::endl;
    std::cout << "taux =" << ui->tauxPret->text().toFloat() << std::endl;

    // tester valeurs MAJ dans ui et MAJ attributs pret
    float mensualite =  ui->mensualite->text().toFloat();
    if (mensualite > 0) {
        pret = new Pret( ui->capitalEmprunte->text().toLong(),
                       ui->tauxPret->text().toFloat(),
                       mensualite,
                       ui->tauxAssurance->text().toFloat(),
                       ui->dateDebloquage->date() );
    } else {
        pret = new Pret( ui->capitalEmprunte->text().toLong(),
                       ui->tauxPret->text().toFloat(),
                       ui->duree->text().toInt(),
                       ui->tauxAssurance->text().toFloat(),
                       ui->dateDebloquage->date() );
    }
    this->projet->addPret( pret );

    std::cout << pret->toString().toStdString() << std::endl;

    this->majEcheancier();
//    ui->labelCoutInteret->setText( QString("%1%L2").arg(QChar(8364)).arg(getSommeInterets(this->echeancierCourant), 0, 'f', 2) );
    ui->labelCoutInteret->setText( QString("%1%L2").arg(QChar(8364)).arg(pret->getCoutTotalCredit(this->echeancierCourant), 0, 'f', 2) );
    ui->labelCoutAssurance->setText( QString("%1%L2").arg(QChar(8364)).arg(getCoutAssurance(this->echeancierCourant), 0, 'f', 2) );
    ui->groupeCoutCredit->setVisible(true);
 }

void MainWindow::on_boutonAjouterEvenement_clicked() {
    std::cout << "boutonAjouterEvenement clicked!" << std::endl;

    EvenementDialog *dialog = new EvenementDialog(this->projet->getPrets().last());
    dialog->exec();
    delete dialog;

    EvenementModel *evenementModel = new EvenementModel( this->projet->getPrets().last()->evenements );
    ui->evenementsTable->setModel( evenementModel );
    std::cout << "pretId=" << this->projet->getPrets().last()->getId() << std::endl;
    this->majEcheancier();
}

void MainWindow::on_boutonSupprimerEvenement_clicked() {
    std::cout << "boutonSupprimerEvenement clicked!" << std::endl;
//    ui->evenementsTable->model()->removeRow( ui->evenementsTable->currentIndex() );

    QItemSelectionModel* selection =  ui->evenementsTable->selectionModel();
    Q_ASSERT( selection != 0 );

    if ( selection->hasSelection() )  {
       QModelIndexList selectedItems = selection->selectedIndexes();
        for (int i = 0; i < selectedItems.count(); i++) {
            // TODO : trouver un moyen de recuperer la valeur de l'index dans la table
//            supprimerEvenement(selectedItems.at(i).data().toInt() + 1, this->pret->getId());
        }
    }
    // MAJ ui
}

/*void MainWindow::initTable(const QString &title, QSqlQueryModel *model) {
    ui->tableView->setModel(model);
    ui->tableView->setWindowTitle(title);
}*/
