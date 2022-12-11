#ifndef ACTIVITY_CELL_H
#define ACTIVITY_CELL_H

#include <QMessageBox>
#include <QString>
#include <QWidget>

namespace Ui
{
class Activity_cell;
}

class Activity_cell : public QWidget // class names start capitalised
{
    Q_OBJECT

  public:
    explicit Activity_cell(QWidget *parent = nullptr);
    ~Activity_cell();
    // camelCase or snake_case, and avoid filler
    int cell_number;
    // QString get_text();
    // QString get_name(); // avoid filler

    //    void set_text(QString cell_text);
    void set_name(QString cell_name);

  private:
    Ui::Activity_cell *ui;

  signals:
    // avoid filler words in name
    void close_cell(int); // one needs signals so that whenever the activity
                          // cell is closed one can invoke it.

  private slots:
    // what?
    void on_delete_activity_button_clicked();
};

#endif // Activity_cell_H
