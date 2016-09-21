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
    const QStringList getAttributesOfCurrentConfig(const QString& objectType);
    //Prend en paramètre une map mapName et retourne la liste de sousMaps contenue dans mapName
    const QList<QMap<QString, QString> > getSmallMapsFromMapName(const QString& mapName, QString codeObject);
    //Prend en parmètre une liste de sous map et renvoie les valeurs faisant partie de la configuration d'attribut
    const QList<QMap<QString, QString> > selectAttributesOfSmallMapsList(const QList<QMap<QString, QString> > maps, const QString codeObject);
    //Insère dans mapConcordance les valeurs pour chaque objet
    void setDataOfMapConcordance();

    QString getCurrentConfigName() const;
    void setCurrentConfigName(const QString &value);

private:

    //Prend en paramètre mapName et vérifie s'il il s'agit bien d'un nom de map, si oui, retourne la map correspondante, sinon, ne renvoie rien
    const QMap<QString, QMap<QString, QString> >* getMapFromName(const QString& mapName) const;

    //On définit les différentes maps
    QMap<QString, QMap<QString, QString> > mapGVE;
    QMap<QString, QMap<QString, QString> > mapGCA;
    QMap<QString, QMap<QString, QString> > mapGAT;
    QMap<QString, QMap<QString, QString> > mapGRS;
    QMap<QString, QMap<QString, QString> > mapGCS;
    QMap<QString, QString> mapConcordance;

    //On définit l'attribut correpondant au nom de la configuration courante
    QString currentConfigName;
};

#endif // DATAMANAGER_H
