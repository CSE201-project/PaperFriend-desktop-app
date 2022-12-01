#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dynamicgraph.h"
#include "cardclasses.h"
#include "all_activities.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // display canvas created in drag-and-drop


    // test for the EntryCard class
    EntryPerso *e = new EntryPerso();

    EntryCard *c = new EntryCard(20, 300, 300, "beige", e);
    c->display(ui->main_frame); //displays the entry in the main_frame.

    //test for the DynamicGraph class
    EntryPerso *e1 = new EntryPerso(); //create an entry
    EntryPerso *e2 = new EntryPerso();
    e1->set_mood(10.); //put a dummy mood
    e2->set_mood(12.);
    std::vector<EntryPerso> entries; //put entries in a vector
    entries.push_back(*e1);
    entries.push_back(*e2);
    DynamicGraph moodGraph = DynamicGraph(entries); // the parent frame (frame in which the graph is going to be displayed) is ui->frame
    moodGraph.display(ui->graph_frame); //displays the graph
    this -> showMaximized();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent (QCloseEvent *event){
    QMessageBox::StandardButton answr_btn = QMessageBox::question( this, tr("Paper friend"), tr("Are you sure?\n"),
                                         QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (answr_btn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}


void MainWindow::on_pushButton_clicked()
{
    hide();
    all_habits = new All_Habits(this);
    all_habits -> showMaximized();
}



void MainWindow::on_activitie_button_clicked()
{
    all_activities my_activities;
    my_activities.setModal(true);
    my_activities.exec();

}

