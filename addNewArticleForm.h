#ifndef ADDNEWARTICLEFORM_H
#define ADDNEWARTICLEFORM_H

#include <QWidget>

struct FArticle;

namespace Ui {
class AddNewArticleFrom;
}

class AddNewArticleForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddNewArticleForm(QWidget *parent = nullptr);
    ~AddNewArticleForm();

signals:
    void RequestHide();
private slots:
    void on_BackButton_clicked();

    void on_AddNewArticleButton_clicked();

private:

    void ClearUserInput();

    std::shared_ptr<FArticle> ValidateArticleDataInput();

    Ui::AddNewArticleFrom *ui;
};

#endif // ADDNEWARTICLEFORM_H
