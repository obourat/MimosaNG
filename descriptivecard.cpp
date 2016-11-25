#include "descriptivecard.h"
#include "ui_descriptivecard.h"
#include "datamanager.h"
#include "mainwindow.h"
#include "dataviewer.h"

#include <QStringBuilder>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QSlider>
#include <QRadioButton>
#include <QPushButton>
#include <QDate>
#include <QActionGroup>
#include <QAction>

DescriptiveCard::DescriptiveCard(DataManager *dataManager, MainWindow *mainWindow, DataViewer *dataViewer, QString codeObject, QString key, QString selection, QString choice, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DescriptiveCard),
    dataManager(dataManager),
    mainWindow(mainWindow),
    key(key),
    choice(choice)
{
    ui->setupUi(this);

    this->codeObject = codeObject;
    this->dataViewer = dataViewer;

    const QMap<QString, QMap<QString, QString> >* selectedMap;
    QMap<QString,QString> mapsOfKey;
    QMap<QString, QString> ::Iterator iterator;
    QString type;
    QString value;
    QString name;
    QString var;
    QStringList attributesOfCurrentConfig;
    QStringList attributesOfExportConfig;
    QStringList attributeNamesOfCurrentConfig;

    mapGVE = dataManager->getMapFromName("mapGVE");

    if(codeObject == "GAT")
    {
        this->setWindowTitle("Fiche descriptive d'Attribut");
        selectedMap = dataManager->getMapFromName("mapGAT");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GAT");

        if(selection == "current")
        {
            ui->label->setText("Fiche d'attribut pour la configuration courante: "+dataManager->getCurrentConfigNameGAT());
        }
        else if(selection == "complete")
        {
            if(choice == "create" || choice == "copy")
            {
                ui->label->setText("Fiche de creation d'attribut");
            }
            else
            {
                ui->label->setText("Fiche complete d'attribut");
            }
        }

        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GAT");
    }
    else if(codeObject == "GCA")
    {
        this->setWindowTitle("Fiche descriptive de Configuration d'Attribut");
        selectedMap = dataManager->getMapFromName("mapGCA");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GCA");

        if(selection == "current")
        {
            ui->label->setText("Fiche de configuration d'attributs pour la configuration courante: "+dataManager->getCurrentConfigNameGAT());
        }
        else if(selection == "complete")
        {
            if(choice == "create" || choice == "copy")
            {
                ui->label->setText("Fiche de creation de configuration d'attribut");
            }
            else
            {
                ui->label->setText("Fiche complete de configuration d'attributs");
            }
        }

        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GCA");
    }
    else if(codeObject == "GRS")
    {
        this->setWindowTitle("Fiche descriptive de Responsable");
        selectedMap = dataManager->getMapFromName("mapGRS");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GRS");

        if(selection == "current")
        {
            ui->label->setText("Fiche de responsable pour la configuration courante: "+dataManager->getCurrentConfigNameGAT());
        }
        else if(selection == "complete")
        {
            if(choice == "create" || choice == "copy")
            {
                ui->label->setText("Fiche de creation de responsable");
            }
            else
            {
                ui->label->setText("Fiche complete de responsable");
            }
        }

        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GRS");
    }
    else if(codeObject == "GVE")
    {
        this->setWindowTitle("Fiche descriptive de Variable d'Environnement");
        selectedMap = dataManager->getMapFromName("mapGVE");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GVE");

        if(selection == "current")
        {
            ui->label->setText("Fiche de variable d'environnement pour la configuration courante: "+dataManager->getCurrentConfigNameGAT());
        }
        else if(selection == "complete")
        {
            if(choice == "create" || choice == "copy")
            {
                ui->label->setText("Fiche de creation de variable d'environnement");
            }
            else
            {
                ui->label->setText("Fiche complete de variable d'environnement");
            }
        }

        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GVE");
    }
    else if(codeObject == "GDO")
    {
        this->setWindowTitle("Fiche descriptive de Document");
        selectedMap = dataManager->getMapFromName("mapGDO");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GDO");

        if(selection == "current")
        {
            ui->label->setText("Fiche de document pour la configuration courante: "+dataManager->getCurrentConfigNameGDO());
        }
        else if(selection == "complete")
        {
            if(choice == "create" || choice == "copy")
            {
                ui->label->setText("Fiche de creation de document");
            }
            else
            {
                ui->label->setText("Fiche complete de document");
            }
        }

        attributesOfCurrentConfig = dataManager->getAttributesOfCurrentConfig("GDO");
    }

    indicFirstCreate = dataManager->getIndicFirstCreate();
    incrementCreation = dataManager->getIncrementCreation();

    if(choice == "create" || choice == "copy")
    {
            QString currentKey;
            int currentKeyInt;
            int currentMaxKey = 0;
            QList<QString> keysList = selectedMap->keys();

            for(int i=0; i<keysList.count(); ++i)
            {
                currentKey = keysList[i];
                currentKeyInt = currentKey.toInt();
                if(currentKeyInt > currentMaxKey)
                {
                    currentMaxKey = currentKeyInt;
                }
            }
            currentMaxKey++;
            if(indicFirstCreate != 1)
            {
                currentMaxKey = currentMaxKey + incrementCreation;
            }
            QString currentMaxKeyString = QString::number(currentMaxKey);
            this->currentMaxKey = currentMaxKeyString;
            int maxNumOdre = keysList.count();
            maxNumOdre++;
            QString maxNumOrdreString = QString::number(maxNumOdre);
            this->currentMaxNumOrdre = maxNumOrdreString;
     }




    mapsOfKey = selectedMap->value(key);

    attributeNamesOfCurrentConfig = dataManager->getAttributesOfCurrentConfigNames(attributesOfCurrentConfig);
    QString testName;
    QString nameAttributeSelected;
    const QMap<QString, QMap<QString, QString> > mapGAT = *dataManager->getMapFromName("mapGAT");
    QStringList list;
    QString baliseName;

    for(iterator = mapsOfKey.begin(); iterator != mapsOfKey.end(); ++iterator)
    {
        name = iterator.key();
        if(choice == "create" || choice =="copy")
        {
            if(codeObject == "GDO")
            {
                if(name == "NumOrdre")
                {
                    value = currentMaxKey;
                }
                else
                {
                    if(choice == "create")
                    {
                        value = "";
                    }
                    else
                    {
                        value = iterator.value();
                    }
                }
            }
            else
            {
                if(name == "Id")
                {
                    value = currentMaxKey;
                }
                else if(name == "NumOrdre")
                {
                    value = currentMaxNumOrdre;
                }
                else
                {
                    if(choice == "create")
                    {
                        value = "";
                    }
                    else
                    {
                        value = iterator.value();
                    }
                }
            }
        }
        else
        {
            value = iterator.value();
        }

        ++iterator;
        type = iterator.value();
        ++iterator;
        var = iterator.value();
        for(int i=0; i!=attributesOfExportConfig.length();i++)
        {
            testName = attributesOfExportConfig[i];
            baliseName = mapGAT[testName]["Titre"];
            list = baliseName.split(" ");
            baliseName = list[0];

            if(baliseName == name)
            {
                if(selection == "current")
                {
                    for(int j=0; j!=attributeNamesOfCurrentConfig.length();++j)
                    {
                        if(attributeNamesOfCurrentConfig[j]==name)
                        {
                            nameAttributeSelected = mapGAT[testName]["NomAttribut"];
                            setNewWidget(type, name, var, value, nameAttributeSelected);
                            break;
                        }
                    }
                }
                else if(selection == "complete")
                {
                    nameAttributeSelected = mapGAT[testName]["NomAttribut"];
                    setNewWidget(type, name, var, value, nameAttributeSelected);
                    break;
                }

            }

        }

    }

    if(choice == "create" || choice =="copy")
    {
        ui->instructionsLabel->setText("Saisissez les champs voulus et cliquez sur OK pour creer un nouvel objet");
    }
    else
    {
       ui->instructionsLabel->setText("Modifiez les attributs editables voulus et cliquez sur OK pour valider les modifications \n(Mode modification et administration uniquement)");
    }

    //Permet d'enlever la croix de fermeture de la fiche descritive (génère des bugs)
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint| Qt::WindowSystemMenuHint);

    //Si l'utilisateur ne possède pas le niveau d'autorisation il ne peut pas valider les modifications
    if(dataManager->getAccessLevel() < 1)
    {
        ui->buttonBox->setEnabled(false);
    }

    QPalette pal(palette());
    QLinearGradient gradient(this->rect().topLeft(), this->rect().bottomRight());
    gradient.setColorAt(0, QColor(255,255,255,255));
    gradient.setColorAt(1, QColor(245,255,255,255));
    pal.setBrush(QPalette::Background, QBrush(gradient));
    this->setPalette(pal);
}



DescriptiveCard::~DescriptiveCard()
{
    delete ui;
}


void DescriptiveCard::setNewWidget(QString type, QString name, QString var,  QString value, QString nameAttributeSelected)
{
    if(type == "string")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        lineEdit->setText(value);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);
    }
    else if(type == "num")
    {
        QSpinBox *spinBox = new QSpinBox;
        spinBox->setMaximum(100000);
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        if(name=="Id" || name == "NumOrdre")
        {
            spinBox->setEnabled(false);
        }
        int valueInt = value.toInt();
        spinBox->setValue(valueInt);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        spinBox->setSpecialValueText(tr("-"));
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->addWidget(spinBox);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);
    }
    else if(type == "enum")
    {
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        QButtonGroup *exclusiveGroup = new QButtonGroup;
        QString valueVE;
        int row = 0;
        if(var != "-")
        {
            QStringList list;
            QMap<QString, QMap<QString, QString> >::ConstIterator iteratorGVE;
            for(iteratorGVE = mapGVE->begin(); iteratorGVE != mapGVE->end(); ++iteratorGVE)
            {
                if(iteratorGVE.value()["Nom"] == var)
                {
                    valueVE = iteratorGVE.value()["Valeur"];
                    list = valueVE.split(":");
                    int column = 0;
                    for(int i=0; i < list.length(); ++i)
                    {
                        QRadioButton *radioButton = new QRadioButton(this);
                        radioButton->setText(list[i]);
                        radioButton->setAutoExclusive(true);
                        if(list[i] == value)
                        {
                            radioButton->setChecked(true);
                        }
                        if(column <= 3)
                        {
                            exclusiveGroup->addButton(radioButton);
                            gLayout2->addWidget(radioButton,row,column,1,1,Qt::AlignLeft);
                            ++column;
                        }
                        else
                        {
                            exclusiveGroup->addButton(radioButton);
                            ++row;
                            column =0;
                            gLayout2->addWidget(radioButton,row,column,1,1,Qt::AlignLeft);
                            ++column;
                        }
                    }
                }
            }
        }
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->setVerticalSpacing(0);
        ui->verticalLayout->addLayout(hLayout);
        exclusiveGroup->setExclusive(true);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);
    }
    else if(type == "strdate")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        lineEdit->setText(value);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);

    }
    else if(type == "strtime")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        lineEdit->setText(value);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);

    }
    else if(type == "time")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        lineEdit->setText(value);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);
    }
    else if(type == "bool")
    {
        QComboBox *comboBox = new QComboBox;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        QStringList list = QStringList() <<tr("Oui") <<tr("Non");
        comboBox->addItems(list);
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        for(int i=0; i<list.length();++i)
        {
            if (list[i] == value)
            {
                comboBox->setCurrentIndex(i);
            }
        }
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->addWidget(comboBox);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);

    }
    else if(type == "strlong")
    {
        QTextEdit *textEdit = new QTextEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        textEdit->setText(value);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->addWidget(textEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);
    }
    else
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        lineEdit->setText(value);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
        parameters.append(var);
        parameters.append(type);

    }

}



void DescriptiveCard::on_buttonBox_accepted()
{
    QString mapName = "map" % codeObject;
    const QMap<QString, QMap<QString, QString> > *mapSelected = dataManager->getMapFromName(mapName);
    QString id = key;
    QString keyName;
    QMap<QString, QString> mapTemp;

    if(codeObject == "GDO")
    {
        keyName = "NumOrdre";
    }

    else
    {
        keyName = "Id";
    }

    int lineCount = ui->verticalLayout->count();
    QString valueOfLineEdit = "";
    QString title;
    int count = (lineCount-1);
    QLayoutItem* nameLayout;
    QLayoutItem* titleEdit;
    QLabel* pLabel;
    QLayoutItem* pLine;
    QLayoutItem* pLineWidget;
    QString widgetType;

    QMap <QString, QString> mapOfSelectedId = mapSelected->value(id);

    int iteratorParameters = 0;

    for(int i=0; i<=count;++i)
    {
        nameLayout = ui->verticalLayout->itemAt(i);
        titleEdit = nameLayout->layout()->itemAt(0);

        pLabel = (QLabel*)titleEdit->widget();
        title = pLabel->accessibleName();
        ++i;
        pLine = ui->verticalLayout->itemAt(i);
        pLineWidget = pLine->layout()->itemAt(0);
        if(pLine->layout()->count() != 0)
        {
            widgetType = pLineWidget->widget()->metaObject()->className();
        }

        if(widgetType == "QLineEdit")
        {
            QLineEdit* pLineEdit = (QLineEdit*)pLineWidget->widget();
            valueOfLineEdit = pLineEdit->text();
        }
        else if(widgetType == "QSpinBox")
        {
            QSpinBox* pLineEdit = (QSpinBox*)pLineWidget->widget();
            if(pLineEdit->text() == "-")
            {
                valueOfLineEdit = "";
            }
            else
            {
                valueOfLineEdit = pLineEdit->text();
            }
        }
        else if(widgetType == "QComboBox")
        {
            QComboBox* pLineEdit = (QComboBox*)pLineWidget->widget();
            valueOfLineEdit = pLineEdit->currentText();
        }
        else if(widgetType == "QTextEdit")
        {
            QTextEdit* pLineEdit = (QTextEdit*)pLineWidget->widget();
            valueOfLineEdit = pLineEdit->toPlainText();
        }

        else if(widgetType == "QRadioButton")
        {
            int numberOfWidgets = pLine->layout()->count();
            for(int j=0; j<numberOfWidgets; ++j)
            {
                pLineWidget = pLine->layout()->itemAt(j);
                QRadioButton* pLineEdit = (QRadioButton*)pLineWidget->widget();
                if(pLineEdit->isChecked())
                {
                    valueOfLineEdit = pLineEdit->text();
                }
            }
        }

        QString valueMap = mapOfSelectedId[title];
        if(choice == "create" || choice == "copy")
        {
            mapTemp.insert(title, parameters[iteratorParameters]);
            ++iteratorParameters;
            mapTemp.insertMulti(title,parameters[iteratorParameters]);
            ++iteratorParameters;
            mapTemp.insertMulti(title, valueOfLineEdit);
        }
        else
        {
            if(valueMap != valueOfLineEdit)
            {
                dataManager->replaceDataOfMap(mapName,id, valueOfLineEdit,title);
                dataManager->addKeyToMapChangeList(mapName, id);
            }
        }
    }

    if(choice == "create" || choice == "copy")
    {
        dataManager->insertDataToMap(mapName, currentMaxKey, mapTemp);
        dataManager->addKeyToMapAddList(mapName, currentMaxKey);

        if(indicFirstCreate == 1)
        {
           dataManager->setIndicFirstCreate(0);
        }
        dataManager->setIncrementCreation(incrementCreation+50);
        dataManager->setIdOfLastCreatedObject(currentMaxKey);

        if(choice == "create")
        {
            mainWindow->setChoiceAddObject("new");
        }
        else if (choice == "copy")
        {
            mainWindow->setChoiceAddObject("copy");
        }
    }

    else if(choice == "modify")
    {
        mainWindow->setChoiceAddObject("modify");
    }

    mainWindow->setKeysToTreat(key);
    mainWindow->updateLayoutsViewers();
    mainWindow->updateLayoutsOptions();
    mainWindow->setChoiceAddObject("none");
    if(codeObject == "GAT")
    {
        //On emet le signal qui conduit au slot de changement des colonnes
        mainWindow->triggerSignalChangeColumn();
    }
    mainWindow->resetKeysToTreat();
}

