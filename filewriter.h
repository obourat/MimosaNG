#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>
#include <QMap>


class DataManager;

class FileWriter
{
public:
    //Constructeur
    FileWriter(DataManager *dataManager);
    //Destructeur
    ~FileWriter();
    //Modifie le fichier XML avec en le comparant avec la map associée
    void modifyFiles(const QString mapName);
    void addToFiles(const QString mapName);
    void eraseOfFiles(const QString mapName);

private:
    DataManager *dataManager;
    QMap<QString, QString> mapChangeList;
    QMap<QString, QString> mapAddList;
    QMap<QString, QString> mapEraseList;
};

#endif // FILEWRITER_H
