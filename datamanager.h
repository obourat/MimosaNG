#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QStringList>
#include <QMap>

class DataManager
{
public:
    //Constructeur
    DataManager();
    //Destructeur
    ~DataManager();
    // Permet d'insérer des données dans la map qui a pour nom mapName, qui a pour clé key, et on met la valeur map
    void insertDataToMap(const QString& mapName, const QString& key, const QMap<QString, QString>& map);
    //Fonction qui retourne la liste des clés pertinentes en fonction des critères de l'objet objectType (retourne les attributs de la configuration courante)
    const QStringList getKeysOfCurrentConfig(const QString& objectType);
    //Prend en paramètre une map mapName et retourne la liste de sousMaps contenue dans mapName
    const QList<QMap<QString, QString> > getSmallMapsFromMapName(const QString& mapName);
private:

    //Prend en paramètre mapName et vérifie s'il il s'agit bien d'un nom de map, si oui, retourne la map correspondante, sinon, ne renvoie rien
    const QMap<QString, QMap<QString, QString> >* getMapFromName(const QString& mapName) const;

    //On définit les différentes maps
    QMap<QString, QMap<QString, QString> > mapGVE;
    QMap<QString, QMap<QString, QString> > mapGCA;
    QMap<QString, QMap<QString, QString> > mapGAT;
    QMap<QString, QMap<QString, QString> > mapGRS;
};

#endif // DATAMANAGER_H
