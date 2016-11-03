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
}

void FileWriter::modifyFiles(const QString mapName)
{

    mapChangeList = dataManager->getMapChangeList();

    if(mapName == "mapGCS")
    {
        QDomDocument dom("Filegenerated");
        QFile file("GCS_Export.xml");

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
        QFile file(nameObject % "_Export.xml");
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
                                   //QDomElement newElement = dom.createElement(baliseName);
                                   //newElement.appendChild(dom.createTextNode(newValue));
                                   e.firstChild().setNodeValue(newValue);
                                   //e.parentNode().replaceChild(newElement,e);

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

