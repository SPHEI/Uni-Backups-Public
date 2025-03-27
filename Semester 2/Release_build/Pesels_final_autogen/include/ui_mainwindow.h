/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *peselView;
    QListWidget *peselList;
    QLabel *label;
    QPushButton *AddPesel;
    QPushButton *AddFromFile;
    QPushButton *ExportPesels;
    QPushButton *BackButton;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *BaseLabel;
    QLineEdit *singlePesel;
    QLineEdit *fromFile;
    QLineEdit *toFile;
    QCheckBox *validCheck;
    QDateEdit *searchDate;
    QDateEdit *searchDaterange;
    QDateEdit *searchDaterange_2;
    QCheckBox *ageRange;
    QCheckBox *age;
    QComboBox *gender;
    QLabel *label_2;
    QCheckBox *mustValid;
    QCheckBox *mustInvalid;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QWidget *baseView;
    QPushButton *pushButton_3;
    QListWidget *dbaseList;
    QLineEdit *newBaseName;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QLineEdit *peselSearch;
    QComboBox *peselCombo;
    QSpinBox *peselAmount;
    QPushButton *pushButton_7;
    QPushButton *pushButton_10;
    QWidget *historyView;
    QPushButton *pushButton_6;
    QListWidget *searchHistory;
    QPushButton *pushButton_8;
    QLabel *userName;
    QWidget *widget;
    QLineEdit *userInput;
    QPushButton *pushButton_9;
    QGroupBox *groupBox;
    QLabel *feedback;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(720, 560);
        MainWindow->setWindowOpacity(1.000000000000000);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setEnabled(true);
        peselView = new QWidget(centralwidget);
        peselView->setObjectName("peselView");
        peselView->setEnabled(true);
        peselView->setGeometry(QRect(0, 0, 721, 541));
        peselList = new QListWidget(peselView);
        peselList->setObjectName("peselList");
        peselList->setGeometry(QRect(340, 30, 371, 491));
        peselList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label = new QLabel(peselView);
        label->setObjectName("label");
        label->setGeometry(QRect(340, 5, 371, 21));
        AddPesel = new QPushButton(peselView);
        AddPesel->setObjectName("AddPesel");
        AddPesel->setGeometry(QRect(180, 110, 121, 24));
        AddFromFile = new QPushButton(peselView);
        AddFromFile->setObjectName("AddFromFile");
        AddFromFile->setGeometry(QRect(180, 140, 121, 24));
        ExportPesels = new QPushButton(peselView);
        ExportPesels->setObjectName("ExportPesels");
        ExportPesels->setGeometry(QRect(180, 210, 121, 24));
        BackButton = new QPushButton(peselView);
        BackButton->setObjectName("BackButton");
        BackButton->setGeometry(QRect(10, 30, 75, 24));
        pushButton = new QPushButton(peselView);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 60, 91, 24));
        pushButton_2 = new QPushButton(peselView);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(50, 430, 251, 24));
        BaseLabel = new QLabel(peselView);
        BaseLabel->setObjectName("BaseLabel");
        BaseLabel->setGeometry(QRect(10, 6, 321, 20));
        singlePesel = new QLineEdit(peselView);
        singlePesel->setObjectName("singlePesel");
        singlePesel->setGeometry(QRect(50, 110, 113, 22));
        singlePesel->setMaxLength(11);
        singlePesel->setAlignment(Qt::AlignCenter);
        fromFile = new QLineEdit(peselView);
        fromFile->setObjectName("fromFile");
        fromFile->setGeometry(QRect(50, 140, 113, 22));
        fromFile->setAlignment(Qt::AlignCenter);
        toFile = new QLineEdit(peselView);
        toFile->setObjectName("toFile");
        toFile->setGeometry(QRect(50, 210, 113, 22));
        toFile->setAlignment(Qt::AlignCenter);
        validCheck = new QCheckBox(peselView);
        validCheck->setObjectName("validCheck");
        validCheck->setGeometry(QRect(110, 170, 121, 20));
        validCheck->setChecked(true);
        searchDate = new QDateEdit(peselView);
        searchDate->setObjectName("searchDate");
        searchDate->setGeometry(QRect(50, 300, 110, 22));
        searchDaterange = new QDateEdit(peselView);
        searchDaterange->setObjectName("searchDaterange");
        searchDaterange->setGeometry(QRect(50, 360, 110, 22));
        searchDaterange_2 = new QDateEdit(peselView);
        searchDaterange_2->setObjectName("searchDaterange_2");
        searchDaterange_2->setGeometry(QRect(50, 390, 110, 22));
        ageRange = new QCheckBox(peselView);
        ageRange->setObjectName("ageRange");
        ageRange->setGeometry(QRect(50, 330, 101, 20));
        age = new QCheckBox(peselView);
        age->setObjectName("age");
        age->setGeometry(QRect(50, 270, 101, 20));
        age->setChecked(false);
        gender = new QComboBox(peselView);
        gender->addItem(QString());
        gender->addItem(QString());
        gender->addItem(QString());
        gender->setObjectName("gender");
        gender->setGeometry(QRect(200, 300, 69, 21));
        label_2 = new QLabel(peselView);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(200, 270, 49, 16));
        mustValid = new QCheckBox(peselView);
        mustValid->setObjectName("mustValid");
        mustValid->setGeometry(QRect(200, 360, 111, 20));
        mustInvalid = new QCheckBox(peselView);
        mustInvalid->setObjectName("mustInvalid");
        mustInvalid->setGeometry(QRect(200, 390, 111, 20));
        label_3 = new QLabel(peselView);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(19, 360, 31, 21));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(peselView);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 390, 20, 20));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_5 = new QLabel(peselView);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(17, 300, 31, 21));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ExportPesels->raise();
        label->raise();
        AddPesel->raise();
        AddFromFile->raise();
        peselList->raise();
        BackButton->raise();
        pushButton->raise();
        pushButton_2->raise();
        BaseLabel->raise();
        singlePesel->raise();
        fromFile->raise();
        toFile->raise();
        validCheck->raise();
        searchDate->raise();
        searchDaterange->raise();
        searchDaterange_2->raise();
        ageRange->raise();
        age->raise();
        gender->raise();
        label_2->raise();
        mustValid->raise();
        mustInvalid->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        baseView = new QWidget(centralwidget);
        baseView->setObjectName("baseView");
        baseView->setGeometry(QRect(0, 0, 461, 541));
        pushButton_3 = new QPushButton(baseView);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(310, 40, 141, 24));
        dbaseList = new QListWidget(baseView);
        dbaseList->setObjectName("dbaseList");
        dbaseList->setEnabled(true);
        dbaseList->setGeometry(QRect(20, 10, 281, 511));
        dbaseList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        dbaseList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        newBaseName = new QLineEdit(baseView);
        newBaseName->setObjectName("newBaseName");
        newBaseName->setGeometry(QRect(310, 10, 141, 22));
        newBaseName->setAlignment(Qt::AlignCenter);
        pushButton_4 = new QPushButton(baseView);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(310, 100, 141, 24));
        pushButton_5 = new QPushButton(baseView);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(310, 280, 141, 24));
        peselSearch = new QLineEdit(baseView);
        peselSearch->setObjectName("peselSearch");
        peselSearch->setGeometry(QRect(310, 210, 141, 22));
        peselSearch->setMaxLength(11);
        peselSearch->setAlignment(Qt::AlignCenter);
        peselCombo = new QComboBox(baseView);
        peselCombo->addItem(QString());
        peselCombo->addItem(QString());
        peselCombo->addItem(QString());
        peselCombo->setObjectName("peselCombo");
        peselCombo->setGeometry(QRect(310, 240, 41, 31));
        peselAmount = new QSpinBox(baseView);
        peselAmount->setObjectName("peselAmount");
        peselAmount->setGeometry(QRect(361, 240, 81, 31));
        pushButton_7 = new QPushButton(baseView);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(310, 320, 141, 24));
        pushButton_10 = new QPushButton(baseView);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(310, 500, 141, 24));
        historyView = new QWidget(centralwidget);
        historyView->setObjectName("historyView");
        historyView->setGeometry(QRect(0, 0, 721, 541));
        pushButton_6 = new QPushButton(historyView);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(10, 10, 75, 24));
        searchHistory = new QListWidget(historyView);
        searchHistory->setObjectName("searchHistory");
        searchHistory->setGeometry(QRect(100, 40, 611, 491));
        pushButton_8 = new QPushButton(historyView);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(10, 510, 75, 24));
        userName = new QLabel(historyView);
        userName->setObjectName("userName");
        userName->setGeometry(QRect(100, 10, 611, 21));
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 721, 541));
        userInput = new QLineEdit(widget);
        userInput->setObjectName("userInput");
        userInput->setGeometry(QRect(290, 230, 113, 22));
        userInput->setAlignment(Qt::AlignCenter);
        pushButton_9 = new QPushButton(widget);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(310, 270, 75, 24));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 540, 721, 21));
        feedback = new QLabel(groupBox);
        feedback->setObjectName("feedback");
        feedback->setGeometry(QRect(0, 0, 721, 20));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
        QObject::connect(BackButton, &QPushButton::clicked, peselView, qOverload<>(&QWidget::hide));
        QObject::connect(BackButton, &QPushButton::clicked, baseView, qOverload<>(&QWidget::show));
        QObject::connect(dbaseList, &QListWidget::itemDoubleClicked, baseView, qOverload<>(&QWidget::hide));
        QObject::connect(dbaseList, &QListWidget::itemDoubleClicked, peselView, qOverload<>(&QWidget::show));
        QObject::connect(pushButton_6, &QPushButton::clicked, historyView, qOverload<>(&QWidget::hide));
        QObject::connect(pushButton_6, &QPushButton::clicked, baseView, qOverload<>(&QWidget::show));
        QObject::connect(pushButton_7, &QPushButton::clicked, historyView, qOverload<>(&QWidget::show));
        QObject::connect(pushButton_7, &QPushButton::clicked, baseView, qOverload<>(&QWidget::hide));
        QObject::connect(pushButton_10, &QPushButton::clicked, baseView, qOverload<>(&QWidget::hide));
        QObject::connect(pushButton_10, &QPushButton::clicked, widget, qOverload<>(&QWidget::show));
        QObject::connect(pushButton_9, &QPushButton::clicked, widget, qOverload<>(&QWidget::hide));
        QObject::connect(pushButton_9, &QPushButton::clicked, baseView, qOverload<>(&QWidget::show));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "PESEL Extraction", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID                        PESEL                BIRTH DATE     GENDER       CTRL_NUM", nullptr));
        AddPesel->setText(QCoreApplication::translate("MainWindow", "Add Pesel", nullptr));
        AddFromFile->setText(QCoreApplication::translate("MainWindow", "Add Pesels From File", nullptr));
        ExportPesels->setText(QCoreApplication::translate("MainWindow", "Export Pesels To File", nullptr));
        BackButton->setText(QCoreApplication::translate("MainWindow", "Back", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Save Database", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Search Database", nullptr));
        BaseLabel->setText(QCoreApplication::translate("MainWindow", "Default", nullptr));
        singlePesel->setText(QString());
        singlePesel->setPlaceholderText(QCoreApplication::translate("MainWindow", "PESEL", nullptr));
        fromFile->setPlaceholderText(QCoreApplication::translate("MainWindow", "FILE NAME", nullptr));
        toFile->setPlaceholderText(QCoreApplication::translate("MainWindow", "FILE NAME", nullptr));
        validCheck->setText(QCoreApplication::translate("MainWindow", "Only Add If Valid", nullptr));
        ageRange->setText(QCoreApplication::translate("MainWindow", "Age Range", nullptr));
        age->setText(QCoreApplication::translate("MainWindow", "Age", nullptr));
        gender->setItemText(0, QCoreApplication::translate("MainWindow", "-", nullptr));
        gender->setItemText(1, QCoreApplication::translate("MainWindow", "M", nullptr));
        gender->setItemText(2, QCoreApplication::translate("MainWindow", "F", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "Gender", nullptr));
        mustValid->setText(QCoreApplication::translate("MainWindow", "Must Be Valid", nullptr));
        mustInvalid->setText(QCoreApplication::translate("MainWindow", "Must Be Invalid", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "From", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "To", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Create New", nullptr));
        newBaseName->setPlaceholderText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        peselSearch->setPlaceholderText(QCoreApplication::translate("MainWindow", "PESEL", nullptr));
        peselCombo->setItemText(0, QCoreApplication::translate("MainWindow", "=", nullptr));
        peselCombo->setItemText(1, QCoreApplication::translate("MainWindow", ">", nullptr));
        peselCombo->setItemText(2, QCoreApplication::translate("MainWindow", "<", nullptr));

        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Search History", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "Log Out", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Back", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        userName->setText(QCoreApplication::translate("MainWindow", "Search history of user:", nullptr));
        userInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Username", nullptr));
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "Log In", nullptr));
        groupBox->setTitle(QString());
        feedback->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
