#include "dataviewer.h"
#include "ui_dataviewer.h"
#include "model.h"

#include <QSortFilterProxyModel>

//Constructeur
DataViewer::DataViewer(const QStringList& keysAttr, const QList<QMap<QString, QString> >& maps, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    //On instancie la fenêtre
    ui->setupUi(this);

    //On définit un modèle qui contient toutes les clés récupérées
    myModel = new Model(keysAttr, maps);
    //On définit un proxyModel servant pour le tri (croissant, décroissant, alphabétique, numérique)
    proxyModel = new QSortFilterProxyModel(this);

    //On associe les données du model au proxyModel
    proxyModel->setSourceModel(myModel);
    //On associe la vue au modèle contenu dans le proxyModel
    ui->tableView->setModel(proxyModel);

    //On ajuste la taille des colonnes au contenu
    ui->tableView->resizeColumnsToContents();
}

//Destructeur
DataViewer::~DataViewer()
{
    delete ui;
}
