#include "filereader.h"
#include "datamanager.h"
#include <QFile>

//Constructeur
FileReader::FileReader(DataManager *dataManager, const QString &fileName):dataManager(dataManager), fileName(fileName)
{

}

//Destructeur
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
    QString key;
    QString type;
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
                if(!type.isEmpty())
                {
                    mapTemp.insert(baliseName, type);
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
            if(!type.isEmpty())
            {
                mapTemp.insert(baliseName, type);
            }
            else if(type.isEmpty())
            {
                type = "string";
                mapTemp.insert(baliseName, type);
            }
            mapTemp.insertMulti(baliseName, baliseData);
            type.clear();
        }
    }
    if(!key.isEmpty())
    {
        dataManager->insertDataToMap(mapName, key, mapTemp);
    }
}
