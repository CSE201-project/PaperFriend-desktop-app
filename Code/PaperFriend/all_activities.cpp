#include "all_activities.h"
#include"activity_cell.h"
#include "ui_all_activities.h"
#include<QFile>
#include <QMessageBox>
#include"activityclasses.h"
#include "mainwindow.h"



all_activities::all_activities(MainWindow *mainwindow,std::vector<Activity> &vector_activity,QWidget *parent) :
    mainwindowptr(mainwindow),
    vector_activities(vector_activity),
    QDialog(parent),
    ui(new Ui::all_activities)
{
    ui->setupUi(this);
    int ActivitiesCellNumberTotal = 0;
    this->add_previous_cells();
}

all_activities::~all_activities()
{
    delete ui;
}

void all_activities::on_add_activity_button_clicked()
{
    addNewCell();
}

void all_activities::addNewCell(QString cellText, QString cellName, int type)
{
    activity_cell *new_activity_cell = new activity_cell(this); // Create a new activity_cell.
    ui->activities_cell_layout->addWidget(new_activity_cell); // add a widget in the activities_cell_layout.
    connect(new_activity_cell, SIGNAL(closeThisCell(int)),this,SLOT(closeCell(int)));
    new_activity_cell->setAttribute(Qt::WA_DeleteOnClose, true); // here we set an attribute so whenever we close the cell it is indeed deleted.
    new_activity_cell->ActivitiesCellNumber = ActivitiesCellNumberTotal; // here in the new_activity_cell I defined the activity cell number to be equal to the total activities cell number.
    //new_activity_cell->setTextData(cellText); // this line set the text.
    new_activity_cell->set_activity_cell_name(cellName);
    allCellPtr.append(new_activity_cell); // here I append this new activity cell to the data vector.
    /*QString name_activity = allCellPtr[ActivitiesCellNumberTotal]->get_activity_name();
    int type_activity = allCellPtr[ActivitiesCellNumberTotal]->get_activity_type();
    qDebug()<< name_activity<<type_activity;*/
    new_activity_cell->set_activity_type(type);
    vector_activities.push_back(Activity(cellName.toStdString(), type, 0));
    ActivitiesCellNumberTotal++; // the total number of activities is incremented by 1.
}

void all_activities::add_previous_cells(){
    /*for(int i=0; i < allCellPtr.size(); i++){
        allCellPtr.at(i)->setParent(this); // set the new window to be the parent
        ui->activities_cell_layout->addWidget(allCellPtr.at(i));
    }*/
    for(long long unsigned i = 0; i< vector_activities.size(); i++){
        addNewCell("", QString::fromStdString(vector_activities.at(i).get_name()), vector_activities.at(i).get_type());
        vector_activities.pop_back();
    }

}

void all_activities::closeCell(int ActivitiesCellNumber){
    allCellPtr[ActivitiesCellNumber]->close();
    for(int i=ActivitiesCellNumber+1; i<allCellPtr.length(); i++){
        allCellPtr[i]->ActivitiesCellNumber--;
    }
    ActivitiesCellNumberTotal--;
    allCellPtr.remove(ActivitiesCellNumber);
}

void all_activities::on_save_activity_button_clicked()
{
    QFile activities_file("./activities.txt");
    activities_file.open(QIODevice::WriteOnly | QIODevice::Text); //Opens activities_file and allow to write in the text file.
    QApplication::processEvents();
    QString name_activity;
    int type_activity;
    vector_activities.clear();
    for(int i=0;i<allCellPtr.size();++i){
        name_activity = allCellPtr[i]->get_activity_name();
        type_activity = allCellPtr[i]->get_activity_type();
        qDebug()<< name_activity<<type_activity;
        QTextStream out(&activities_file);
        vector_activities.push_back(Activity(name_activity.toStdString(),type_activity,0));
        out << name_activity << " , " << type_activity << "???n";
    }
    save_activities(vector_activities);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this, "Save Confirmation", "All your activities are saved.",QMessageBox::Ok);
    if (reply == QMessageBox::Ok){
        this->close();
        }

    // we add the activity with value 0 to all existing entryPerso.
    //In terms of complexity it is not the best option but given that the number of entries will reasonably be less than 1000 it is going to be immediate in practice and saves us a lot of time in terms of coding
    mainwindowptr->add_new_activities_to_old_enties();


}

void all_activities::closeEvent (QCloseEvent *event){
    if(allCellPtr.size() != vector_activities.size()){
        QMessageBox::StandardButton answr_btn = QMessageBox::warning( this, tr("Paper friend"), tr("Have you saved your activities ?"),
                                                                      QMessageBox::Yes | QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (answr_btn != QMessageBox::Yes) {
            event->ignore();
        } else {
            this->close();
        }
    }else{
        for(int i=0; i<allCellPtr.size();++i){
            QString name_activity_acp;
            name_activity_acp = allCellPtr[i]->get_activity_name();
            std::string name_activity_vec;
            name_activity_vec = vector_activities[i].get_name();
            int type_activity_acp;
            type_activity_acp = allCellPtr[i]->get_activity_type();
            int type_activity_vec;
            type_activity_vec = vector_activities[i].get_type();
            if(name_activity_acp.toStdString() != name_activity_vec || type_activity_acp != type_activity_vec ){
                QMessageBox::StandardButton answr_btn = QMessageBox::warning( this, tr("Paper friend"), tr("Have you saved your activities ?"),
                                                                              QMessageBox::Yes | QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

                if (answr_btn != QMessageBox::Yes) {
                    event->ignore();
                } else {
                    this->close();
                }
            }if(type_activity_acp == 0){
                QMessageBox::StandardButton answr_btn = QMessageBox::warning( this, tr("Paper friend"), tr("Please enter an activity type."),
                                                                              QMessageBox::Yes | QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

                if (answr_btn != QMessageBox::Yes) {
                    event->ignore();
                } else {
                    event->ignore();
                }
            }if(name_activity_acp.toStdString() == "Activity name"){
                QMessageBox::StandardButton answr_btn = QMessageBox::warning( this, tr("Paper friend"), tr("Please enter an activity name."),
                                                                              QMessageBox::Yes | QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

                if (answr_btn != QMessageBox::Yes) {
                    event->ignore();
                } else {
                    event->ignore();
                }
            }
            }
        }
    }



