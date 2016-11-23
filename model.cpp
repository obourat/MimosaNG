#include "model.h"

#include <QStandardItemModel>

//Constructeur
Model::Model(const QList<QMap<QString, QString> > &smallMapsFromMapName)
{
#if 1

    //On définit un nouveau modèle de données
    model = new QStandardItemModel(this);

    for (int i = 0; i < smallMapsFromMapName.count(); ++i)
    {
        // recuperation des valeurs de la qmap courante (pour chaque clé i on récupère les valeurs dans values)
        QList<QString> valuesOfSmallMaps;
        valuesOfSmallMaps = smallMapsFromMapName[i].values();

        // On définit une QList items qui prend les items à inclure dans le model (en fonction de la configuration d'attributs)
        QList<QStandardItem*> itemsToInclude;
        for (int j = 0; j < valuesOfSmallMaps.count(); ++j)
        {
            QStandardItem *item;
            item = new QStandardItem(valuesOfSmallMaps[j]);

            itemsToInclude << item;
        }
        // Ajout de la liste d'elements au modele
        model->appendRow(itemsToInclude);
    }

    // Mise a jour des en tetes (on cherche le nombre de colonnes maximum pour les objets, ce qui correspond au nombre de clés pour chaque objet)
    QList<QString> keys;
    for(int k=0; k< smallMapsFromMapName.count(); ++ k)
    {
        if(smallMapsFromMapName[k].keys().count() > keys.count())
        {
            keys = smallMapsFromMapName[k].keys();
        }
    }
    //On remplit l'en tête avec le nom ds clés comme noms de colonnes
    int nbColumnsToInsert = keys.count();
    --nbColumnsToInsert;
    beginInsertColumns(QModelIndex(),0,nbColumnsToInsert);
    model->setHorizontalHeaderLabels(keys);
    endInsertColumns();
    headerLabels = keys;

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

void Model::updateModelRows(const QList<QMap<QString, QString> > &smallMapsFromMapName, QStringList keysToTreat, QString choiceAddObject, int columnOfKey)
{
    if(choiceAddObject == "copy" || choiceAddObject == "new")
    {
        for (int i = 0; i < smallMapsFromMapName.count(); ++i)
        {
            // recuperation des valeurs de la qmap courante (pour chaque clé i on récupère les valeurs dans values)
            QList<QString> valuesOfSmallMaps;
            QString valueOfKey;
            valueOfKey = smallMapsFromMapName[i].value("key");
            if(keysToTreat.contains(valueOfKey))
            {
                valuesOfSmallMaps = smallMapsFromMapName[i].values();

                // On définit une QList items qui prend les items à inclure dans le model (en fonction de la configuration d'attributs)
                QList<QStandardItem*> itemsToInclude;
                for (int j = 0; j < valuesOfSmallMaps.count(); ++j)
                {
                    QStandardItem *item;
                    item = new QStandardItem(valuesOfSmallMaps[j]);

                    itemsToInclude << item;
                }
                // Ajout de la liste d'elements au modele
                int rowToAdd;
                rowToAdd = model->rowCount();
                beginInsertRows(QModelIndex(),rowToAdd,rowToAdd);
                model->appendRow(itemsToInclude);
                endInsertRows();
            }
        }
    }
    else if(choiceAddObject == "suppr")
    {
        for(int j=0; j<keysToTreat.count();++j)
        {
            QList<QStandardItem*> items = model->findItems(keysToTreat[j],Qt::MatchExactly,columnOfKey);
            int rowToRemove;
            for(int i=0; i<items.count();++i)
            {
                rowToRemove = items[i]->row();
                beginRemoveRows(QModelIndex(),rowToRemove,rowToRemove);
                model->removeRow(rowToRemove,QModelIndex());
                endRemoveRows();
            }
        }
    }
    else if(choiceAddObject == "modify")
    {
        for(int j=0; j<keysToTreat.count();++j)
        {
            for(int k=0; k<smallMapsFromMapName.count();++k)
            {
                if(smallMapsFromMapName[k].value("key")== keysToTreat[j])
                {
                    // recuperation des valeurs de la qmap courante (pour chaque clé i on récupère les valeurs dans values)
                    QList<QString> valuesOfSmallMaps;
                    valuesOfSmallMaps = smallMapsFromMapName[k].values();

                    // On définit une QList items qui prend les items à inclure dans le model (en fonction de la configuration d'attributs)
                    QList<QStandardItem*> itemsToInclude;
                    for (int l = 0; l < valuesOfSmallMaps.count(); ++l)
                    {
                        QStandardItem *item;
                        item = new QStandardItem(valuesOfSmallMaps[l]);

                        itemsToInclude << item;
                    }

                    QList<QStandardItem*> items = model->findItems(keysToTreat[j],Qt::MatchExactly,columnOfKey);
                    int rowToModify;
                    QModelIndex indexToModify;
                    for(int i=0; i<items.count();++i)
                    {
                        rowToModify = items[i]->index().row();
                        indexToModify = items[i]->index();
                        //On modifie chaque cellule de la ligne
                        for(int c=0;c<valuesOfSmallMaps.count();++c)
                        {
                            QString test = itemsToInclude[c]->text();
                            model->item(rowToModify,c)->setData(itemsToInclude[c]->text(),Qt::DisplayRole);
                        }
                        dataChanged(indexToModify,indexToModify);
                    }
                    break;
                }
            }
        }

    }
}

void Model::removeModelColumn(int columnToRemoveIndex)
{
    beginRemoveColumns(QModelIndex(),columnToRemoveIndex,columnToRemoveIndex);
    model->removeColumn(columnToRemoveIndex, QModelIndex());
    headerLabels.removeAt(columnToRemoveIndex);
    endRemoveColumns();
}

void Model::addModelColumn(const QMap<QString, QMap<QString, QString> > *map, int columnToAddIndex, int columnOfKey, QString nameOfAttrToAdd, QString nameOfColumnToAdd)
{
    beginInsertColumns(QModelIndex(),columnToAddIndex,columnToAddIndex);
    model->insertColumn(columnToAddIndex,QModelIndex());
    headerLabels.append(nameOfColumnToAdd);
    model->setHorizontalHeaderLabels(headerLabels);
    endInsertColumns();
    QString keyOfLine;
    QString dataToAdd;
    QMap<QString, QString> currentTestedMap;
    QMap<QString, QMap<QString, QString> >::ConstIterator iterator;

    for(int i=0; i<model->rowCount();++i)
    {
        keyOfLine = model->data(model->index(i,columnOfKey)).toString();
        iterator = map->find(keyOfLine);
        currentTestedMap = map->value(iterator.key());
        dataToAdd = currentTestedMap[nameOfAttrToAdd];
        model->setData(model->index(i,columnToAddIndex),dataToAdd,Qt::DisplayRole);
    }
    //updateModelRows(smallMapsFromMapName,);
}

void Model::resetModel(const QList<QMap<QString, QString> > &smallMapsFromMapName)
{

    //On reinitialise le modele
    beginResetModel();
    model->clear();
    endResetModel();

    //On supprime les colonnes existantes
    beginRemoveColumns(QModelIndex(),0,model->columnCount());
    removeColumns(0,model->columnCount(QModelIndex()),QModelIndex());
    endRemoveColumns();

    //On ajoute les nouvelles colonnes
    int nbColumnsToInsert = smallMapsFromMapName[0].keys().count();
    --nbColumnsToInsert;
    beginInsertColumns(QModelIndex(),0,nbColumnsToInsert);
    // Mise a jour des en tetes (on cherche le nombre de colonnes maximum pour les objets, ce qui correspond au nombre de clés pour chaque objet)
    QList<QString> keys;
    for(int k=0; k< smallMapsFromMapName.count(); ++ k)
    {
        if(smallMapsFromMapName[k].keys().count() > keys.count())
        {
            keys = smallMapsFromMapName[k].keys();
        }
    }
    //On remplit l'en tête avec le nom ds clés comme noms de colonnes
    model->setHorizontalHeaderLabels(keys);
    endInsertColumns();

    headerLabels = keys;

    //On insere les nouvelles données
    int nbRowsToInsert = smallMapsFromMapName.count();
    --nbRowsToInsert;
    beginInsertRows(QModelIndex(),0,nbRowsToInsert);
    for (int i = 0; i < smallMapsFromMapName.count(); ++i)
    {
        // recuperation des valeurs de la qmap courante (pour chaque clé i on récupère les valeurs dans values)
        QList<QString> valuesOfSmallMaps;
        valuesOfSmallMaps = smallMapsFromMapName[i].values();

        // On définit une QList items qui prend les items à inclure dans le model (en fonction de la configuration d'attributs)
        QList<QStandardItem*> itemsToInclude;
        for (int j = 0; j < valuesOfSmallMaps.count(); ++j)
        {
            QStandardItem *item;
            item = new QStandardItem(valuesOfSmallMaps[j]);

            itemsToInclude << item;
        }
        // Ajout de la liste d'elements au modele

        model->appendRow(itemsToInclude);
    }
    endInsertRows();

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


