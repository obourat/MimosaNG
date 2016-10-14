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
    //Fonction qui retourne le nom de des attributs de la configuration courante
    QStringList getAttributesOfCurrentConfigNames(QStringList attributesKeyList);
    //Fonction qui retourne les attributs de la configuration Export (celle qui contient tous les attributs)
    const QStringList getAttributesOfExportConfig(const QString &objectType);
    //Prend en paramètre une map mapName et retourne la liste de sousMaps contenue dans mapName
    const QList<QMap<QString, QString> > getSmallMapsFromMapName(const QString& mapName, QString codeObject);
    //Prend en paramètre  une map mapName et retourne la liste des configurations d'attributs correspondants au code objet codeObject
    const QList<QMap<QString, QString> > getSmallMapsFromMapNameOptions(const QString& mapName, QString codeObject, QString codeObjectForOptions);
    //Prend en parmètre une liste de sous map et renvoie les valeurs faisant partie de la configuration d'attribut
    const QList<QMap<QString, QString> > selectAttributesOfSmallMapsList(const QList<QMap<QString, QString> > maps, const QString codeObject, const QString keyName);
    //Selectionne les attributs à afficher pour les options uniquement
    const QList<QMap<QString, QString> > selectAttributesOfSmallMapsListOptions(const QList<QMap<QString, QString> > maps, const QString codeObject, const QString keyName);
    //Cherche dans le fichier GCS le nom de la configuration standard pour chaque objet
    const QString getStandardConfigName(QString codeObject);
    //Cherche à partir du nom de la configuration courante l'Id associé à cette configuration pour le type d'objet donné dans la map donnée
    const QString getCurrentConfigId(const QString currentConfigName, const QString mapName, const QString codeObject);
    //Prend en paramètre mapName et vérifie s'il il s'agit bien d'un nom de map, si oui, retourne la map correspondante, sinon, ne renvoie rien
    const QMap<QString, QMap<QString, QString> >* getMapFromName(const QString& mapName) const;
    //Insère dans mapConcordance les valeurs pour chaque objet
    void setDataOfMapConcordance();


    //getters et setters des noms de configurations courantes pour les différents objets
    QString getCurrentConfigNameGCA() const;
    void setCurrentConfigNameGCA(const QString &value);

    QString getCurrentConfigNameGAT() const;
    void setCurrentConfigNameGAT(const QString &value);

    QString getCurrentConfigNameGRS() const;
    void setCurrentConfigNameGRS(const QString &value);

    QString getCurrentConfigNameGVE() const;
    void setCurrentConfigNameGVE(const QString &value);

private:


    //On définit les différentes maps des objets
    QMap<QString, QMap<QString, QString> > mapGVE;
    QMap<QString, QMap<QString, QString> > mapGCA;
    QMap<QString, QMap<QString, QString> > mapGAT;
    QMap<QString, QMap<QString, QString> > mapGRS;
    QMap<QString, QMap<QString, QString> > mapGCS;
    //Map qui possède comme clé un triplé codeObjet, numéroInterne, InfoInterne permettant d'accéder à la valeur de la balise de titre des attributs
    QMap<QString, QString> mapConcordance;

    //On définit l'attribut correpondant au nom de la configuration courante
    QString currentConfigNameGCA;
    QString currentConfigNameGAT;
    QString currentConfigNameGRS;
    QString currentConfigNameGVE;
};

#endif // DATAMANAGER_H
