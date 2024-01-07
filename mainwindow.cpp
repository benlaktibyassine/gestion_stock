#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptrAddProduit = new AddProduit(this);
    ptrAddFourniseur = new AddFourniseur(this);
    ptrGereLentre = new GereLentre(this);
    ptrGereStock = new GereStock(this);

    // Display the list of products on startup
    displayProductList();
    // Connect the productAdded signal to the displayProductList slot
    connect(ptrAddProduit, &AddProduit::productAdded, this, &MainWindow::displayProductList);
    // Inside the MainWindow constructor
    connect(this, &MainWindow::productDeleted, this, &MainWindow::displayProductList);
    connect(this, &MainWindow::stockAdded, this, &MainWindow::displayProductList);
    connect(this, &MainWindow::stockRemoved, this, &MainWindow::displayProductList);
    connect(ui->ProductListButton, &QPushButton::clicked, this, &MainWindow::onShowProductListButtonClicked);
    connect(ui->showSupplierListButton, &QPushButton::clicked, this, &MainWindow::onShowSupplierListButtonClicked);

    // mainwindow.cpp (add this in the constructor or a setup function)
    connect(ptrGereLentre, &GereLentre::productPurchaseAdded, this, &MainWindow::handleProductPurchaseAdded);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete ptrAddProduit;
    delete ptrAddFourniseur;
    delete ptrGereLentre;
    delete ptrGereStock;
}

void MainWindow::on_pushButton_clicked()
{
    ptrAddProduit->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    ptrAddFourniseur->show();
}

void MainWindow::on_pushButton_5_clicked()
{
    ptrGereLentre->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    ptrGereStock->show();
}

void MainWindow::displayProductList()
{
    // Assuming you have a QTableWidget named tableWidget
    ui->tableWidget->clear();  // Clear existing content
    ui->tableWidget->setRowCount(0);  // Reset row count

    // Set up the header
    ui->tableWidget->setColumnCount(8); // 5 columns for product information, 3 columns for buttons
    ui->tableWidget->setHorizontalHeaderLabels({"Name", "Code", "Description", "Price", "Stock ", "Suprimer", "Add Stock", "Remove Stock"});

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

    if (database.open())
    {
        QSqlQuery query(database);
        query.prepare("SELECT id_produit, name, code, description, prix, stockInitial FROM Produit");

        if (query.exec())
        {
            int row = 0;
            while (query.next())
            {
                ui->tableWidget->insertRow(row);

                // Populate product information
                for (int col = 0; col < 5; ++col)
                {
                    QTableWidgetItem *item = new QTableWidgetItem(query.value(col + 1).toString());
                    ui->tableWidget->setItem(row, col, item);
                }

                // Add "Supprimer" button
                QPushButton *deleteButton = new QPushButton("Supprimer");
                deleteButton->setStyleSheet("background-color: red; color: white;");
                connect(deleteButton, &QPushButton::clicked, [this, row, id_produit = query.value(0).toInt()](){
                    // Call a function to handle the delete action
                    handleDeleteProduct(id_produit, row);
                });
                ui->tableWidget->setCellWidget(row, 5, deleteButton);

                // Add "Ajouter Stock" button
                QPushButton *addStockButton = new QPushButton("Ajouter Stock");
                addStockButton->setStyleSheet("background-color: green; color: white;");
                connect(addStockButton, &QPushButton::clicked, [this, id_produit = query.value(0).toInt()](){
                    // Call a function to handle adding stock
                    handleAddStock(id_produit);
                });
                ui->tableWidget->setCellWidget(row, 6, addStockButton);

                // Add "Diminuer Stock" button
                QPushButton *removeStockButton = new QPushButton("Diminuer Stock");
                connect(removeStockButton, &QPushButton::clicked, [this, id_produit = query.value(0).toInt()](){
                    // Call a function to handle removing stock
                    handleRemoveStock(id_produit);
                });
                ui->tableWidget->setCellWidget(row, 7, removeStockButton);

                ++row;
            }
        }
        else
        {
            qDebug() << "Error executing query:" << query.lastError().text();
        }

        database.close();
    }
    else
    {
        qDebug() << "Error opening database:" << database.lastError().text();
    }
}
void MainWindow::handleDeleteProduct(int id_produit, int row)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

    if (database.open())
    {
        QSqlQuery query(database);
        query.prepare("DELETE FROM Produit WHERE id_produit = :id_produit");
        query.bindValue(":id_produit", id_produit);

        if (query.exec())
        {
            qDebug() << "Product with ID:" << id_produit << "deleted successfully.";

            // Emit signal to notify that a product is deleted
            emit productDeleted();

            // Remove the row from the table
            ui->tableWidget->removeRow(row);
        }
        else
        {
            qDebug() << "Error executing query:" << query.lastError().text();
        }

        database.close();
    }
    else
    {
        qDebug() << "Error opening database:" << database.lastError().text();
    }
}


void MainWindow::handleAddStock(int id_produit)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

        if (database.open())
        {
            QSqlQuery query(database);
            query.prepare("UPDATE Produit SET stockInitial = stockInitial + 1 WHERE id_produit = :id_produit");
            query.bindValue(":id_produit", id_produit);

            if (query.exec())
            {
                qDebug() << "Stock added for product with ID:" << id_produit;

                // Emit signal to notify that stock is added
                emit stockAdded();

                // Call displayProductList to update the table
                displayProductList();
            }
            else
            {
                qDebug() << "Error executing query:" << query.lastError().text();
            }

            database.close();
        }
        else
        {
            qDebug() << "Error opening database:" << database.lastError().text();
        }
}


void MainWindow::displaySupplierList()
{
    // Assuming you have a QTableWidget named tableWidget for suppliers
    ui->tableWidget->clear();  // Clear existing content
    ui->tableWidget->setRowCount(0);  // Reset row count

    // Set up the header
    ui->tableWidget->setColumnCount(5); // Adjust the column count based on your data model
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "First Name", "Last Name", "Email", "Phone"});

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

    if (database.open())
    {
        QSqlQuery query(database);
        query.prepare("SELECT id_fournisseur, firstname, lastname, email, phone FROM Fournisseur");

        if (query.exec())
        {
            int row = 0;
            while (query.next())
            {
                ui->tableWidget->insertRow(row);
                for (int col = 0; col < 5; ++col) // Assuming 5 columns
                {
                    QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                    ui->tableWidget->setItem(row, col, item);
                }
                ++row;
            }
        }
        else
        {
            qDebug() << "Error executing query:" << query.lastError().text();
        }

        database.close();
    }
    else
    {
        qDebug() << "Error opening database:" << database.lastError().text();
    }
}

void MainWindow::onShowProductListButtonClicked()
{
    displayProductList();
}

void MainWindow::onShowSupplierListButtonClicked()
{
    displaySupplierList();
}

// ... (existing code)

void MainWindow::handleRemoveStock(int id_produit)
{
    // Implement the logic to remove stock for the product in the database
    // You can use id_produit to identify the product uniquely

    // Example code (replace with your own logic):
    qDebug() << "Removing stock for product with ID:" << id_produit;
}// mainwindow.cpp
void MainWindow::handleProductPurchaseAdded()
{
    // Call the function to display the product list (similar to displayProductList)
    displayProductList();
}

