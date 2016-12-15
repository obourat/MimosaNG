#ifndef OPTIONSVIEWER_H
#define OPTIONSVIEWER_H

#include <QDialog>
#include <QtGui/QWidget>
#include <QModelIndex>

class QSortFilterProxyModel;
class Model;
class DataManager;
class DataViewer;
class DescriptiveCard;
class MainWindow;

namespace Ui {
class OptionsViewer;
}

class OptionsViewer : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsViewer(QString codeObject, DataManager *dataManager, MainWindow *mainWindow, DataViewer *dataViewer,const QList<QMap<QString, QString> >& maps,QString selectedOption, QWidget *parent = 0);
    ~OptionsViewer();

    //Met à jour le modèle en tenant compte des objets ajoutés ou supprimés
    void updateLayout();

public slots:
    //Slot du clic droit
    void customMenuRequested(QPoint pos);

private slots:
    //Slot de validation
    void on_confirmButtonBox_accepted();
    //Slot du clic sur un élément
    void on_optionsView_clicked();

    //Slots des options du menu
    void onDisplayDescriptiveCardButtonTriggered();
    void onDisplayDescriptiveCardCompleteButtonTriggered();
    void onCopyButtonTriggered();
    void onPasteButtonTriggered();
    void onEraseButtonTriggered();
    void onItemDoubleClicked();

    //Slot de changement dans le modèle et affichage
    void slotUpdateLayout();


private:
    //User interface
    Ui::OptionsViewer *ui;

    //Definition des objets dont on a besoin pour le traitement des données et l'affichage
    Model *myOptionsModel;
    QSortFilterProxyModel *proxyOptionsModel;
    QString codeObject;
    DataManager *dataManager;
    MainWindow *mainWindow;
    DataViewer *dataViewer;
    DescriptiveCard *descriptiveCard;

    //Nom de la configuration courante
    QString currentConfigSelectedName;
    //Nom de l'option selectionnée dans dataViewer
    QString selectedOption;
    //Liste de clés associée aux objets sélectionnés
    QStringList keysList;
    //Id de la configuration courante
    QString idCurrentConfig;

};

#endif // OPTIONSVIEWER_H
