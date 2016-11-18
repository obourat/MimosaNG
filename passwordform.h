#ifndef PASSWORDFORM_H
#define PASSWORDFORM_H

#include <QDialog>
#include <QLineEdit>

class DataManager;

namespace Ui {
class PasswordForm;
}

class PasswordForm : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordForm(DataManager *dataManager, int levelRequested, QWidget *parent = 0);
    ~PasswordForm();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PasswordForm *ui;

    DataManager *dataManager;
    int levelRequested;
    QLineEdit *password;
};

#endif // PASSWORDFORM_H
