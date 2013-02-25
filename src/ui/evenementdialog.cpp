#include "evenementdialog.h"
#include "ui_evenementdialog.h"
#include "../db.h"
#include <QtSql>
#include <QDate>
#include <iostream>


EvenementDialog::EvenementDialog(QWidget *parent) :
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

    // TODO : maj le libelle valeur en fonction du type choisi

}

EvenementDialog::~EvenementDialog()
{
    delete ui;
}


void EvenementDialog::accept ()
{
    int pretId = 1;
    float value = ui->valeurInput->text().toFloat();
    int currentIndex = ui->evtTypeComboBox->currentIndex();
    int idIndex = 0; //ui->evtTypeComboBox->model()->index("id");
    int eventTypeId = ui->evtTypeComboBox->model()->data(ui->evtTypeComboBox->model()->index(currentIndex, idIndex)).toInt();

    ajouterEvenement(ui->dateInput->date(), ui->libelleInput->text(), value, pretId, eventTypeId, true);
    std::cout << "accept eventtype=" << eventTypeId << std::endl;
    QDialog::accept();
}
