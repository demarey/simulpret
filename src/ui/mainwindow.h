#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "../projet.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_actionNouveau_projet_triggered();
    void on_actionSauvegarder_triggered();

private:
    Ui::MainWindow *ui;
    Projet *projet;
};

#endif // MAINWINDOW_H
