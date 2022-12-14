#include "entryclasses.h"
#include"friendclasses.h"
#include"activityclasses.h"

#include <random>
#include <iostream>
#include <string>
#include <vector>


Entry::Entry(std::string text, std::string title) : title(title),
                                                    text(text) {
    this->set_qdate(QDate::currentDate());
}
Entry::~Entry() {}

std::string Entry::get_text() const {
    return text;
}

void Entry::set_text(std::string text) {
    this->text= text;
}

std::string Entry::get_title() const {
    return title;
}

void Entry::set_title(std::string title) {
    this->title=title;
}

std::string Entry::get_date() const {
    return date;
}

void Entry::set_date(std::string format_date){
    const char *s = "MM.dd.yyyy";
    std::string str(s);
    QString format = QString::fromStdString(s);
    QDate new_date = QDate::fromString(QString::fromStdString(format_date), format);
    this->set_qdate(new_date);
}

QDate Entry::get_qdate() const {
    return qdate;
}

void Entry::set_qdate(QDate qdate) {
    this->qdate=qdate;
    this->date = qdate.toString("MM.dd.yyyy").toStdString();
    this->weekday = qdate.toString("dddd").toStdString(); // Day name : "Monday", "Tuesday", ...
    this->absolute_day = qdate.toJulianDay();
}

std::string Entry::get_weekday() const{
    return weekday;
}

std::string Entry::get_month_name() const{
    return qdate.toString("MMMM").toStdString();
}

int Entry::get_absolute_day() const{
    return absolute_day;
}

int Entry::entry_type() const{
    return 0;
}


EntryPerso::EntryPerso(std::string title, std::string text,
                       std::vector<Activity*> p_activities, std::vector<Friend*> friends,
                       double mood,
                       double sleep,
                       double eating_healthy,
                       double productivity,
                       double communications,
                       double screen_time) : Entry(title, text),
                                             activities(p_activities), friends(friends),
                                             mood(mood),
                                             sleep(sleep),
                                             eating_healthy(eating_healthy),
                                             productivity(productivity),
                                             communications(communications),
                                             screen_time(screen_time) {

    all_activities.push_back(Activity("mood", mood));
    all_activities.push_back(Activity("sleep", sleep));
    all_activities.push_back(Activity("eating_healthy", eating_healthy));
    all_activities.push_back(Activity("productivity", productivity));
    all_activities.push_back(Activity("communications", communications));
    all_activities.push_back(Activity("screen_time", screen_time));

    for (auto& ptr : activities) all_activities.push_back(*ptr);
}
EntryPerso::~EntryPerso() {
    /* makes the build crash, btw this is unecessary as the destructors of vectors are called by themselves by the default destructor.
    activities.~vector();
    friends.~vector();
    */
}

double EntryPerso::get_var_value(int index) const{
    return all_activities[index].get_value();
}

std::string EntryPerso::get_var_name(int index) const{
    return all_activities[index].get_name();
}

std::vector<Activity*> EntryPerso::get_activities() const {
    return activities;
}

void EntryPerso::set_activities(std::vector<Activity*> activities) {
    this->activities = activities;
}
void EntryPerso::add_activity(Activity* activity) {
    activities.push_back(activity);
}

std::vector<Friend*> EntryPerso::get_friends() const {
    return friends;
}

void EntryPerso::set_friends(std::vector<Friend*> friends) {
    this->friends = friends;
}

double EntryPerso::get_mood() const {
    return mood;
}

void EntryPerso::set_mood(double mood) {
    this->mood = mood;
}

double EntryPerso::get_sleep() const {
    return sleep;
}

void EntryPerso::set_sleep(double sleep) {
    this->sleep = sleep;
}

double EntryPerso::get_eating_healthy() const {
    return eating_healthy;
}

void EntryPerso::set_eating_healthy(double eating_healthy) {
    this->eating_healthy = eating_healthy;
}

double EntryPerso::get_productivity() const {
    return productivity;
}

void EntryPerso::set_productivity(double productivity) {
    this->productivity = productivity;
}

double EntryPerso::get_communications() const {
    return communications;
}

void EntryPerso::set_communications(double communications) {
    this->communications = communications;
}

double EntryPerso::get_screen_time() const {
    return screen_time;
}

void EntryPerso::set_screen_time(double screen_time) {
    this->screen_time = screen_time;
}

int EntryPerso::entry_type() const{
    return 1;
}


std::vector<EntryPerso*> sample_entries(int n,std::vector<Activity*> possible_activities, std::vector<Friend*> possible_friends )
{
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(65,25);
    std::vector<EntryPerso*> res = std::vector<EntryPerso*>();
    for(int i=n;i>5;--i){
        std::vector<Activity*> activities;
        std::vector<Friend*> friends;

        int activity_index = rand()%possible_activities.size();
        activities.push_back(possible_activities[activity_index]);

        int friend_index = rand()%possible_friends.size();
        friends.push_back(possible_friends[friend_index]);

        double mood = distribution(generator) ;
        mood = std::llround(mood*2) / 2.0;
        if(mood>100){mood=100;}
        if(mood<0){mood=0;}
        double sleep = rand()%101;
        double eating_healthy = rand()%101;
        double productivity = rand()%101;
        double communications = rand()%101;
        double screen_time = rand()%101;
        EntryPerso *entry = new EntryPerso("sample entry text","The title of the entry",activities,friends,mood,sleep,eating_healthy,productivity,communications,screen_time);
        entry->set_qdate((QDate::currentDate()).addDays(-i));
        res.push_back(entry);
    }
    return res;
}
