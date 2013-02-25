#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QSqlQueryModel>
#include "../pret.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    void initTable(const QString &title, QSqlQueryModel *model) {

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_simulationButton_clicked();
    void on_boutonAjouterEvenement_clicked();
    void on_boutonSupprimerEvenement_clicked();

private:
    Ui::MainWindow *ui;
    Pret *pret;
    int echeancierCourant;
};

#endif // MAINWINDOW_H
