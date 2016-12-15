#include "filewriter.h"
#include "datamanager.h"
using namespace std;

#include <QMessageBox>
#include <QStringBuilder>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include <QMap>

FileWriter::FileWriter(DataManager *dataManager):
dataManager(dataManager)
{
    mapAddList = dataManager->getMapAddList();
    mapEraseList = dataManager->getMapEraseList();
    settings = dataManager->getFileSetting();
}

void FileWriter::modifyFiles(const QString mapName)
{
    mapChangeList = dataManager->getMapChangeList();

    if(mapName == "mapGCS")
    {
        QDomDocument dom("Filegenerated");
        QFile file(settings->value("fileGCS").toString());

        if(!file.open(QIODevice::ReadOnly))
        {
            file.close();
            return;
        }

        if(!dom.setContent(&file)) {
            file.close();
            return;
        }

        file.close();

        QDomNode docElem = dom.documentElement().firstChild();

        QString keyName = "CodeObjet";
        const QMap <QString, QMap<QString, QString> > *mapGCS = dataManager->getMapFromName("mapGCS");
        QDomNode n;
        QDomElement e;
        QString baliseName;
        QString baliseValue;
        QString newValue;
        QMap< QString, QString> smallMap;

        while(!docElem.isNull())
        {
            n= docElem.firstChild();
            while(!n.isNull())
            {
               e = n.toElement();
               baliseName = e.tagName();
               if(baliseName == keyName)
               {
                    baliseValue = e.text();
                    n = docElem.firstChild();
                    smallMap = mapGCS->value(baliseValue);
                    while(!n.isNull())
                    {
                        e=n.toElement();
                        baliseName = e.tagName();
                        baliseValue = e.text();
                        newValue = smallMap[baliseName];
                        if(newValue != baliseValue)
                        {
                            QDomElement newElement = dom.createElement(baliseName);
                            newElement.appendChild(dom.createTextNode(newValue));
                            e.parentNode().replaceChild(newElement,e);

                        }
                        n = n.nextSibling();
                    }
                    break;
               }
               else
               {
                   n = n.nextSibling();
               }
            }
            docElem = docElem.nextSibling();

        }

        file.open(QIODevice::WriteOnly);
        file.write(dom.toByteArray());
        file.close();

    }

    else
    {
        QDomDocument dom("Filegenerated");
        QStringList nameObjectDec = mapName.split("p");
        QString nameObject = nameObjectDec[1];
        QFile file(settings->value("file"%nameObject).toString());
        QString keyName;

        if(!file.open(QIODevice::ReadOnly))
        {
            file.close();
            return;
        }

        if(!dom.setContent(&file)) {
            file.close();
            return;
        }

        file.close();

        QDomNode docElem = dom.documentElement().firstChild();
        if(mapName == "mapGDO")
        {
            keyName = "NumOrdre";
        }
        else
        {
            keyName = "Id";
        }
        const QMap <QString, QMap<QString, QString> > *mapGDO = dataManager->getMapFromName(mapName);
        QDomNode n;
        QDomElement e;
        QString baliseName;
        QString baliseValue;
        QString newValue;
        QMap< QString, QString> smallMap;

        QStringList currentMapChangeList = mapChangeList.values(mapName);
        QString mapToChange;


        while(!docElem.isNull())
        {
            n= docElem.firstChild();
            while(!n.isNull())
            {
               e = n.toElement();
               baliseName = e.tagName();
               baliseValue = e.text();
               if(baliseName == keyName)
               {
                   for(int j=0; j<currentMapChangeList.length(); ++j)
                   {
                       mapToChange = currentMapChangeList[j];
                       if(mapToChange == baliseValue)
                       {
                           n = docElem.firstChild();
                           smallMap = mapGDO->value(baliseValue);
                           while(!n.isNull())
                           {
                               e=n.toElement();
                               baliseName = e.tagName();
                               baliseValue = e.text();
                               newValue = smallMap[baliseName];
                               if(newValue != baliseValue)
                               {
                                   if(baliseValue != "")
                                   {
                                        e.firstChild().setNodeValue(newValue);
                                   }
                                   else
                                   {
                                       QDomElement newElement = dom.createElement(baliseName);
                                       newElement.appendChild(dom.createTextNode(newValue));
                                       e.parentNode().replaceChild(newElement,e);
                                   }
                               }
                               n = n.nextSibling();
                           }

                           //currentMapChangeList.erase();
                           docElem = docElem.nextSibling();
                           break;
                       }
                   }
                   //docElem = docElem.nextSibling();
                   break;
               }
               else
               {
                   n = n.nextSibling();
               }
            }
            docElem = docElem.nextSibling();

        }

        file.open(QIODevice::WriteOnly);
        file.write(dom.toByteArray());
        file.close();
    }
}

void FileWriter::addToFiles(const QString mapName)
{
    mapAddList = dataManager->getMapAddList();

    const QMap<QString, QMap<QString, QString> > *selectedMap = dataManager->getMapFromName(mapName);

    QDomDocument dom("Filegenerated");
    QStringList nameObjectDec = mapName.split("p");
    QString nameObject = nameObjectDec[1];
    QFile file(settings->value("file"%nameObject).toString());
    QString keyName;

    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return;
    }

    if(!dom.setContent(&file)) {
        file.close();
        return;
    }

    file.close();

    QStringList listToadd = mapAddList.values(mapName);
    QStringList smallMapListKeys;
    QStringList smallMapListValues;
    QString smallMapKey;
    QString smallMapValue;
    QString type;
    QString var;
    QString keyToAdd;
    QMap<QString, QString> smallMap;

    for(int i=0; i<listToadd.count(); ++i)
    {
        keyToAdd = listToadd[i];
        smallMap = selectedMap->value(keyToAdd);
        smallMapListKeys = smallMap.keys();
        smallMapListValues = smallMap.values();
        QDomElement newObject = dom.createElement(QString("Object"));
        QDomElement newElem;
        QDomText newElemText;
        for(int j=0; j<smallMapListKeys.length(); ++j)
        {
            smallMapKey = smallMapListKeys[j];
            smallMapValue = smallMapListValues[j];
            ++j;
            type = smallMapListValues[j];
            ++j;
            var = smallMapListValues[j];

            newElem = dom.createElement(QString(smallMapKey));
            if(var == "-"&& type != "string")
            {
               newElem.setAttribute("type", type);
            }
            else if(var != "-" && type == "string")
            {
                newElem.setAttribute("var", var);
            }
            else if(var != "-" && type != "string")
            {
                newElem.setAttribute("type", type);
                newElem.setAttribute("var", var);
            }
            newElemText = dom.createTextNode(QString(smallMapValue));
            newElem.appendChild(newElemText);
            newObject.appendChild(newElem);
        }
        dom.documentElement().appendChild(newObject);

    }

    file.open(QIODevice::WriteOnly);
    file.write(dom.toByteArray());
    file.close();
}

void FileWriter::eraseOfFiles(const QString mapName)
{
    mapEraseList = dataManager->getMapEraseList();

    QDomDocument dom("Filegenerated");
    QStringList nameObjectDec = mapName.split("p");
    QString nameObject = nameObjectDec[1];
    QFile file(settings->value("file"%nameObject).toString());

    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return;
    }

    if(!dom.setContent(&file)) {
        file.close();
        return;
    }

    file.close();


    QStringList listToErase = mapEraseList.values(mapName);

    QDomNode docElem = dom.documentElement().firstChild();
    QString keyName;
    if(mapName == "mapGDO")
    {
        keyName = "NumOrdre";
    }
    else
    {
        keyName = "Id";
    }

    QDomNode n;
    QDomElement e;
    QString baliseName;
    QString baliseValue;
    QDomNodeList nodesToErase;


    while(!docElem.isNull())
    {
        n= docElem.firstChild();
        while(!n.isNull())
        {
           e = n.toElement();
           baliseName = e.tagName();
           baliseValue = e.text();
           if(baliseName == keyName)
           {
               for(int i=0; i< listToErase.count(); ++i)
               {
                   if(baliseValue == listToErase[i])
                   {
                       dom.documentElement().removeChild(docElem);
                       break;
                   }
               }
               //docElem = docElem.nextSibling();
               break;
           }
           else
           {
            n = n.nextSibling();
           }
        }
        docElem = docElem.nextSibling();
    }
#if 0
    nodesToErase = dom.elementsByTagName("Object/");
    for(int i=0; i<nodesToErase.count(); ++i)
    {
        dom.documentElement().removeChild(nodesToErase.item(i));
    }
#endif
    file.open(QIODevice::WriteOnly);
    file.write(dom.toByteArray());
    file.close();
}

void FileWriter::compareCurrentMaps(const QString mapName)
{
    mapAddList = dataManager->getMapAddList();
    mapEraseList = dataManager->getMapEraseList();

    QStringList keysToAdd = mapAddList.values(mapName);
    QStringList keysToErase = mapEraseList.values(mapName);
    QStringList listOfMatchingKeys;
    for(int i=0; i<keysToAdd.count(); ++i)
    {
        for(int j=0; j<keysToErase.count();++j)
        {
            if(keysToAdd[i]==keysToErase[j])
            {
                listOfMatchingKeys.append(keysToAdd[i]);
                break;
            }
        }
    }

    if(!listOfMatchingKeys.isEmpty())
    {
        QStringList valuesErase;
        QStringList valuesAdd;
        valuesErase = mapEraseList.values(mapName);
        valuesAdd = mapAddList.values(mapName);

        for(int k=0; k<listOfMatchingKeys.count();++k)
        {
            valuesErase.removeOne(listOfMatchingKeys[k]);
            valuesAdd.removeOne(listOfMatchingKeys[k]);
        }

        mapAddList.remove(mapName);
        mapEraseList.remove(mapName);

        for(int l=0; l<valuesAdd.count();++l)
        {
            if(mapAddList.value(mapName).isEmpty())
            {
                mapAddList.insert(mapName,valuesAdd[l]);
            }
            else
            {
                mapAddList.insertMulti(mapName,valuesAdd[l]);
            }
        }

        for(int l=0; l<valuesErase.count();++l)
        {
            if(mapEraseList.value(mapName).isEmpty())
            {
                mapEraseList.insert(mapName,valuesErase[l]);
            }
            else
            {
                mapEraseList.insertMulti(mapName,valuesErase[l]);
            }
        }
#if 0
            for(int i=0; i< mapEraseList.values(mapName).count(); ++i)
            {
                if(values.value(i) == listOfMatchingKeys[k])
                {
                    mapEraseList.values(mapName).removeAt(i);
                    mapEraseList.values(mapName).removeOne(listOfMatchingKeys[k]);
                    break;
                }
            }
            mapEraseList.value(mapName,listOfMatchingKeys[k]).clear();
            mapAddList.value(mapName,listOfMatchingKeys[k]).clear();
#endif
        dataManager->setMapAddList(mapAddList);
        dataManager->setMapEraseList(mapEraseList);
    }
}
