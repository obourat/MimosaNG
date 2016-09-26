#include "descriptivecard.h"
#include "ui_descriptivecard.h"

DescriptiveCard::DescriptiveCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DescriptiveCard)
{
    ui->setupUi(this);
}

DescriptiveCard::~DescriptiveCard()
{
    delete ui;
}
