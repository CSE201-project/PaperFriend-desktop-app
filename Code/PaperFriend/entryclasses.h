#ifndef ENTRYCLASSES_H
#define ENTRYCLASSES_H

#include <iostream>
#include <string>

#include "activityclasses.h"
#include "friendclasses.h"

class Entry {
public:
    Entry();
    Entry(std::string text, std::string title);
    ~Entry();

    std::string get_text() const;
    void set_text(std::string text);

    std::string get_title() const;
    void set_title(std::string text);

    std::string get_date() const;

    std::string get_weekday() const;

    int get_absolute_day() const;

protected:
    time_t time_log;
    std::string text;
    std::string title;
    std::string date; // "MM/DD/YYYY"
    std::string weekday; // "Monday", "Tuesday", ...
    int absolute_day; // Number of days since Epoch, considering a UTC+1 (Paris) time zone, about 5 minute precision on day change.
};


class EntryPerso : public Entry {
public:
    EntryPerso();
    EntryPerso(std::string text, std::string title, Activity* activities,Friend* friends, double mood);
    ~EntryPerso();

    Activity* get_activities() const;
    void set_activities(Activity* activities);

    Friend* get_friends() const;
    void set_friends(Friend* friends);

    double get_mood() const;
    void set_mood(double mood);

    double get_sleep() const;
    void set_sleep(double sleep);

    double get_eating_healthy() const;
    void set_eating_healthy(double eating_healthy);

    double get_productivity() const;
    void set_productivity(double productivity);

    double get_communications() const;
    void set_communications(double communications);

    double get_screen_time() const;
    void set_screen_time(double screen_time);

private:
    Activity* activities;
    Friend* friends;
    double mood;
    double sleep;
    double eating_healthy;
    double productivity;
    double communications;
    double screen_time;
};

#endif // ENTRYCLASSES_H
