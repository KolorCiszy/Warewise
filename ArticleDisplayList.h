#ifndef ARTICLEDISPLAYLIST_H
#define ARTICLEDISPLAYLIST_H

#include <QWidget>

namespace Ui {
class ArticleDisplayList;
}

class ArticleDisplayList : public QWidget
{
    Q_OBJECT

public:
    explicit ArticleDisplayList(QWidget *parent = nullptr);
    ~ArticleDisplayList();

private:
    Ui::ArticleDisplayList *ui;
};

#endif // ARTICLEDISPLAYLIST_H
