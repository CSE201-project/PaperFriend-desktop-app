#include "mainwindow.h"
#include "all_activities.h"
#include "cardclasses.h"
#include "dynamicgraph.h"
#include "file_processing/file_processing/file_save_and_load.h"
#include "loadHabits.h"
#include "mascot.h"
#include "mascotchat.h"
#include "settings.h"
#include "texteditor.h"
#include "ui_all_activities.h"
#include "ui_mainwindow.h"
#include "ui_texteditor.h"
#include <testing_lib.h>

#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    // use uniform initialization (the brackets)
    : QMainWindow{parent}, ui{new Ui::MainWindow}
{
    ui->setupUi(this); // display canvas created in drag-and-drop

    // create layout for central scrollable area
    QVBoxLayout *entries_layout{new QVBoxLayout()}; // uniform!
    ui->EntriesScroll->widget()->setLayout(entries_layout);

    // We use Friend function to embed the TextEditor into the MainWindow
    textEditor = new TextEditor();
    textEditor->mainUi = this;

    entries = testing::random_entries(10);
    display_graph(entries, ui);
    display_entries(entries, ui);

    MascotChat chat_box{MascotChat(ui->scrollArea)};

    // tests
    for (int i = 0; i < 20; i++)
        chat_box << QString::number(i); // no brackets needed if it's one line

    // avoid creating variables that are immediately discarded,
    // unless they really improve readability
    chat_box << chat_box.get_last_message();

    auto settings{findChild<QWidget *>("settings_frame")}; // uniform!
    settings->hide(); // hide the settings menu on launch
    // setting the icon for the setting button
    QPixmap pix(":/pictures/rsc/checklist icon.png");
    int w{ui->settingsButton->width()};  // uniform!
    int h{ui->settingsButton->height()}; // uniform!
    ui->settingsButton->setIcon(QIcon(pix.scaled(w, h, Qt::KeepAspectRatio)));

    // prefer range based for loops (this thing) whenever possible
    for (const auto &habit : load_habits())
    {
        ui->habits_label->setText(ui->habits_label->text() + "\n" +
                                  QString::fromStdString(habit));
    }
}

MainWindow::~MainWindow()
{
    // why didn't they get deallocated?
    delete textEditor;
    delete entries_layout;

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // the full name is easier to read
    auto answer_button{QMessageBox::question(
        this, tr("Paper friend"), tr("Are you sure?\n"),
        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes)};
    if (answer_button != QMessageBox::Yes)
        event->ignore(); // no brackets needed if it's one line
    else
        event->accept();
}

void MainWindow::toggle_visibility(QWidget *component)
{
    if (component->isVisible())
        component->hide(); // no brackets needed if it's one line
    else
        component->show();
}

// pass vectors by *const* reference. You get all the memory benefits, and it's
// significantly more safe and readable
void MainWindow::display_entries(const std::vector<EntryPerso> &entries,
                                 Ui::MainWindow *ui)
{
    while (!ui->EntriesScroll->widget()->layout()->isEmpty())
    {
        ui->graph_frame->removeItem(
            ui->EntriesScroll->widget()->layout()->takeAt(0));
    }
    // use references to avoid copying, and const if they don't have to be
    // modified
    for (const auto &entry : entries)
    {
        // when does this get deallocated??
        // I'll assume the pointer aspect is required by Qt
        // displays the entry in the main_frame.
        EntryCard *c{new EntryCard(20, 300, 300, "white", entry)}; // uniform!
        c->display(ui->EntriesScroll->widget()->layout());
    }
}

void MainWindow::display_graph(const std::vector<EntryPerso> &entries,
                               Ui::MainWindow *ui)
{
    // no brackets needed if it's one line
    while (!ui->graph_frame->isEmpty())
        ui->graph_frame->removeItem(ui->graph_frame->takeAt(0));

    DynamicGraph moodGraph{DynamicGraph(entries)}; // uniform!
    moodGraph.display(ui->graph_frame);            // displays the graph
    this->showMaximized();
}

void MainWindow::on_pushButton_clicked()
{
    hide();
    all_habits{new All_Habits(this)}; // uniform!
    all_habits->showMaximized();
}

void MainWindow::on_activitie_button_clicked()
{
    all_activities *my_activities{new all_activities()}; // uniform!
    my_activities->add_previous_cells();
    my_activities->setModal(true);
    my_activities->exec();
}

void MainWindow::on_settingsButton_clicked()
{
    findChild<QCheckBox *>("mood")->setChecked(saved_mood());
    findChild<QCheckBox *>("sleep")->setChecked(saved_sleep());
    findChild<QCheckBox *>("eating_healthy")
        ->setChecked(saved_eating_healthy());
    findChild<QCheckBox *>("productivity")->setChecked(saved_productivity());
    findChild<QCheckBox *>("communications")
        ->setChecked(saved_communications());
    findChild<QCheckBox *>("screen_time")->setChecked(saved_screen_time());

    // uniform! GG for auto!
    auto settings{findChild<QWidget *>("settings_frame")};
    toggle_visibility(settings);
    auto chat_box{findChild<QWidget *>("scrollArea")};
    toggle_visibility(chat_box);
}

void MainWindow::on_save_settings_clicked()
{
    std::ofstream myfile;
    myfile.open("settings.txt");
    myfile << findChild<QCheckBox *>("mood")->isChecked() << "\n";
    myfile << findChild<QCheckBox *>("sleep")->isChecked() << "\n";
    myfile << findChild<QCheckBox *>("eating_healthy")->isChecked() << "\n";
    myfile << findChild<QCheckBox *>("productivity")->isChecked() << "\n";
    myfile << findChild<QCheckBox *>("communications")->isChecked() << "\n";
    myfile << findChild<QCheckBox *>("screen_time")->isChecked() << "\n";
    myfile.close();
    auto settings{findChild<QWidget *>("settings_frame")};
    settings->hide();
    auto chat_box{findChild<QWidget *>("scrollArea")}; // uniform!
    chat_box->show();
}

void MainWindow::on_filterButton_clicked()
{
    auto spinBox = findChild<QSpinBox *>("numberOfEntries");
    int n = spinBox->value();
    entries = testing::random_entries(n);
    display_graph(entries, ui);
    display_entries(entries, ui);
}

void MainWindow::on_newEntryButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    EntryPerso *e = new EntryPerso();
    e->set_mood(0);
    e->set_qdate(QDate::currentDate());
    std::vector<Friend *> fr;
    fr.push_back(new Friend("fr", 1));
    std::vector<Activity *> activity;
    activity.push_back(new Activity("act", 1));
    e->set_friends(fr);
    e->set_activities(activity);
    e->set_title("");
    e->set_text("");
    entries.insert(entries.begin(), e);
    card = new EntryCard(20, 300, 300, "white", e);
    card->display(ui->newEntry);
    card->change();
    card->update();
}

void MainWindow::on_saveEntryButton_clicked()
{
    display_graph(entries, ui);
    display_entries(entries, ui);
    ui->stackedWidget->setCurrentIndex(0);
    ui->newEntry->removeItem(ui->newEntry->takeAt(0));
}
