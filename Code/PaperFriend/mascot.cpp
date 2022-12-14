#include "mascot.h"
#include <QLayout>

Mascot::Mascot(){
    QString peaceout = ":/pictures/rsc/peaceout-resized.png";
    QString  excited = ":/pictures/rsc/excited-resized.png";
    QString pleading = ":/pictures/rsc/pleading-resized.png";
    mascot = peaceout;
    imagepath_strings_vect = {excited,pleading,peaceout};}

QString Mascot::get_mascot(){return mascot;}

void Mascot::image_to_display(DataAnalysis *data_analysis){
    std::string str = data_analysis -> suggestion(0); //0 is just there until the analysis team
                                                  //make the funtion take no variable or sth else
    size_t found = str.find(" today is better than average! \n");
    if (found != std::string::npos) {
        img_path = imagepath_strings_vect[0] ;
    }
    else{
         img_path = imagepath_strings_vect[1];
    }
}


