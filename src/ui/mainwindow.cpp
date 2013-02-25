#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../pret.h"
#include "../db.h"
#include "currencydelegate.h"
#include "evenementdialog.h"
#include <QtSql>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QObject::connect(ui->boutonAjouterEvenement, SIGNAL(clicked()), this, SLOT(on_boutonAjouterEvenement_clicked()));

    ui->dateDebloquage->setDate( QDate::currentDate() );

    ui->tableView->setItemDelegateForColumn(1, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(2, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(3, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(4, new CurrencyDelegate());
    ui->tableView->setItemDelegateForColumn(5, new CurrencyDelegate());

    ui->groupeCoutCredit->setVisible(false);

    this->echeancierCourant = -1;

    float mensualite =  ui->mensualite->text().toFloat();
    if (mensualite > 0) {
        this->pret = new Pret( ui->capitalEmprunte->text().toLong(),
                       ui->tauxPret->text().toFloat(),
                       mensualite,
                       ui->tauxAssurance->text().toFloat(),
                       ui->dateDebloquage->date() );
    } else {
        this->pret = new Pret( ui->capitalEmprunte->text().toLong(),
                       ui->tauxPret->text().toFloat(),
                       ui->duree->text().toInt(),
                       ui->tauxAssurance->text().toFloat(),
                       ui->dateDebloquage->date() );
    }
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

void createView(const QString &title, QSqlQueryModel *model);
void initializeModel(QSqlQueryModel *model);

void MainWindow::on_simulationButton_clicked() {
    std::cout << "simulation button clicked!" << std::endl;
    std::cout << "capital emprunte =" << ui->capitalEmprunte->text().toLong() << std::endl;
    std::cout << "taux =" << ui->tauxPret->text().toFloat() << std::endl;

    // tester valeurs MAJ dans ui et MAJ attributs pret
    float mensualite =  ui->mensualite->text().toFloat();
    if (mensualite > 0) {
        this->pret->majPret( ui->capitalEmprunte->text().toLong(),
                             ui->tauxPret->text().toFloat(),
                             mensualite,
                             ui->tauxAssurance->text().toFloat(),
                             ui->dateDebloquage->date() );
    } else {
        this->pret->majPret( ui->capitalEmprunte->text().toLong(),
                             ui->tauxPret->text().toFloat(),
                             ui->duree->text().toInt(),
                             ui->tauxAssurance->text().toFloat(),
                             ui->dateDebloquage->date() );
    }


    std::cout << this->pret->toString().toStdString() << std::endl;

    this->echeancierCourant = this->pret->calculerEcheancier();
    QSqlQueryModel *echeancierModel = new QSqlQueryModel();
    initializeEcheancierModel(echeancierModel, this->echeancierCourant);
//    createView(QObject::tr("Simulation de pret"), plainModel);
    ui->tableView->setModel(echeancierModel);
//    ui->labelCoutInteret->setText( QString("%1%L2").arg(QChar(8364)).arg(getSommeInterets(this->echeancierCourant), 0, 'f', 2) );
    ui->labelCoutInteret->setText( QString("%1%L2").arg(QChar(8364)).arg(this->pret->getCoutTotalCredit(this->echeancierCourant), 0, 'f', 2) );
    ui->labelCoutAssurance->setText( QString("%1%L2").arg(QChar(8364)).arg(getCoutAssurance(this->echeancierCourant), 0, 'f', 2) );
    ui->groupeCoutCredit->setVisible(true);
 }

void MainWindow::on_boutonAjouterEvenement_clicked() {
    std::cout << "boutonAjouterEvenement clicked!" << std::endl;

    EvenementDialog *dialog = new EvenementDialog();
    dialog->exec();
    delete dialog;

//    ajouterEvenement(QDate::currentDate(), "differet de 18 mois", 18, this->pret->getId(), Evenement::DIFFERE_PARTIEL);
//    ajouterEvenement(QDate::currentDate(), "differet de 18 mois", 18, this->pret->getId(), Evenement::DIFFERE_TOTAL);
//    ajouterEvenement(QDate::currentDate().addYears(1), "augmentation mensualite a 2000 euros", 2000, this->pret->getId(), Evenement::MAJ_MENSUALITE);
//    ajouterEvenement(QDate::currentDate().addYears(1), "rbt anticipe de 2500 euros", 2500, this->pret->getId(), Evenement::RBT_ANTICIPE);

    QSqlQueryModel *evenementModel = new QSqlQueryModel();
    initializeEvenementModel(evenementModel, this->pret->getId());
    ui->evenementsTable->setModel(evenementModel);
    std::cout << "pretId=" << this->pret->getId() << std::endl;
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
