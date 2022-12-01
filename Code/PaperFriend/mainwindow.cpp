#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dynamicgraph.h"
#include "cardclasses.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // display canvas created in drag-and-drop


    // test for the EntryCard class
    EntryPerso *e = new EntryPerso();
    EntryCard *c = new EntryCard(20, 300, 300, "beige", e);
    //c->display(ui->frame_5); //displays the entry in the frame_5

    EntryCard *c2 = new EntryCard(20, 300, 300, "beige", e);
    //c2->display(ui->frame_5); //displays the entry in the frame_5

    EntryCard *c3 = new EntryCard(20, 300, 300, "beige", e);
    //c3->display(ui->frame_5); //displays the entry in the frame_5

    EntryCard *c4 = new EntryCard(20, 300, 300, "beige", e);
    //c4->display(ui->frame_5); //displays the entry in the frame_5


    EntryCard *c5 = new EntryCard(20, 300, 300, "beige", e);
    //c4->display(ui->frame_5); //displays the entry in the frame_5

    QVBoxLayout *qvbox = new QVBoxLayout(this);

    qvbox->addWidget(c);
    qvbox->addWidget(c2);
    qvbox->addWidget(c3);
    qvbox->addWidget(c4);
    qvbox->addWidget(c5);

    ui->frame_5->setLayout(qvbox);

    //test for the DynamicGraph class
    EntryPerso *e1 = new EntryPerso(); //create an entry
    EntryPerso *e2 = new EntryPerso();
    e1->set_mood(10.); //put a dummy mood
    e2->set_mood(12.);
    std::vector<EntryPerso> entries; //put entries in a vector
    entries.push_back(*e1);
    entries.push_back(*e2);
    DynamicGraph moodGraph = DynamicGraph(ui->frame,entries); // the parent frame (frame in which the graph is going to be displayed) is ui->frame
    moodGraph.display(); //displays the graph
}

MainWindow::~MainWindow()
{
    delete ui;
}

