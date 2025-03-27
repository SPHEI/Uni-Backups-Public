#include "pesels.h"

std::string Person::getId()
{
    return std::to_string(id);
}
std::string Person::getPESEL()
{
    return PESEL;
}
std::string Person::getAge()
{
    return birth_date;
}
char Person::getGender()
{
    return gender;
}
bool Person::isValid()
{
    return controlNum;
}
std::string Person::getAll()
{
    return (std::to_string(id) + "\t" + PESEL + "\t" + birth_date + "\t" + gender + "\t" + btos(controlNum));
}

std::string btos(bool x)
{
    if (x)
    {
        return "Yes";
    }
    else
    {
        return "No";
    }
}

QString Interface::CreatePerson(QString x, bool check)
{
    Person y = Createperson(x);
    if(y.getAge() == "AGE")
    {
        return "PESEL CONTAINS IMPOSSIBLE BIRTH DATE";
    }
    if(y.getAge() == "NOTNUMBER")
    {
        return "THIS IS NOT A PESEL";
    }
    if (check)
    {
        if(y.isValid())
        {
            LogOut("ADDED\t" + y.getAll() + "\tTO DATABASE\t" + currentDbase->getname().toStdString());
            *currentDbase + y;
        }
        return "ADDED " + x;
    }
    else
    {
        LogOut("ADDED\t" + y.getAll() + "\tTO DATABASE\t" + currentDbase->getname().toStdString());
        *currentDbase + y;
        return "ADDED " + x;
    }
    LogOut("TRIED TO ADD\t" + x.toStdString() + "\tTO DATABASE\t" + currentDbase->getname().toStdString() + "\tERROR");
    return "UNKNOWN ERROR WHEN ADDING PESEL";
}
Person Createperson(QString pesel)
{
    std::string x = pesel.toStdString();
    char l = 'x';
    if (x.length() != 11)
    {
        return Person(x, "NOTNUMBER", l, l);
    }
    for (char y : x)
    {
        if (!isdigit(y))
        {
            return Person(x, "NOTNUMBER", l, l);
        }
    }

    char g;
    if ((x[9] - '0') % 2 == 0)
    {
        g = 'F';
    }
    else if ((x[9] - '0') % 2 == 1)
    {
        g = 'M';
    }
    else
    {
        g = '?';
    }
    bool cnum = true;
    int controlnum = 10 - (((x[0] - '0') + ((x[1] - '0') * 3) % 10 + ((x[2] - '0') * 7) % 10 + ((x[3] - '0') * 9) % 10 + (x[4] - '0') + ((x[5] - '0') * 3) % 10 + ((x[6] - '0') * 7) % 10 + ((x[7] - '0') * 9) % 10 + (x[8] - '0') + ((x[9] - '0') * 3) % 10) % 10);
    if (controlnum != (x[10] - '0'))
    {
        cnum = false;
    }

    std::string RR = "";
    std::string DD = "";
    RR.push_back(x[0]);
    RR.push_back(x[1]);
    DD.push_back(x[4]);
    DD.push_back(x[5]);
    std::string RRx = "";
    switch (x[2])
    {
    case '0':
        RRx = "19";
        break;
    case '1':
        RRx = "19";
        break;
    case '2':
        RRx = "20";
        break;
    case '3':
        RRx = "20";
        break;
    case '4':
        RRx = "21";
        break;
    case '5':
        RRx = "21";
        break;
    case '6':
        RRx = "22";
        break;
    case '7':
        RRx = "22";
        break;
    case '8':
        RRx = "18";
        break;
    case '9':
        RRx = "18";
        break;
    default:
        RRx = "??";
        break;
    }
    std::string MM = "";
    switch (x[3])
    {
    case '0':
        MM = "10";
        break;
    case '1':
        if ((x[2] - '0') % 2 == 0)
        {
            MM = "01";
        }
        else if ((x[2] - '0') % 2 == 1)
        {
            MM = "11";
        }
        else
        {
            MM = "??";
        }
        break;
    case '2':
        if ((x[2] - '0') % 2 == 0)
        {
            MM = "02";
        }
        else if ((x[2] - '0') % 2 == 1)
        {
            MM = "12";
        }
        else
        {
            MM = "??";
        }
        break;
    case '3':
        MM = "03";
        break;
    case '4':
        MM = "04";
        break;
    case '5':
        MM = "05";
        break;
    case '6':
        MM = "06";
        break;
    case '7':
        MM = "07";
        break;
    case '8':
        MM = "08";
        break;
    case '9':
        MM = "09";
        break;
    default:
        MM = "??";
        break;
    }

    if (stoi(DD) < 1)
    {
        return Person(x, "AGE", g, cnum);
    }

    if (MM == "01" || MM == "03" || MM == "05" || MM == "07" || MM == "08" || MM == "10" || MM == "12")
    {
        if (stoi(DD) > 31)
        {
            return Person(x, "AGE", g, cnum);
        }
    }
    else if (MM == "04" || MM == "06" || MM == "09" || MM == "11")
    {
        if (stoi(DD) > 30)
        {
            return Person(x, "AGE", g, cnum);
        }
    }
    else
    {
        if (stoi(RRx + RR) % 400 == 0)
        {
            if (stoi(DD) > 29)
            {
                return Person(x, "AGE", g, cnum);
            }
        }
        else if (stoi(RRx + RR) % 100 == 0)
        {
            if (stoi(DD) > 28)
            {
                return Person(x, "AGE", g, cnum);
            }
        }
        else if (stoi(RRx + RR) % 4 == 0)
        {
            if (stoi(DD) > 29)
            {
                return Person(x, "AGE", g, cnum);
            }
        }
        else
        {
            if (stoi(DD) > 28)
            {
                return Person(x, "AGE", g, cnum);
            }
        }
    }
    std::string dateResult = DD + "-" + MM + "-" + RRx + RR;
    return Person(x, dateResult, g, cnum);
}

QString Interface::NewBase(QString name)
{
    if(!dabaes->Containz(name))
    {
        std::ofstream out;
        out.open("dbases/" + name.toStdString() + ".csv");
        out.close();
        dabaes->Add(PersonDatabase(name.toStdString()));
        LogOut("CREATED NEW DATABASE WITH NAME\t" + name.toStdString());
        return "CREATED DATABASE " + name;
    }
    return "DATABASE ALREADY EXISTS, LOADING";
}
QString Interface::CreateFromFile(QString x, bool check)
{
    std::fstream file;
    file.open(x.toStdString());
    if (file.is_open())
    {
        std::string tmp;
        LogOut("STARTED ADDING FROM FILE\t" + x.toStdString() + "\tTO DATABASE\t" + currentDbase->getname().toStdString());
        while (std::getline(file, tmp))
        {
            CreatePerson(QString::fromStdString(tmp), check);
        }
        LogOut("FINISHED ADDING FROM FILE\t" + x.toStdString() + "\tTO DATABASE\t" + currentDbase->getname().toStdString());
        return "FINISHED ADDING FROM FILE " + x;
    }
    return "FILE NOT FOUND";
}
QString Interface::SaveToFile(QString x)
{
    std::ofstream out;
    out.open(x.toStdString());
    if(out.is_open())
    {
    for (auto& x : currentDbase->GetData())
    {
        out << x.getPESEL() << "\n";
    }
    LogOut("EXPORTED PESELS FROM DATABASE:\t" + currentDbase->getname().toStdString());
    return "PESELS HAVE BEEN EXPORTED";
    }
    return "PLEASE PROVIDE A VALID FILE NAME";
}
QString Interface::SaveDatabase()
{
    std::ofstream out;
    out.open("dbases/" + currentDbase->getname().toStdString() + ".csv");
    for (auto& x : currentDbase->GetData())
    {
        out << x.getId() << "," << x.getPESEL() << "," << x.getAge() << "," << x.getGender() << ",";
        if(x.isValid())
        {
            out << "YES\n";
        }
        else
        {
            out <<"NO\n";
        }
    }
    LogOut("SAVED DATABASE:\t" + currentDbase->getname().toStdString());
    return "SAVED DATABASE " + currentDbase->getname();
}
QString Interface::Delete(QString x)
{
    std::string hlp = x.toStdString();
    std::string indx = "";
    for (int i = 0; i < hlp.length(); i++)
    {
        if (hlp[i] == '\t')
        {
            break;
        }
        indx.push_back(hlp[i]);
    }
    *currentDbase - indx;
    LogOut("REMOVED PESEL ID:\t" + indx + "\tFROM DATABASE:\t" + currentDbase->getname().toStdString());
    return "REMOVED PESEL ID " + QString::fromStdString(indx);
}

void Interface::SaveUser()
{
    std::ofstream out;
    out.open("users/" + user->GetName() + ".csv");
    out << user->GetName() << "\n";
    for (auto& x : user->GetHistory())
    {
        out << x.toStdString() << "\n";
    }
    return;
}
QString Interface::LoadUser(QString name)
{
    std::ifstream load;
    user->Clear();
    load.open("users/" + name.toStdString() + ".csv");
    if(load.is_open())
    {
        std::string hlp;
        getline(load, hlp);
        user->SetName(hlp);
        while(getline(load, hlp))
        {
            user->Add(QString::fromStdString(hlp));
        }
    }
    else
    {
        user->SetName(name.toStdString());
        SaveUser();
    }
    load.close();
    LogOut("LOGGED IN");
    return "WELCOME " + name;
}

QVector<QString> Interface::searchDbase(bool checkage, bool checkagerange, QDate& exact, QDate& rangeMin, QDate& rangeMax, bool M, bool F, bool valid, bool invalid)
{
    QVector<QString> toDisplay;
    for (Person& x : currentDbase->GetData())
    {
        if(checkage || checkagerange)
        {
            std::string tmp = x.getAge();
            std::string tmp2 = "";
            tmp2.push_back(tmp[6]);
            tmp2.push_back(tmp[7]);
            tmp2.push_back(tmp[8]);
            tmp2.push_back(tmp[9]);
            int year = std::stoi(tmp2);
            tmp2 = "";
            tmp2.push_back(tmp[3]);
            tmp2.push_back(tmp[4]);
            int month = std::stoi(tmp2);
            tmp2 = "";
            tmp2.push_back(tmp[0]);
            tmp2.push_back(tmp[1]);
            int day = std::stoi(tmp2);
            QDate date = QDate(year,month,day);

            if(checkage && date != exact)
            {
                continue;
            }
            if(checkagerange && !(rangeMin <= date && date <= rangeMax))
            {
                continue;
            }
        }

        if ((valid && !x.isValid()) || (invalid && x.isValid()))
        {
            continue;
        }
        if ((M && x.getGender() == 'F') || (F && x.getGender() == 'M'))
        {
            continue;
        }
        toDisplay.push_back(QString::fromStdString(x.getAll()));
    }
    if(!(!checkage && !checkagerange && !M && !F && !valid && !invalid))
    {
    QString histMessage = "";
    if(M)
    {
        histMessage += "MEN ";
    }
    else if(F)
    {
        histMessage += "WOMEN ";
    }
    else
    {
        histMessage += "PEOPLE ";
    }

    if(checkage)
    {
        histMessage += "BORN ON " + exact.toString(Qt::ISODate) + " ";
    }
    else if(checkagerange)
    {
        histMessage += "BORN BETWEEN " + rangeMin.toString(Qt::ISODate) + " AND " + rangeMax.toString(Qt::ISODate) + " ";
    }

    if(valid)
    {
        histMessage += "WITH VALID CONTROL NUMBERS ";
    }
    else if(invalid)
    {
        histMessage += "WITH INVALID CONTROL NUMBERS ";
    }
    histMessage += "IN DATABASE " + currentDbase->getname();
    user->AddNew(histMessage);
    }

    SaveUser();
    return toDisplay;
}
QVector<QString> Interface::searchForBases(std::string pesel, int req_amount, bool equal,bool higher)
{
    if(pesel.length() != 11)
    {
        QVector<QString> toDisplay;
        for (auto& x : dabaes->getDbases())
        {
            toDisplay.push_back(x.first);
        }
        return toDisplay;
    }
    QVector<QString> toDisplay;
    for (auto& x : dabaes->getDbases())
    {
        int tmp = x.second.ContainsPesel(pesel);
        if(equal && tmp == req_amount)
        {
            toDisplay.push_back(x.first);
        }
        if(!equal)
        {
            if(higher && tmp > req_amount)
            {
                toDisplay.push_back(x.first);
            }
            if(!higher && tmp < req_amount)
            {
                toDisplay.push_back(x.first);
            }
        }
    }
    QString histMessage = "DATABASES WITH ";
    if(equal)
    {
        histMessage += "EXACTLY ";
    }
    else if(higher)
    {
        histMessage += "MORE THAN ";
    }
    else
    {
        histMessage += "LESS THAN ";
    }
    histMessage += QString::number(req_amount) + " OF " + QString::fromStdString(pesel);
    user->AddNew(histMessage);
    SaveUser();
    return toDisplay;
}
