#include "cardclasses.h"
//#include "file_processing\file_processing\file_save_and_load.h"
#include <cmath>

#include <QCalendar>
#include <QDate>
#include <QMessageBox>

Card::Card(int border_radius, int width, int height, QString color)
    : border_radius(border_radius), background_color(color)
{
    this->resize(width, height);
    this->rect().setHeight(height);
    this->rect().setWidth(width);
    this->setStyleSheet("background-color: " + color +
                        "; border: 1px solid black; border-radius: " +
                        QString::number(border_radius) + "px;");
    vb_layout = new QVBoxLayout(this);
    vb_layout->setContentsMargins(0, 0, 0, 0);
    vb_layout->setSpacing(0);
    this->setLayout(vb_layout);
}

Card::~Card() { delete vb_layout; }

int Card::get_width() const { return this->width(); }

int Card::get_height() const { return this->height(); }

int Card::get_border_radius() const { return border_radius; }

QString Card::get_background_color() const { return background_color; }

void Card::set_width(int width)
{
    this->resize(width, this->height());
    this->rect().setWidth(width);
}

void Card::set_height(int height)
{
    this->resize(this->width(), height);
    this->rect().setHeight(height);
}

void Card::set_border_radius(int border_radius)
{
    this->border_radius = border_radius;
}

void Card::set_background_color(QString color)
{
    this->background_color = color;
}

void Card::display(QLayout *layout)
{
    this->setStyleSheet("background-color: " + background_color +
                        "; border: 1px solid black; border-radius:" +
                        QString::number(border_radius) + "px;");
    layout->addWidget(this);
}

QString generate_date_string(QDate date)
{
    /* Using the date of the entry generates the QString of the form
     * 'Day - dd days ago
     * date of the entry '
     */

    int day_int = date.dayOfWeek();
    QString day_string;
    switch (day_int)
    {
    case 1:
        day_string = "Mon";
        break;
    case 2:
        day_string = "Tue";
        break;
    case 3:
        day_string = "Wed";
        break;
    case 4:
        day_string = "Thu";
        break;
    case 5:
        day_string = "Fri";
        break;
    case 6:
        day_string = "Sat";
        break;
    case 7:
        day_string = "Sun";
        break;
    default:
        day_string = QString::number(day_int);
        break;
    }

    QDate today = QDate::currentDate();
    int days_ago = date.daysTo(today);
    QString days_ago_string;
    if (days_ago == 0)
    {
        return day_string + " - today \n" + date.toString("MM-dd-yyyy");
    }
    else if (days_ago % 100 == 1)
    {
        days_ago_string = " day ago \n";
    }
    else
    {
        days_ago_string = " days ago \n";
    }

    return day_string + " - " + QString::number(days_ago) + days_ago_string +
           date.toString("MM-dd-yyyy");
}

void generate_rgb(QString &red, QString &green, double m)
{
    /* Associate a color on a scale from red to green
     * according to the mood parameter
     */
    if (m <= 0.5)
    {
        red = QString::number(255);
        green = QString::number(255 * m / (1 - m));
    }
    else
    {
        green = QString::number(255);
        red = QString::number(255 * (1 - m) / m);
    }
}

EntryCard::EntryCard(int border_radius, int width, int height, QString color,
                     Entry &entry, bool readOnly)
    : Card(border_radius, width, height, color), entry(entry),
      readOnly(readOnly)
{
    display_layout = new QVBoxLayout();
    entry_perso = nullptr;
    mood_slider_w = new QWidget(this);
    mood_slider_instr = new QLabel(mood_slider_w);
    mood_slider = new QSlider(Qt::Horizontal, mood_slider_w);
    mood_slider_vb = new QVBoxLayout(mood_slider_w);
    if (static_cast<EntryPerso *>(entry) != nullptr)
    {
        entry_perso = static_cast<EntryPerso *>(entry);
    }

    // display date
    date_display = new QLabel();
    date_display->setMaximumHeight(45);
    date_display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    date_display->setText(generate_date_string(entry->get_qdate()));
    date_display->setAlignment(Qt::AlignCenter);
    date_display->setStyleSheet(
        "font-weight: bold; border-right: 1px solid black; border-radius: 0px; "
        "border-top-left-radius: " +
        QString::number(border_radius) + "px; border: 1px solid black;");

    // entry text and title
    text_title_w = new QWidget(this);
    text_title_vb = new QVBoxLayout(text_title_w);

    // text-editor

    edit_and_return = new QGroupBox(this);
    edit_text_w = new QStackedWidget();
    edit_text = new TextEditor();
    if ((entry->get_title() + entry->get_text()) != "")
    {
        edit_text->set_title(QString::fromStdString(entry->get_title() + "\n"));
        edit_text->append_text(QString::fromStdString(entry->get_text()));
    }

    // buttons
    modify = new QPushButton("Modify this entry", text_title_w);
    back_to_display = new QPushButton("Exit editing mode", edit_and_return);

    // entry text and title
    text_title_w->setStyleSheet(
        "border-style: none; border-radius: 0px; border-bottom-left-radius: " +
        QString::number(border_radius) + "px; border-bottom-right-radius: " +
        QString::number(border_radius) + "px; border-bottom: 1px solid black;");

    title =
        new QLabel(QString::fromStdString(entry->get_title()), text_title_w);
    title->setMinimumHeight(40);
    title->setMinimumWidth(this->get_width());
    title->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    title->setAlignment(Qt::AlignLeft);
    title->setStyleSheet("font: 18px; font-weight: bold; border-style: none;");
    title->setContentsMargins(5, 10, 5, 0);

    text_field =
        new QTextEdit(QString::fromStdString(entry->get_text()), text_title_w);
    text_field->setMinimumWidth(this->get_width());
    text_field->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    text_field->setAlignment(Qt::AlignLeft);
    text_field->setStyleSheet("font: 14px; border-style: none;");
    text_field->setReadOnly(true);
    text_field->setContentsMargins(10, 0, 10, 5);

    // modify
    modify->setMinimumWidth(40);
    modify->setStyleSheet(
        "QPushButton{color: white; background-color: black; font-weight: bold; "
        "font: 15px; border: 2px solid black; border-radius: 5px;} "
        "QPushButton:hover{background-color:white; color:black;}");
    connect(modify, &QPushButton::released, this, &EntryCard::handleModify);

    // handle layout
    text_title_vb->addWidget(title);
    text_title_vb->addWidget(text_field);
    text_title_vb->addWidget(modify);
    text_title_w->setLayout(text_title_vb);

    // text-editor
    edit_text_w->setMinimumWidth(this->get_width());
    edit_text_w->setMinimumHeight(this->get_height());
    edit_text_w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    edit_text_w->setStyleSheet("border-style: none;");
    edit_text_w->setContentsMargins(10, 0, 10, 5);

    // back_to_display
    back_to_display->setMinimumWidth(40);
    back_to_display->setStyleSheet(
        "QPushButton{color: white; background-color: black; font-weight: bold; "
        "font: 15px; border: 2px solid black; border-radius: 5px;} "
        "QPushButton:hover{background-color:white; color:black;}");
    connect(back_to_display, &QPushButton::released, this,
            &EntryCard::handleBack);

    // edit_text layout
    edit_text_w->addWidget(edit_text);
    edit_text_w->setCurrentWidget(edit_text);
    edit_text_w->setParent(edit_and_return);
    edit_vb = new QVBoxLayout(edit_and_return);
    edit_vb->addWidget(edit_text_w);
    edit_vb->addWidget(back_to_display);
    edit_and_return->setLayout(edit_vb);
    edit_and_return->setStyleSheet("border-style: none;");

    // top menu
    top_menu = new QHBoxLayout(this);
    top_menu->setAlignment(Qt::AlignTop);
    top_menu->setSpacing(0);
    date_display->setParent(this);
    top_menu->addWidget(date_display);
    fr_act_display = new QListWidget(this);

    // display specific for entryPerso
    if (entry_perso != nullptr)
    {

        // display activities and friends
        fr_act_display->setMaximumHeight(45);
        fr_act_display->setSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Minimum);
        fr_act_display->setStyleSheet(
            "font-weight: bold; border-radius: 0px; border: 1px solid black;");
        for (long long unsigned fr = 0;
             fr < (entry_perso->get_friends()).size(); fr++)
        {
            fr_act_display->addItem(QString::fromStdString(
                (entry_perso->get_friends()[fr])->get_name()));
        }
        for (long long unsigned act = 0;
             act < (entry_perso->get_activities()).size(); act++)
        {
            fr_act_display->addItem(QString::fromStdString(
                (entry_perso->get_activities()[act])->get_name()));
        }

        // display mood
        mood_display = new QLabel();
        mood_display->setText(
            "Mood: " + QString::number(std::round(entry_perso->get_mood())) +
            "%");
        mood_display->setMaximumHeight(45);
        mood_display->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Minimum);
        mood_display->setAlignment(Qt::AlignCenter);
        QString red, green;
        generate_rgb(red, green, entry_perso->get_mood() / 100);
        mood_display->setStyleSheet(
            "font-weight: bold; color: rgb(" + red + ", " + green +
            ", 0); border-left: 1px solid black; border-radius: 0px; "
            "border-top-right-radius: " +
            QString::number(border_radius) + "px;");

        // get mood
        mood_slider->setMinimum(0);
        mood_slider->setMaximum(100);
        mood_slider->setValue(int(this->entry_perso->get_mood()));
        mood_slider->setTickInterval(50);

        mood_slider->setTickPosition(QSlider::TicksBelow);
        mood_slider->setStyleSheet(
            "QSlider::groove:horizontal{border: 1px solid grey; background: "
            "qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 rgb(255, 0, "
            "0), stop: 0.5 rgb(255, 255, 0), stop: 1 rgb(0, 255, 0)); height: "
            "10px; border-radius: "
            "5px;}QSlider::sub-page:horizontal{background: transparent;border: "
            "1px solid grey;height: 10px;border-radius: 5px;} "
            "QSlider::add-page:horizontal {background: white; border: 1px "
            "solid grey;height: 10px;border-radius: 5px;} "
            "QSlider::handle:horizontal {background: grey; border: 1px solid "
            "dark-grey; width: 16px;margin-top: -3px;margin-bottom: "
            "-3px;border-radius: 5px;} QSlider::handle:horizontal:hover "
            "{background: dark-grey; border: 1px solid black; border-radius: "
            "5px;}");
        mood_slider_instr->setText("Slide the bar to enter your mood");
        mood_slider_instr->setStyleSheet(
            "font-weight: bold; border-style: none; border-radius: " +
            QString::number(get_border_radius()) + "px;");
        mood_slider_w->setMaximumHeight(45);
        mood_slider_w->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Minimum);
        mood_slider_instr->setAlignment(Qt::AlignCenter);
        mood_slider_vb->addWidget(mood_slider_instr);
        mood_slider_vb->addWidget(mood_slider);
        mood_slider_w->setLayout(mood_slider_vb);
        mood_slider_w->setStyleSheet(
            "border-radius: 0px; border-top-right-radius: " +
            QString::number(get_border_radius()) + "px;");

        if (fr_act_display->count() != 0)
        {
            date_display->setMinimumWidth(this->get_width() / 3);
            fr_act_display->setMinimumWidth(this->get_width() / 3);
            mood_display->setMinimumWidth(this->get_width() / 3);
            mood_slider_w->setMinimumWidth(this->get_width() / 3);
        }
        else
        {
            date_display->setMinimumWidth(this->get_width() / 2);
            fr_act_display->setMaximumWidth(0);
            fr_act_display->setVisible(false);
            mood_display->setMinimumWidth(this->get_width() / 2);
            mood_slider_w->setMinimumWidth(this->get_width() / 2);
        }

        // top menu
        fr_act_display->setParent(this);
        top_menu->addWidget(fr_act_display);
        mood_display->setParent(this);
        top_menu->addWidget(mood_display);
        top_menu->addWidget(mood_slider_w);
        if (isReadOnly())
        {
            mood_display->setVisible(true);
            mood_slider_w->setVisible(false);
        }
        else
        {
            mood_slider_w->setVisible(true);
            mood_display->setVisible(false);
        }
    }
    // add to the layout
    vb_layout->addItem(top_menu);
    vb_layout->addWidget(text_title_w);
    vb_layout->addWidget(edit_and_return);
    if (isReadOnly())
    {
        text_title_w->setVisible(true); // for readOnly
        edit_and_return->setVisible(false);
    }
    else
    {
        edit_and_return->setVisible(true); // for editor
        text_title_w->setVisible(false);
    }
    this->setLayout(vb_layout);
}

EntryCard::~EntryCard()
{
    delete entry;
    delete date_display;
    delete mood_display;
    delete fr_act_display;
    delete top_menu;
    delete title;
    delete text_field;
    delete text_title_vb;
    delete text_title_w;
    delete entry_perso;
    delete edit_text;
    delete edit_text_w;
    delete edit_vb;
    delete edit_and_return;
    delete modify;
    delete back_to_display;
    delete display_layout;
    delete mood_slider_w;
    delete mood_slider_instr;
    delete mood_slider;
    delete mood_slider_vb;
}

void EntryCard::handleModify()
{
    this->change();
    this->update();
}

void EntryCard::handleBack()
{
    QMessageBox alert;
    alert.setText("Do you want to save the changes to your entry?");
    alert.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                             QMessageBox::Cancel);
    alert.setDefaultButton(QMessageBox::Save);
    int choice = alert.exec();
    std::string retrieve_text = (edit_text->get_text()).toStdString();
    std::string new_title = retrieve_text.substr(0, retrieve_text.find("\n"));
    std::string new_text = retrieve_text.substr(retrieve_text.find("\n") + 1);
    bool saved;
    switch (choice)
    {
    case QMessageBox::Save:
        entry->set_title(new_title);
        entry->set_text(new_text);
        this->change();
        this->update();
        /*saved = save_entryperso(*entry_perso);
        if(saved){
            QMessageBox msg;
            msg.setText("successfully saved changes to the file");
            msg.exec();
        }
        else{
            QMessageBox msg;
            msg.setText("saving changes to the file failed");
            msg.exec();
        }*/
        break;
    case QMessageBox::Discard:
        this->change();
        break;
        alert.close();
    }
}

void EntryCard::change()
{
    readOnly = !readOnly;
    if (isReadOnly())
    {
        text_title_w->setVisible(true);
        edit_and_return->setVisible(false); // for readOnly text
        if (entry_perso != nullptr)
        {
            mood_slider_w->setVisible(false);
            mood_display->setVisible(true);
        }
    }
    else
    {
        text_title_w->setVisible(false);
        edit_and_return->setVisible(true); // for editor
        if (entry_perso != nullptr)
        {
            mood_display->setVisible(false);
            mood_slider_w->setVisible(true);
        }
    }
}

void EntryCard::update()
{
    // update values
    date_display->setText(generate_date_string(entry->get_qdate()));
    title->setText(QString::fromStdString(entry->get_title()));
    text_field->setText(QString::fromStdString(entry->get_text()));
    if ((entry->get_title() + entry->get_text()) != "")
    {
        edit_text->set_title(QString::fromStdString(entry->get_title() + "\n"));
        edit_text->append_text(QString::fromStdString(entry->get_text()));
    }
    else
    {
        edit_text->set_title("");
    }
    if (entry_perso != nullptr)
    {
        this->entry_perso->set_mood(this->mood_slider->value());
        mood_display->setText(
            "Mood: " + QString::number(std::round(entry_perso->get_mood())) +
            "%");
        mood_slider->setValue(int(this->entry_perso->get_mood()));
        // friends and activities
    }

    // update style
    this->setStyleSheet("background-color: " + get_background_color() +
                        "; border: 1px solid black; border-radius: " +
                        QString::number(get_border_radius()) + "px;");
    date_display->setStyleSheet("font-weight: bold; border-style: none; "
                                "border-radius: 0px; border-top-left-radius: " +
                                QString::number(this->get_border_radius()) +
                                "px; border: 1px solid black;");
    text_title_w->setStyleSheet(
        "border-style: none; border-radius: 0px; border-bottom-left-radius: " +
        QString::number(this->get_border_radius()) +
        "px; border-bottom-right-radius: " +
        QString::number(this->get_border_radius()) +
        "px; border-bottom: 1px solid black;");
    edit_text_w->setStyleSheet("border-style: none;");
    edit_and_return->setStyleSheet("border-style: none;");
    if (entry_perso != nullptr)
    {
        QString red, green;
        generate_rgb(red, green, entry_perso->get_mood() / 100);
        mood_display->setStyleSheet(
            "font-weight: bold; color: rgb(" + red + ", " + green +
            ", 0); border-left: 1px solid black; border-radius: 0px; "
            "border-top-right-radius:" +
            QString::number(this->get_border_radius()) + "px;");
        mood_slider->setStyleSheet(
            "QSlider::groove:horizontal{border: 1px solid grey; background: "
            "qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 rgb(255, 0, "
            "0), stop: 0.5 rgb(255, 255, 0), stop: 1 rgb(0, 255, 0)); height: "
            "10px; border-radius: "
            "5px;}QSlider::sub-page:horizontal{background: transparent;border: "
            "1px solid grey;height: 10px;border-radius: 5px;} "
            "QSlider::add-page:horizontal {background: white; border: 1px "
            "solid grey;height: 10px;border-radius: 5px;} "
            "QSlider::handle:horizontal {background: grey; border: 1px solid "
            "dark-grey; width: 16px;margin-top: -3px;margin-bottom: "
            "-3px;border-radius: 5px;} QSlider::handle:horizontal:hover "
            "{background: dark-grey; border: 1px solid black; border-radius: "
            "5px;}");
        mood_slider_instr->setStyleSheet(
            "font-weight: bold; border-style: none; border-radius: " +
            QString::number(get_border_radius()) + "px;");
        mood_slider_w->setStyleSheet(
            "border-radius: 0px; border-top-right-radius: " +
            QString::number(get_border_radius()) + "px;");
    }
}

void EntryCard::display(QLayout *layout)
{
    layout->addWidget(this);
    display_layout = layout;
}

bool EntryCard::isReadOnly() { return readOnly; }

void EntryCard::setReadOnly(bool readOnly) { this->readOnly = readOnly; }
