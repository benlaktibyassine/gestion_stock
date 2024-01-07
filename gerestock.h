// gerestock.h

#ifndef GERESTOCK_H
#define GERESTOCK_H

#include <QDialog>
#include <QMap>
#include <QtSql>

namespace Ui {
class GereStock;
}

class GereStock : public QDialog
{
    Q_OBJECT

public:
    explicit GereStock(QWidget *parent = nullptr);
    ~GereStock();

signals:
    // Declare the signal here
    void productPurchaseAdded();

private slots:
    void on_btnAdd_clicked();
    void populateComboBoxes();

private:
    Ui::GereStock *ui;
    QMap<QString, QString> productNameToIdMap; // Map to store product name to ID
    QMap<QString, QString> supplierNameToIdMap; // Map to store supplier name to ID

    QStringList fetchProductNamesFromDatabase();
    QStringList fetchSupplierNamesFromDatabase();
};

#endif // GERESTOCK_H
