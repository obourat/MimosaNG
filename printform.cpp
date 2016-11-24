#include "printform.h"
#include "ui_printform.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringBuilder>

PrintForm::PrintForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Options d'impression");

    ui->orientationComboBox->addItem("Portrait");
    ui->orientationComboBox->addItem("Paysage");


    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);
}

PrintForm::~PrintForm()
{
    delete ui;
}

void PrintForm::on_buttonBox_accepted()
{
    QString title = ui->titleLineEdit->text();
    QString pages = ui->pageNbCheckBox->text();
    QString orientation = ui->orientationComboBox->currentText();
    QString nbObj = ui->objectNumberSpinBox->text();
    QString nbLines = ui->lineNumberSpinBox->text();

    QStringList parameters;
    if(ui->titleCheckBox->isChecked())
    {
        parameters.append(ui->titleCheckBox->text());
    }
    if(ui->pageNbCheckBox->isChecked())
    {
        parameters.append(ui->pageNbCheckBox->text());
    }
    if(ui->dateCheckBox->isChecked())
    {
        parameters.append(ui->dateCheckBox->text());
    }
    if(ui->hourCheckBox->isChecked())
    {
        parameters.append(ui->hourCheckBox->text());
    }
    if(ui->sigleCheckBox->isChecked())
    {
        parameters.append(ui->sigleCheckBox->text());
    }
    if(ui->linesCheckBox->isChecked())
    {
        parameters.append(ui->linesCheckBox->text());
    }
    if(ui->grayCheckBox->isChecked())
    {
        parameters.append(ui->grayCheckBox->text());
    }

    QString directory = QDir::currentPath();
    directory = directory % "/printConfig.txt";
    QFile file(directory);
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << title << endl;
        stream << pages << endl;
        stream << orientation << endl;
        stream << nbObj << endl;
        stream << nbLines << endl;
        for(int i=0; i<parameters.count();++i)
        {
            stream << parameters[i];
            stream << "/";
        }
        file.close();
    }
}
