#include "filewriter.h"
#include "datamanager.h"
using namespace std;

#include <QMessageBox>
#include <QFile>

FileWriter::FileWriter(DataManager *dataManager):
dataManager(dataManager)
{
}

void FileWriter::modifyFiles(const QString mapName)
{
#if 0
    QDomDocument dom("Filegenerated");
    QFile file("GCS_Export.xml");

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier XML");
        file.close();
        return;
    }

    if(!dom.setContent(&file)) {
        QMessageBox::critical(this, "erreur", "Impossible de lire le fichier XML");
        file.close();
        return;
    }
    file.close();
#endif
}

