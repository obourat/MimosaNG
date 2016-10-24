#ifndef DESCRIPTIVECARD_H
#define DESCRIPTIVECARD_H

#include <QDialog>
#include <QtGui/QWidget>
#include <QModelIndex>
#include <QMap>
#include <QGroupBox>

class DataManager;
class DataViewer;

namespace Ui {
class DescriptiveCard;
}

class DescriptiveCard : public QDialog
{
    Q_OBJECT

public:
    explicit DescriptiveCard(DataManager *dataManager, DataViewer *dataViewer, QString codeObject, QString key,QString selection, QWidget *parent = 0);
    ~DescriptiveCard();

private slots:
    void on_buttonBox_accepted();

private:
    void setNewWidget(QString type, QString name, QString var, QString value, QString nameAttributeSelected);
private:
    Ui::DescriptiveCard *ui;

    DataManager *dataManager;
    DataViewer *dataViewer;
    const QMap<QString, QMap<QString, QString> >* mapGVE;
    QString codeObject;
};

#endif // DESCRIPTIVECARD_H
