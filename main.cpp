#include "mainwindow.h"

#include <QApplication>
#include "dashboardWindow.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow MainWind;
    MainWind.show();

    std::shared_ptr<DashboardWindow> DashboardWind = nullptr;
    // Connect the loginSuccessful signal from LoginForm to a lambda function
    QObject::connect(&MainWind, &MainWindow::loginSuccessful, [&](std::shared_ptr<User> loggedUser) // Capture user
     {
         MainWind.hide(); // Hide the login form
         if (!DashboardWind) { // Create dashboard if it doesn't exist
             DashboardWind = std::make_shared<DashboardWindow>(nullptr); // Pass the user
             QObject::connect(DashboardWind.get(), &DashboardWindow::logoutRequested,
                              [&]() {
                                  DashboardWind->hide();
                                  DashboardWind = nullptr; // deletes dashboard

                                  MainWind.show();
                              });
         }
         DashboardWind->show(); // Show the main window
     });
    // Uncomment to show dashboard at launch
    //MainWind.loginSuccessful(nullptr);
    return a.exec();
}
