// gerelentre.h
#ifndef GERELANTRE_H
#define GERELANTRE_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class GereLentre;
}

class GereLentre : public QDialog
{
    Q_OBJECT

public:
    explicit GereLentre(QWidget *parent = nullptr);
    ~GereLentre();

signals:
    void productPurchaseAdded();

private slots:
    void on_btnAdd_clicked();
    void populateComboBoxes();

private:
    Ui::GereLentre *ui;
    QMap<QString, QString> productNameToIdMap;
    QMap<QString, QString> supplierNameToIdMap;

    QStringList fetchProductNamesFromDatabase();
    QStringList fetchSupplierNamesFromDatabase();
};

#endif // GERELANTRE_H
