#include "All_activities.h"
#include "activity_cell.h"
#include "ui_All_activities.h"

int All_activities::ActivitiesCellNumberTotal = 0;
QVector<activity_cell *> All_activities::allCellPtr;

All_activities::All_activities(QWidget *parent)
    : QDialog{parent}, ui{new Ui::All_activities} // uniform
{
    ui->setupUi(this);
    // ActivitiesCellNumberTotal=0;
}

All_activities::~All_activities() { delete ui; }

void All_activities::on_add_activity_button_clicked() { addNewCell(); }

void All_activities::add_new_cell(QString cellText, QString cellName)
{
    activity_cell *new_activity_cell =
        new activity_cell(this); // Create a new activity_cell.
    ui->activities_cell_layout->addWidget(
        new_activity_cell); // add a widget in the activities_cell_layout.
    connect(new_activity_cell, SIGNAL(closeThisCell(int)), this,
            SLOT(close_cell(int)));
    new_activity_cell->setAttribute(
        Qt::WA_DeleteOnClose, true); // here we set an attribute so whenever we
                                     // close the cell it is indeed deleted.
    new_activity_cell->ActivitiesCellNumber =
        ActivitiesCellNumberTotal; // here in the new_activity_cell I defined
                                   // the activity cell number to be equal to
                                   // the total activities cell number.
    // new_activity_cell->setTextData(cellText); // this line set the text.
    new_activity_cell->set_activity_cell_name(cellName);
    allCellPtr.append(new_activity_cell); // here I append this new activity
                                          // cell to the data vector.
    ActivitiesCellNumberTotal++;          // the total number of activities is
                                          // incremented by 1.
}

void All_activities::add_previous_cells()
{
    for (int i = 0; i < allCellPtr.size(); i++)
    {
        allCellPtr.at(i)->setParent(
            this); // set the new window to be the parent
        ui->activities_cell_layout->addWidget(allCellPtr.at(i));
    }
}

void All_activities::close_cell(int ActivitiesCellNumber)
{
    allCellPtr[ActivitiesCellNumber]->close();
    for (int i = ActivitiesCellNumber + 1; i < allCellPtr.length(); i++)
    {
        allCellPtr[i]->ActivitiesCellNumber--;
    }
    ActivitiesCellNumberTotal--;
    allCellPtr.remove(ActivitiesCellNumber);
}
