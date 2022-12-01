#include "moodanalysis.h"

#include <string>
#include <math.h>
#include <vector>

// DayLog implementation:
string DayLog::get_day() const{
    return day;
}

double DayLog::get_mood() const{
    return mood;
}

vector<DayLog> MoodAnalysis::anomalies_detection(vector<DayLog> log){
    /**
     * @param vector of DayLogs [string day, double mood].
     * @return vector of dates at which anomalie in mood was detected (value is 2 SDs far from its mean).
     */
    vector<double> mood_list{};
    vector<DayLog> res{};  // stores output info about anomaly points

    for(vector<DayLog>::iterator i = log.begin(); i < log.end(); i++){
            mood_list.push_back(i->get_mood());
    }
    /*double mean = calculate_mean(mood_list);
    for(int i = 0; i < log.size(); i++){
        if (*(mood_list.begin() + i) - mean >= 2 * calculateSD(mood_list)){
            DayLog temp = {(log.begin() + i)->get_day(), (log.begin() + i)->get_mood()};
            res.push_back(temp);
        }
    }*/
    return res;
}

/* This will be uncommented once get_lastn_average is implemented
// MoodAnalisys implementation:
bool MoodAnalysis::alert_depression(int n, int m){
    return get_lastn_average(n) < get_lastn_average(m);
}
*/
