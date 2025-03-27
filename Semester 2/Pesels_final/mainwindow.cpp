#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->findChild<QWidget*>("peselView")->hide();
    this->findChild<QWidget*>("historyView")->hide();
    this->findChild<QWidget*>("baseView")->hide();
    this->findChild<QLineEdit*>("peselSearch")->setValidator( new QDoubleValidator(0, 9999999999, 2, this) );
    this->findChild<QLineEdit*>("singlePesel")->setValidator( new QDoubleValidator(0, 9999999999, 2, this) );
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_dbaseList_itemDoubleClicked(QListWidgetItem *item)
{
    intrfc->setBase(item->text());
    this->findChild<QLabel*>("BaseLabel")->setText(item->text());
    UpdatePesels();
}


void MainWindow::on_AddPesel_clicked()
{
    this->findChild<QLabel*>("feedback")->setText(intrfc->CreatePerson(this->findChild<QLineEdit*>("singlePesel")->text(),this->findChild<QCheckBox*>("validCheck")->isChecked()));
    UpdatePesels();
}


void MainWindow::on_AddFromFile_clicked()
{
    this->findChild<QLabel*>("feedback")->setText(intrfc->CreateFromFile(this->findChild<QLineEdit*>("fromFile")->text(),this->findChild<QCheckBox*>("validCheck")->isChecked()));
    UpdatePesels();
}


void MainWindow::on_ExportPesels_clicked()
{
    this->findChild<QLabel*>("feedback")->setText(intrfc->SaveToFile(this->findChild<QLineEdit*>("toFile")->text()));
}


void MainWindow::on_pushButton_clicked()
{
    this->findChild<QLabel*>("feedback")->setText(intrfc->SaveDatabase());
}



void MainWindow::on_age_clicked()
{
    if(this->findChild<QCheckBox*>("age")->isChecked() && this->findChild<QCheckBox*>("ageRange")->isChecked())
    {
        this->findChild<QCheckBox*>("ageRange")->setChecked(false);
    }
}


void MainWindow::on_ageRange_clicked()
{
    if(this->findChild<QCheckBox*>("age")->isChecked() && this->findChild<QCheckBox*>("ageRange")->isChecked())
    {
        this->findChild<QCheckBox*>("age")->setChecked(false);
    }
}


void MainWindow::on_mustValid_clicked()
{
    if(this->findChild<QCheckBox*>("mustValid")->isChecked() && this->findChild<QCheckBox*>("mustInvalid")->isChecked())
    {
        this->findChild<QCheckBox*>("mustInvalid")->setChecked(false);
    }
}


void MainWindow::on_mustInvalid_clicked()
{
    if(this->findChild<QCheckBox*>("mustValid")->isChecked() && this->findChild<QCheckBox*>("mustInvalid")->isChecked())
    {
        this->findChild<QCheckBox*>("mustValid")->setChecked(false);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    QDate x = this->findChild<QDateEdit*>("searchDate")->date();
    QDate y = this->findChild<QDateEdit*>("searchDaterange")->date();
    QDate z = this->findChild<QDateEdit*>("searchDaterange_2")->date();
    bool m = false;
    bool f = false;
    if(this->findChild<QComboBox*>("gender")->currentText() == "M")
    {
        m = true;
        f = false;
    }
    else if(this->findChild<QComboBox*>("gender")->currentText() == "F")
    {
        f = true;
        m = false;
    }
    bool age = false;
    if(this->findChild<QCheckBox*>("age")->isChecked())
    {
        age = true;
    }
    bool agerange = false;
    if(this->findChild<QCheckBox*>("ageRange")->isChecked())
    {
        agerange = true;
    }
    bool valid = false;
    if(this->findChild<QCheckBox*>("mustValid")->isChecked())
    {
        valid = true;
    }
    bool invalid = false;
    if(this->findChild<QCheckBox*>("mustInvalid")->isChecked())
    {
        invalid = true;
    }


    QListWidget* n = this->findChild<QListWidget*>("peselList");
    n->clear();
    for(auto& c : intrfc->searchDbase(age, agerange,x,y,z,m,f,valid,invalid))
    {
        n->addItem(c);
    }
}


void MainWindow::on_peselList_itemDoubleClicked(QListWidgetItem *item)
{
    this->findChild<QLabel*>("feedback")->setText(intrfc->Delete(item->text()));
    UpdatePesels();
}


void MainWindow::on_BackButton_clicked()
{
    UpdateBases();
}


void MainWindow::on_pushButton_3_clicked()
{
    if(this->findChild<QLineEdit*>("newBaseName")->text() != "")
    {
        this->findChild<QLabel*>("feedback")->setText(intrfc->NewBase(this->findChild<QLineEdit*>("newBaseName")->text()));
        intrfc->setBase(this->findChild<QLineEdit*>("newBaseName")->text());
        this->findChild<QLabel*>("BaseLabel")->setText(this->findChild<QLineEdit*>("newBaseName")->text());
        this->findChild<QWidget*>("baseView")->hide();
        this->findChild<QWidget*>("peselView")->show();
        UpdatePesels();
    }
    else
    {
        this->findChild<QLabel*>("feedback")->setText("PLEASE PROVIDE A NAME FOR THE DATABASE");
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    if((this->findChild<QListWidget*>("dbaseList")->selectedItems().size()) == 1)
    {
        this->findChild<QLabel*>("feedback")->setText(intrfc->DeleteBase(this->findChild<QListWidget*>("dbaseList")->selectedItems()[0]->text()));
        UpdateBases();
    }
    else
    {
        this->findChild<QLabel*>("feedback")->setText("SELECT A DATABASE TO DELETE FIRST");
    }
}


void MainWindow::on_pushButton_5_clicked()
{
    QListWidget* n = this->findChild<QListWidget*>("dbaseList");
    n->clear();
    bool equal = false;
    bool greater = false;
    if(this->findChild<QComboBox*>("peselCombo")->currentText() == "=")
    {
        equal = true;
    }
    if(this->findChild<QComboBox*>("peselCombo")->currentText() == ">")
    {
        greater = true;
    }
    for(auto& x : intrfc->searchForBases(this->findChild<QLineEdit*>("peselSearch")->text().toStdString(),this->findChild<QSpinBox*>("peselAmount")->value(),equal,greater))
    {
        n->addItem(x);
    }
}


void MainWindow::on_pushButton_7_clicked()
{
    UpdateSearchHistory();
}


void MainWindow::on_pushButton_9_clicked()
{
    this->findChild<QLabel*>("feedback")->setText(intrfc->LoadUser(this->findChild<QLineEdit*>("userInput")->text()));
}


void MainWindow::on_pushButton_8_clicked()
{
    this->findChild<QLabel*>("feedback")->setText(intrfc->ClearHistory());
    UpdateSearchHistory();
}


void MainWindow::on_searchHistory_itemDoubleClicked(QListWidgetItem *item)
{
    std::stringstream s;
    s << item->text().toStdString();
    std::string tmp;

    bool baseseacrch = false;

    bool age = false;
    bool agerange = false;
    QDate exact = QDate(1,1,1900);
    QDate rangeMin = QDate(1,1,1900);;
    QDate rangeMax = QDate(1,1,1900);;
    bool M = false;
    bool F = false;
    bool valid = false;
    bool invalid = false;

    std::string pesel = "";
    int req_amount = 0;
    bool equal = false;
    bool higher = false;

    std::string dbase = "";
    while(s>>tmp)
    {
        if(tmp == "MEN")
        {
            M = true;
        }
        if(tmp == "WOMEN")
        {
            F = true;
        }

        if(tmp == "VALID")
        {
            valid = true;
        }
        if(tmp == "INVALID")
        {
            invalid = true;
        }

        if(tmp == "DATABASE")
        {
            baseseacrch = true;
            while(s>>tmp)
            {
                dbase += tmp + " ";
            }
            dbase.erase(dbase.end() - 1,dbase.end());
        }
        if(tmp == "BORN")
        {
            s>>tmp;
            if(tmp == "ON")
            {
                age = true;
                s>>tmp;
                int year = stoi(tmp.substr(0,4));
                int month = stoi(tmp.substr(5,7));
                int day = stoi(tmp.substr(8,10));
                exact = QDate(year,month,day);
                rangeMin = QDate(year,month,day);
                rangeMax = QDate(year,month,day);
            }
            if(tmp == "BETWEEN")
            {
                s>>tmp;
                int year = stoi(tmp.substr(0,4));
                int month = stoi(tmp.substr(5,7));
                int day = stoi(tmp.substr(8,10));
                rangeMin = QDate(year,month,day);
                s>>tmp;
                s>>tmp;
                year = stoi(tmp.substr(0,4));
                month = stoi(tmp.substr(5,7));
                day = stoi(tmp.substr(8,10));
                rangeMax = QDate(year,month,day);
                exact = QDate(year,month,day);
                agerange = true;
            }
        }

        if(tmp == "EXACTLY")
        {
            equal = true;
            s>>tmp;
            req_amount = stoi(tmp);
            s>>tmp;
            s>>tmp;
            pesel = tmp;
        }
        if(tmp == "MORE")
        {
            higher = true;
            s>>tmp;
            s>>tmp;
            req_amount = stoi(tmp);
            s>>tmp;
            s>>tmp;
            pesel = tmp;
        }
    }
    if(baseseacrch)
    {
        if(intrfc->setBase(QString::fromStdString(dbase)))
        {
        this->findChild<QCheckBox*>("age")->setChecked(age);
        this->findChild<QCheckBox*>("ageRange")->setChecked(agerange);

        this->findChild<QDateEdit*>("searchDate")->setDate(exact);
        this->findChild<QDateEdit*>("searchDaterange")->setDate(rangeMin);
        this->findChild<QDateEdit*>("searchDaterange_2")->setDate(rangeMax);

        if(M)
        {
            this->findChild<QComboBox*>("gender")->setCurrentText("M");
        }
        else if(F)
        {
            this->findChild<QComboBox*>("gender")->setCurrentText("F");
        }
        else
        {
            this->findChild<QComboBox*>("gender")->setCurrentText("-");
        }

        this->findChild<QCheckBox*>("mustValid")->setChecked(valid);
        this->findChild<QCheckBox*>("mustInvalid")->setChecked(invalid);

        this->findChild<QWidget*>("peselView")->show();
        this->findChild<QWidget*>("historyView")->hide();

        QListWidget* n = this->findChild<QListWidget*>("peselList");
        n->clear();
        for(auto& c : intrfc->searchDbase(age, agerange,exact,rangeMin,rangeMax,M,F,valid,invalid))
        {
            n->addItem(c);
        }
        }
        else
        {
        hist->show();
        }
    }
    else
    {
        this->findChild<QLineEdit*>("peselSearch")->setText(QString::fromStdString(pesel));
        this->findChild<QSpinBox*>("peselAmount")->setValue(req_amount);
        if(equal)
        {
            this->findChild<QComboBox*>("peselCombo")->setCurrentText("=");
        }
        else if(higher)
        {
            this->findChild<QComboBox*>("peselCombo")->setCurrentText(">");
        }
        else
        {
            this->findChild<QComboBox*>("peselCombo")->setCurrentText("<");
        }
        this->findChild<QWidget*>("baseView")->show();
        this->findChild<QWidget*>("historyView")->hide();

        QListWidget* n = this->findChild<QListWidget*>("dbaseList");
        n->clear();
        for(auto& x : intrfc->searchForBases(pesel,req_amount,equal,higher))
        {
            n->addItem(x);
        }
    }
}

void MainWindow::UpdateBases()
{
    this->findChild<QListWidget*>("dbaseList")->clear();
    for(auto& y : intrfc->getBases())
    {
        this->findChild<QListWidget*>("dbaseList")->addItem(y);
    }
}

void MainWindow::UpdatePesels()
{
    QListWidget* n = this->findChild<QListWidget*>("peselList");
    n->clear();
    for(auto& y : intrfc->getDbase())
    {
        n->addItem(y);
    }
}

void MainWindow::UpdateSearchHistory()
{
    this->findChild<QLabel*>("userName")->setText("Search history of user: " + intrfc->GetUsername());
    QListWidget* n = this->findChild<QListWidget*>("searchHistory");
    n->clear();
    auto y = intrfc->getHistory();
    for(int i = std::distance(y.begin(), y.end()) - 1; i > -1; i--)
    {
        n->addItem(y[i]);
    }
}
