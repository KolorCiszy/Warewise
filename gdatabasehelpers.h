#ifndef GDATABASEHELPERS_H
#define GDATABASEHELPERS_H

#include <qsqldatabase.h>

struct FArticle;
class User;

class GDatabaseHelpers
{

public:
    // Static method to get the singleton instance
    static std::shared_ptr<GDatabaseHelpers> GetInstance();

    // Destructor
    ~GDatabaseHelpers();

    // Disable copy constructor and assignment operator to prevent copying
    GDatabaseHelpers(const GDatabaseHelpers&) = delete;
    GDatabaseHelpers& operator=(const GDatabaseHelpers&) = delete;

    std::shared_ptr<User> GetLoggedUser();

    QSqlDatabase GetDB_Connection() const;

    // Helper Functions
    bool tableExists(const QString& tableName);

    bool SetUserPassword(const QString &username, const QString &email, const QString &HashedPass);

    std::shared_ptr<User> HandleUserLogin(const QString& Login, const QString&PassRaw);

    bool AddArticleToDatabase(const FArticle* inArticle);

    bool ArticleIdExists(int64_t articleId) const;


private:
    void InitializeDB();

    GDatabaseHelpers();

    QSqlDatabase DB_Connection;

    std::shared_ptr<User> LoggedUser;
};

#endif // GDATABASEHELPERS_H
