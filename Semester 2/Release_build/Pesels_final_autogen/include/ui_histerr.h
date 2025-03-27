/********************************************************************************
** Form generated from reading UI file 'histerr.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTERR_H
#define UI_HISTERR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_HistErr
{
public:
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QDialog *HistErr)
    {
        if (HistErr->objectName().isEmpty())
            HistErr->setObjectName("HistErr");
        HistErr->resize(117, 77);
        label = new QLabel(HistErr);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 101, 31));
        label->setWordWrap(true);
        pushButton = new QPushButton(HistErr);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(20, 50, 75, 24));

        retranslateUi(HistErr);
        QObject::connect(pushButton, &QPushButton::clicked, HistErr, qOverload<>(&QDialog::hide));

        QMetaObject::connectSlotsByName(HistErr);
    } // setupUi

    void retranslateUi(QDialog *HistErr)
    {
        HistErr->setWindowTitle(QCoreApplication::translate("HistErr", "Not found", nullptr));
        label->setText(QCoreApplication::translate("HistErr", "This database has been deleted.", nullptr));
        pushButton->setText(QCoreApplication::translate("HistErr", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HistErr: public Ui_HistErr {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTERR_H
