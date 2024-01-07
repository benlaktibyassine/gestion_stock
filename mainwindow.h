#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addproduit.h"
#include "addfourniseur.h"
#include "gerelentre.h"
#include "gerestock.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    // Inside the MainWindow class definition
    signals:
        void productDeleted();
        void stockAdded();
        void stockRemoved();

public slots:
    void onShowProductListButtonClicked();
    void onShowSupplierListButtonClicked();
    void displaySupplierList();
    void handleProductPurchaseAdded();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;

    AddProduit *ptrAddProduit;
    AddFourniseur *ptrAddFourniseur;
    GereLentre *ptrGereLentre;
    GereStock *ptrGereStock;
    void displayProductList();
    void handleDeleteProduct(int,int);
    void handleAddStock(int);
    void handleRemoveStock(int);
};
#endif // MAINWINDOW_H
