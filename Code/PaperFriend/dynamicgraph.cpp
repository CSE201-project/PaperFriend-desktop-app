/*

  To implement: if there is a too large gap in the dates, leave an empty space
  in the graph x axis in absolute value

*/

#include "dynamicgraph.h"
#include <QApplication>
#include <QChartView>
#include <QtCharts/QLegend>
#include <QtCharts>
// Adds categories to the charts axes
#include <QtCharts/QBarCategoryAxis>
// Used to create a line chart
#include <QtCharts/QLineSeries>
// Used to change names on axis
#include "entryclasses.h"
#include <QtCharts/QCategoryAxis>

//////////////////
// private methods useful for display
DynamicGraph::moodlevel DynamicGraph::associated_mood_level(double mood) const
{
    /*
     returns the mood level corresponding to the given mood value
     */
    if (mood >= 75)
    {
        return good;
    }
    else if (mood >= 35)
    {
        return medium;
    }
    return bad;
}

void DynamicGraph::set_color(QLineSeries *series, moodlevel level)
{ // can transform into a template to handle line/spline customization
    /*
     changes the color of the line corresponding to this series on the graph
     with the color corresponding to the level of mood. If the mood is bad, the
     color will be set to red, etc...
     */
    QPen pen = series->pen();
    pen.setWidth(3); // to change the line's width
    pen.setColor(
        level_colors[level]); // level_colors defined in hpp file indicates the
                              // color associated to each mood level
    series->setPen(pen);
}

void DynamicGraph::get_dummy_point(double y1, double y2, double x1, double x2,
                                   double (&res)[2])
{
    /*
     returns the dummy point that one need to add to the series to make it
     change color at the righ location. For example, if the mood goes from y1=6
     to y2=14 then the line has to change color at y=7. We have to compute the
     associated x to keep the same slope as before.
     */
    double target; // y coordinate of the dummy point
    int previous = associated_mood_level(y1);
    int current = associated_mood_level(y2);
    // important: we assume here that the mood is only changing by 1 level. For
    // example, if the new mood level is "good", we assume that it was "medium"
    // before. If this goes from "bad" to "good", this function should be called
    // twice with well chosen parameters.

    // we first get the target
    if (previous == 0)
    { // then the mood was bad and it turns medium
        target = 35;
    }
    else if (previous == 1)
    {
        if (current == 2)
        { // then it was medium before and good now
            target = 75;
        }
        else
        {
            target = 35;
        }
    }
    else
    { // the mood was good and is now medium
        target = 75;
    }

    // We got the target. Now, let's compute the slope between our two points
    // (x1,y1) and (x2,y2) and find what should be x so that (x,target) is
    // correctly placed
    double slope = (y2 - y1) / (x2 - x1);
    // y1 + slope*x = target <=> x= (target-y1)/slope
    double x = x1 + (target - y1) / slope;
    res[0] = x;
    res[1] = target;
    // we don't return anything as res is given by reference.
}

/////////////////////
// Constructor
DynamicGraph::DynamicGraph(const std::vector<EntryPerso> &entries)
    : listofseries(std::vector<QLineSeries *>()), entries(entries)
{
    QDate today = QDate::currentDate();
    QLineSeries *series = new QLineSeries(); // series in which we add the
                                             // points to display on the graph
    moodlevel current_mood_level =
        associated_mood_level(entries[0]->get_mood());
    set_color(
        series,
        current_mood_level); // initialize the right color for the first point
    int n = entries.size() -
            1; // the x axis will be negative numbers: x days ago = -x.
    int i = 0;
    double y1;
    double x1;
    for (auto e : entries)
    {
        double x2 = -(e->get_qdate().daysTo(today));
        double y2 = e->get_mood(); // current point
        moodlevel level = associated_mood_level(y2);
        if (level != current_mood_level)
        { // the mood level changed, we need to change the line's color, so
          // create a new series (a series can only have one single color)
            double dummypoint1[2];
            double dummypoint2[2];
            if (abs(level - current_mood_level) >= 2)
            { // if we jump directly from bad mood to good mood, then we have to
              // create a fake series to make the line orange between the first
              // dummy point and the second dummy point.
                get_dummy_point(
                    y1, y2, x1, x2,
                    dummypoint1); // see description of corresponding function
                get_dummy_point(dummypoint1[1], y2, dummypoint1[0], x2,
                                dummypoint2);
                *series << QPointF(dummypoint1[0], dummypoint1[1]);
                listofseries.push_back(series);
                series = new QLineSeries(); // we create anew curve with a
                                            // different color
                *series << QPointF(
                    dummypoint1[0],
                    dummypoint1[1]); // the new series needs to start with the
                                     // last point of the previous one to make
                                     // them appear connected on the graph
                *series << QPointF(dummypoint2[0], dummypoint2[1]);
                set_color(series, medium);
                listofseries.push_back(series);
                series = new QLineSeries();
                *series << QPointF(dummypoint2[0], dummypoint2[1]);
            }
            else
            { // jump of only one mood level
                get_dummy_point(y1, y2, x1, x2, dummypoint1);
                *series << QPointF(dummypoint1[0], dummypoint1[1]);
                listofseries.push_back(series);
                series = new QLineSeries();
                *series << QPointF(dummypoint1[0], dummypoint1[1]);
            }

            current_mood_level = level;
            set_color(series, level);
        } // if the mood level remained the same, we just add th epoint to the
          // current series
        *series << QPointF(x2, y2);
        y1 = y2;
        x1 = x2;
        i++;
    }
    listofseries.push_back(series);
}

////////////////////////
// setters and getters
std::vector<EntryPerso *> DynamicGraph::get_entries() const { return entries; }

////////////////////////
// display the graph
void DynamicGraph::display(QLayout *layout) const
{

    QChart *mood_chart = new QChart();
    for (int i = 0; i < listofseries.size(); i++)
    { // we add every curve to the graph: one curve per colored section on the
      // graph
        mood_chart->addSeries(listofseries[i]);
    }
    mood_chart->legend()->hide();

    mood_chart->createDefaultAxes();
    mood_chart->axes(Qt::Vertical).first()->setRange(0, 100);
    // mood_chart->axes(Qt::Vertical).first()->setTitleText(QString::fromStdString("Mood"));
    QFont font = QFont();
    font.setWeight(QFont::Thin);
    mood_chart->axes(Qt::Horizontal).first()->setTitleFont(font);
    mood_chart->axes(Qt::Horizontal)
        .first()
        ->setTitleText(QString::fromStdString("Days ago"));
    QValueAxis *xaxis = static_cast<QValueAxis *>(
        mood_chart->axes(Qt::Horizontal)
            .first()); // used to be able to call QValueAxis methods as we know
                       // that the axis is of type QValueAxis, which inherits
                       // from QAbstractAxis, returned by the function
    xaxis->setLabelFormat("%0.0f");

    // mood_chart->axes(Qt::Horizontal).first()->setLabelFormat("%.2f");
    // tests to put absolute value as an x axis.
    /*
    //QValueAxis *axisX = new QValueAxis;
    mood_chart->Xaxis.setLabelFormat("%.2f");
    */

    mood_chart->setAnimationOptions(QChart::GridAxisAnimations);
    QFont titlefont = QFont();
    titlefont.setWeight(QFont::Bold);
    mood_chart->setTitleFont(titlefont);
    mood_chart->setTitle("My mood");
    QChartView *mood_view = new QChartView(mood_chart);
    mood_view->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(
        mood_view); // displays the graph on the screen in the indicated layout
}
