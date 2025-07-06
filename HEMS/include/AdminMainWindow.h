#ifndef ADMINMAINWINDOW_H
#define ADMINMAINWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

class AdminMainWindow : public Fl_Window {
private:
    Fl_Button* viewUsersBtn;
    Fl_Button* systemInfoBtn;
    Fl_Button* logoutBtn;


    static void on_button_click(Fl_Widget* w, void* data);
public:
    AdminMainWindow();
};

#endif // ADMINMAINWINDOW_H
