#include "cardclasses.h"

#include <QDate>
#include <QCalendar>

Card::Card(int border_radius, int width, int height, QString color) : border_radius(border_radius), background_color(color){
    this->resize(width, height);
    this->rect().setHeight(height);
    this->rect().setWidth(width);
    this->setStyleSheet("background-color: " + color + ";");
}

int Card::get_width() const{
    return this->width();
}

int Card::get_height() const{
    return this->height();
}

int Card::get_border_radius() const{
    return border_radius;
}

QString Card::get_background_color() const{
    return background_color;
}

void Card::set_width(int width){
    this->resize(width, this->height());
    this->rect().setWidth(width);
}

void Card::set_height(int height){
    this->resize(this->width(), height);
    this->rect().setHeight(height);
}

void Card::set_border_radius(int border_radius){
    this->border_radius = border_radius;
}

void Card::set_background_color(QString color){
    this->background_color = color;
}

void Card::display(QLayout *layout){
    QPainterPath path;
    path.addRoundedRect(this->rect(), border_radius, border_radius);
    QRegion mask = QRegion(path.toFillPolygon().toPolygon());
    this->setMask(mask);
    //QGraphicsView *gv = new QGraphicsView();
    //this->setParent(gv);
    //layout->addWidget(gv);
    layout->addWidget(this);

}

QString generate_date_string(std::string date_string){
    /* Using the date of the entry generates the QString of the form
     * 'Day - dd days ago
     * date of the entry '
     */

    //convert date_string to QDate object date
    int month = std::stoi(date_string.substr(0, 2));
    int day = std::stoi(date_string.substr(3, 2));
    int year = std::stoi(date_string.substr(6, 4));
    QDate date = QDate(year, month, day);

    int day_int = date.dayOfWeek();
    QString day_string;
    switch(day_int){
    case 1:
        day_string = "Mon"; break;
    case 2:
        day_string = "Tue"; break;
    case 3:
        day_string = "Wed"; break;
    case 4:
        day_string = "Thu"; break;
    case 5:
        day_string = "Fri"; break;
    case 6:
        day_string = "Sat"; break;
    case 7:
        day_string = "Sun"; break;
    default:
        day_string = QString::number(day_int); break;
    }

    QDate today = QDate::currentDate();
    int days_ago = date.daysTo(today);
    QString days_ago_string;
    if(days_ago % 100 == 1){
        days_ago_string = " day ago \n";
    }
    else{
        days_ago_string = " days ago \n";
    }

    return day_string + " - " + QString::number(days_ago) + days_ago_string + date.toString("MM-dd-yyyy");
}

void generate_rgb(QString &red, QString &green, double m){
    /* Associate a color on a scale from red to green
     * according to the mood parameter
     */
    if(m <= 0.5){
        red = QString::number(255);
        green = QString::number(255 * m / (1-m));
    }
    else{
        green = QString::number(255);
        red = QString::number(255 * (1-m) / m);
    }
}

EntryCard::EntryCard(int border_radius, int width, int height, QString color, EntryPerso *entry) : Card(border_radius, width, height, color), entry(entry){
    // display date
    date_display = new QLabel();
    date_display->setFixedWidth(this->get_width() / 3); // to be changed depending on the number of widgets
    date_display->setFixedHeight(50);
    date_display->setText(generate_date_string(entry->get_date()));
    date_display->setAlignment(Qt::AlignCenter);
    date_display->setStyleSheet("font: 8px; font-weight: bold;");

    // display activities and friends
    fr_act_display = new QGraphicsView();
    fr_act_display->setFixedWidth(this->get_width() / 3); // to be changed depending on the number of widgets
    fr_act_display->setFixedHeight(50);
    fr_act_labels = new QLabel[10];

    //display mood
    mood_display = new QLabel();
    mood_display->setText("Mood: " + QString::number(std::round(entry->get_mood() * 100)) + "%");
    mood_display->setFixedWidth(this->get_width() / 3); // to be changed depending on the number of widgets
    mood_display->setFixedHeight(50);
    mood_display->setAlignment(Qt::AlignCenter);
    QString red, green;
    generate_rgb(red, green, entry->get_mood());
    mood_display->setStyleSheet("font: 12px; font-weight: bold; background-color: rgb(" + red + ", " + green + ", 0);");


    //borders
    v_line = new QFrame();
    v_line->setFrameShape(QFrame::VLine);
    v_line->setFrameShadow(QFrame::Plain);
    v_line->setLineWidth(1);
    v_line1 = new QFrame();
    v_line1->setFrameShape(QFrame::VLine);
    v_line1->setFrameShadow(QFrame::Plain);
    v_line1->setLineWidth(1);

    //top menu
    top_menu = new QHBoxLayout(this);
    top_menu->setAlignment(Qt::AlignTop);
    top_menu->setSpacing(0);
    top_menu->addWidget(date_display);
    top_menu->addWidget(v_line);
    top_menu->addWidget(fr_act_display);
    top_menu->addWidget(v_line1);
    top_menu->addWidget(mood_display);
}

EntryCard::~EntryCard(){
    delete entry;
    delete date_display;
    delete mood_display;
    delete fr_act_display;
    delete top_menu;
    delete v_line;
    delete v_line1;
}
