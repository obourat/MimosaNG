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
    QString getFileValue(const QMap<QString, QMap<QString, QString> > map, const QString key, const QString searchName);
    ~SearchCard();

    QList<QString> getSearchResults() const;
    void setSearchResults(const QList<QString> &value);

    int getConfirmSearch() const;
    void setConfirmSearch(int value);

private slots:
    void on_buttonBox_accepted();


    void on_buttonBox_rejected();

private:
    void setNewWidget(QString type, QString var, QString name, QString nameAttributeSelected);
    QList<QString> searchMatches(const QMap<QString, QMap<QString, QString> >& map, const QMap<QString, QString >& mapSearch);

private:
    Ui::SearchCard *ui;

    DataManager *dataManager;
    DataViewer *dataViewer;
    QString codeObject;
    QString key;
    QString valueOfEditedLine;
    QMap<QString, QString> mapOfSearch;
    const QMap<QString, QMap<QString, QString> >* mapGVE;
    const QMap<QString, QMap<QString, QString> >* selectedMap;
    QList<QString> searchResults;
    int confirmSearch;
};

#endif // SEARCHCARD_H
