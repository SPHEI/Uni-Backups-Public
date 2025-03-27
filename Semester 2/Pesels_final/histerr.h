#ifndef HISTERR_H
#define HISTERR_H

#include <QDialog>

namespace Ui {
class HistErr;
}

class HistErr : public QDialog
{
    Q_OBJECT

public:
    explicit HistErr(QWidget *parent = nullptr);
    ~HistErr();

private:
    Ui::HistErr *ui;
};

#endif // HISTERR_H
