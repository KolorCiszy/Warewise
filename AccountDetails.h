#ifndef ACCOUNTDETAILS_H
#define ACCOUNTDETAILS_H

#include <QWidget>

namespace Ui {
class AccountDetails;
}

class AccountDetails : public QWidget
{
    Q_OBJECT

public:
    explicit AccountDetails(QWidget *parent = nullptr);
    ~AccountDetails();
signals:
    void RequestHide();
private slots:
    void on_BackButton_clicked();

private:

    void Refresh();

    Ui::AccountDetails *ui;
};

#endif // ACCOUNTDETAILS_H
