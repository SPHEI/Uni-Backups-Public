
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pesels.h"
#include "qlistwidget.h"
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "histerr.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT
    Interface* intrfc;
    HistErr* hist;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetText(Interface* x)
    {
        intrfc = x;
    }
    void SetErr(HistErr* x)
    {
        hist = x;
    }
    void UpdateBases();
    void UpdatePesels();
    void UpdateSearchHistory();
private slots:
    void on_dbaseList_itemDoubleClicked(QListWidgetItem *item);

    void on_AddPesel_clicked();

    void on_AddFromFile_clicked();

    void on_ExportPesels_clicked();

    void on_pushButton_clicked();

    void on_age_clicked();

    void on_ageRange_clicked();

    void on_mustValid_clicked();

    void on_mustInvalid_clicked();

    void on_pushButton_2_clicked();

    void on_peselList_itemDoubleClicked(QListWidgetItem *item);

    void on_BackButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_searchHistory_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
