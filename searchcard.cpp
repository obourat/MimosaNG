 #include "searchcard.h"
#include "ui_searchcard.h"
#include "datamanager.h"

#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QStringBuilder>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QDate>
#include <QProgressDialog>
#include <QCheckBox>

SearchCard::SearchCard(DataManager *dataManager, QString codeObject, QString key, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchCard),
    dataManager(dataManager),
    codeObject(codeObject),
    key(key)

{
    ui->setupUi(this);

    QMap<QString,QString> mapsOfKey;
    QMap<QString, QString> ::Iterator iterator;
    QString type;
    QString name;
    QString var;
    QStringList attributesOfExportConfig;
    QStringList attributeNamesOfCurrentConfig;

    mapGVE = dataManager->getMapFromName("mapGVE");

    ui->label->setText("Recherche par criteres");
    ui->labelInfo->setText("Les criteres non renseignes seront ignores pour la recherche");

    if(codeObject == "GAT")
    {
        this->setWindowTitle("Filtre de recherche d'attribut");
        selectedMap = dataManager->getMapFromName("mapGAT");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GAT");
    }
    else if(codeObject == "GCA")
    {
        this->setWindowTitle("Filtre de recherche de configuration d'attribut");
        selectedMap = dataManager->getMapFromName("mapGCA");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GCA");
    }
    else if(codeObject == "GRS")
    {
        this->setWindowTitle("Fiche descriptive de Responsable");
        selectedMap = dataManager->getMapFromName("mapGRS");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GRS");
    }
    else if(codeObject == "GVE")
    {
        this->setWindowTitle("Fiche descriptive de Variable d'Environnement");
        selectedMap = dataManager->getMapFromName("mapGVE");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GVE");
    }
    else if(codeObject == "GDO")
    {
        this->setWindowTitle("Fiche descriptive de Document");
        selectedMap = dataManager->getMapFromName("mapGDO");
        attributesOfExportConfig = dataManager->getAttributesOfExportConfig("GDO");
    }

    //On selectionne tous les attributs de la clé key
    mapsOfKey = selectedMap->value(key);

    //On cherche tous les noms des attributs la configuration export
    attributeNamesOfCurrentConfig = dataManager->getAttributesOfCurrentConfigNames(attributesOfExportConfig);
    QString testName;
    QString nameAttributeSelected;
    QStringList list;
    QString baliseName;
    const QMap<QString, QMap<QString, QString> > mapGAT = *dataManager->getMapFromName("mapGAT");

    for(iterator = mapsOfKey.begin(); iterator != mapsOfKey.end(); ++iterator)
    {
        name = iterator.key();
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
                nameAttributeSelected = mapGAT[testName]["NomAttribut"];

                setNewWidget(type,var, name,nameAttributeSelected);
                break;
            }

        }
    }

    QPalette Pal(palette());
    Pal.setColor(QPalette::Window, QColor(255,255,255,240));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->show();

    confirmSearch = 0;


}



void SearchCard::setNewWidget(QString type,QString var, QString name, QString nameAttributeSelected)
{
    if(type == "string")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        QComboBox *options = new QComboBox;
        QStringList list = QStringList() << tr("~") <<tr("!~") <<tr("=") <<tr("!=") <<tr("<") <<tr("<=") <<tr(">") <<tr(">=");
        options->addItems(list);
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        hLayout->addWidget(options);
        hLayout->setAlignment(options,Qt::AlignRight);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
    }
    else if(type == "num")
    {
        QSpinBox *spinBox = new QSpinBox;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        spinBox->setSpecialValueText(tr("-"));
        QHBoxLayout *hLayout = new QHBoxLayout;
        QComboBox *options = new QComboBox;
        QStringList list = QStringList() << tr("~") <<tr("!~") <<tr("=") <<tr("!=") <<tr("<") <<tr("<=") <<tr(">") <<tr(">=");
        options->addItems(list);
        hLayout->addWidget(label);
        hLayout->addWidget(options);
        hLayout->setAlignment(options,Qt::AlignRight);
        gLayout2->addWidget(spinBox);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
    }
    else if(type == "enum")
    {
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        QString value;
        int row = 0;
        if(var != "-")
        {
            QStringList list;
            QMap<QString, QMap<QString, QString> >::ConstIterator iteratorGVE;
            for(iteratorGVE = mapGVE->begin(); iteratorGVE != mapGVE->end(); ++iteratorGVE)
            {
                if(iteratorGVE.value()["Nom"] == var)
                {
                    value = iteratorGVE.value()["Valeur"];
                    list = value.split(":");
                    //list.append("");
                    int column = 0;
                    for(int i=0; i < list.length(); ++i)
                    {
                        QCheckBox *checkbox = new QCheckBox;
                        checkbox->setText(list[i]);
                        checkbox->setContentsMargins(0,0,0,0);
                        if(column <= 3)
                        {
                            gLayout2->addWidget(checkbox,row,column,1,1,Qt::AlignLeft);
                            ++column;
                        }
                        else
                        {
                            ++row;
                            column =0;
                            gLayout2->addWidget(checkbox,row,column,1,1,Qt::AlignLeft);
                            ++column;
                        }
                    }
                }
            }
        }
        else
        {
            //QCheckBox *checkbox = new QCheckBox;
            //checkbox->setText("");
            //gLayout2->addWidget(checkbox);
        }
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        QComboBox *options = new QComboBox;
        QStringList list = QStringList() <<tr("=") <<tr("!=") <<tr("ou") <<tr("ni");
        options->addItems(list);
        hLayout->addWidget(label);
        hLayout->addWidget(options);
        hLayout->setAlignment(options,Qt::AlignRight);
        gLayout2->setVerticalSpacing(0);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
    }
    else if(type == "strdate")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        QComboBox *options = new QComboBox;
        QStringList list = QStringList() <<tr("=") <<tr("!=") <<tr("<") <<tr("<=") <<tr(">") <<tr(">=");
        options->addItems(list);
        hLayout->addWidget(label);
        hLayout->addWidget(options);
        hLayout->setAlignment(options,Qt::AlignRight);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);

    }
    else if(type == "strtime")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        QComboBox *options = new QComboBox;
        QStringList list = QStringList() <<tr("=") <<tr("!=") <<tr("<") <<tr("<=") <<tr(">") <<tr(">=");
        options->addItems(list);
        hLayout->addWidget(label);
        hLayout->addWidget(options);
        hLayout->setAlignment(options,Qt::AlignRight);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);

    }
    else if(type == "time")
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        QComboBox *options = new QComboBox;
        QStringList list = QStringList() <<tr("=") <<tr("!=") <<tr("<") <<tr("<=") <<tr(">") <<tr(">=");
        options->addItems(list);
        hLayout->addWidget(label);
        hLayout->addWidget(options);
        hLayout->setAlignment(options,Qt::AlignRight);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);
    }
    else if(type == "bool")
    {
        QComboBox *comboBox = new QComboBox;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        QStringList list = QStringList() << tr("") <<tr("Oui") <<tr("Non");
        comboBox->addItems(list);
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        QComboBox *options = new QComboBox;
        QStringList listOptions = QStringList() <<tr("=") <<tr("!=") <<tr("ou") <<tr("ni");
        options->addItems(listOptions);
        hLayout->addWidget(label);
        hLayout->addWidget(options);
        hLayout->setAlignment(options,Qt::AlignRight);
        gLayout2->addWidget(comboBox);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);

    }
//    else if(type == "comment")
//    {
//        QTextEdit *textEdit = new QTextEdit;
//        QLabel *label = new QLabel;
//        label->setText(nameAttributeSelected);
//        label->setAccessibleName(name);
//        ui->verticalLayout->addWidget(label);
//        ui->verticalLayout->addWidget(textEdit);
//    }
    else
    {
        QLineEdit *lineEdit = new QLineEdit;
        QLabel *label = new QLabel;
        QGridLayout *gLayout2 = new QGridLayout;
        QComboBox *options = new QComboBox;
        QStringList list = QStringList() << tr("~") <<tr("!~") <<tr("=") <<tr("!=") <<tr("<") <<tr("<=") <<tr(">") <<tr(">=");
        options->addItems(list);
        label->setText(nameAttributeSelected);
        label->setAccessibleName(name);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        hLayout->addWidget(options);
        hLayout->setAlignment(options,Qt::AlignRight);
        gLayout2->addWidget(lineEdit);
        ui->verticalLayout->addLayout(hLayout);
        ui->verticalLayout->addLayout(gLayout2);

    }

}






//Fonction de recherche qui listes les clés de la map "map" vérifiant la présence des couples clé, valeur de la map "mapSearch"
QList<QString> SearchCard::searchMatches(const QMap<QString, QMap<QString, QString> >& map, const QMap<QString, QString >& mapSearch)
{
    QString currentMapSearchName;
    QString currentFilterOnSearch;
    QStringList currentMapSearchValues;
    QStringList valuesOfCurrentMapSearchValues;
    QString fileValue;
    int iteratorSearchListKeys = 0;
    int iteratorListKeys = 0;
    //Créations de listes contenant les clés des elements recherchés et des objets a traiter
    QList<QString> mapSearchListKeys = mapSearch.keys();
    int sizeToIterate = mapSearchListKeys.size();
    QList<QString> mapListKeys = map.keys();
    int mapListKeysSize = mapListKeys.size();
    QList<QString> resultList;
    QString currentMapValue;
    QStringList valuesOfMapKey;
    QProgressDialog progress("Recherche de resultats...", "Annuler", 0, mapListKeysSize, this);
    progress.setWindowModality(Qt::WindowModal);

    //Tant que toutes les clés de la map n'ont pas ete traitées
    while(iteratorListKeys <= mapListKeys.size())
    {
        if(progress.wasCanceled())
        {
            break;
        }

        //Si il n'y a pas de critère de recherche, on affiche tout
        if(mapSearchListKeys.size() == 0)
        {
            currentMapValue = mapListKeys.value(iteratorListKeys); //valeur de la clé à tester
            resultList.append(currentMapValue);
            ++iteratorListKeys;
        }
        else
        {
            int playAgain = 1;
            //Tant que tous les champs de recherches n'ont pas été vérifiés
            while(playAgain == 1)
            {
                currentMapValue = mapListKeys.value(iteratorListKeys); //valeur de la clé à tester
                currentMapSearchName = mapSearchListKeys.value(iteratorSearchListKeys); //nom du champ à vérifier
                //currentMapSearchValue = mapSearch[currentMapSearchName]; //valeur du nom du champ à vérifier
                currentMapSearchValues = mapSearch.values(currentMapSearchName);

                QStringList ::ConstIterator iterator;
                iterator = currentMapSearchValues.begin();
                QString value = *iterator;
                ++iterator;
                QString oper = *iterator;

                ++iteratorSearchListKeys;
                ++iteratorSearchListKeys;

                fileValue = getFileValue(map, currentMapValue, currentMapSearchName);

                if(oper == "~")
                {
                    if(fileValue.contains(value))
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;

                    }
                }
                else if(oper == "=")
                {
                    if(fileValue == value)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;

                    }
                }
                else if(oper == "!=")
                {
                    if(fileValue != value)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;

                    }
                }
                else if(oper == "!~")
                {
                    if(!fileValue.contains(value))
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;

                    }
                }
                else if(oper == "<" && currentMapSearchName.contains("Date"))
                {
                    QStringList listOfValue = value.split("/");
                    QString valueConverted = listOfValue[2] % listOfValue[1] % listOfValue[0];
                    int newValue = valueConverted.toInt();
                    listOfValue.clear();
                    valueConverted.clear();
                    listOfValue = fileValue.split("/");
                    valueConverted = listOfValue[2] % listOfValue[1] % listOfValue[0];
                    int newFileValue = valueConverted.toInt();

                    if(newFileValue < newValue)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;

                    }

                }
                else if(oper == "<=" && currentMapSearchName.contains("Date"))
                {
                    QStringList listOfValue = value.split("/");
                    QString valueConverted = listOfValue[2] % listOfValue[1] % listOfValue[0];
                    int newValue = valueConverted.toInt();
                    listOfValue.clear();
                    valueConverted.clear();
                    listOfValue = fileValue.split("/");
                    valueConverted = listOfValue[2] % listOfValue[1] % listOfValue[0];
                    int newFileValue = valueConverted.toInt();

                    if(newFileValue <= newValue)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;

                    }

                }
                else if(oper == ">" && currentMapSearchName.contains("Date"))
                {
                    QStringList listOfValue = value.split("/");
                    QString valueConverted = listOfValue[2] % listOfValue[1] % listOfValue[0];
                    int newValue = valueConverted.toInt();
                    listOfValue.clear();
                    valueConverted.clear();
                    listOfValue = fileValue.split("/");
                    valueConverted = listOfValue[2] % listOfValue[1] % listOfValue[0];
                    int newFileValue = valueConverted.toInt();

                    if(newFileValue > newValue)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;

                    }

                }
                else if(oper == ">=" && currentMapSearchName.contains("Date"))
                {
                    QStringList listOfValue = value.split("/");
                    QString valueConverted = listOfValue[2] % listOfValue[1] % listOfValue[0];
                    int newValue = valueConverted.toInt();
                    listOfValue.clear();
                    valueConverted.clear();
                    listOfValue = fileValue.split("/");
                    valueConverted = listOfValue[2] % listOfValue[1] % listOfValue[0];
                    int newFileValue = valueConverted.toInt();

                    if(newFileValue >= newValue)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;

                    }

                }
                else if(oper == "<" && !currentMapSearchName.contains("Date"))
                {
                    int compare = fileValue.compare(value);
                    if(compare < 0)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;
                    }
                }
                else if(oper == "<=" && !currentMapSearchName.contains("Date"))
                {
                    int compare = fileValue.compare(value);
                    if(compare <= 0)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;
                    }
                }
                else if(oper == ">" && !currentMapSearchName.contains("Date"))
                {
                    int compare = fileValue.compare(value);
                    if(compare > 0)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;
                    }
                }
                else if(oper == ">=" && !currentMapSearchName.contains("Date"))
                {
                    int compare = fileValue.compare(value);
                    if(compare >= 0)
                    {
                        if(iteratorSearchListKeys == sizeToIterate)
                        {
                            resultList.append(currentMapValue);
                            ++iteratorListKeys;
                            iteratorSearchListKeys = 0;
                            playAgain = 0;
                        }
                    }
                    else
                    {
                        iteratorSearchListKeys = 0;
                        ++iteratorListKeys;
                        playAgain = 0;
                    }
                }
                else if(oper == "ou")
                {
                    QStringList listOfValue = value.split("/");
                    int valueCount = listOfValue.length();
                    --valueCount;
                    for(int i=0; i <= valueCount;++i)
                    {
                        QString valueSplitted = listOfValue[i];
                        if(fileValue == valueSplitted)
                        {
                            if(iteratorSearchListKeys == sizeToIterate)
                            {
                                resultList.append(currentMapValue);
                                ++iteratorListKeys;
                                iteratorSearchListKeys = 0;
                                playAgain = 0;
                            }
                        }
                        else if(fileValue != valueSplitted && i== valueCount)
                        {
                            iteratorSearchListKeys = 0;
                            ++iteratorListKeys;
                            playAgain = 0;

                        }
                    }
                }
                else if(oper == "ni")
                {
                    QStringList listOfValue = value.split("/");
                    int valueCount = listOfValue.length();
                    --valueCount;
                    for(int i=0; i <= valueCount;++i)
                    {
                        QString valueSplitted = listOfValue[i];
                        if(fileValue == valueSplitted)
                        {
                            iteratorSearchListKeys = 0;
                            ++iteratorListKeys;
                            playAgain = 0;
                        }
                        else if(fileValue != valueSplitted && i== valueCount)
                        {
                            if(iteratorSearchListKeys == sizeToIterate)
                            {
                                resultList.append(currentMapValue);
                                ++iteratorListKeys;
                                iteratorSearchListKeys = 0;
                                playAgain = 0;
                            }
                        }
                    }
                }
            }
            progress.setValue(iteratorListKeys);
        }

    }
    progress.setValue(mapListKeysSize);

    return resultList;
}
int SearchCard::getConfirmSearch() const
{
    return confirmSearch;
}

void SearchCard::setConfirmSearch(int value)
{
    confirmSearch = value;
}

QList<QString> SearchCard::getSearchResults() const
{
    return searchResults;
}

void SearchCard::setSearchResults(const QList<QString> &value)
{
    searchResults = value;
}



SearchCard::~SearchCard()
{
    delete ui;
}

void SearchCard::on_buttonBox_accepted()
{
    int lineCount = ui->verticalLayout->count();
    QString valueOfLineEdit = "";
    QString title;
    QString valueOfChoiceFilter;
    int count = (lineCount-1);
    QLayoutItem* nameLayout;
    QLayoutItem* titleEdit;
    QLayoutItem* choiceFilter;
    QComboBox* pChoiceFilter;
    QLabel* pLabel;
    QLayoutItem* pLine;
    QLayoutItem* pLineWidget;
    QString widgetType;

    for(int i=0; i<=count;++i)
    {
        nameLayout = ui->verticalLayout->itemAt(i);
        titleEdit = nameLayout->layout()->itemAt(0);
        choiceFilter = nameLayout->layout()->itemAt(1);
        pChoiceFilter = (QComboBox*)choiceFilter->widget();
        valueOfChoiceFilter = pChoiceFilter->currentText();

        pLabel = (QLabel*)titleEdit->widget();
        title = pLabel->accessibleName();
        ++i;
        pLine = ui->verticalLayout->itemAt(i);
        pLineWidget = pLine->layout()->itemAt(0);
        if(pLine->layout()->count() != 0)
        {
            widgetType = pLineWidget->widget()->metaObject()->className();

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

            else if(widgetType == "QCheckBox")
            {
                int numberOfWidgets = pLine->layout()->count();
                for(int j=0; j<numberOfWidgets; ++j)
                {
                    pLineWidget = pLine->layout()->itemAt(j);
                    QCheckBox* pLineEdit = (QCheckBox*)pLineWidget->widget();
                    if(pLineEdit->isChecked())
                    {
                        if(valueOfLineEdit == "")
                        {
                            valueOfLineEdit = pLineEdit->text();
                        }
                        else
                        {
                            valueOfLineEdit = valueOfLineEdit % "/" % pLineEdit->text();
                        }
                    }
                }
            }

            if(valueOfLineEdit != "")
            {
                mapOfSearch.insert(title,valueOfChoiceFilter);
                mapOfSearch.insertMulti(title,valueOfLineEdit);
            }

        }
        valueOfLineEdit = "";

    }

    QMap<QString, QString> testMap = mapOfSearch;
    searchResults = searchMatches(*selectedMap,mapOfSearch);
    confirmSearch = 1;

    //emit accepted();


}

//Fonction qui retourne la valeur d'une balise de nom "searchName" dans la clé "key", dans la map "map"
QString SearchCard::getFileValue(const QMap<QString, QMap<QString, QString> > map, const QString key, const QString searchName)
{
    QString fileValue = map[key][searchName];
    return fileValue;
}



void SearchCard::on_buttonBox_rejected()
{

}
