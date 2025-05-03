#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSqlDatabase>

class User;
class QMessageBox;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_ExitButton_clicked();

    void on_LoginButton_clicked();

    void on_BackButton_clicked();

    void on_LoginToAppButton_clicked();

    void on_Register_clicked();

    void on_BackButton_2_clicked();

    void on_RegisterUserButton_clicked();

    void on_RecoverPassButton_clicked();

    void on_BackButton_3_clicked();

    void on_RecoverPassFromDB_Button_clicked();

signals:
    void loginSuccessful(std::shared_ptr<User> user);
private:
    Ui::MainWindow *ui;

    QStackedWidget *stackedWidget;

    // Callend when user tries to log in
    void HandleUserLogin();

    // Called when user tries to register
    void HandleUserRegister();

    void HandleRecoverUserPassword();

    // Sets up database connection
    void SetupDB_Connection();

    // Adds a user to users table in database
    bool AddUserToDB(std::shared_ptr<User> InUserToAdd);


    QSqlDatabase DB_Connection;
};
#endif // MAINWINDOW_H
