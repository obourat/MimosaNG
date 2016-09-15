#include "model.h"

#include <QStandardItemModel>

//Constructeur
Model::Model(
        const QStringList &keysAttr,
        const QList<QMap<QString, QString> > &maps
        )
{
#if 1
    // Compter le nombre d'attribut à afficher
    int nbAttrToDisplay = 0;

    // Faux
    nbAttrToDisplay = keysAttr.count();
    //On définit un nouveau modèle de données
    model = new QStandardItemModel(this);

    for (int i = 0; i < maps.count(); ++i)
    {
        // recuperation des valeurs de la qmap courante (pour chaque clé i on récupère les valeurs dans values)
        QList<QString> values;
        values = maps[i].values();

        // On définit une QList items qui prend les items à inclure dans le model (en fonction de la configuration d'attributs)
        QList<QStandardItem*> items;
        for (int j = 0; j < values.count(); ++j)
        {
            // Ici il faut selectionner les valeurs qu'il faut afficher selon la cfg d'attributs

            QStandardItem *item;
            item = new QStandardItem(values[j]);

            items << item;
        }
        // Ajout de la liste d'elements au modele
        model->appendRow(items);
    }

    // Mise a jour des en tetes (on cherche le nombre de colonnes maximum pour les objets, ce qui correspond au nombre de clés pour chaque objet)
    QList<QString> keys;
    for(int k=0; k< maps.count(); ++ k)
    {
        if(maps[k].keys().count() > keys.count())
        {
            keys = maps[k].keys();
        }
    }
    //On remplit l'en tête avec le nom ds clés comme noms de colonnes
    model->setHorizontalHeaderLabels(keys);
#else
    model = new QStandardItemModel(this);

    for (int i = 0; i < maps.count(); ++i)
    {
        // recuperation des valeurs de la qmap courante
        QList<QString> values;
        values = maps[i].values();

        // listes des éléments de la ieme ligne du modele
        QList<QStandardItem*> items;
        for (int j = 0; j < values.count(); ++j)
        {
            QStandardItem *item;
            item = new QStandardItem(values[j]);

            items << item;
        }
        // Ajout de la liste d'elements au modele
        model->appendRow(items);
    }

    // Mise a jour des en tetes
    QList<QString> keys;
    for(int k=0; k< maps.count(); ++ k)
    {
        if(maps[k].keys().count() > keys.count())
        {
            keys = maps[k].keys();
        }
    }

    model->setHorizontalHeaderLabels(keys);
#endif
}

//Destructeur
Model::~Model()
{

}

int Model::rowCount(const QModelIndex &parent) const
{
    //On s'assure que l'on est au rootItem
    if(parent.isValid())
    {
        return 0;
    }

    //On envoie au model le nombre de lignes
    return model->rowCount();
}

int Model::columnCount(const QModelIndex &parent) const
{
    //On s'assure que l'on est au rootItem
    if(parent.isValid())
    {
        return 0;
    }

    //On envoie au model le nombre de colonnes
    return model->columnCount();
}


QVariant Model::data(const QModelIndex &index, int role) const
{
    //Si l'index n'est pas valide, on ne renvoie rien
    if(!index.isValid())
    {
        return QVariant();
    }

    //Si le role indiqué par la vue n'est pas sous forme de texte, on ne renvoie rien
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    QVariant currentData;
    QModelIndex index2;
    index2 = model->index(index.row(), index.column());

    currentData = index2.data();

    //On détecte si les données sélectionnées sont sous forme d'entier
    bool ok = false;
    currentData.toInt(&ok);

    //Si c'est le cas, on retourne les données sous forme d'entier
    if(ok)
    {
       return currentData.toInt();
    }

    //Sinon sous forme de texte
    return currentData.toString();

    //return model->index(index.row(), index.column()).data();

}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    //Si l'orientation n'est pas sous forme horizontale et que le rôle n'est pas sous forme de texte, on ne revoie rien
    if(orientation != Qt::Horizontal || role != Qt::DisplayRole)
    {
        return QVariant();
    }
    //Dans le cas contraire, on renvoie les données du header au modèle avec le numéro de section et l'orientation
    return model->headerData(section, orientation);
}

#if 0
//Fonction de tri des données d'un QTableView (surcharge d'une fonction virtuelle)
void Model::sort(int column, Qt::SortOrder order)
{
    // Prendre en considération le rang de l'attribut
    QVariant currentData;
    QModelIndex index;
    index = model->index(0, column);

    currentData = index.data();

    bool ok = false;
    currentData.toInt(&ok);

    if(ok)
    {
        int a = 1;
    }
    else
    {
        model->sort(column, order);
    }
    dataChanged(model->index(0,0), model->index(model->rowCount() - 1, model->columnCount() - 1));


}
#endif


