#include "user.h"
#include <QCryptographicHash>
User::User() {}

User::User(int id, const QString &username, const QString &HashedPassword,const QString &firstName, const QString &lastName, const QString &email, bool isActive)
{
    Id = id;
    Username = username;
    Password=HashedPassword;
    FirstName=firstName;
    LastName=lastName;
    Email=email;
    IsActive=isActive;
}
User::User(const QString &username, const QString &password, const QString &firstName, const QString &lastName, const QString &email, bool isActive)
{
    Username = username;
    Password=HashPassword(password);
    FirstName=firstName;
    LastName=lastName;
    Email=email;
    IsActive=isActive;
}

//Boilerplates getters, setters


    void User::setFirstName(const QString &firstName)
    {
        if(!firstName.isEmpty())
        {
            FirstName = firstName;
        }
    }

    void User::setLastName(const QString &lastName)
    {
        LastName = lastName;
    }

    void User::setEmail(const QString &email)
    {
        Email = email;
    }

    void User::setIsActive(bool isActive)
    {
        IsActive = isActive;
    }

    void User::addRole(ERole role)
    {
        Roles.push_back(role);
    }

    void User::removeRole(ERole role)
    {
        Roles.removeIf([&, role](ERole inRole){ return inRole == role; });
    }

    bool User::hasRole(const ERole inRole) const
    {
        return Roles.contains(inRole);
    }

    QString User::getFullName() const
    {
        return FirstName + " " + LastName;
    }

    bool User::checkPassword(const QString &HashedPassword) const
    {
        return Password == HashedPassword;
    }

    void User::printDetails() const
    {
        // TODO
    }
    QString User::getFirstName() const
    {
        return FirstName;
    }

    QString User::getPassword() const
    {
        return Password;
    }

    QString User::getEmail() const
    {
        return Email;
    }

    QString User::getLastName() const
    {
        return LastName;
    }

    QString User::getUsername() const
    {
        return Username;
    }


QString User::HashPassword(const QString &inRawPassword)
{
    QByteArray passwordBytes = inRawPassword.toUtf8();
    QByteArray hashedPassword = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256); //  Use SHA-256 (or a stronger algorithm)
    return hashedPassword.toHex();
}

