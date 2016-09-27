#ifndef DESCRIPTIVECARD_H
#define DESCRIPTIVECARD_H

#include <QDialog>
#include <QtGui/QWidget>
#include <QModelIndex>

namespace Ui {
class DescriptiveCard;
}

class DescriptiveCard : public QDialog
{
    Q_OBJECT

public:
    explicit DescriptiveCard(QWidget *parent = 0);
    ~DescriptiveCard();

private:
    Ui::DescriptiveCard *ui;
};

#endif // DESCRIPTIVECARD_H
