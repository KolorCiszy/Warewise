#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "user.h"
#include "gdatabasehelpers.h"
#include "gProjectVersion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->WidgetSwitcher->setCurrentIndex(0);
    ui->ProjectVersionText->setText(GetVersionAsString());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ExitButton_clicked()
{
    exit(0);
}


void MainWindow::on_LoginButton_clicked()
{
    ui->WidgetSwitcher->setCurrentIndex(1);
}


void MainWindow::on_LoginToAppButton_clicked()
{
    HandleUserLogin();
}
 void MainWindow::HandleUserLogin()
{
    const QString Login = ui->LoginInputBox->text();
    const QString PassRaw = ui->PasswordInputBox->text();

    auto LoggedUser = GDatabaseHelpers::GetInstance()->HandleUserLogin(Login,PassRaw);
    if(LoggedUser)
    {
        emit loginSuccessful(LoggedUser);
        ui->PasswordInputBox->clear();
        ui->LoginInputBox->clear();
    }
}


void MainWindow::SetupDB_Connection()
{
    // 1. Add the database
    DB_Connection = QSqlDatabase::addDatabase("QSQLITE"); // Use SQLite
    DB_Connection.setDatabaseName("warehouse.db"); // Database file name

    if (!DB_Connection.isValid()) {
        QMessageBox::critical(this, "Database Error", "SQLite driver not loaded.  Ensure Qt SQL is properly installed and configured.");
        return;
    }


    if (!DB_Connection.open()) {
        QMessageBox::critical(this, "Database Error", "Could not open database: " + DB_Connection.lastError().text());
        // Consider handling this more gracefully (e.g., disable login, show a different error, etc.)
        return;
    }

    // 2.  Create table if it does not exist
    QSqlQuery query(DB_Connection);
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "username VARCHAR(255) NOT NULL UNIQUE, "
            "password VARCHAR(255) NOT NULL, " // Store HASHED passwords
            "firstName VARCHAR(255), "
            "lastName VARCHAR(255), "
            "email VARCHAR(255), "
            "isActive BOOLEAN DEFAULT 1"
            ")"
            )) {
        QMessageBox::critical(this, "Database Error", "Error creating table: " + query.lastError().text());
        DB_Connection.close();
        return; // Stop here, cannot continue without a database
    }

    // Create roles table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS roles ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name VARCHAR(255) NOT NULL UNIQUE, "
            "description VARCHAR(255)"
            ")"
            )) {
        QMessageBox::critical(this, "Database Error", "Error creating roles table: " + query.lastError().text());
        DB_Connection.close();
        return;
    }

    // Create user_roles join table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS user_roles ("
            "user_id INTEGER NOT NULL, "
            "role_id INTEGER NOT NULL, "
            "PRIMARY KEY (user_id, role_id), "
            "FOREIGN KEY (user_id) REFERENCES users(id), "
            "FOREIGN KEY (role_id) REFERENCES roles(id)"
            ")"
            )) {
        QMessageBox::critical(this, "Database Error", "Error creating user_roles table: " + query.lastError().text());
        DB_Connection.close();
        return;
    }

    // 3. Populate with dummy data.  Use a function, and only insert if table is empty.
    QSqlQuery checkQuery(DB_Connection);
    checkQuery.exec("SELECT COUNT(*) FROM users");
    checkQuery.next();
    int userCount = checkQuery.value(0).toInt();
    if (userCount == 0) {
        //populateUsers(); // Only populate if the table is empty
    }
}

bool MainWindow::AddUserToDB(std::shared_ptr<User> InUserToAdd)
{
    if(!InUserToAdd)
    {
        QMessageBox::critical(this, "Null user to add", "No user to add to database specified");
        return false;
    }
    QSqlQuery query(DB_Connection);
    query.prepare("INSERT INTO users (username, password, firstName, lastName, email) VALUES (?, ?, ?, ?, ?)");

    query.addBindValue(InUserToAdd->getUsername());
    query.addBindValue(InUserToAdd->getPassword()); //already hashed
    query.addBindValue(InUserToAdd->getFirstName());
    query.addBindValue(InUserToAdd->getLastName());
    query.addBindValue(InUserToAdd->getEmail());

    if (!query.exec())
    {
        QMessageBox::critical(this, "Database Error", "Error registering user: " + query.lastError().text());
        return false;
    }
    else
    {
        QMessageBox::information(this, "Registration Successful", "User registered successfully! Now Log in with supplied credentials");

        return true;
    }
    return false;
}

void MainWindow::on_BackButton_clicked()
{
    ui->WidgetSwitcher->setCurrentWidget(ui->MainPage);
}

void MainWindow::on_BackButton_2_clicked()
{
    ui->WidgetSwitcher->setCurrentWidget(ui->MainPage);
}

void MainWindow::on_BackButton_3_clicked()
{
    ui->WidgetSwitcher->setCurrentWidget(ui->MainPage);
}

void MainWindow::on_Register_clicked()
{
    ui->WidgetSwitcher->setCurrentWidget(ui->RegisterPage);
}

void MainWindow::on_RegisterUserButton_clicked()
{
    HandleUserRegister();
}

void MainWindow::HandleUserRegister()
{
    const QString username = ui->RegisterLoginInputBox->text();
    const QString password = ui->RegisterPasswordInputBox->text();
    const QString firstName = ui->RegisterNameInputBox->text();
    const QString lastName = ui->RegisterLastnameInputBox->text();
    const QString email = ui->RegisterEmailInputBox->text();

    if (username.isEmpty() || password.isEmpty() || firstName.isEmpty() || lastName.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Registration Error", "All fields are required.");
        return;
    }
    if(!email.contains('@') || email.length() <=3)
    {
        QMessageBox::warning(this, "Registration Error", "Provided email seems invalid.");
        return;
    }
    if(password.length() <5)
    {
        QMessageBox::warning(this, "Registration Error", "Provided password is too short, it has to be at least 5 characters.");
        return;
    }
    // You could add more validation here (e.g., for email format, password strength, etc.)
    std::shared_ptr<User> InUserToAdd = std::make_shared<User>(username, password, firstName, lastName, email);
    if(AddUserToDB(InUserToAdd))
    {
        on_LoginButton_clicked();
    }

}

void MainWindow::HandleRecoverUserPassword()
{
    const QString NewPass = ui->PassRecoveryNewPassInputBox->text();
    const QString UserLogin = ui->PassRecoveryLoginInputBox->text();
    const QString UserEmail = ui->PassRecoveryEmailInputBox->text();
    if(NewPass.length() < 5)
    {
        QMessageBox::critical(this, "Wrong password", "Provided password is too short");
        return;
    }
    if(UserLogin.length() < 1)
    {
        QMessageBox::critical(this, "Wrong login", "Provided login is too short");
        return;
    }
    if(UserEmail.length() < 1 || !UserEmail.contains('@'))
    {
        QMessageBox::critical(this, "Wrong e-mail", "Provided e-mail is too short");
        return;
    }
    if(GDatabaseHelpers::GetInstance()->SetUserPassword(UserLogin,UserEmail,User::HashPassword(NewPass)))
    {
        ui->PassRecoveryNewPassInputBox->clear();
        ui->PassRecoveryLoginInputBox->clear();
        ui->PassRecoveryEmailInputBox->clear();
    }
}


void MainWindow::on_RecoverPassButton_clicked()
{
    ui->WidgetSwitcher->setCurrentWidget(ui->RecoverPassPage);
}




void MainWindow::on_RecoverPassFromDB_Button_clicked()
{
    HandleRecoverUserPassword();
}

