#include "evenementdialog.h"
#include "ui_evenementdialog.h"
#include "../db.h"
#include "../evenement.h"
#include <QtSql>
#include <QDate>
#include <iostream>


EvenementDialog::EvenementDialog(Pret *pret, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EvenementDialog)
{
    ui->setupUi(this);

    // intialiser la liste des types d'évènements
    QSqlQueryModel *typesEvenementModel = new QSqlQueryModel();
    initializeTypesEvenementModel(typesEvenementModel);
    ui->evtTypeComboBox->setModel(typesEvenementModel);
    ui->evtTypeComboBox->setModelColumn(1); // cacher la colonne id

    ui->dateInput->setDate( QDate::currentDate() );
    this->pret = pret;
}

EvenementDialog::~EvenementDialog()
{
    delete ui;
}


void EvenementDialog::accept ()
{
    QString libelle = ui->libelleInput->text();
    QDate date = ui->dateInput->date();
    float valeur = ui->valeurInput->text().toFloat();
    int currentIndex = ui->evtTypeComboBox->currentIndex();
    int idIndex = 0; //ui->evtTypeComboBox->model()->index("id");
    Evenement::EvenementType type = (Evenement::EvenementType) ui->evtTypeComboBox->model()->data(ui->evtTypeComboBox->model()->index(currentIndex, idIndex)).toInt();
    bool permanent = true;

    Evenement *evt = new Evenement(libelle, date, valeur, type, permanent);
    std::cout << "event " << evt->getType() << " valeur:" << evt->getValeur() << std::endl;
    pret->ajouterEvenement(evt);

//    ajouterEvenement(ui->dateInput->date(), ui->libelleInput->text(), value, pretId, eventTypeId, true);
//    std::cout << "accept eventtype=" << eventTypeId << std::endl;
    std::cout << "accept eventtype=" << type << std::endl;
    QDialog::accept();
}
