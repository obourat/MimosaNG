#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include <QtGui/QWidget>
#include <QSettings>
#include <QModelIndex>
#include <QMap>

namespace Ui {
class DataViewer;
}

class QSortFilterProxyModel;
class Model;
class OptionsViewer;
class DataManager;
class FileReader;
class DescriptiveCard;
class SearchCard;

class DataViewer : public QDialog
{
    Q_OBJECT

public:
    //Constructeur
    explicit DataViewer(DataManager *dataManager, const QList<QMap<QString, QString> >& maps, const QString codeObject, QWidget *parent = 0);
    void updateLayout();
    void updateKeyRowMap();
    //Destructeur
    ~DataViewer();

    //Getters et Setters
    QStringList getKeysList() const;
    void setKeysList(const QStringList &value);

    int getIndicFirstSearch() const;
    void setIndicFirstSearch(int value);

    QList<QString> getResultList() const;
    void setResultList(const QList<QString> &value);

    QString getIndicSearch() const;
    void setIndicSearch(const QString &value);

    QList<QString> getDisplayedRowsBeforeUpdate() const;
    void setDisplayedRowsBeforeUpdate(const QList<QString> &value);

    QString getChoiceAddObject() const;
    void setChoiceAddObject(const QString &value);

public slots:
    //Slot pour le menu
    void customMenuRequested(QPoint pos);
    void customHeaderMenuRequested(QPoint pos);

private slots:
    //Slots pour les bouttons du menu
    void onChangeCurrentConfigButtonTriggered();
    void onChangeCurrentConfigAttributesButtonTriggered();
    void onDisplayDescriptiveCardButtonTriggered();
    void onDisplayDescriptiveCardCompleteButtonTriggered();
    void onValidateSelectionButtonTriggered();
    void onHideSelectionButtonTriggered();
    void onResetSelectionButtonTriggered();
    void onTotalSelectionButtonTriggered();
    void onSubListRestrainButtonTriggered();
    void onSubListAddButtonTriggered();
    void onItemDoubleClicked();
    void onCreateNewButtonTrigerred();
    void onCreateCopyButtonTrigerred();
    void onEraseButtonTriggered();
    void setColumnHidden();
    void onSortContent();
    void onCopyButtonTrigerred();

private:
    //User Interface
    Ui::DataViewer *ui;

    //Définition des objets dont on a besoin pour l'affichage, et pour la visualisation des options
    Model *myModel;
    QSortFilterProxyModel *proxyModel;
    OptionsViewer *optionsViewerCurrentConfig;
    OptionsViewer *optionsViewerCurrentConfigAttributes;
    DataManager *dataManager;
    FileReader *fileReaderOptions;
    DescriptiveCard *descriptiveCard;
    SearchCard *searchCard;

    //Code Objet associé au type de données affichées
    QString codeObject;
    //Liste de clés associée aux objets sélectionnés
    QStringList keysList;
    //Map associé à la clé selectionnée pour une ligne
    QMap<QString, QString> keyRowMap;
    //Compteurs de lignes et colonnes
    int rowCount;
    int columnCount;
    int rowsDisplayed;
    //Settings
    QSettings mySetting;
    //Indicateurs de recherche
    int index;
    int indicFirstSearch;
    QString indicSearch;
    //Liste des resultats de recherche trouvés dans searchCard.cpp
    QList<QString> resultList;
    //Liste des clés affichés avant la création d'un nouveau doc dansla cas d'une recherche
    QList<QString> displayedRowsBeforeUpdate;
    //Choix de l'objet à ajouter, renseigné lors du clic sur le menu
    QString choiceAddObject;
};

#endif // DATAVIEWER_H
