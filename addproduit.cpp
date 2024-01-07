#include "addproduit.h"
#include "ui_addproduit.h"



AddProduit::AddProduit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProduit)
{
    ui->setupUi(this);
}

AddProduit::~AddProduit()
{
    delete ui;
}

void AddProduit::on_btnAdd_clicked()
{
    ui->lblInfo->setText("");
    QString produitName = ui->txtName->text();
    QString code = ui->txtCode->text();
    QString description = ui->txtDescription->text();
    QString prix = ui->txtprix->text();
    QString stockInitial = ui->txtQuntiti->text();

    qDebug() <<"Produit Name "<<produitName<<" code "<<code<<" description "<<description<<" selling Price "<<prix <<" stock initial " <<stockInitial;

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
    query.prepare("insert into Produit (name,code,description,prix,stockInitial) values ('" +produitName +"','" +code + "','"+ description + "','" +prix + "','"+ stockInitial+ "')");

    if(!query.exec())
    {
        qDebug() << query.lastError().text() << query.lastQuery();
        ui->lblInfo->setText("Unable to Add Product");
    }
    else
    {
        if(query.numRowsAffected() > 0)
        {
            qDebug() <<"read was successful "<< query.lastQuery();
            ui->lblInfo->setText("Product Added Successfully!");
            // Refresh the product list in the MainWindow
            emit productAdded();
             close();
        }
        else
            qDebug() <<"Unable to Add new Product";
     }



    database.close();

}

