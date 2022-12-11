#ifndef TESTING_H
#define TESTING_H

#include "entryclasses.h"

namespace testing
{
std::vector<EntryPerso *> random_entries(int n)
{
    std::vector<EntryPerso *> entries;
    for (int i = n; i >= 1; i--)
    {
        EntryPerso *e = new EntryPerso();
        e->set_mood(1 + std::rand() % 100);
        e->set_qdate(QDate(2022, 11, i));
        std::vector<Friend *> fr;
        fr.push_back(new Friend("fr", 1));
        std::vector<Activity *> activity;
        activity.push_back(new Activity("act", 1));
        e->set_friends(fr);
        e->set_activities(activity);
        e->set_title("THIS IS A TITLE");
        e->set_text("some text ...");
        entries.push_back(e);
    }
    return entries;
}
} // namespace testing

#endif