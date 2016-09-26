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

class OptionsViewer : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsViewer(QString codeObject, DataManager *dataManager, const QList<QMap<QString, QString> >& maps, QWidget *parent = 0);
    ~OptionsViewer();

private slots:
    void on_confirmButtonBox_accepted();


    void on_optionsView_clicked();

private:
    Ui::OptionsViewer *ui;

    Model *myOptionsModel;
    QSortFilterProxyModel *proxyOptionsModel;
    DataManager *dataManager;

    QString codeObject;
    QString currentConfigSelectedName;
};

#endif // OPTIONSVIEWER_H
