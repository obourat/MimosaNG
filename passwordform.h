#ifndef PASSWORDFORM_H
#define PASSWORDFORM_H

#include <QDialog>

namespace Ui {
class PasswordForm;
}

class DataManager;

class PasswordForm : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordForm(DataManager *dataManager, int levelRequested, QWidget *parent = 0);
    ~PasswordForm();

private:
    Ui::PasswordForm *ui;

    DataManager *dataManager;
    int levelRequested;
};

#endif // PASSWORDFORM_H
