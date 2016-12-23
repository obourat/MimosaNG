#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include <QtGui/QWidget>
#include <QSettings>
#include <QModelIndex>
#include <QMap>

class QSortFilterProxyModel;
class Model;
class OptionsViewer;
class DataManager;
class FileReader;
class DescriptiveCard;
class SearchCard;
class MainWindow;
class ExportForm;
class PrintForm;
class ImportForm;
class FileWriter;

namespace Ui {
class DataViewer;
}

class DataViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DataViewer(DataManager *dataManager, MainWindow *mainWindow ,const QList<QMap<QString, QString> >& maps, const QString codeObject, QWidget *parent = 0);
    ~DataViewer();

    //Met à jour le modèle en tenant compte des données ajoutées ou supprimées
    void updateLayout();
    //Met à jour la map d'association clé/ligne
    void updateKeyRowMap();
    //Recherche l'index de la colonne a supprimer
    void searchColumnToRemoveIndex();
    //Initialise la reinitialisation du modèle
    void resetModel();
    //Cache la colonne clé dans la vue (après un reset du modèle)
    void hideKeyColumn();

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

    QString getCurrentConfigName() const;
    void setCurrentConfigName(const QString &value);

    QString getCodeObject() const;
    void setCodeObject(const QString &value);

public slots:
    //Slot clic droit tableau
    void customMenuRequested(QPoint pos);

    //Slot clic droit header
    void customHeaderMenuRequested(QPoint pos);

private slots:
    //Slots des options du menu
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
    void onCreateNewButtonTriggered();
    void onCreateCopyButtonTriggered();
    void onEraseButtonTriggered();
    void setColumnHidden();
    void onSortContent();
    void onCopyButtonTriggered();
    void onExportButtonTriggered();
    void onPrintButtonTriggered();
    void onImportButtonTriggered();

    //Slots de changement dans le modèle et affichage
    void slotUpdateLayout();
    void slotChangeColumn();

    //Slot du bouton sauver
    void on_savePushButton_released();

private:
    //User Interface
    Ui::DataViewer *ui;

    //Définition des objets dont on a besoin pour l'affichage, et pour la visualisation des options
    Model *myModel;
    QSortFilterProxyModel *proxyModel;
    OptionsViewer *optionsViewerCurrentConfig;
    OptionsViewer *optionsViewerCurrentConfigAttributes;
    DataManager *dataManager;
    MainWindow *mainWindow;
    FileReader *fileReaderOptions;
    DescriptiveCard *descriptiveCard;
    SearchCard *searchCard;
    ExportForm *exportForm;
    PrintForm *printForm;
    ImportForm *importForm;
    FileWriter *fileWriter;

    //Code Objet associé au type de données affichées
    QString codeObject;
    //Nom de la configuration courante
    QString currentConfigName;
    //Liste de clés associée aux objets sélectionnés
    QStringList keysList;
    //Map associé à la clé selectionnée pour une ligne
    QMap<QString, QString> keyRowMap;
    //Compteurs de lignes et colonnes
    int rowCount;
    int columnCount;
    //Settings d'affichage
    QSettings mySetting;
    //Index de l'élément sélectionné lors d'un clic
    int index;
    //Indicateur de première recherche
    int indicFirstSearch;
    //Indicateur du mode de recherche (restreindre ou ajouter)
    QString indicSearch;
    //Liste des resultats de recherche trouvés dans searchCard.cpp
    QList<QString> resultList;
    //Liste des clés affichés avant la création d'un nouveau doc dansla cas d'une recherche
    QList<QString> displayedRowsBeforeUpdate;
    //Indicateur de mise a jour du QSetting (permet d'eviter le bug d'ajout de colonne)
    int indicUpdateSetting;
};

#endif // DATAVIEWER_H
