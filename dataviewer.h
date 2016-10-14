#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include <QtGui/QWidget>

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
    //Destructeur
    ~DataViewer();

public slots:
    //Slot pour le menu
    void customMenuRequested(QPoint pos);

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
    void onSubListButtonTriggered();
    void getSearchResults();

private:
    //User Interface
    Ui::DataViewer *ui;

    //On définit les objets dont on a besoin pour l'affichage, et pour la visualisation des options
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
    int rowCount;
    int columnCount;
    int rowsDisplayed;
};

#endif // DATAVIEWER_H
