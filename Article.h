#ifndef ARTICLE_H
#define ARTICLE_H

#include <QString>
#include <QMap>
#include <qdebug.h>

enum class EMassUnit : uint8_t
{

    mg,
    g,
    kg,
    t,
    kt,
    COUNT
};
// Create a static array of enum values, without the last special value
static QMap<EMassUnit, QString> MassUnitsToText =
{
    {EMassUnit::mg, "mg"},
    {EMassUnit::g, "g"},
    {EMassUnit::kg, "kg"},
    {EMassUnit::t, "t"},
    {EMassUnit::kt, "kt"}
};


struct FPrice
{
    FPrice(){};

    FPrice(int32_t inMainPart, int32_t inFractionalPart, QString inISO_Code)
        : MainPart{inMainPart},
        FractionalPart{inFractionalPart},
        ISO4217Code{inISO_Code}
    {}
    int32_t MainPart = 0;
    int32_t FractionalPart = 0;
    QString ISO4217Code = "000";

    QString GetAsString()
    {
        return QString("%1.%2 %3").arg(MainPart).arg(FractionalPart).arg(ISO4217Code);
    }
};
static bool IsValidPrice(const FPrice& price)
{
    return price.MainPart >= 0 && price.FractionalPart >= 0 && price.ISO4217Code.length() == 3;
}
struct FMass
{
    FMass(){};
    FMass(double inAmount, EMassUnit inMassUnit)
        : Amount{inAmount},
        MassUnit{inMassUnit} {};

    double Amount = 0;
    EMassUnit MassUnit = EMassUnit::kg; // wastes 7 bytes :(

    QString GetAsString()
    {
        return QString("%1 %2").arg(Amount).arg(MassUnitsToText[MassUnit]);
    }
};
static bool IsValidMass(FMass inMass)
{
    return inMass.Amount > 0 && inMass.MassUnit != EMassUnit::COUNT;
}

struct FArticle
{
public:
    // Default constructor
    FArticle(): ID{-1}, Name{"Unknown"}{};

    // Parameterized constructor
    FArticle(int64_t id, const QString& name, FPrice price, int32_t count, const QString& comments, FMass mass, const QString& category)
        :ID{id},
        Name{name},
        Price{price},
        Count{count},
        Comments{comments},
        Mass{mass},
        Category{category}{};

    QString GetAsString()
    {
        return QString("ID: %1, Name: %2, Price: %3, Count: %4, Category: %5, Mass: %6, Comments: %7")
            .arg(ID)
            .arg(Name, Price.GetAsString())
            .arg(Count)
            .arg(Category, Mass.GetAsString(), Comments);
    }

    // Getters and setters
    int64_t getId() const
    {
        return ID;
    };
    void setId(int64_t id)
    {
        if(id >0)
        {
            ID = id;
        }
        qDebug()<<"ID has to be non negative for it to be valid";
    };

    QString getName() const
    {
        return Name;
    };
    void setName(const QString& name)
    {
        if(!name.isEmpty())
            Name = name;
    };

    FPrice getPrice() const
    {
        return Price;
    };
    void setPrice(FPrice price)
    {
        Price = price;
    };

    int32_t getCount() const
    {
        return Count;
    };
    void setCount(int32_t count)
    {
        if(count >=0)
            Count = count;
    };

    QString getComments() const
    {
        return Comments;
    };
    void setComments(const QString& comments)
    {
        Comments = comments;
    };

    FMass getMass() const
    {
        return Mass;
    };
    void setMass(FMass mass)
    {
        Mass = mass;
    };

    QString getCategory() const
    {
        return Category;
    };
    void setCategory(const QString &newCategory)
    {
        Category = newCategory;
    };

    QString getStorageSector() const
    {
        return StorageSector;
    };
    void setStorageSector(const QString &newStorageSector)
    {
        StorageSector = newStorageSector;
    };

private:
    int64_t ID;
    QString Name;
    FPrice Price;
    int32_t Count;
    QString Comments;
    FMass Mass;
    QString Category;
    QString StorageSector;
};



#endif // ARTICLE_H
