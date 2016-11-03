#include "passwordform.h"
#include "ui_passwordform.h"
#include <QLineEdit>

PasswordForm::PasswordForm(DataManager *dataManager, int levelRequested,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordForm),
    dataManager(dataManager),
    levelRequested(levelRequested)
{
    ui->setupUi(this);

    this->setWindowTitle("Mot de passe requis");
    QLineEdit *password = new QLineEdit(this);
    password->setEchoMode(QLineEdit::Password);
    password->setInputMethodHints(Qt::ImhHiddenText | Qt::ImhNoPredictiveText | Qt::ImhNoAutoUppercase);
    ui->verticalLayout->addWidget(password);
    if(levelRequested == 1)
    {
        ui->label->setText("Saisir le mot de passe pour passer en mode modification");
    }
    if(levelRequested == 2)
    {
        ui->label->setText("Saisir le mot de passe pour passer en mode administration");
    }
}

PasswordForm::~PasswordForm()
{
    delete ui;
}
