#ifndef FILEREADER_H
#define FILEREADER_H

#include <QString>
#include <QMap>
#include <QXmlStreamReader>

class DataManager;

class FileReader
{
public:
    //Constructeur (un fileReader est associé à un dataManager qui gère les données lues et à un nom de fichier qui devra être lu)
    FileReader(DataManager *dataManager, const QString& fileName = QString());
    //Destructeur
    ~FileReader();
    //Associe à l'attribut fileName la valeur rentrée en paramètre
    void setFileName(const QString& fileName);
    //Parse le fichier en tenant compte de son premier élément, du nom de chaque bloc, de la clé pour chaque blox, et stocke les données dans la map mapName
    bool parseFile(const QString& rootName, const QString& blockName, const QString& keyBlock, const QString& mapName);

private:
    //Lit un objet et stocke ses données dans une mapTemp, puis stocke la mapTemp dans la map mapName dans dataManager
    void handlesBlock(const QString &keyBlock, const QString& mapName);

private:
    //On définit les objets dont on a besoin pour lire le fichier et transmettre les données au datamanager
    DataManager *dataManager;
    QString fileName;
    QXmlStreamReader reader;

};

#endif // FILEREADER_H
