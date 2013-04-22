#include <QtGui/QApplication>
#include "ui/mainwindow.h"
#include "db.h"
#include <QtSql>
#include <QtGui/qtableview.h>
#include <iostream>


void initializeModel(QSqlQueryModel *model)
{
    model->setQuery("select * from people");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));

    for (int i = 0; i < model->rowCount(); ++i) {
             int id = model->record(i).value("id").toInt();
             QString name = model->record(i).value("name").toString();
    }
}

void createView(const QString &title, QSqlQueryModel *model)
{
    static int offset = 0;

    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    view->move(100 + offset, 100 + offset);
    offset += 20;
    view->show();

    std::cout << "createView\n";
    for (int i = 0; i < model->rowCount(); ++i) {
        int id = model->record(i).value("id").toInt();
        QString name = model->record(i).value("name").toString();
        std::cout << name.toStdString() << "\n";
    }
    std::cout.flush();
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    createInMemoryDB();

    MainWindow window;
    QSqlQueryModel plainModel;
//    initializeModel(&plainModel);
//    createView(QObject::tr("Simulation de prêt"), &plainModel);


    window.show();
    return app.exec();
}
