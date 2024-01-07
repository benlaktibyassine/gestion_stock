#ifndef ADDPRODUIT_H
#define ADDPRODUIT_H

#include <QDialog>
#include "Mydb.h"

namespace Ui {
class AddProduit;
}

class AddProduit : public QDialog
{
    Q_OBJECT
signals:
    void productAdded();




public:
    explicit AddProduit(QWidget *parent = nullptr);
    ~AddProduit();

private slots:
    void on_btnAdd_clicked();

private:
    Ui::AddProduit *ui;
};

#endif // ADDPRODUIT_H
