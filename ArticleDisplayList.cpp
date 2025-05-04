#include "ArticleDisplayList.h"
#include "ui_ArticleDisplayList.h"
#include "gdatabasehelpers.h"
#include <QStringListModel>
#include "Article.h"
#include <QMessageBox>
ArticleDisplayList::ArticleDisplayList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArticleDisplayList)
{
    ui->setupUi(this);
    m_SelectedArticle = nullptr;
    ui->DeleteArticleButton->setEnabled(false);
}

ArticleDisplayList::~ArticleDisplayList()
{
    delete ui;
}

void ArticleDisplayList::Refresh()
{
    m_Articles.clear();
    m_SelectedArticle = nullptr;
    GDatabaseHelpers::GetInstance()->GetAllArticles(m_Articles);
    ui->listView->reset();
    QStringListModel* model = new QStringListModel(ui->listView);
    QStringList articleList; // To store article info as strings
    QList<int> articleIdList;

    for(auto& article : m_Articles)
    {
        QString articleString = article->GetAsString();
        articleList << articleString;
        articleIdList << article->getId();
    }
    model->setStringList(articleList);
    ui->listView->setModel(model);
    ui->listView->setProperty("articleIdList", QVariant::fromValue(articleIdList));
}

void ArticleDisplayList::on_listView_clicked(const QModelIndex &index)
{

    QList<int> articleIdList = ui->listView->property("articleIdList").value<QList<int>>();
    if (index.row() >= 0 && index.row() < articleIdList.size())
    { // check if index is valid.
        m_SelectedArticle = m_Articles[index.row()];
        ui->DeleteArticleButton->setEnabled(true);
    }
    else
    {
        ui->DeleteArticleButton->setEnabled(false);
        m_SelectedArticle = nullptr;
    }


}
void ArticleDisplayList::on_DeleteArticleButton_clicked()
{
    if(!m_SelectedArticle)
    {
        return;
    }
    auto articleID = m_SelectedArticle->getId();
    if(GDatabaseHelpers::GetInstance()->RemoveArticleFromDatabase(articleID))
    {
        Refresh();
        QMessageBox::information(ui->listView->parentWidget(), "Success", "Article deleted successfully.");
    }
    else
    {
        QMessageBox::warning(nullptr, "Error", "Something went wrong.");
    }
}

void ArticleDisplayList::on_BackButton_clicked()
{
    emit RequestHide();
}

