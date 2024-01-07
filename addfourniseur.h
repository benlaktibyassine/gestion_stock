#ifndef ADDFOURNISEUR_H
#define ADDFOURNISEUR_H

#include <QDialog>
#include "Mydb.h"

namespace Ui {
class AddFourniseur;
}

class AddFourniseur : public QDialog
{
    Q_OBJECT

public:
    explicit AddFourniseur(QWidget *parent = nullptr);
    ~AddFourniseur();

private slots:
    void on_btnAdd_clicked();

private:
    Ui::AddFourniseur *ui;
};

#endif // ADDFOURNISEUR_H
