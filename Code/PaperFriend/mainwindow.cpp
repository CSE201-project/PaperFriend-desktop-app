#include "mainwindow.h"
#include "all_activities.h"
#include "all_people.h"
#include "cardclasses.h"
#include "dynamicgraph.h"
#include "entryfilter.h"
#include "error_dlg.h"
#include "file_processing/file_processing/file_save_and_load.h"
#include "mascot.h"
#include "mascotchat.h"
#include "settings.h"
#include "texteditor.h"
#include "ui_all_activities.h"
#include "ui_all_habits.h"
#include "ui_mainwindow.h"
#include "ui_texteditor.h"
#include "add_habit.h"
#include <QDebug>
#include <QString>
#include <fstream>
#include <iostream>
#include <sstream>

//// Declarations
std::vector<Filter_param> filter_params;
std::vector<EntryPerso *> MainWindow::vector_entries;//All the entries
std::vector<Activity> MainWindow::vector_activities;//All the possible activities to choose from
std::vector<Friend> MainWindow::vector_friends;//All the friends we can choose from

//// Helper functions
bool sort_by_date(const EntryPerso *e1, const EntryPerso *e2){
    return e1->get_qdate().daysTo(e2->get_qdate()) > 0;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),reacted_to_entry(false) {

  //////// Layout
  ui->setupUi(this); // display canvas created in drag-and-drop

  // create layout for central scrollable area
  QVBoxLayout *entries_layout = new QVBoxLayout();
  ui->EntriesScroll->widget()->setLayout(entries_layout);

  // get the width of the scrollable area
  // We use Friend function to embed the TextEditor into the MainWindow
  textEditor = new TextEditor();
  textEditor->mainUi = this;
  // we want to set the textEditor with the same size as the place of embedding

  connect(ui->type_filter, SIGNAL(currentTextChanged(const QString &)), this,
          SLOT(MainWindow::on_type_filter_currentTextChanged(const QString &)));

  /////// Loading of parameters and data
  // create a folder for the entries if it doesn't already exist
  if (!QDir("Entries").exists()) {
    QDir().mkdir("Entries");
  }

  //load previously entered activities
  if (!std::filesystem::exists("activities.json")) {
    save_activities(vector_activities);
  }
  vector_activities = load_activities();
  qDebug()<<vector_activities.size();

  // load previous entries
  QDir dir(QDir::currentPath() + "/Entries");
  for (const QString &filename : dir.entryList(QDir::Files)) {
    vector_entries.push_back(
        load_entryperso(filename.toStdString(),vector_activities));
  }
  sort(vector_entries.begin(), vector_entries.end(), sort_by_date);
  qDebug()<<QString::fromStdString((vector_entries[0]->get_activities())[0]->get_name());

  //Load habits
  std::vector<QStringList> current_habits = load_habits();
  if (current_habits.size() == 1){
      ui->habits_label_1->setText(current_habits[0][0] + ", " + current_habits[0][1]);
  }
  if (current_habits.size() == 2){
      ui->habits_label_1->setText(current_habits[0][0] + ", " + current_habits[0][1]);
      ui->habits_label_2->setText(current_habits[1][0] + ", " + current_habits[1][1]);
  }
  if (current_habits.size() == 3){
      ui->habits_label_1->setText(current_habits[0][0] + ", " + current_habits[0][1]);
      ui->habits_label_2->setText(current_habits[1][0] + ", " + current_habits[1][1]);
      ui->habits_label_3->setText(current_habits[2][0] + ", " + current_habits[2][1]);
  }
  if (current_habits.size() == 4){
      ui->habits_label_1->setText(current_habits[0][0] + ", " + current_habits[0][1]);
      ui->habits_label_2->setText(current_habits[1][0] + ", " + current_habits[1][1]);
      ui->habits_label_3->setText(current_habits[2][0] + ", " + current_habits[2][1]);
      ui->habits_label_4->setText(current_habits[3][0] + ", " + current_habits[3][1]);
  }
  if (current_habits.size() == 5){
      ui->habits_label_1->setText(current_habits[0][0] + ", " + current_habits[0][1]);
      ui->habits_label_2->setText(current_habits[1][0] + ", " + current_habits[1][1]);
      ui->habits_label_3->setText(current_habits[2][0] + ", " + current_habits[2][1]);
      ui->habits_label_4->setText(current_habits[3][0] + ", " + current_habits[3][1]);
      ui->habits_label_5->setText(current_habits[4][0] + ", " + current_habits[4][1]);
  }
  if (current_habits.size() >= 6){
      ui->habits_label_1->setText(current_habits[0][0] + ", " + current_habits[0][1]);
      ui->habits_label_2->setText(current_habits[1][0] + ", " + current_habits[1][1]);
      ui->habits_label_3->setText(current_habits[2][0] + ", " + current_habits[2][1]);
      ui->habits_label_4->setText(current_habits[3][0] + ", " + current_habits[3][1]);
      ui->habits_label_5->setText(current_habits[4][0] + ", " + current_habits[4][1]);
      ui->habits_label_6->setText(current_habits[5][0] + ", " + current_habits[5][1]);
  }

  displayed_entries = vector_entries;

  // save the card corresponding to the current day in case it has to be modified
  if (!vector_entries.empty()) {
    EntryPerso *newest_entry = vector_entries.back();
    if (newest_entry->get_qdate() == QDate::currentDate()) {
      today_card = new EntryCard(20, 300, 300, "white", newest_entry, true, this);
    }
  }

  //Load previous recaps dates
  std::vector<QString> last_recaps_dates = load_last_recaps_dates();

  //// frontend that needs data to be rendered
  display_entries();




  // Chatbox
  chat = MascotChat(ui->scrollArea);
  welcome();
  chat.add_mascot();


  //Settings
  auto settings = findChild<QWidget *>("settings_frame");
  settings->hide(); // hide the settings menu on launch
  // setting the icon for the setting button
  QPixmap pix(":/pictures/rsc/checklist icon.png");
  int w = ui->settingsButton->width();
  int h = ui->settingsButton->height();
  ui->settingsButton->setIcon(QIcon(pix.scaled(w, h, Qt::KeepAspectRatio)));

  update_graphs();

}

MainWindow::~MainWindow() { delete ui; }

std::vector<Activity> MainWindow::get_activities() { return vector_activities; }

Activity *MainWindow::get_activity_at_i(long long unsigned i) {
  return &vector_activities.at(i);
}

std::vector<Friend> MainWindow::get_friends() { return vector_friends; }

Friend *MainWindow::get_friend_at_i(long long unsigned i) {
  return &vector_friends.at(i);
}

void MainWindow::update_activities(std::vector<Activity> activities) {
  vector_activities = activities;
}

void MainWindow::update_friends(std::vector<Friend> friends) {
  vector_friends = friends;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  QMessageBox::StandardButton answr_btn = QMessageBox::question(
      this, tr("Paper friend"), tr("Are you sure?\n"),
      QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
      QMessageBox::Yes);
  if (answr_btn != QMessageBox::Yes) {
    event->ignore();
  } else {
    event->accept();
  }
}

void MainWindow::toggle_visibility(QWidget *component) {
  if (component->isVisible()) {
    component->hide();
  } else {
    component->show();
  }
}

void MainWindow::update_graphs() {
    ui->tabWidget->clear();
    if (saved_mood()) {
        display_graph("mood");
    }
    if (saved_sleep()) {
        display_graph("sleep");
    }
    if (saved_eating_healthy()) {
        display_graph("eating healthy");
    }
    if (saved_productivity()) {
        display_graph("productivity");
    }
    if (saved_communications()) {
        display_graph("communications");
    }
    if (saved_screen_time()) {
        display_graph("screen time");
    }
}

void MainWindow::remove_non_existent_friends(EntryPerso* entry){
    std::vector<Friend*> friends;
    for(long long unsigned fr = 0; fr < entry->get_friends().size(); fr++){
        for(long long unsigned i = 0; i < vector_friends.size(); i++){
            if((entry->get_friends().at(fr))->equal(vector_friends.at(i))){
                friends.push_back(&vector_friends.at(i));
                break;
            }
        }
    }
    entry->set_friends(friends);
}
void MainWindow::display_entries() {


  while (!ui->EntriesScroll->widget()->layout()->isEmpty()) {
    QLayoutItem *item = ui->EntriesScroll->widget()->layout()->takeAt(0);
    //ui->graph_frame->removeItem(item);
    delete item->widget();
    delete item;
    qDebug() << "removed";
  }
  /*for (auto entry: entries) {
      EntryCard *c = new EntryCard(20, 300, 300, "white", entry, true, this);
      c->display(ui->EntriesScroll->widget()->layout()); //displays the entry in
  the main_frame. qDebug()<< "displayed";
  }*/

  // displaying in reversed order
  for (auto entry = displayed_entries.rbegin(); entry != displayed_entries.rend(); ++entry) {
    //remove friends and activities that shouldn't be displayed
      remove_non_existent_friends(*entry);
    if ((*entry)->get_qdate() == QDate::currentDate()) {
      today_card = new EntryCard(20, 300, 300, "white", *entry, true, this);
      today_card->display(ui->EntriesScroll->widget()->layout());
    } else {
      EntryCard *c = new EntryCard(20, 300, 300, "white", *entry, true, this);
      c->display(ui->EntriesScroll->widget()->layout()); // displays the entry in the main_frame.
      qDebug() << "displayed";
    }
  }
}

void MainWindow::display_graph(QString tracked_parameter) {
    QWidget *tab = new QWidget();
    QHBoxLayout *graph = new QHBoxLayout(tab);
    ui->tabWidget->addTab(tab, tracked_parameter);
    DynamicGraph dynamicGraph = DynamicGraph(displayed_entries, tracked_parameter);
    dynamicGraph.display(graph);
    this->showMaximized();
}

void MainWindow::on_pushButton_clicked() {
  all_habits = new All_Habits(this);
  all_habits->setModal(true);
  all_habits->exec();
}

void MainWindow::on_activitie_button_clicked() {
  all_activities *my_activities = new all_activities(this,vector_activities);
  //my_activities->add_previous_cells();
  my_activities->setModal(true);
  my_activities->exec();
}

void MainWindow::on_settingsButton_clicked() {
  findChild<QCheckBox *>("mood")->setChecked(saved_mood());
  findChild<QCheckBox *>("sleep")->setChecked(saved_sleep());
  findChild<QCheckBox *>("eating_healthy")->setChecked(saved_eating_healthy());
  findChild<QCheckBox *>("productivity")->setChecked(saved_productivity());
  findChild<QCheckBox *>("communications")->setChecked(saved_communications());
  findChild<QCheckBox *>("screen_time")->setChecked(saved_screen_time());
  auto settings = findChild<QWidget *>("settings_frame");
  toggle_visibility(settings);
  auto chat = findChild<QWidget *>("scrollArea");
  toggle_visibility(chat);
}

void MainWindow::on_save_settings_clicked() {
  std::ofstream myfile;
  myfile.open("settings.txt");
  myfile << findChild<QCheckBox *>("mood")->isChecked() << "\n";
  myfile << findChild<QCheckBox *>("sleep")->isChecked() << "\n";
  myfile << findChild<QCheckBox *>("eating_healthy")->isChecked() << "\n";
  myfile << findChild<QCheckBox *>("productivity")->isChecked() << "\n";
  myfile << findChild<QCheckBox *>("communications")->isChecked() << "\n";
  myfile << findChild<QCheckBox *>("screen_time")->isChecked() << "\n";
  myfile.close();
  update_graphs();
  auto settings = findChild<QWidget *>("settings_frame");
  settings->hide();
  auto chat = findChild<QWidget *>("scrollArea");
  chat->show();
}

void MainWindow::on_filterButton_clicked() {
  std::cout << "sleep";
  std::cout << vector_entries[0]->get_sleep() << std::endl;

  // vector_entries = sample_entries(100); // this line should be changed to
  // aquire source of entries

  QString type_filter_value =
      findChild<QComboBox *>("type_filter")->currentText();
  std::string type_filter_str = type_filter_value.toStdString();
  QString operator_filter_value =
      findChild<QComboBox *>("operation_filter")->currentText();
  std::string operator_filter_str = operator_filter_value.toStdString();
  //QString value_filter_value =
      //findChild<QDoubleSpinBox *>("value_filter")->text();
  double value = findChild<QDoubleSpinBox *>("value_filter")->value();
  int n = 20;
  

  // construct a filter_param object
  struct Filter_param filt;
  filt.is_value_compare = true;
  filt.keyword = type_filter_str;
  filt.opt = operator_filter_str;
  filt.value = value;
  filt.display_num = n;

  
  // handling duplicated filters
  bool is_insert = true;
  for (int i = 0; i < filter_params.size(); i++) {
    if (filter_params[i].keyword == filt.keyword &&
        filter_params[i].opt == filt.opt &&
        filter_params[i].value == filt.value) {
      is_insert = false;
      break;
    }
    if (filter_params[i].keyword == filt.keyword && filt.opt == "=") {
      filter_params.erase(filter_params.begin() + i);
      i--;
    } else if (filter_params[i].keyword == filt.keyword &&
               filter_params[i].opt == "=") {
      filter_params.erase(filter_params.begin() + i);
      i--;
    }
    if (filter_params[i].keyword == filt.keyword &&
        filter_params[i].opt == filt.opt) {
      filter_params.erase(filter_params.begin() + i);
      i--;
    }
  }

  if (is_insert) {
    filter_params.push_back(filt);
  }

  for(int i=0; i<filter_params.size(); i++){
    if(filter_params[i].keyword == "last_n_entries"){
      n = filter_params[i].value;
      break;
    }
  }


  // filter the entries
  std::vector<EntryPerso *> filtered_entries  = vector_entries;
  for (int i = 0; i < filter_params.size(); i++) {
    if (filter_params[i].keyword == "last_n_entries") {
      continue;
    }
    filtered_entries = filter(filtered_entries, filter_params[i]);
  }

  // error handling
  std::cout << "filtered" << std::endl;
  if (filtered_entries.size() == 0) {
    std::cout << "no entries" << std::endl;
    if (is_insert) {
      filter_params.pop_back();
    }
    // To implement a error dialog here. we have a filter_error.ui file. show
    // the dialog
    QMessageBox::warning(this, "Error",
                         "No entries found with the given filter");
    return;
  }

  // update and display the filters
  std::string f = "Filters:   ";
  for (int i = 0; i < filter_params.size(); i++) {
    // value keeps 2 digits after the decimal point
    std::stringstream stream;
    //qDebug()<<filter_params[i].value<<"HERE";
    stream << std::fixed << std::setprecision(1) << filter_params[i].value;
    std::string s = stream.str();
    f +=
        filter_params[i].keyword + " " + filter_params[i].opt + " " + s + ";  ";
  }
  findChild<QLabel *>("existing_filters")->setText(QString::fromStdString(f));

  if (filtered_entries.size() < n) {
    n = filtered_entries.size();
  }

  // select the n last entries
  std::vector<EntryPerso *> entries_to_display;
  for (int i = filtered_entries.size() - n; i < filtered_entries.size(); i++) {
    entries_to_display.push_back(filtered_entries[i]);
  }

  displayed_entries = entries_to_display;
  display_entries();
  update_graphs();
}

void MainWindow::on_helpFilterBox_clicked() {
  // implement a help dialog here. show the dialog.
  QMessageBox::information(this, "Help", "This is a help dialog");
}

void MainWindow::on_clear_button_clicked() {
  filter_params.clear();
  findChild<QLabel *>("existing_filters")->setText("Filters: ");
  // vector_entries = sample_entries(10); // this line should be changed to
  // aquire source of entries
  displayed_entries = vector_entries;
  display_entries();
  update_graphs();
}

// if type_filter is updated
void MainWindow::on_type_filter_currentTextChanged(const QString &arg1) {
  // update the operator_filter
  std::string type_filter_str = arg1.toStdString();
  
  if (type_filter_str == "last_n_entries") {
    //change the operator_filter to "="
    findChild<QComboBox *>("operation_filter")->clear();
    findChild<QComboBox *>("operation_filter")->addItem("=");
    findChild<QComboBox *>("operation_filter")->setEnabled(false);
  }else{
    findChild<QComboBox *>("operation_filter")->clear();
    findChild<QComboBox *>("operation_filter")->addItem("=");
    findChild<QComboBox *>("operation_filter")->addItem(">");
    findChild<QComboBox *>("operation_filter")->addItem("<");
    findChild<QComboBox *>("operation_filter")->setEnabled(true);
  }
}

void MainWindow::on_newEntryButton_clicked() {
  // more compact code for when the test entries are replaced with real entries
  /*
  if
  (!std::filesystem::exists("Entries/"+QDate::currentDate().toString("MM.dd.yyyy").toStdString()+".json"))
  { EntryPerso *today_entry = new EntryPerso(); save_entryperso(*today_entry);
      vector_entries.push_back(today_entry);
      displayed_entries.push_back(today_entry);
      today_card = new EntryCard(20, 300, 300, "white", today_entry, true,
  this); display_entries();
  }
  */
  if (vector_entries.empty()) {
    EntryPerso *today_entry = new EntryPerso();
    //ADD ACTIVITIES BY DEF TO 0
    qDebug()<<QString("Start to add activities by default to 0");
    for(Activity const& activity : vector_activities){
        qDebug()<<QString::fromStdString(activity.get_name());
        Activity *to_add = new Activity(activity.get_name(),activity.get_type(),0);
        today_entry->add_activity(to_add);
    }
    save_entryperso(*today_entry);
    vector_entries.push_back(today_entry);
    displayed_entries.push_back(today_entry);
    today_card = new EntryCard(20, 300, 300, "white", today_entry, true, this);
    display_entries();
  } else {
    if (vector_entries.back()->get_qdate() != QDate::currentDate()) {
      EntryPerso *today_entry = new EntryPerso();
      save_entryperso(*today_entry);
      vector_entries.push_back(today_entry);
      displayed_entries.push_back(today_entry);
      today_card = new EntryCard(20, 300, 300, "white", today_entry, true, this);
      display_entries();
    }
  }
  ui->EntriesScroll->verticalScrollBar()->setValue(0);
  today_card->change();
}

void MainWindow::generate_recap() {
    // first check if we need to generate a weekly/monthly/yearly recap
    // last_recaps_dates is the vector containing the string of the dates of the last [0]weekly, [1]monthly and [2]yearly recap.
    std::vector<QString> last_recaps_dates = load_last_recaps_dates();
    //weekly
    if(QDate::currentDate().dayOfWeek()==7)//If it's Sunday
    {
       QString date_last_recap = last_recaps_dates[0];
        if(date_last_recap!=QDate::currentDate().toString("yyyy.MM.dd"))
        {
            chat<<QString("It's Sunday! Time for a weekly recap ????");
            chat.add_mascot();
            last_recaps_dates[0] = QDate::currentDate().toString("yyyy.MM.dd");
            save_last_recaps_dates(last_recaps_dates);
        }


    }
    //monthly
    if(QDate::currentDate().daysInMonth()==QDate::currentDate().day())//If it's the last day of the month
    {
        QString date_last_recap = last_recaps_dates[1];
        if(date_last_recap!=QDate::currentDate().toString("yyyy.MM.dd"))
        {
        chat<<QString("Last day of the month means time for a montly recap!");
        chat.add_mascot();
        last_recaps_dates[1] = QDate::currentDate().toString("yyyy.MM.dd");
        save_last_recaps_dates(last_recaps_dates);
        }
    }
    //yearly
    if(QDate::currentDate().month()==12 && QDate::currentDate().day()==31)//If it's December 31st
    {
        QString date_last_recap = last_recaps_dates[2];
        if(date_last_recap!=QDate::currentDate().toString("yyyy.MM.dd"))
        {
        chat<<QString("Before celebrating the new year, let's look back to ")+QString::number(QDate::currentDate().year())+QString(" and ponder.");
        chat.add_mascot();
        last_recaps_dates[2] = QDate::currentDate().toString("yyyy.MM.dd");
        save_last_recaps_dates(last_recaps_dates);
        }
    }
}

void MainWindow::react_to_last_entry(){
    if(!reacted_to_entry){
        EntryPerso* last_entry = vector_entries[vector_entries.size()-1];//called before generate recap so we are sure it is an EntryPerso
        if(last_entry->get_mood()==0){
            chat<<QString("Did you forget to put in your mood ?<br> If not, I'm very sorry for the day you had. It's good that you put your thoughts on paper.<br> Don't hesitate to seek the help of a relative or of a professional if you feel like you loose control. Don't worry, everything eventually gets better.");
        }else if(last_entry->get_mood()<0.3){
            chat<<QString("Oh, I'm sorry for the day you had. Don't forget that you are never alone and talking to a relative or a professional can help you going through hard times.");
        }else if(last_entry->get_mood()<0.5){
            chat<<QString("Seems like you spent a pretty tough day... I hope it'll be better tomorrow.");
        }else if(last_entry->get_mood()>0.85){
            chat<<QString("What a great day! Take the time to savor it.");
        }else if(last_entry->get_mood()>0.95){
            chat<<QString("Wow, you spent an amazing day! It hope it will stay anchored in your memory forever.");
        }
        chat.add_mascot();
        reacted_to_entry = true;
    }
}

void MainWindow::welcome(){
    if(!vector_entries.empty()){
        EntryPerso* last_entry = vector_entries[vector_entries.size()-1];
        int daysago = (last_entry->get_qdate()).daysTo(QDate::currentDate());
        if(daysago==0){
            chat<<QString("Hello again!");
        }else if(daysago==1){
            chat<<QString("Hello!");
        }else if(daysago>365){
            chat<<QString("Oh, you! I though I would never see you again! How are you?");
        }else if(daysago>14){
        chat<<QString("It's been a while! Good to see you again.");
        }else if(daysago>6){
            chat<<QString("Welcome back! How has it been going?");
        }else if(daysago>1){
            chat<<QString("Hi! How did it go since last time?");
        }
    }else{
        chat<<QString("Hello, it seems like it's your first time here! I'm Rooxie, your well-being assistant! You can create an entry in you diary by clicking the \"New entry\" button on the top of the screen.");
    }
}


void MainWindow::on_Test_entries_clicked() {
  vector_entries = sample_entries(100);
  displayed_entries = vector_entries;
  display_entries();
  update_graphs();
}

void MainWindow::on_ppl_button_clicked(){
    all_people *my_people = new all_people(vector_friends);
    my_people->setModal(true);
    my_people->exec();
}

void MainWindow::add_new_activities_to_old_enties(){
    qDebug()<<QString("called");

    for(EntryPerso *entry : vector_entries){//to modify if we change the type of vector_entries
        if(entry->entry_type()==1){//useful when we change the type of vector_entries
            //we now know it's an entry perso so we can cast it
            //qDebug()<<QString("cast");
            //EntryPerso *entry = static_cast<EntryPerso*>(entry); //would be the way, but makes the program crash at the line indicated in the following comment
            for(Activity const&activity : vector_activities){
                Activity *to_add = new Activity();
                to_add->set_name(activity.get_name());
                to_add->set_type(activity.get_type());
                std::vector<Activity*> entry_activities = entry->get_activities();//crashes here if static cast used
                if(std::find(entry_activities.begin(), entry_activities.end(), to_add) == entry_activities.end()) {
                    //does not contain activity
                    Activity *to_add = new Activity();
                    to_add->set_name(activity.get_name());
                    to_add->set_type(activity.get_type());
                    to_add->set_value(0.0);
                    entry->add_activity(to_add);
                }
            }
        }
    }


}

