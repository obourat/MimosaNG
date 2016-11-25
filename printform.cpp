#include "printform.h"
#include "ui_printform.h"
#include "model.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringBuilder>
#include <QTableView>

PrintForm::PrintForm(QTableView * view, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintForm),
    view(view)
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

void PrintForm::makePrintConfigFile()
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
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
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

void PrintForm::makePrintDataFile()
{
    QString textData;
    int keyColumn;
    int rows = view->model()->rowCount(QModelIndex());
    int columns = view->model()->columnCount(QModelIndex());

    for(int c=0; c<columns;++c)
    {
        //On cherche le numéro de colonne de la clé, pour l'enlever du fichier d'impression
        if(view->model()->headerData(c,Qt::Horizontal, 0).toString() == "key")
        {
            keyColumn = c;
        }
        if(view->model()->headerData(c,Qt::Horizontal, 0).toString() != "key")
        {
            textData+=view->model()->headerData(c,Qt::Horizontal, 0).toString();
            if(c < columns-1)
            {
                textData += "/";
            }
        }
    }
    textData += "\n";

    for (int i = 0; i < rows; i++)
    {
        if(view->isRowHidden(i) == false)
        {
            for (int j = 0; j < columns; j++)
            {
                if(j != keyColumn)
                {
                    textData += view->model()->data(view->model()->index(i,j),0).toString();
                    if(j < columns-1)
                    {
                        textData += "/";
                    }
                }
            }
            textData += "\n";             // (optional: for new line segmentation)
        }
    }

    QString directory = QDir::currentPath();
    directory = directory % "/printData.txt";
    QFile file(directory);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QTextStream out(&file);
        out << textData;

        file.close();
    }
}

void PrintForm::on_buttonBox_accepted()
{
    makePrintConfigFile();
    makePrintDataFile();
}
