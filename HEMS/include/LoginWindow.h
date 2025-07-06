#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <string>
#include <vector>

struct User {
    std::string username;
    std::string password;
    std::string role;
};

class LoginWindow : public Fl_Window {
private:
    Fl_Input* usernameInput;
    Fl_Input* passwordInput;
    Fl_Choice* roleChoice;
    Fl_Button* loginButton;
    Fl_Box* messageBox;

    std::vector<User> users;

    static void login_cb(Fl_Widget* w, void* data);
    void checkLogin();

public:
    LoginWindow(int w, int h, const char* title);
};

#endif
