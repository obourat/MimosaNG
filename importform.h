#ifndef IMPORTFORM_H
#define IMPORTFORM_H

#include <QDialog>
#include <QButtonGroup>

class DataManager;

namespace Ui {
class ImportForm;
}

class ImportForm : public QDialog
{
    Q_OBJECT

public:
    explicit ImportForm(DataManager *dataManager, QString codeObjet, QWidget *parent = 0);
    ~ImportForm();

private slots:
    void on_buttonBox_accepted();

    void on_browsePushButton_released();

private:
    Ui::ImportForm *ui;
    DataManager *dataManager;
    QString codeObjet;
    QList<QMap<QString, QString> > configurationsKeys;
    QButtonGroup *buttonGroup;
};

#endif // IMPORTFORM_H
