#include "mainwindow.h"
#include "histerr.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    DatabaseDatabase dbases;
    for (const auto& entry : std::filesystem::directory_iterator("dbases/"))
    {
        std::ifstream currentFile;
        currentFile.open(entry.path().string());
        std::string hlp = entry.path().string().substr(7,entry.path().string().length());
        PersonDatabase temp = PersonDatabase(hlp.substr(0,hlp.length() - 4));
        while (getline(currentFile, hlp))
        {
            temp + Person(hlp);
        }
        dbases.Add(temp);
    }
    QApplication a(argc, argv);

    Interface x;
    CurrentUser u("TEST");
    x.SetDatabaseDatabase(&dbases);
    x.SetUser(&u);

    MainWindow w;
    HistErr m;
    w.SetText(&x);
    w.SetErr(&m);
    w.show();
    w.UpdateBases();
    return a.exec();
}
