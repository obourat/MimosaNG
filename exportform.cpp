#include "exportform.h"
#include "ui_exportform.h"
#include "dataviewer.h"
#include "datamanager.h"

#include <QAction>

ExportForm::ExportForm(DataManager *dataManager, QStringList keysList, QString codeObjet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportForm),
    dataManager(dataManager),
    keysList(keysList),
    codeObjet(codeObjet)
{
    ui->setupUi(this);
    this->setWindowTitle("Options d'exportation");
    ui->destinationComboBox->addItem("Fichier");
    ui->destinationComboBox->addItem("Ecran");
    ui->concatenationComboBox->addItem("Oui");
    ui->concatenationComboBox->addItem("Non");

    configurationsKeys = dataManager->getSmallMapsFromMapNameOptions("mapGCA","GCA", codeObjet);
    QStringList values;
    for(int i=0; i<configurationsKeys.count();++i)
    {
        values = configurationsKeys[i].values();
        ui->configComboBox->addItem(values.at(0));
    }

    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);
}

ExportForm::~ExportForm()
{
    delete ui;
}

void ExportForm::on_confirmButtonBox_accepted()
{
    QString destinationChoice = ui->destinationComboBox->currentText();
    QString concatenationChoice = ui->concatenationComboBox->currentText();
    QString fileName = ui->fileNameLineEdit->text();
    QString configurationChoice = ui->configComboBox->currentText();
    QString configKey;

    for(int i=0; i<configurationsKeys.count();++i)
    {

        if(configurationChoice == configurationsKeys[i].values().at(0))
        {
            configKey = configurationsKeys[i].values().at(2);
            break;
        }
    }


}
