#pragma once
#ifndef PESEL_H
#define PESEL_H
#include <Qt>
#include <QObject>
#include <QCoreApplication>
#include <QWindow>
#include <QString>
#include <QGuiApplication>
#include <QtDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <unordered_map>
#include "qdatetime.h"

template<typename T>
class Counter {
public:
    Counter() { ++count; }
    Counter(const Counter&) { ++count; }
    ~Counter() { --count; }

    int howMany()
    {
        return count;
    }

private:
    static int count;
};
template<typename T>

int Counter<T>::count = 0;


class Person
{
private:
    int id;
    std::string PESEL;
    std::string birth_date;
    char gender;
    bool controlNum;
    Counter<Person> c;
public:
    void Reasignid(int x)
    {
        id = x;
    }
    int Num()
    {
        return c.howMany();
    }
    Person()
    {
        id = Num();
        birth_date = "unknown";
        gender = '?';
        controlNum = false;
    };
    Person(std::string x)
    {
        std::stringstream temp;
        temp << x;
        getline(temp,x,',');
        id = stoi(x);
        getline(temp,PESEL,',');
        getline(temp,birth_date,',');
        getline(temp,x,',');
        gender = x[0];
        getline(temp,x,',');
        if (x == "YES")
        {
            controlNum = true;
        }
        else
        {
            controlNum = false;
        }
    }
    Person(std::string a, std::string d, char f, bool x)
    {
        id = Num();
        PESEL = a;
        birth_date = d;
        gender = f;
        controlNum = x;
    };
    std::string getId();
    std::string getPESEL();
    std::string getAge();
    char getGender();
    bool isValid();
    std::string getAll();
};

Person Createperson(QString pesel);
std::string btos(bool x);

class PersonDatabase
{
    int id;
    std::string name;
    std::vector<Person> people;
    Counter<PersonDatabase> c;
public:
    int Num()
    {
        return c.howMany();
    }
    void operator+(Person x)
    {
        people.push_back(x);
        for (int n = 0; n < people.size(); n++)
        {
            people[n].Reasignid(n + 1);
        }
    }
    void operator-(std::string x)
    {
        for (int m = 0; m < people.size(); m++)
        {
            if (people[m].getId() == x)
            {
                people.erase(people.begin() + m);
                for (int n = 0; n < people.size(); n++)
                {
                    people[n].Reasignid(n + 1);
                }
                return;
            }
        }
    }
    PersonDatabase()
    {
        id = Num();
        name = "Default";
    }
    PersonDatabase(std::string n)
    {
        id = Num();
        name = n;
    }
    QString getname()
    {
        return QString::fromStdString(name);
    }
    QVector<QString> DebugLogBase()
    {
        QVector<QString> tmp;
        for (Person& x : people)
        {
            tmp.push_back(QString::fromStdString(x.getAll()));
        }
        return tmp;
    }
    std::vector<Person>& GetData()
    {
        return people;
    };
    int ContainsPesel(std::string pesel)
    {
        int result = 0;
        for(auto& x : people)
        {
            if(x.getPESEL() == pesel)
            {
                result++;
            }
        }
        return result;
    }
};

class DatabaseDatabase
{
    std::unordered_map<QString,PersonDatabase> dbases;
public:
    PersonDatabase* GetDbase(QString x)
    {
        if(dbases.count(x) > 0)
        {
            return &dbases[x];
        }
        else
        {
            return nullptr;
        }
    }
    void Add(PersonDatabase x)
    {
        dbases[x.getname()] = x;
    }
    void Remove(QString name)
    {
        dbases.erase(name);
    }
    std::unordered_map<QString,PersonDatabase>& getDbases()
    {
        return dbases;
    }
    bool Containz(QString x)
    {
        if(dbases.count(x) > 0)
        {
            return true;
        }
        return false;
    }

};

class CurrentUser
{
    std::string name;
    QVector<QString> searchHistory;

public:
    CurrentUser()
    {
        name = "NULL";
    }
    CurrentUser(std::string x)
    {
        name = x;
    }
    void SetName(std::string x)
    {
        name = x;
    }
    std::string GetName()
    {
        return name;
    }
    QVector<QString> GetHistory()
    {
        return searchHistory;
    }
    void AddNew(QString x)
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream temp;
        temp << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        QString a = QString::fromStdString(temp.str()) + " " + x;
        searchHistory.push_back(a);
    }
    void Add(QString x)
    {
        searchHistory.push_back(x);
    }
    void Clear()
    {
        searchHistory.clear();
    }
};

class Interface : public QObject
{
    PersonDatabase* currentDbase;
    DatabaseDatabase* dabaes;
    CurrentUser* user;
    std::ofstream log;
    Q_OBJECT
public:
    Interface()
    {
        currentDbase = nullptr;
        dabaes = nullptr;
        user = nullptr;
        log.open("log.txt", std::ios::app);
    };
public slots:
    void LogOut(std::string txt)
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        log << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "\tUSER:\t" << user->GetName() << "\t" << txt << std::endl;
    }
    void SetDatabaseDatabase(DatabaseDatabase* x)
    {
        dabaes = x;
    }
    void SetUser(CurrentUser* x)
    {
        user = x;
    }
    QString getDbaseName()
    {
        return currentDbase->getname();
    }
    QString NewBase(QString name);
    QString DeleteBase(QString name)
    {
        LogOut("DELETED DATABASE " + name.toStdString());
        dabaes->Remove(name);
        remove(("dbases/" + name.toStdString() + ".csv").c_str());
        return "DELETED " + name;
    }
    bool setBase(QString i)
    {
        if(dabaes->Containz(i))
        {
            currentDbase = dabaes->GetDbase(i);
            return true;
        }
        return false;
    }
    QString CreatePerson(QString x, bool check);
    QString CreateFromFile(QString x, bool check);
    QString SaveToFile(QString x);
    QString SaveDatabase();
    void SaveUser();
    QString GetUsername()
    {
        return QString::fromStdString(user->GetName());
    }
    QString LoadUser(QString name);
    QString ClearHistory()
    {
        LogOut("CLEARED THEIR SEARCH HISTORY");
        user->Clear();
        SaveUser();
        return "CLEARED";
    }
    QVector<QString> getDbase()
    {
        return currentDbase->DebugLogBase();
    };
    QVector<QString> getBases()
    {
        QVector<QString> ret;
        for (auto& x : dabaes->getDbases())
        {
            ret.push_back(x.second.getname());
        }
        return ret;
    }
    QVector<QString> getHistory()
    {
        return user->GetHistory();
    }
    QString Delete(QString x);
    QVector<QString> searchDbase(bool checkage, bool checkagerange, QDate& exact, QDate& rangeMin, QDate& rangeMa, bool M, bool F, bool valid, bool invalid);
    QVector<QString> searchForBases(std::string pesel, int req_amount, bool equal,bool higher);
};
#endif // !PESEL_H
