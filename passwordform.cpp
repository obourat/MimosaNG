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

    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);
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
        if(hashedResult == "6837f21bced828f5c056e059fee1298b")
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
        if(hashedResult == "f7ab1ea1137877706c67b20f19c64dd1")
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
