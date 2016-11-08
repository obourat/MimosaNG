#include "passwordform.h"
#include "ui_passwordform.h"
#include "datamanager.h"
#include <QLineEdit>
#include <QCryptographicHash>
#include <QMessageBox>

PasswordForm::PasswordForm(DataManager *dataManager, int levelRequested,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordForm),
    dataManager(dataManager),
    levelRequested(levelRequested)
{
    ui->setupUi(this);

    this->setWindowTitle("Mot de passe requis");
    password = new QLineEdit(this);
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

void PasswordForm::on_buttonBox_accepted()
{
    QCryptographicHash* hash;
    QByteArray passwordGiven = hash->hash(password->text().toAscii(), QCryptographicHash::Md5);
    QString hashedResult(passwordGiven.toHex());
    if(levelRequested == 1)
    {
        if(hashedResult == "e8cfbc52c87859884531b4b9871f6711")
        {
            dataManager->setAccessLevel(1);
            QMessageBox::information(this, "Acces autorise", "L'atelier passe en mode modification");
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Mot de passe incorrect. Reessayez");
        }
    }
    else if(levelRequested == 2)
    {
        if(hashedResult == "0b5c12b883e411b07dfaca2c0882051d")
        {
            dataManager->setAccessLevel(2);
            QMessageBox::information(this, "Acces autorise", "L'atelier passe en mode administration");
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Mot de passe incorrect. Reessayez");
        }
    }
}
