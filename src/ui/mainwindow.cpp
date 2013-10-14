#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../db.h"
#include "../dbdumper.h"
#include "../pret.h"
#include "../projet.h"
#include "pretwidget.h"
#include <QtSql>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->projet = new Projet();
    Pret* pret = new Pret();
    PretWidget *tab = new PretWidget(pret, ui->tabWidget);
    ui->tabWidget->insertTab(0, tab, QString::fromUtf8("Pret #1"));
    ui->tabWidget->setCurrentIndex(0);
    this->projet->addPret(pret);
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

/*void MainWindow::initTable(const QString &title, QSqlQueryModel *model) {
    ui->tableView->setModel(model);
    ui->tableView->setWindowTitle(title);
}*/
