#include "dashboardWindow.h"
#include "./ui_dashboardWindow.h"
#include "ArticleDisplayList.h"
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


    m_ArticleDisplayList = new ArticleDisplayList(this);
    QVBoxLayout *pageLayout_3 = new QVBoxLayout(ui->ArticleListDisplayPage);
    pageLayout_3->addWidget(m_ArticleDisplayList);
    pageLayout_3->setAlignment(m_ArticleDisplayList, Qt::AlignCenter);
    pageLayout_3->setSizeConstraint(QLayout::SetMinimumSize);


    QObject::connect(m_SetNewPassForm, &SetNewPasswordForm::RequestHide, [this]()
                     {
                        ShowMainPage();
                     });

    QObject::connect(m_AddNewArticleForm, &AddNewArticleForm::RequestHide, [this]()
                     {
                          ShowMainPage();
                     });
    QObject::connect(m_ArticleDisplayList, &ArticleDisplayList::RequestHide, [this]()
                     {
                         ShowMainPage();
                     });

    // end

    // Update ui
    ui->MainWidgetSwitcher->setCurrentWidget(ui->MainPage);

    ui->AddNewDeliveryButton->setEnabled(false);
    ui->ShowDeliveriesButton->setEnabled(false);
    ui->ShowShipmentsButton->setEnabled(false);




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


void DashboardWindow::on_ShowArticlesButton_clicked()
{
    ui->MainWidgetSwitcher->setCurrentWidget(ui->ArticleListDisplayPage);
    m_ArticleDisplayList->Refresh();
}


void DashboardWindow::on_ShowAccountDetailsButton_clicked()
{

}

