#include "importform.h"
#include "ui_importform.h"
#include "datamanager.h"

#include <QFileDialog>
#include <QMessageBox>

ImportForm::ImportForm(DataManager *dataManager, QString codeObjet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportForm),
    dataManager(dataManager),
    codeObjet(codeObjet)
{
    ui->setupUi(this);
    this->setWindowTitle("Importer");

    //Insertion des configurations dans la comboBox
    configurationsKeys = dataManager->getSmallMapsFromMapNameOptions("mapGCA","GCA", codeObjet);
    QStringList values;
    for(int i=0; i<configurationsKeys.count();++i)
    {
        values = configurationsKeys[i].values();
        ui->configComboBox->addItem(values.at(0));
    }

    //Insertion des boutons de selection dans le groupe de bouton, pour savoir lequel est coché lors de la validation
    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->updatecreateRadioButton);
    buttonGroup->addButton(ui->updateRadioButton);
    buttonGroup->addButton(ui->visualiseRadioButton);
    buttonGroup->addButton(ui->compareRadioButton);
    buttonGroup->addButton(ui->createRadioButton);


    //RadioButton coché par défaut
    {
        ui->visualiseRadioButton->setChecked(true);
    }

    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);
}

ImportForm::~ImportForm()
{
    delete ui;
}

void ImportForm::on_buttonBox_accepted()
{
    QString selectedOption = buttonGroup->checkedButton()->text();
    QString fileName = ui->fileLineEdit->text();
    QString configName = ui->configComboBox->currentText();

    if(!fileName.contains("*.xml"))
    {
        QMessageBox::warning(this, "Erreur", "L'extention du fichier a importer est incorrecte");
    }
}

void ImportForm::on_browsePushButton_released()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setWindowTitle("Selectionnez un fichier");
    dialog->setDefaultSuffix("xml");
    dialog->setNameFilter("*.xml");
    if(dialog->exec())
    {
        QStringList fileNames = dialog->selectedFiles();
        QString fileName = fileNames[0];
        ui->fileLineEdit->setText(fileName);
    }
}
