#include "addfourniseur.h"
#include "ui_addfourniseur.h"

AddFourniseur::AddFourniseur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFourniseur)
{
    ui->setupUi(this);
}

AddFourniseur::~AddFourniseur()
{
    delete ui;
}

void AddFourniseur::on_btnAdd_clicked()
{
    ui->lblInfo->setText("");
    QString firstname = ui->txtFirstName->text();
    QString lastname = ui->txtLastName->text();
    QString email = ui->txtEmail->text();
    QString address = ui->txtAddress->text();
    QString phone = ui->txtPhone->text();

    qDebug() <<"firstname "<<firstname<<" lastname "<<lastname<<" email "<<email<<" address "<<address <<" phone " <<phone;

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

    if(QFile::exists("C:/Users/Administrator/Desktop/GestionStock1.db"))
    {
        qDebug() <<"databses file exist";
    }
    else
    {
        qDebug() <<"DB file doesn't exists";
        return;
    }

    if(!database.open())
    {
        qDebug() <<"Error: Unable to open Database" ;
        return;
    }
    else
    {
        qDebug() <<" Database open successfully..";
    }

    QSqlQuery query(database);
    query.clear();

    query.prepare("insert into Fournisseur (firstname,lastname,email,address,phone) values ('" +firstname +"','" +lastname + "','"+ email + "','" +address + "','"+ phone+ "')");

    if(!query.exec())
    {
        qDebug() << query.lastError().text() << query.lastQuery();
        ui->lblInfo->setText("Unable to Add Fourniseur");
    }
    else
    {
        if(query.numRowsAffected() > 0)
        {
            qDebug() <<"read was successful "<< query.lastQuery();
            ui->lblInfo->setText("Fourniseur Added Successfully!");
             close();
        }
        else
            qDebug() <<"Unable to Add new Fourniseur";
     }



    database.close();

}

