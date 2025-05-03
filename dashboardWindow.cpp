#include "dashboardWindow.h"
#include "./ui_dashboardWindow.h"
#include "addNewArticleForm.h"
#include "gProjectVersion.h"
#include "user.h"
#include "setNewPasswordForm.h"

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DashboardWindow)
{
    this->ui->setupUi(this);

    // Setup New pass form widget
    m_SetNewPassForm = new SetNewPasswordForm(this);

    QVBoxLayout *pageLayout_1 = new QVBoxLayout(ui->SetNewPassPage);

    pageLayout_1->addWidget(m_SetNewPassForm);
    pageLayout_1->setAlignment(m_SetNewPassForm, Qt::AlignCenter);
    pageLayout_1->setSizeConstraint(QLayout::SetMinimumSize);

     m_AddNewArticleForm = new AddNewArticleForm(this);
    QVBoxLayout *pageLayout_2 = new QVBoxLayout(ui->AddArticlePage);
     pageLayout_2->addWidget(m_AddNewArticleForm);
     pageLayout_2->setAlignment(m_AddNewArticleForm, Qt::AlignCenter);
     pageLayout_2->setSizeConstraint(QLayout::SetMinimumSize);

    QObject::connect(m_SetNewPassForm, &SetNewPasswordForm::HideSetNewPassForm, [this]()
                     {
                        ShowMainPage();
                     });

    QObject::connect(m_AddNewArticleForm, &AddNewArticleForm::HideAddNewArticleForm, [this]()
                     {
                          ShowMainPage();
                     });
    // end
    ui->MainWidgetSwitcher->setCurrentWidget(ui->MainPage);





    // update project version
    ui->ProjectVersionText->setText(GetVersionAsString());
}

DashboardWindow::~DashboardWindow()
{
    delete ui;
}


void DashboardWindow::on_LogoutButton_clicked()
{
    emit logoutRequested();
}


void DashboardWindow::on_ChangePasswordButton_clicked()
{
    m_SetNewPassForm->show();
    ui->MainWidgetSwitcher->setCurrentWidget(ui->SetNewPassPage);
}

void DashboardWindow::ShowMainPage()
{
    ui->MainWidgetSwitcher->setCurrentWidget(ui->MainPage);
}


void DashboardWindow::on_AddNewArticleButton_clicked()
{
    ui->MainWidgetSwitcher->setCurrentWidget(ui->AddArticlePage);
}

