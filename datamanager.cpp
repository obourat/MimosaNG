#include "datamanager.h"

//Constructeur
DataManager::DataManager()
{
}

//Destructeur
DataManager::~DataManager()
{

}

void DataManager::insertDataToMap(const QString &mapName, const QString &key, const QMap<QString, QString> &map)
{
    // Si la clef est vide
    if(key.isEmpty())
    {
        // Quitter
        return;
    }

    // Choix de la map par rapport au nom donné
    if(mapName == "mapGCA")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGCA.contains(mapName))
        {
            // Insertion des données
            mapGCA.insert(key, map);
        }
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGAT")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGAT.contains(mapName))
        {
            // Insertion des données
            mapGAT.insert(key, map);
        }
    }
    // Choix de la map par rapport au nom donné
    else if(mapName == "mapGVE")
    {
        // Si la map ne contient pas deja la clef key
        if(!mapGVE.contains(mapName))
        {
            // Insertion des données
            mapGVE.insert(key, map);
        }
    }
}


const QMap<QString, QMap<QString, QString> >* DataManager::getMapFromName(const QString &mapName) const
{
    //Retourne la map correspondante sur le bon nom est rentré
    if(mapName == "mapGCA")
    {
        return &mapGCA;
    }
    //Retourne la map correspondante sur le bon nom est rentré
    else if(mapName == "mapGAT")
    {
        return &mapGAT;
    }
    //Retourne la map correspondante sur le bon nom est rentré
    else if(mapName == "mapGVE")
    {
        return &mapGVE;
    }
    //Si un nom invalide ou nul est rentré, on ne renvoie rien
    else
    {
        return NULL;
    }
}

const QStringList DataManager::getKeysOfCurrentConfig(const QString &objectType)
{
    //On définit une liste de clés keys
    QStringList keys;

    //On définit un iterateur sur la grande qMap pour passer en revue les clés
    QMap<QString, QMap<QString, QString> >::Iterator iterator;

    //Pour toutes les clés de la qMap
    for (iterator = mapGCA.begin(); iterator != mapGCA.end(); ++iterator)
    {
        //Si l'élément CodeObjet de la clé sélectionnée par l'iterateur est égal au type d'objet rentré
        if(iterator.value()["CodeObjet"] == objectType)
        {
            //On ajoute la clé sélectionnée par l'itérateur dans la liste keys
            keys.append(iterator.key());
        }
    }
    //On définit un string currentKey qui sert a sélectionner la clé pour la configuration courante
    QString currentKey;
    //Pour toutes les clés de la liste keys des clés sélectionnées
    for (int i = 0; i <  keys.length(); ++i)
    {
#warning probleme signe euro
        //Si la balise NomConf de la clé sélectionnée contient le mot standard
        if (mapGCA[keys[i]]["NomConf"].contains("standard"))
        {
            //on sélectionne cette clé comme configuration courante
            currentKey = keys[i];
            //exit
            break;
        }
    }

    //On efface la liste de clés
    keys.clear();
    //Pour chaque clé de la mapGAT
    for (iterator = mapGAT.begin(); iterator != mapGAT.end(); ++iterator)
    {
        //Si la valeur de la balise InCnfAtt est égale à la valeur de la clé de la configuration courante
        if(iterator.value()["IdCnfAtt"] == currentKey)
        {
            //On ajoute la clé sélectionnée par l'iterateur dans la liste keys
            keys.append(iterator.key());
        }
    }
    //On renvoie la liste de clés (correspondant à la liste des clés des attributs pour la configuration d'attribut courante)
    return keys;
}

const QList<QMap<QString, QString> > DataManager::getSmallMapsFromMapName(const QString &mapName)
{
    //On définit une liste de maps<clé, valeur>
    QList<QMap<QString, QString> > maps;
    //On définit une map selectedMap qui sera égale à la map sélectionnée
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    //retourne la map correspondante au nom rentré par l'utilisateur (si ce nom est valide)
    selectedMap = getMapFromName(mapName);

    //Si la map est nulle
    if(selectedMap == NULL)
    {
        //On retourne une liste vide
        return maps;
    }

    //On définit un iterateur qui parcours les clés de la map sélectionnée
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;
    //Tant que l'iterateur n'a pas parcouru toutes les clés de la map sélectionnée
    for (iterator= selectedMap->begin(); iterator != selectedMap->end(); ++iterator)
    {
        //On ajoute dans la liste maps les valeurs correpsondantes au contenu de la clé (la sous map)
        maps << selectedMap->value(iterator.key());
    }

    //On retourne la liste des sous maps pour la map mapName
    return maps;
}
