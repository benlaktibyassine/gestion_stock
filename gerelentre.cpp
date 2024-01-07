// gerelentre.cpp
#include "gerelentre.h"
#include "ui_gerelentre.h"

GereLentre::GereLentre(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GereLentre)
{
    ui->setupUi(this);

    // Connect the signal to the slot to populate the combo boxes
    connect(this, &GereLentre::productPurchaseAdded, this, &GereLentre::populateComboBoxes);

    // Populate the combo boxes initially
    populateComboBoxes();
}

GereLentre::~GereLentre()
{
    delete ui;
}

void GereLentre::populateComboBoxes()
{
    // Fetch the list of product names from the database and populate the product combo box
    QStringList productNames = fetchProductNamesFromDatabase();
    ui->comboBoxProductId->addItems(productNames);

    // Fetch the list of supplier names from the database and populate the supplier combo box
    QStringList supplierNames = fetchSupplierNamesFromDatabase();
    ui->comboBoxSupplierId->addItems(supplierNames);
}

QStringList GereLentre::fetchProductNamesFromDatabase()
{
    QStringList productNames;

    // Connect to the database
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

    if (database.open())
    {
        // Execute a query to fetch product names and IDs
        QSqlQuery query(database);
        if (query.exec("SELECT id_produit, name FROM Produit"))
        {
            while (query.next())
            {
                // Add each product name to the list
                QString productName = query.value(1).toString();
                // Use the product ID as the value
                QString productId = query.value(0).toString();
                productNames.append(productName);
                // Store the mapping of product name to product ID for later use
                productNameToIdMap[productName] = productId;
            }
        }
        else
        {
            qDebug() << "Error fetching product names:" << query.lastError().text();
        }

        // Close the database connection
        database.close();
    }
    else
    {
        qDebug() << "Error opening database:" << database.lastError().text();
    }

    return productNames;
}

QStringList GereLentre::fetchSupplierNamesFromDatabase()
{
    QStringList supplierNames;

    // Connect to the database
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

    if (database.open())
    {
        // Execute a query to fetch supplier names and IDs
        QSqlQuery query(database);
        if (query.exec("SELECT id_fournisseur, firstname, lastname FROM Fournisseur"))
        {
            while (query.next())
            {
                // Combine first name and last name as the supplier name
                QString supplierName = query.value(1).toString() + " " + query.value(2).toString();
                // Use the supplier ID as the value
                QString supplierId = query.value(0).toString();
                supplierNames.append(supplierName);
                // Store the mapping of supplier name to supplier ID for later use
                supplierNameToIdMap[supplierName] = supplierId;
            }
        }
        else
        {
            qDebug() << "Error fetching supplier names:" << query.lastError().text();
        }

        // Close the database connection
        database.close();
    }
    else
    {
        qDebug() << "Error opening database:" << database.lastError().text();
    }

    return supplierNames;
}

void GereLentre::on_btnAdd_clicked()
{
    ui->lblInfo->setText("");

    // Get the selected product name from the combo box
    QString productName = ui->comboBoxProductId->currentText();
    // Retrieve the corresponding product ID using the stored mapping
    QString idProduit = productNameToIdMap.value(productName);

    // Get the selected supplier name from the combo box
    QString supplierName = ui->comboBoxSupplierId->currentText();
    // Retrieve the corresponding supplier ID using the stored mapping
    QString idFournisseur = supplierNameToIdMap.value(supplierName);

    // Get other input values as needed
    QString quntite = ui->txtQuntite->text();
    QString dateEntre = ui->txtDateEntre->text();

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

    if (QFile::exists("C:/Users/Administrator/Desktop/GestionStock1.db"))
    {
        qDebug() << "Database file exists";
    }
    else
    {
        qDebug() << "DB file doesn't exist";
        return;
    }

    if (!database.open())
    {
        qDebug() << "Error: Unable to open Database";
        return;
    }
    else
    {
        qDebug() << "Database opened successfully..";
    }

    QSqlQuery query(database);

    // Start a transaction to ensure atomicity
    database.transaction();

    // Insert into Entree table
    query.prepare("INSERT INTO Entree (id_produit, id_fournisseur, Quantite, dateEntre) VALUES (:idProduit, :idFournisseur, :quntite, :dateEntre)");
    query.bindValue(":idProduit", idProduit);
    query.bindValue(":idFournisseur", idFournisseur);
    query.bindValue(":quntite", quntite);
    query.bindValue(":dateEntre", dateEntre);

    if (!query.exec())
    {
        qDebug() << query.lastError().text() << query.lastQuery();
        ui->lblInfo->setText("Unable to Add New Product Purchase");

        // Rollback the transaction in case of an error
        database.rollback();
    }
    else
    {
        // Update Quantite in Produit table
        query.clear();
        query.prepare("UPDATE Produit SET stockInitial = stockInitial + :quantity WHERE id_produit = :id_produit");
        query.bindValue(":quantity", quntite.toInt());
        query.bindValue(":id_produit", idProduit.toInt());

        if (query.exec())
        {
            qDebug() << "Product Quantity updated successfully!";
            ui->lblInfo->setText("New Product Purchase Added Successfully!");

            // Commit the transaction if everything is successful
            database.commit();
            close();
            emit productPurchaseAdded();
        }
        else
        {
            qDebug() << "Error updating Product Quantity:" << query.lastError().text();
            ui->lblInfo->setText("Unable to Update Product Quantity");

            // Rollback the transaction in case of an error
            database.rollback();
        }
    }

    database.close();
}
