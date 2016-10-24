#include "filewriter.h"
#include "datamanager.h"
using namespace std;

#include <QMessageBox>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>


FileWriter::FileWriter(DataManager *dataManager):
dataManager(dataManager)
{
}

void FileWriter::modifyFiles(const QString mapName)
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
    if(mapName == "mapGCS")
    {
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

}

