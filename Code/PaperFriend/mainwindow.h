#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "all_habits.h"
#include "cardclasses.h"
#include "entryclasses.h"

#include <QApplication>
#include <QChartView>
#include <QLineSeries>
#include <QMainWindow>
#include <QtCharts>
// Represents 1 set of bars in a bar chart
#include <QtCharts/QBarSet>
// Displays the color used to represent each QBarSet
#include <QtCharts/QLegend>
// Adds categories to the charts axes
#include <QtCharts/QBarCategoryAxis>
// Used to create a line chart
#include <QtCharts/QLineSeries>
// Used to change names on axis
#include <QtCharts/QCategoryAxis>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Main_window;
}
QT_END_NAMESPACE

class Main_window : public QMainWindow
{
    Q_OBJECT

  public: // Do not use 0 in place of nullptr
    explicit Main_window(QWidget *parent = nullptr);
    ~Main_window();
    friend class Text_editor; // camelCase *or* snake_case
    Text_editor *text_editor;
    void close_event(QCloseEvent *event); // camelCase *or* snake_case!

  private slots:
    void on_push_button_clicked();
    void on_settings_button_clicked();
    void on_save_settings_clicked();
    void on_activities_button_clicked();
    void on_filter_button_clicked();
    void on_new_entry_button_clicked();
    void on_save_entry_button_clicked();

  private:
    void toggle_visibility(QWidget *component);
    void display_entries(std::vector<Entry_perso *> entries,
                         Ui::Main_window *ui);
    void display_graph(std::vector<Entry_perso *> entries, Ui::Main_window *ui);
    Ui::Main_window *ui;
    All_habits *all_habits;
    // do not use pointers unless you have no other choice
    std::vector<Entry_perso> entries;
    Entry_card *card;
};

// helps with debugging; to be replaced later

#endif // MAINWINDOW_H
