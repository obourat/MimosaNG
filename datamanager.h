#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QSettings>

class DataManager
{
public:
    //Constructeur
    DataManager();
    //Destructeur
    ~DataManager();
    // Permet d'insérer des données dans la map qui a pour nom mapName, qui a pour clé key, et on met la valeur map
    void insertDataToMap(const QString& mapName, const QString& key, const QMap<QString, QString>& map);
    //Permet d'effacer la clé de la map selectionnée
    void eraseDataOfMap(const QString& mapName, const QString& key);
    //Fonction qui remplace les données d'une clé
    void replaceDataOfMap(const QString& mapName, const QString& key, const QString& valueToAdd, const QString smallKeyNameToReplace);
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
    //Retourne pamConcordance
    const QMap<QString, QString>* getMapConcordance() const;
    //Insère dans mapConcordance les valeurs pour chaque objet
    void setDataOfMapConcordance();
    //Ajoute un id a modifier dans le XML
    void addKeyToMapChangeList(QString mapName, QString id);
    //Ajoute un id a ajouter dans le XML
    void addKeyToMapAddList(QString mapName, QString id);
    //Ajoute un Id a supprimer dans le XML
    void addKeyToMapEraseList(QString mapName, QString id);
    //Ajoute à la map l'attribut collé à une configuration
    void pasteAttribute(QString idCurrentConfig, QString codeObjectPaste);
    //Renvoie la valeur voulue de la map slectionnée
    QString findValueOfMap(QString mapName, QString key, QString nameAttribute);

    //Getters et Setters
    QString getCurrentConfigNameGCA() const;
    void setCurrentConfigNameGCA(const QString &value);

    QString getCurrentConfigNameGAT() const;
    void setCurrentConfigNameGAT(const QString &value);

    QString getCurrentConfigNameGRS() const;
    void setCurrentConfigNameGRS(const QString &value);

    QString getCurrentConfigNameGVE() const;
    void setCurrentConfigNameGVE(const QString &value);

    QString getCurrentConfigNameGDO() const;
    void setCurrentConfigNameGDO(const QString &value);

    QMap<QString, QString> getMapChangeList() const;
    void setMapChangeList(const QMap<QString, QString> &value);

    QMap<QString, QString> getMapAddList() ;
    void setMapAddList(QMap<QString, QString> &value);

    QMap<QString, QString> getMapEraseList() ;
    void setMapEraseList(QMap<QString, QString> &value);

    int getIndicFirstCreate() const;
    void setIndicFirstCreate(int value);

    int getIncrementCreation() const;
    void setIncrementCreation(int value);

    QString getIdOfLastCreatedObject() const;
    void setIdOfLastCreatedObject(const QString &value);

    int getAccessLevel() const;
    void setAccessLevel(int value);

    int getIndicRestoreState() const;
    void setIndicRestoreState(int value);

    QStringList getCopiedKeys() const;
    void setCopiedKeys(const QStringList &value);

    int getNumOrdreMax() const;
    void setNumOrdreMax(int value);


    QStringList getIdOfLastSupprObjects() const;
    void setIdOfLastSupprObjects(const QStringList &value);

    QString getColumnToTreatCodeObject() const;
    void setColumnToTreatCodeObject(const QString &value);

    QString getColumnToTreatConfigName() const;
    void setColumnToTreatConfigName(const QString &value);

    QString getColumnToTreatName() const;
    void setColumnToTreatName(const QString &value);

    int getSignalChangeColumn() const;
    void setSignalChangeColumn(int value);


    QSettings *getFileSetting() const;
    void setFileSetting(QSettings *value);

private:
    //On définit les différentes maps des objets
    QMap<QString, QMap<QString, QString> > mapGVE;
    QMap<QString, QMap<QString, QString> > mapGCA;
    QMap<QString, QMap<QString, QString> > mapGAT;
    QMap<QString, QMap<QString, QString> > mapGRS;
    QMap<QString, QMap<QString, QString> > mapGCS;
    QMap<QString, QMap<QString, QString> > mapGDO;

    //Map qui possède comme clé un triplé codeObjet, numéroInterne, InfoInterne permettant d'accéder à la valeur de la balise de titre des attributs
    QMap<QString, QString> mapConcordance;

    //Map qui content les nouvelles clés a mettre a jour dans le XML en fonction du type d'objet
    QMap<QString, QString> mapChangeList;
    //Map qui contient les nouvelles clés à ajouter dans le XML en fonction du type d'objet
    QMap<QString, QString> mapAddList;
    //Map qui contient les nouvelles clés à supprimer dans le XML en fonction du type d'objet
    QMap<QString, QString> mapEraseList;

    //On définit l'attribut correpondant au nom de la configuration courante
    QString currentConfigNameGCA;
    QString currentConfigNameGAT;
    QString currentConfigNameGRS;
    QString currentConfigNameGVE;
    QString currentConfigNameGDO;

    //Indicateur de première création de document
    int indicFirstCreate;
    //Incrémenteur d'identifiant pour la creation
    int incrementCreation;
    //Identifiant du dernier élément crée pour l'afficher lorsqu'il est crée après une recherche
    QString idOfLastCreatedObject;
    //Identifiant des derniers éléments supprimés pour les éliminer de la liste d'éléments à afficher lors d'une recherche
    QStringList idOfLastSupprObjects;
    //Indicateur des autorisations utilisateur
    int accessLevel;
    //Indicateur de restauration de l'état actuel de la configuration d'affiche dans dataViewer
    int indicRestoreState;
    //Liste des clés copiées
    QStringList copiedKeys;
    //CodeObjet des clés copiées
    QString codeObjectOfCopiedKeys;
    //Numéro d'ordre max des attributs affichés dans optionsViewer (sert dans le cas d'ajout d'un attribut)
    int numOrdreMax;
    //Code object de la colonne a supprimer des modeles de données
    QString columnToTreatCodeObject;
    //Nom de la configuration de la colonne a supprimer des modeles de données
    QString columnToTreatConfigName;
    //Nom de balise de l'attribut a supprimer du modele
    QString columnToTreatName;
    //Signal pour enlever une colonne quand on a mis sur Non indicAffichage dans une fiche descriptive d'attribut
    int signalChangeColumn;
    //Settings des noms de fichiers
    QSettings *fileSetting;

};
#endif // DATAMANAGER_H
