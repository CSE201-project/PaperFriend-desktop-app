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
    static int totol_cells; // avoid filler, avoid obvious comments.
    static QVector<Activity_cell *> cells; // see above
    void add_previous_cells();

  private slots:
    void on_add_activity_button_clicked();

    void add_new_cell(QString cell_text = "", QString cell_name = "Cell Name");

  public slots:
    // use camel case *or* snake case. Not both. Since we are all using snake,
    // it's better to stick to it
    void close_cell(int);

  private:
    Ui::All_activities *ui;
};

#endif // All_activities_H
