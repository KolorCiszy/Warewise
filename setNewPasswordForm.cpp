#include "setNewPasswordForm.h"
#include "./ui_setNewPasswordForm.h"
#include "dashboardWindow.h"
#include <QMessageBox>
#include "gdatabasehelpers.h"
#include "user.h"

SetNewPasswordForm::SetNewPasswordForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SetNewPasswordForm)
{
    ui->setupUi(this);
}

SetNewPasswordForm::~SetNewPasswordForm()
{
    delete ui;
}

void SetNewPasswordForm::on_BackButton_clicked()
{
    emit HideSetNewPassForm();
}


void SetNewPasswordForm::on_SetNewPassButton_clicked()
{
    auto LocLoggedUser = GDatabaseHelpers::GetInstance()->GetLoggedUser();
    bool bSuccess = false;
    if(LocLoggedUser)
    {
        if(!LocLoggedUser->checkPassword(User::HashPassword(ui->PassRecoveryCurrentPassInputBox->text())))
        {
            QMessageBox::warning(nullptr, "Set new password Failed", "Invalid current password.");
            return;
        }

        const QString NewPass = (ui->PassRecoveryNewPassInputBox->text());
        if(NewPass.length() < 5)
        {
            QMessageBox::warning(nullptr, "Set new password Failed", "Invalid password length.");
            return;
        }

        bSuccess = GDatabaseHelpers::GetInstance()->SetUserPassword(LocLoggedUser->getUsername(), LocLoggedUser->getEmail(), User::HashPassword(NewPass));
    }
    if(bSuccess)
    {
        ui->PassRecoveryNewPassInputBox->clear();
        ui->PassRecoveryCurrentPassInputBox->clear();
        emit HideSetNewPassForm();
    }

}

