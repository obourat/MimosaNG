#ifndef EXPORTFORM_H
#define EXPORTFORM_H

#include <QDialog>

class DataViewer;
class DataManager;

namespace Ui {
class ExportForm;
}

class ExportForm : public QDialog
{
    Q_OBJECT

public:
    explicit ExportForm(DataManager *dataManager, QStringList keysList, QString codeObjet, QWidget *parent = 0);
    ~ExportForm();

private slots:
    void on_confirmButtonBox_accepted();

    void on_browseButton_released();

private:
    Ui::ExportForm *ui;
    DataManager *dataManager;
    QStringList keysList;
    QString codeObjet;
    QList<QMap<QString, QString> > configurationsKeys;
};

#endif // EXPORTFORM_H
