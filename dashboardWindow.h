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

private:
    Ui::DashboardWindow *ui = nullptr;
    SetNewPasswordForm* m_SetNewPassForm = nullptr;
    AddNewArticleForm* m_AddNewArticleForm = nullptr;

    void ShowMainPage();

};

#endif // DASHBOARDWINDOW_H
