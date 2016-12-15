#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>

class QStandardItemModel;

class Model : public QAbstractTableModel
{
public:
    explicit Model(const QList<QMap<QString, QString> > &smallMapsFromMapName);
    ~Model();

    //Mets à jour les lignes dans le modèle
    void updateModelRows(const QList<QMap<QString, QString> > &smallMapsFromMapName, QStringList keysToTreat, QString choiceAddObject, int columnOfKey);
    //Enlève la colonne du modèle à l'index renseigné
    void removeModelColumn(int columnToRemoveIndex);
    //Ajoute une colonne au modèle
    void addModelColumn(const QMap<QString, QMap<QString, QString> > *map, int columnToAddIndex, int columnOfKey, QString nameOfAttrToAdd, QString nameOfColumnToAdd);
    //Réinitialise le modèle
    void resetModel(const QList<QMap<QString, QString> > &smallMapsFromMapName);

    //Fonctions virtuelles
    // Compteur de lignes
    int rowCount(const QModelIndex &parent) const;
    //Compteur de colonnes
    int columnCount(const QModelIndex &parent) const;
    //Détemine les données présentes dans une case
    QVariant data(const QModelIndex &index, int role) const;
    //Détermine les données présentes dans le header
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
#if 0
    void sort(int column, Qt::SortOrder order);
#endif

private:
    QStandardItemModel *model;
    //Permet de stocker le nom des colonnes du header pour les réutiliser dans le cas d'ajout d'une colonne
    QList<QString> headerLabels;
};


#endif // MODEL_H
