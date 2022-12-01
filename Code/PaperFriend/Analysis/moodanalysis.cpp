#include "moodanalysis.h"
#include "entryclasses.h"

#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
#include <map>

// MoodAnalysis implementation:
double MoodAnalysis::get_lastn_average(int n, std::string item){
    /**
     * @param int n: number of n last days we take into account, string item: name of item (e.g. "mood" or "screen_time")
     * @return double average mood for last n days
     */
    std::vector<double> mood_list{};
    std::vector<double> sleep_list{};
    std::vector<double> eating_healthy_list{};
    std::vector<double> productivity_list{};
    std::vector<double> communications_list{};
    std::vector<double> screen_time_list{};

    for(int i = std::max<int>(0, log.size() - n); i < log.size(); i++){
        if (item == "eating_healthy") {
            eating_healthy_list.push_back((log.begin() + i)->get_eating_healthy());
        } else if (item == "sleep") {
            sleep_list.push_back((log.begin() + i)->get_sleep());
        } else if (item == "productivity") {
            productivity_list.push_back((log.begin() + i)->get_productivity());
        } else if (item == "communications") {
            communications_list.push_back((log.begin() + i)->get_communications());
        } else if (item == "screen_time") {
            screen_time_list.push_back((log.begin() + i)->get_screen_time());
        } else {
            mood_list.push_back((log.begin() + i)->get_mood());
        }
    }

    if (item == "eating_healthy") {
        return avg<double>(eating_healthy_list);
    } else if (item == "sleep") {
        return avg<double>(sleep_list);
    } else if (item == "productivity") {
        return avg<double>(productivity_list);
    } else if (item == "communications") {
        return avg<double>(communications_list);
    } else if (item == "screen_time") {
        return avg<double>(screen_time_list);
    } else {
        return avg<double>(mood_list);
    }

}

std::vector<EntryPerso> MoodAnalysis::anomalies_detection(std::vector<EntryPerso> log){
    /**
     * @param vector of EntryPersos.
     * @return vector of dates at which anomalie in mood was detected (value is 2 SDs far from its mean).
     */
    std::vector<double> mood_list{};
    std::vector<EntryPerso> res{};  // stores output info about anomaly points

    for(std::vector<EntryPerso>::iterator i = log.begin(); i < log.end(); i++){
            mood_list.push_back(i->get_mood());
    }
    double mean = avg<double>(mood_list);
    for(int i = 0; i < log.size(); i++){
        if (*(mood_list.begin() + i) - mean >= 2 * stddev(mood_list)){
            EntryPerso temp;
            temp.set_mood((log.begin() + i)->get_mood());
            temp.set_date((log.begin() + i)->get_date());
            res.push_back(temp);
        }
    }
    return res;
}

bool MoodAnalysis::alert_depression(int n, int m){
    /**
    * @param int n: number n and m of days with the assumption (needed only for it to make sense)
    *               that n < m                 
    * @returns the mood has been lower on average over the last n days than it has been over the last m
    */
    return get_lastn_average(n, "mood") < get_lastn_average(m, "mood");
}

std::multimap<double, std::string> invert(std::map<std::string, double> & mymap){
    /**
    * @param map
    * @returns inversed map (swaps keys with values)
    */
    std::multimap<double, std::string> multiMap;

    std::map<std::string, double> :: iterator it;
    for (it=mymap.begin(); it!=mymap.end(); it++)
    {
        multiMap.insert(make_pair(it->second, it->first));
    }

    return multiMap;
}

std::vector<std::string> MoodAnalysis::item_priority(std::vector<EntryPerso> log){

    std::vector<double> mood_list{};
    for(std::vector<EntryPerso>::iterator i = log.begin(); i < log.end(); i++){
            mood_list.push_back(i->get_mood());
    }

    std::vector<double> sleep_list{};
    for(std::vector<EntryPerso>::iterator i = log.begin(); i < log.end(); i++){
            sleep_list.push_back(i->get_sleep());
    }

    std::vector<double> eating_healthy_list{};
    for(std::vector<EntryPerso>::iterator i = log.begin(); i < log.end(); i++){
            eating_healthy_list.push_back(i->get_eating_healthy());
    }

    std::vector<double> productivity_list{};
    for(std::vector<EntryPerso>::iterator i = log.begin(); i < log.end(); i++){
            productivity_list.push_back(i->get_productivity());
    }

    std::vector<double> communications_list{};
    for(std::vector<EntryPerso>::iterator i = log.begin(); i < log.end(); i++){
            communications_list.push_back(i->get_communications());
    }

    std::vector<double> screen_time_list{};
    for(std::vector<EntryPerso>::iterator i = log.begin(); i < log.end(); i++){
            screen_time_list.push_back(i->get_screen_time());
    }

    //correlations with mood
    double cor_sleep_mood = cor(sleep_list, mood_list);
    double cor_eating_healthy_mood = cor(eating_healthy_list, mood_list);
    double cor_productivity_mood = cor(productivity_list, mood_list);
    double cor_communications_mood = cor(communications_list, mood_list);
    double cor_screen_time_mood = cor(screen_time_list, mood_list);

    int N = log.size();
    //deviation of avg of last week from avg of last month ( 0, when N <= 7)
    double sleep_dev = abs(get_lastn_average(std::min(7, N), "sleep") - get_lastn_average(std::min(N, 30), "sleep"));
    double eating_healthy_dev = abs(get_lastn_average(std::min(7, N), "eating_healthy") - get_lastn_average(std::min(N, 30), "eating_healthy"));
    double productivity_dev = abs(get_lastn_average(std::min(7, N), "productivity") - get_lastn_average(std::min(N, 30), "productivity"));
    double communications_dev = abs(get_lastn_average(std::min(7, N), "communications") - get_lastn_average(std::min(N, 30), "communications"));
    double screen_time_dev = abs(get_lastn_average(std::min(7, N), "screen_time") - get_lastn_average(std::min(N, 30), "screen_time"));

    std::map<std::string, double> mood_influence;  // correlation with mood * item deviation from avg
    mood_influence["sleep"] = cor_sleep_mood * sleep_dev;
    mood_influence["eating_healthy"] = cor_eating_healthy_mood * eating_healthy_dev;
    mood_influence["productivity"] = cor_productivity_mood * productivity_dev;
    mood_influence["communications"] = cor_communications_mood * communications_dev;
    mood_influence["screen_time"] = cor_screen_time_mood * screen_time_dev;

    //Sort map (done with MULTIMAP)
    std::map<double, std::string> :: iterator iter;

    // invert mymap using the invert function created above
    std::multimap<double, std::string> newmap = invert(mood_influence);
    //pushing sorted item names in the vector
    std::vector<std::string> vec{};
    for (iter=newmap.begin(); iter!=newmap.end(); iter++)
        {
            vec.push_back(iter->second);
        }
    return vec;
}


std::string MoodAnalysis::suggestion(std::vector<EntryPerso> log){  // some more exciting gameplay can be implementen later
    /**
    * @param NAN
    *
    * @returns string "general mood review + suggestion"
    */
    std::string str{};
    if (log.end()->get_mood() >= get_lastn_average(7, "mood")){
        str += "Your mood today is better than averadge! \n";
        str += "For now " + *(item_priority(log).begin()) + " and " + *(item_priority(log).begin() + 1) + "improve your mood the most, keep it up!";  // suggest top two items which affected the mood the most
    }
    else{
        str += "Your mood today is less than averadge:( \n";
        str += "Try to increase " + *(item_priority(log).begin()) + " and " + *(item_priority(log).begin() + 1) + "!";
    }

    return str;
}
