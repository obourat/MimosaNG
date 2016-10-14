#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QString>


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

private:
    DataManager *dataManager;
};

#endif // FILEWRITER_H
