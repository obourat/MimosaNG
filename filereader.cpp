#include "filereader.h"
#include "datamanager.h"
#include <QFile>
#include <QDate>
#include <QStringBuilder>

FileReader::FileReader(DataManager *dataManager, const QString &fileName):
    dataManager(dataManager),
    fileName(fileName)
{

}

FileReader::~FileReader()
{

}

//Allocation d'un nom de fichier a fileName
void FileReader::setFileName(const QString &fileName)
{
    this->fileName = fileName;
}

//Fonction qui parse le fichier fileName
bool FileReader::parseFile(const QString &rootName, const QString &blockName, const QString &keyBlock, const QString& mapName)
{
    if(fileName.isEmpty())
    {
        return false;
    }

    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    reader.setDevice(&file);


    if(reader.readNextStartElement())
    {
        if(reader.name() != rootName)
        {
            return false;
        }

        while(reader.readNextStartElement())
        {
            if(reader.name() != blockName)
            {
                return false;
            }
            else
            {
                handlesBlock(keyBlock, mapName);
            }

        }
    }

    return true;
}

//Fonction qui lit un objet et stocke ses données dans une mapTemp, puis stocke la mapTemp dans la map mapName dans dataManager
void FileReader::handlesBlock(const QString &keyBlock, const QString &mapName)
{
    // Lecture d'un block
    QString baliseName;
    QString baliseData;
    QDate initialDate;
    QDate date;
    QString dateString;
    int nameInt;
    int days;
    int day;
    int month;
    int year;
    QString dayStr;
    QString monthStr;
    QString yearStr;
    QString dateFormed;
    QString key;
    QString type;
    QString var;
    QMap<QString, QString> mapTemp;

    while(reader.readNextStartElement())
    {
        baliseName = reader.name().toString();
        foreach(const QXmlStreamAttribute &attr, reader.attributes())
        {
            if (attr.name().toString() == QLatin1String("type"))
            {
                type = attr.value().toString();
            }
            if (attr.name().toString() == QLatin1String("var"))
            {
                var = attr.value().toString();
            }
        }

        if(baliseName.isEmpty())
        {
            reader.skipCurrentElement();
        }
        else if(baliseName == keyBlock)
        {
            QString keyData;
            keyData = reader.readElementText();

            if(keyData.isEmpty())
            {
                reader.skipCurrentElement();
            }
            else
            {
                key = keyData;
                if(!var.isEmpty())
                {
                    mapTemp.insert(baliseName, var);
                }
                else if(var.isEmpty())
                {
                    var="-";
                    mapTemp.insertMulti(baliseName, var);
                }
                if(!type.isEmpty())
                {
                    mapTemp.insertMulti(baliseName, type);
                }
                else if(type.isEmpty())
                {
                    type = "string";
                    mapTemp.insertMulti(baliseName, type);

                }
                mapTemp.insertMulti(baliseName, keyData);
                type.clear();
            }
        }
        else
        {
            baliseData = reader.readElementText();

            if(!var.isEmpty())
            {
                mapTemp.insert(baliseName, var);
            }
            else if(var.isEmpty())
            {
                var="-";
                mapTemp.insert(baliseName, var);
            }

            if(!type.isEmpty())
            {
                //Cas particulier pour la conversion de la date
                if(type == "time")
                {
                    initialDate = QDate::fromString("01011970","ddMMyyyy");
                    nameInt = baliseData.toInt();
                    days = nameInt/86400;
                    date = initialDate.addDays(days);
                    //dateString = date.toString();
                    //baliseData = dateString;
                    day = date.day();
                    month = date.month();
                    year = date.year();
                    dayStr = QString::number(day);
                    monthStr = QString::number(month);
                    yearStr = QString::number(year);

                    if(dayStr.length() == 1)
                    {
                        dayStr = "0" % dayStr;
                    }
                    if(monthStr.length() == 1)
                    {
                        monthStr = "0" % monthStr;
                    }
                    dateFormed = dayStr % "/" % monthStr % "/" % yearStr;
                    baliseData = dateFormed;


                }
                mapTemp.insertMulti(baliseName, type);
            }

            else if(type.isEmpty())
            {
                type = "string";
                mapTemp.insertMulti(baliseName, type);
            }

            mapTemp.insertMulti(baliseName, baliseData);
            type.clear();
            var.clear();
        }
    }
    if(!key.isEmpty())
    {
        dataManager->insertDataToMap(mapName, key, mapTemp);
    }
}
