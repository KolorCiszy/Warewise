#ifndef ARTICLEDISPLAYLIST_H
#define ARTICLEDISPLAYLIST_H

#include <QWidget>
#include <QList>

struct FArticle;

namespace Ui {
class ArticleDisplayList;
}

class ArticleDisplayList : public QWidget
{
    Q_OBJECT

public:
    explicit ArticleDisplayList(QWidget *parent = nullptr);
    ~ArticleDisplayList();

    void Refresh();
signals:
    void RequestHide();
private slots:
    void on_listView_clicked(const QModelIndex &index);

    void on_DeleteArticleButton_clicked();

    void on_BackButton_clicked();

private:
    Ui::ArticleDisplayList *ui;

    QList<std::shared_ptr<FArticle>> m_Articles;

    std::shared_ptr<FArticle> m_SelectedArticle;
};

#endif // ARTICLEDISPLAYLIST_H
