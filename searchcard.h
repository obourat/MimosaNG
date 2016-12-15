#ifndef SEARCHCARD_H
#define SEARCHCARD_H

#include <QDialog>
#include <QMap>
#include <QList>

class DataManager;
class DataViewer;

namespace Ui {
class SearchCard;
}

class SearchCard : public QDialog
{
    Q_OBJECT

public:
    explicit SearchCard(DataManager *dataManager, DataViewer *dataViewer, QString codeObject, QString key, QWidget *parent);
    ~SearchCard();

    //Fonction qui retourne la valeur d'une balise de nom "searchName" dans la clé "key", dans la map "map"
    QString getFileValue(const QMap<QString, QMap<QString, QString> > map, const QString key, const QString searchName);

    //Getters et Setters
    QList<QString> getSearchResults() const;
    void setSearchResults(const QList<QString> &value);
    int getConfirmSearch() const;
    void setConfirmSearch(int value);

private slots:
    //Slots de validation et d'annulation
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    //Fonction qui génère un nouveau champ de recherhce dans la fiche de recherche
    void setNewWidget(QString type, QString var, QString name, QString nameAttributeSelected);
    //Fonction qui renvoie les clés de map qui vérifient les critères de recherche de mapSearch
    QList<QString> searchMatches(const QMap<QString, QMap<QString, QString> >& map, const QMap<QString, QString >& mapSearch);

private:
    //User Interface
    Ui::SearchCard *ui;

    //Definition des objets pour le traitement des données
    DataManager *dataManager;
    DataViewer *dataViewer;

    //Code objet de l'objet affiché
    QString codeObject;
    //Clé de l'objet
    QString key;
    //Map rassemblant les critères de recherche rentrés dans la fiche
    QMap<QString, QString> mapOfSearch;
    //Map dont on a besoin recupérés dans dataManager
    const QMap<QString, QMap<QString, QString> >* mapGVE;
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    //Liste des clés répondant aux critères de recherche
    QList<QString> searchResults;
    //Indicateur de confirmation de recherche
    int confirmSearch;
};

#endif // SEARCHCARD_H
