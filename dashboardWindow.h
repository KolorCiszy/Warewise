#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H


#include <QMainWindow.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class DashboardWindow;
}
QT_END_NAMESPACE

// Forward declarations
class AddNewArticleForm;
class User;
class SetNewPasswordForm;
class ArticleDisplayList;
class AccountDetails;

class DashboardWindow : public QMainWindow
{
    Q_OBJECT
public:
    DashboardWindow(QWidget *parent);
    ~DashboardWindow();


signals:
    void logoutRequested();

private slots:
    void on_LogoutButton_clicked();

    void on_ChangePasswordButton_clicked();

    void on_AddNewArticleButton_clicked();

    void on_ShowArticlesButton_clicked();

    void on_ShowAccountDetailsButton_clicked();

private:
    Ui::DashboardWindow *ui = nullptr;
    std::unique_ptr<SetNewPasswordForm> m_SetNewPassForm;
    std::unique_ptr<AddNewArticleForm> m_AddNewArticleForm;
    std::unique_ptr<ArticleDisplayList> m_ArticleDisplayList;
    std::unique_ptr<AccountDetails> m_AccountDetails;

    void ShowMainPage();

};

#endif // DASHBOARDWINDOW_H
