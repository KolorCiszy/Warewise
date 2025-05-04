#include "addNewArticleForm.h"
#include "ui_addNewArticleForm.h"
#include "Article.h"
#include <QMessageBox>
#include "QIntValidator"
#include "QDoubleValidator"
#include "gdatabasehelpers.h"

AddNewArticleForm::AddNewArticleForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddNewArticleFrom)
{
    ui->setupUi(this);
    QStringList MassList{};
    for(auto& pair : MassUnitsToText)
    {
        MassList.append(pair);
    }
    ui->ArticleMassUnitComboBox->addItems(MassList);
    ui->ArticleMassUnitComboBox->setCurrentIndex(1);

    QValidator* IntValidator = new QIntValidator(0,std::numeric_limits<int>::max());
    ui->ArticlePriceMainPartInputBox->setValidator(IntValidator);
    ui->ArticlePriceFractionalPartInputBox->setValidator(IntValidator);
    ui->ArticleIDInputBox->setValidator(IntValidator);
    ui->ArticleCountInputBox->setValidator(IntValidator);


    QValidator* DoubleValidator = new QDoubleValidator(0, 999999.0, 5);
    ui->ArticleMassInputBox->setValidator(DoubleValidator);
}

AddNewArticleForm::~AddNewArticleForm()
{
    delete ui;
}

void AddNewArticleForm::on_BackButton_clicked()
{
    emit RequestHide();
}


void AddNewArticleForm::on_AddNewArticleButton_clicked()
{

    auto Article = ValidateArticleDataInput();
    if(Article)
    {
        if(GDatabaseHelpers::GetInstance()->AddArticleToDatabase(Article.get()))
        {
            ClearUserInput();
        }
    }
    else
    {
        QMessageBox::warning(this, "Operation failed", "Article couldn't be added.");
        qDebug() << "Article object is null";
    }

}

void AddNewArticleForm::ClearUserInput()
{
    ui->ArticleCategoryInputBox->clear();
    ui->ArticleCountInputBox->clear();
    ui->ArticleCommentsInputBox->clear();
    ui->ArticleIDInputBox->clear();
    ui->ArticleMassInputBox->clear();
    ui->ArticlePriceMainPartInputBox->clear();
    ui->ArticlePriceFractionalPartInputBox->clear();
    ui->ArticleNameInputBox->clear();
}

std::shared_ptr<FArticle> AddNewArticleForm::ValidateArticleDataInput()
{
    bool bIdOK = false;
    auto ID = ui->ArticleIDInputBox->text().toLongLong(&bIdOK);

    // Validate ID: Numeric and non-negative
    if (!bIdOK || ID < 0)
    {
        QMessageBox::warning(this, "Invalid ID", "Provided ID has to be numeric and non-negative.");
        return nullptr;
    }

    bool bArticleAlreadyExists = GDatabaseHelpers::GetInstance()->ArticleIdExists(ID);

    if(bArticleAlreadyExists)
    {
        QMessageBox::warning(this, "Article already exists", "Provided article id already exists.");
        return nullptr;
    }

    const auto ArticleName = ui->ArticleNameInputBox->text();
    FPrice ArticlePrice(ui->ArticlePriceMainPartInputBox->text().toInt(), ui->ArticlePriceFractionalPartInputBox->text().toInt(), ui->ArticlePriceIsoCodeInputBox->text());
    const int ArticleCount = ui->ArticleCountInputBox->text().toInt();
    const FMass ArticleMass(ui->ArticleMassInputBox->text().toDouble(), static_cast<EMassUnit>(ui->ArticleMassUnitComboBox->currentIndex()));
    const auto Comments = ui->ArticleCommentsInputBox->toPlainText();
    const auto Category = ui->ArticleCategoryInputBox->text();
    // Validate Name: Not empty
    if (ArticleName.isEmpty())
    {
        QMessageBox::warning(this, "Invalid Name", "Provided name is empty.");
        return nullptr;
    }

    // Validate PriceMainPart and PriceFractionalPart: Both should be non-negative
    if (!IsValidPrice(ArticlePrice))
    {
        QMessageBox::warning(this, "Invalid Price", "Provided price is invalid.");
        return nullptr;
    }

    // Validate Count: Non-negative
    if (ArticleCount < 0)
    {
        QMessageBox::warning(this, "Invalid Article count", "Article count must be non-negative.");
        return nullptr;
    }

    // Validate MassAmount: Non-negative
    if (!IsValidMass(ArticleMass))
    {
        QMessageBox::warning(this, "Invalid Article mass", "Article mass must be greater than zero.");
        return nullptr;
    }

    //If all checks pass, return constructed article
    return std::make_shared<FArticle>(ID, ArticleName, ArticlePrice, ArticleCount, Comments, ArticleMass, Category);
}



