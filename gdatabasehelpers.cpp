#include "gdatabasehelpers.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "user.h"
#include "Article.h"

std::shared_ptr<GDatabaseHelpers> GDatabaseHelpers::GetInstance()
{
    static std::shared_ptr<GDatabaseHelpers> Instance{new GDatabaseHelpers};
    return Instance;
}

GDatabaseHelpers::~GDatabaseHelpers()
{

}

std::shared_ptr<User> GDatabaseHelpers::GetLoggedUser()
{
    return LoggedUser;
}

bool GDatabaseHelpers::SetUserPassword(const QString& username, const QString& email, const QString& HashedPass)
{
    QSqlQuery query(DB_Connection);
    query.prepare("UPDATE users SET password = ? WHERE username = ? AND email = ?");

    query.addBindValue(HashedPass); // Add the new hashed password
    query.addBindValue(username);
    query.addBindValue(email);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", "Error updating password: " + query.lastError().text());
    } else {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(nullptr, "Password Updated", "Password has been successfully updated.");
            if(LoggedUser)
            {
                LoggedUser->setPassword<false>(HashedPass);
            }
            return true;
        } else {
            QMessageBox::warning(nullptr, "Password Update Failed", "Incorrect username or email.  Password not updated.");
            return false;
        }
    }
    return false;
}

std::shared_ptr<User> GDatabaseHelpers::HandleUserLogin(const QString& Login, const QString&PassRaw)
{
    QSqlQuery query(DB_Connection);
    query.prepare("SELECT u.id, u.username, u.password, u.firstName, u.lastName, u.email, u.isActive, r.name AS role_name "
                  "FROM users u "
                  "LEFT JOIN user_roles ur ON u.id = ur.user_id "
                  "LEFT JOIN roles r ON ur.role_id = r.id "
                  "WHERE u.username = ?");
    query.addBindValue(Login);

    // 2. Execute Query
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", "Error querying database: " + query.lastError().text());
        return nullptr; // Handle the error appropriately
    }

    // 3. Process Result
    if (query.next()) {
        // User found, now verify password
        int id = query.value("id").toInt();
        QString dbUsername = query.value("username").toString();
        QString dbHashedPassword = query.value("password").toString(); // Get HASHED password from DB
        QString firstName = query.value("firstName").toString();
        QString lastName = query.value("lastName").toString();
        QString email = query.value("email").toString();
        bool isActive = query.value("isActive").toBool();

        std::shared_ptr<User> user = std::make_shared<User>(id, dbUsername, dbHashedPassword, firstName, lastName, email, isActive); // Use the constructor that takes a hashed password
        if (user->checkPassword(User::HashPassword((PassRaw))))
        { //check the password
            //  Login successful.
            QMessageBox::information(nullptr, "Login Successful", "Logged in as " + user->getFullName() + "!");
            LoggedUser = user;
            return user;
        }
        else
        {
            QMessageBox::warning(nullptr, "Login Failed", "Invalid password.");
            return nullptr;
        }
    }
    else
    {
        // User not found
        QMessageBox::warning(nullptr, "Login Failed", "Invalid username.");
        return nullptr;
    }
}

bool GDatabaseHelpers::AddArticleToDatabase(const FArticle* inArticle)
{
    if(!inArticle)
    {
        return false;
    }
    // Prepare the query
    QSqlQuery query(DB_Connection);
    query.prepare("INSERT INTO Articles (ID, Name, PriceMainPart, PriceFractionalPart, PriceCurrencyCode, Count, Comments, MassAmount, MassUnit, Category) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    // Bind the values from the FArticle struct.  Use the values from the provided 'article'
    query.addBindValue(inArticle->getId());
    query.addBindValue(inArticle->getName());
    query.addBindValue(inArticle->getPrice().MainPart);
    query.addBindValue(inArticle->getPrice().FractionalPart);
    query.addBindValue(inArticle->getPrice().ISO4217Code);
    query.addBindValue(inArticle->getCount());
    query.addBindValue(inArticle->getComments());
    query.addBindValue(inArticle->getMass().Amount);
    query.addBindValue(static_cast<int>(inArticle->getMass().MassUnit)); // Store the enum as an int
    query.addBindValue(inArticle->getCategory());

    // Execute the query
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error", "Error adding article to database: " + query.lastError().text());
        return false;
    }
    else
    {
        QMessageBox::information(nullptr, "Success", "Successfuly added new article to database");
        return true;
    }
    return true;
}

bool GDatabaseHelpers::RemoveArticleFromDatabase(int64_t ArticleID)
{
    QSqlQuery deleteQuery(DB_Connection);
    deleteQuery.prepare("DELETE FROM Articles WHERE ID = ?");
    deleteQuery.addBindValue(ArticleID);

    if (!deleteQuery.exec()) {
        qDebug() << "Error deleting article: " << deleteQuery.lastError().text();
        QMessageBox::warning(nullptr, "Error", "Failed to delete this article."); // Inform the user
        return false;
    }
    return true;
}

bool GDatabaseHelpers::ArticleIdExists(int64_t articleId) const
{

    if (!DB_Connection.isOpen()) {
        qDebug() << "Database is not open!";
        return false;
    }

    QSqlQuery query(DB_Connection);
    query.prepare("SELECT ID FROM Articles WHERE ID = ?");
    query.addBindValue(articleId);

    if (!query.exec()) {
        qDebug() << "Error checking article ID: " << query.lastError().text();
        return false; // Or throw an exception
    }
    // Check if any rows were returned
    return query.next(); // Returns true if a row was found, false otherwise
}

bool GDatabaseHelpers::GetAllArticles(QList<std::shared_ptr<FArticle>> &OutArticles) const
{
    // 2.  Execute a query to fetch the articles from the database.
    QSqlQuery query(DB_Connection);
    query.prepare("SELECT ID, Name, PriceMainPart, PriceFractionalPart, PriceCurrencyCode, Count, Comments, MassAmount, MassUnit, Category FROM Articles"); // Select all fields

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error fetching article data","Fething articles data failed.");
        return false;
    }

    // 3. Iterate through the query results and populate the QStringList.
    while (query.next())
    {
        std::shared_ptr<FArticle> article = std::make_shared<FArticle>();
        article->setId(query.value("ID").toInt());
        article->setName(query.value("Name").toString());
        FPrice price = FPrice{query.value("PriceMainPart").toInt(), query.value("PriceFractionalPart").toInt(), query.value("PriceCurrencyCode").toString()};
        article->setPrice(price);

        article->setCount(query.value("Count").toInt());
        article->setComments(query.value("Comments").toString());

        FMass mass = FMass{query.value("MassAmount").toDouble(),static_cast<EMassUnit>(query.value("MassUnit").toInt())};

        article->setMass(mass);
        article->setCategory(query.value("Category").toString());
        OutArticles.append(article);
    }
    return !OutArticles.isEmpty();
}

GDatabaseHelpers::GDatabaseHelpers()
{
    // 1. Open Database connection

    DB_Connection = QSqlDatabase::addDatabase("QSQLITE"); // Use SQLite
    DB_Connection.setDatabaseName("warehouse.db"); // Database file name

    if (!DB_Connection.isValid()) {
        QMessageBox::critical(nullptr, "Database Error", "SQLite driver not loaded.  Ensure Qt SQL is properly installed and configured.");
        return;
    }


    if (!DB_Connection.open()) {
        QMessageBox::critical(nullptr, "Database Error", "Could not open database: " + DB_Connection.lastError().text());
        // Consider handling this more gracefully (e.g., disable login, show a different error, etc.)
        return;
    }

    // 2.  Create tables if they don't exist
    // Create users table
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
        QMessageBox::critical(nullptr, "Database Error", "Error creating table: " + query.lastError().text());
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
        QMessageBox::critical(nullptr, "Database Error", "Error creating roles table: " + query.lastError().text());
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
        QMessageBox::critical(nullptr, "Database Error", "Error creating user_roles table: " + query.lastError().text());
        DB_Connection.close();
        return;
    }

    // Create Articles table

    if (!query.exec(
           "CREATE TABLE IF NOT EXISTS Articles ("
           "ID INTEGER PRIMARY KEY, "
           "Name TEXT NOT NULL, "
           "PriceMainPart INTEGER NOT NULL, "
           "PriceFractionalPart INTEGER NOT NULL, "
           "PriceCurrencyCode TEXT NOT NULL, "
           "Count INTEGER NOT NULL, "
           "Comments TEXT, "
           "MassAmount REAL NOT NULL, "
           "MassUnit INTEGER NOT NULL, "
           "Category TEXT NOT NULL"
           ")"
            ))
    {
        QMessageBox::critical(nullptr, "Database Error", "Error creating articles table: " + query.lastError().text());
        DB_Connection.close();
        return;
    };

}
