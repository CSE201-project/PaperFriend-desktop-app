#include "activity_cell.h"
#include "ui_activity_cell.h"

Activity_cell::Activity_cell(QWidget *parent)
    : QWidget{parent}, ui{new Ui::Activity_cell}
{
    ui->setupUi(this);
}

Activity_cell::~Activity_cell() { delete ui; }

// void Activity_cell::set_text(QString cellText){
//     ui->textEdit->clear();
//     ui->textEdit->insertPlainText(cellText);  // the textEdit window will
//     be adding the text as an input.
// }

void Activity_cell::set_name(QString cellName)
{
    ui->activity_name_CB->setText(cellName);
}

void Activity_cell::on_delete_activity_button_clicked()
{
    // use auto whenever possible, use uniform initialization
    auto reply{QMessageBox::question(this, "Delete Confirmation",
                                     "Delete this activity ?",
                                     QMessageBox::Yes | QMessageBox::No)};
    if (reply == QMessageBox::Yes)
    {
        emit closeThisCell(ActivitiesCellNumber);
        this->close();
    }
}
