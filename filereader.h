#ifndef FILEREADER_H
#define FILEREADER_H

#include <QString>
#include <QMap>
#include <QXmlStreamReader>

class DataManager;

class FileReader
{
public:
    FileReader(DataManager *dataManager, const QString& fileName = QString());
    ~FileReader();

    //Associe à l'attribut fileName la valeur rentrée en paramètre
    void setFileName(const QString& fileName);
    //Parse le fichier en tenant compte de son premier élément, du nom de chaque bloc, de la clé pour chaque bloc, et stocke les données dans la map mapName
    bool parseFile(const QString& rootName, const QString& blockName, const QString& keyBlock, const QString& mapName);

private:
    //Lit un objet et stocke ses données dans une mapTemp, puis stocke la mapTemp dans la map mapName dans dataManager
    void handlesBlock(const QString &keyBlock, const QString& mapName);

private:
    //Définition des objets nécessaires à la gestion des données
    DataManager *dataManager;

    //Nom du fichier lu
    QString fileName;
    //Définition du parser XML
    QXmlStreamReader reader;
};

#endif // FILEREADER_H
