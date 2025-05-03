#include "ArticleDisplayList.h"
#include "ui_ArticleDisplayList.h"

ArticleDisplayList::ArticleDisplayList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArticleDisplayList)
{
    ui->setupUi(this);
}

ArticleDisplayList::~ArticleDisplayList()
{
    delete ui;
}
