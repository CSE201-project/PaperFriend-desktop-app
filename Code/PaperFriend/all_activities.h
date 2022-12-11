#ifndef ALL_ACTIVITIES_H // header guards are all caps
#define ALL_ACTIVITIES_H

// use < > only for Qt or STL libraries.
#include "activity_cell.h"
#include <QDialog>
#include <QVector>

namespace Ui
{
class All_activities; // class names start capitalised
}

class All_activities : public QDialog
{
    Q_OBJECT

  public:
    explicit All_activities(QWidget *parent = nullptr);
    ~All_activities();
    static int ActivitiesCellNumberTotal; // The total number of the activities.
    static QVector<activity_cell *>
        allCellPtr; // This vector saves all the data of the activity cell.
    void add_previous_cells();

  private slots:
    void on_add_activity_button_clicked();

    void add_new_cell(QString cellText = "", QString cellName = "Cell Name");

  public slots:
    // use camel case *or* snake case. Not both. Since we are all using snake,
    // it's better to stick to it
    void close_cell(int);

  private:
    Ui::All_activities *ui;
};

#endif // All_activities_H
