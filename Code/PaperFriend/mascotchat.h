#ifndef MASCOTCHAT_H
#define MASCOTCHAT_H
#include "qboxlayout.h"
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <string>
#include <vector>
#include <mascot.h>

class MascotChat
{
public:
    MascotChat();
    MascotChat(QScrollArea *area);
    void operator<<(std::string txt);
    void operator<<(QString txt);
    void add_mascot();
    void display(std::vector<QString> Qstr_vect);
    //getters
    QString get_last_message() const;
    QScrollArea* get_scrollArea();

private:
    void set_scroll_area(QScrollArea *area);
    void add_message(QString txt);
    void add_message(std::string txt);
    void prompt_msg();
    QScrollArea *scrollArea;
    QVBoxLayout *chat_layout;
    Mascot *mascot;
};


class Message : public QLabel
{
public:
    Message();
    virtual void paintEvent( QPaintEvent* aEvent ) override;
};


#endif // MASCOTCHAT_H
