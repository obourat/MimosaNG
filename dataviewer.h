#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>

namespace Ui {
class DataViewer;
}

class QSortFilterProxyModel;
class Model;

class DataViewer : public QDialog
{
    Q_OBJECT

public:
    //Constructeur
    explicit DataViewer(const QStringList &keysAttr, const QList<QMap<QString, QString> >& maps, QWidget *parent = 0);
    //Destructeur
    ~DataViewer();

private:
    //User Interface
    Ui::DataViewer *ui;

    //On définit les objets dont on a besoin
    Model *myModel;
    QSortFilterProxyModel *proxyModel;
};

#endif // DATAVIEWER_H
