#ifndef OPTIONSVIEWER_H
#define OPTIONSVIEWER_H

#include <QDialog>
#include <QtGui/QWidget>
#include <QModelIndex>

namespace Ui {
class OptionsViewer;
}

class QSortFilterProxyModel;
class Model;
class DataManager;
class DataViewer;
class DescriptiveCard;

class OptionsViewer : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsViewer(QString codeObject, DataManager *dataManager , DataViewer *dataViewer,const QList<QMap<QString, QString> >& maps,QString selectedOption, QWidget *parent = 0);
    ~OptionsViewer();
public slots:
    void customMenuRequested(QPoint pos);

private slots:
    //Slots pour le clic sur le bouton "Ok" et pour la sélection d'un élément
    void on_confirmButtonBox_accepted();
    void on_optionsView_clicked();

    //Slots pour les options
    void onDisplayDescriptiveCardButtonTriggered();
    void onDisplayDescriptiveCardCompleteButtonTriggered();


private:
    Ui::OptionsViewer *ui;

    Model *myOptionsModel;
    QSortFilterProxyModel *proxyOptionsModel;
    QString codeObject;
    DataManager *dataManager;
    DataViewer *dataViewer;
    DescriptiveCard *descriptiveCard;

    QString currentConfigSelectedName;
    QString selectedOption;
    //Liste de clés associée aux objets sélectionnés
    QStringList keysList;

};

#endif // OPTIONSVIEWER_H
