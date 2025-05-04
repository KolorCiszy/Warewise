#include "AccountDetails.h"
#include "ui_AccountDetails.h"
#include "gdatabasehelpers.h"
#include "user.h"

AccountDetails::AccountDetails(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AccountDetails)
{
    ui->setupUi(this);
    Refresh();
}

AccountDetails::~AccountDetails()
{
    delete ui;
}

void AccountDetails::Refresh()
{
    auto LoggedUser = GDatabaseHelpers::GetInstance()->GetLoggedUser();
    if(!LoggedUser)
    {
        return;
    }
    ui->UserFullNameText->setText(LoggedUser->getFullName());
    ui->UserEmailText->setText(LoggedUser->getEmail());
    ui->UserLoginText->setText(LoggedUser->getUsername());

}

void AccountDetails::on_BackButton_clicked()
{
    emit RequestHide();
}

