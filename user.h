#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <QDebug>

enum class ERole : int8_t
{
    Worker,
    Manager,
    Admin,
    MAX
};

class User
{
public:
    User();

    // Full Constructor - use this while creating user object from database
    User(int id, const QString& username, const QString& HashedPassword, const QString& firstName,
         const QString& lastName, const QString& email, bool isActive = true);

    // Constructor without id from database
    User(const QString& username, const QString& password, const QString& firstName,
         const QString& lastName, const QString& email, bool isActive = true);


    // Setters
    template<bool bShouldHash>
    void setPassword(const QString& password)
    {
        if(password.length() > 5)
        {
            if constexpr(bShouldHash)
            {
                this->Password = HashPassword(password); // Store the hash as a hex string.
            }
            else
            {
                this->Password = password;
            }
        }
    }
    void setFirstName(const QString& firstName);
    void setLastName(const QString& lastName);
    void setEmail(const QString& email);
    void setIsActive(bool isActive);

    // Getters
    QString getUsername() const;

    // Methods
    void addRole(ERole role);
    void removeRole(ERole role);
    bool hasRole(const ERole) const;
    QString getFullName() const;

    // Method to check password
    bool checkPassword(const QString& HashedPassword) const;

    // Output for debugging
    void printDetails() const;

    static QString HashPassword(const QString& inRawPassword);


    QString getFirstName() const;

    QString getPassword() const;

    QString getEmail() const;

    QString getLastName() const;

private:
    int Id;
    QString Username;
    QString Password; // In a real application, this should be hashed, not stored in plain text.
    QString FirstName;
    QString LastName;
    QString Email;
    bool IsActive;
    QList<ERole> Roles; // A user can have multiple roles


};


#endif // USER_H
