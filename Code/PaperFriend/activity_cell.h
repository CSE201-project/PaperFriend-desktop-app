#ifndef ACTIVITY_CELL_H
#define ACTIVITY_CELL_H

#include <QWidget>
#include<QString>
#include<QMessageBox>

namespace Ui {
class activity_cell;
}

class activity_cell : public QWidget
{
    Q_OBJECT

public:
    explicit activity_cell(QWidget *parent = nullptr);
    ~activity_cell();
    int ActivitiesCellNumber;
    QString getTextData();
    QString getCellName();
    QString get_activity_name();
    int get_activity_type();

    void setTextData(QString cellText);
    void set_activity_cell_name(QString CellName);
    void set_activity_type(int type);

private:
    Ui::activity_cell *ui;

signals:
    void closeThisCell(int); // one needs signals so that whenever the activity cell is closed one can invoke it.

private slots:
    void on_delete_activity_button_clicked();
};

#endif // ACTIVITY_CELL_H
