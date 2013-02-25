#ifndef EVENEMENTDIALOG_H
#define EVENEMENTDIALOG_H

#include <QDialog>

namespace Ui {
class EvenementDialog;
}

class EvenementDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EvenementDialog(QWidget *parent = 0);
    ~EvenementDialog();
    
private:
    Ui::EvenementDialog *ui;
private slots:
    void accept();
};

#endif // EVENEMENTDIALOG_H
