#ifndef DESCRIPTIVECARD_H
#define DESCRIPTIVECARD_H

#include <QDialog>
#include <QtGui/QWidget>
#include <QModelIndex>

class DataManager;

namespace Ui {
class DescriptiveCard;
}

class DescriptiveCard : public QDialog
{
    Q_OBJECT

public:
    explicit DescriptiveCard(DataManager *dataManager, QString codeObject, QString key,QString selection, QWidget *parent = 0);
    ~DescriptiveCard();

private:
    void setNewWidget(QString type, QString name, QString value, QString nameAttributeSelected);
private:
    Ui::DescriptiveCard *ui;

    DataManager *dataManager;
};

#endif // DESCRIPTIVECARD_H
