#ifndef SETNEWPASSWORDFORM_H
#define SETNEWPASSWORDFORM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class SetNewPasswordForm;
}
QT_END_NAMESPACE

class SetNewPasswordForm : public QWidget
{
    Q_OBJECT

public:
    explicit SetNewPasswordForm(QWidget *parent = nullptr);
    ~SetNewPasswordForm();

signals:
    void HideSetNewPassForm();

private slots:
    void on_BackButton_clicked();

    void on_SetNewPassButton_clicked();

private:
    Ui::SetNewPasswordForm *ui;
};

#endif // SETNEWPASSWORDFORM_H
