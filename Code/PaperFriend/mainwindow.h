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
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public: // Do not use 0 in place of nullptr
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend class TextEditor;
    TextEditor *textEditor;
    void closeEvent(QCloseEvent *event);

  private slots:
    void on_pushButton_clicked();
    void on_settingsButton_clicked();
    void on_save_settings_clicked();
    void on_activitie_button_clicked();
    void on_filterButton_clicked();
    void on_newEntryButton_clicked();
    void on_saveEntryButton_clicked();

  private:
    void toggle_visibility(QWidget *component);
    void display_entries(std::vector<EntryPerso *> entries, Ui::MainWindow *ui);
    void display_graph(std::vector<EntryPerso *> entries, Ui::MainWindow *ui);
    Ui::MainWindow *ui;
    All_Habits *all_habits;
    // do not use pointers unless you have no other choice
    std::vector<EntryPerso> entries;
    EntryCard *card;
};

// helps with debugging; to be replaced later

#endif // MAINWINDOW_H
