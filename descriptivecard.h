#ifndef DESCRIPTIVECARD_H
#define DESCRIPTIVECARD_H

#include <QDialog>
#include <QtGui/QWidget>
#include <QModelIndex>
#include <QMap>
#include <QGroupBox>

class DataManager;
class DataViewer;
class MainWindow;

namespace Ui {
class DescriptiveCard;
}

class DescriptiveCard : public QDialog
{
    Q_OBJECT

public:
    //Constructeur
    explicit DescriptiveCard(DataManager *dataManager, MainWindow *mainWindow, DataViewer *dataViewer, QString codeObject, QString key,QString selection,QString choice ,QWidget *parent = 0);
    //Destructeur
    ~DescriptiveCard();

private slots:
    //Slot de validation de la modification de fiche
    void on_buttonBox_accepted();

private:
    //Génère un nouveau widget en fonction du type de champ dans la fiche descriptive
    void setNewWidget(QString type, QString name, QString var, QString value, QString nameAttributeSelected);
private:
    //User interface
    Ui::DescriptiveCard *ui;
    //Définition des objets dont on a besoin pour la gestion des données et de l'affichage
    DataManager *dataManager;
    MainWindow *mainWindow;
    DataViewer *dataViewer;
    //Définition de mapGVE par copie de dataManager
    const QMap<QString, QMap<QString, QString> >* mapGVE;
    //Indicateur du type d'objet et du choix d'affichage
    QString codeObject;
    //Clé de la fiche descriptive
    QString key;
    //
    QString choice;
    //Indicateur des clés et numOrdre max pour le cas d'une création
    QString currentMaxKey;
    QString currentMaxNumOrdre;
    //Liste des paramètres de champ pour l'ajout dans le XML
    QStringList parameters;
    //Indicateur de première création d'objet
    int indicFirstCreate;
    //Entier d'incrementation de clé et d'identifiant
    int incrementCreation;
};

#endif // DESCRIPTIVECARD_H
