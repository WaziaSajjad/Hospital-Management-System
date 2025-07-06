#include "LoginWindow.h"
#include "DoctorMainWindow.h"
#include "NurseMainWindow.h"
#include "AdminMainWindow.h"
#include "queue.h"

#include <FL/fl_config.h>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl.H>
#include <FL/fl_ask.H>

LoginWindow::LoginWindow(int w, int h, const char* title) : Fl_Window(w, h, title) {
    // Background image covering the entire window
    Fl_PNG_Image* bgImg = new Fl_PNG_Image("images/backgroundimg.png");
    if (bgImg->fail()) {
        fl_alert("Background image failed to load!");
    } else {
        Fl_Image* scaledBg = bgImg->copy(w, h); // Resize to window size
        Fl_Box* bgBox = new Fl_Box(0, 0, w, h);
        bgBox->image(scaledBg);
    }

    // Hardcoded users
    users.push_back({"doctor1", "pass123", "Doctor"});
    users.push_back({"nurse1", "nursepass", "Nurse"});
    users.push_back({"admin", "adminpass", "Admin"});

    // Positioning
    int x = 80, y = 180, input_w = 200, input_h = 30, gap = 50;

    // Username
    new Fl_Box(x, y, 100, 25, "Username:");
    usernameInput = new Fl_Input(x + 100, y, input_w, input_h);

    // Password
    y += gap;
    new Fl_Box(x, y, 100, 25, "Password:");
    passwordInput = new Fl_Input(x + 100, y, input_w, input_h);
    passwordInput->type(FL_SECRET_INPUT);

    // Role
    y += gap;
    new Fl_Box(x, y, 100, 25, "Role:");
    roleChoice = new Fl_Choice(x + 100, y, input_w, input_h);
    roleChoice->add("Doctor");
    roleChoice->add("Nurse");
    roleChoice->add("Admin");
    roleChoice->value(0);

    // Login button
    y += gap + 10;
    loginButton = new Fl_Button(x + 150, y, 100, 30, "Login");
    loginButton->callback(login_cb, this);

    // Message
    y += gap;
    messageBox = new Fl_Box(x, y, input_w + 110, 25, "");

    // Widget styling
    usernameInput->color(FL_WHITE);
    passwordInput->color(FL_WHITE);
    loginButton->color(FL_DARK_BLUE);
    loginButton->labelcolor(FL_WHITE);

    color(FL_WHITE);  // fallback if background fails
    end();
    set_non_modal();
    show();
}

void LoginWindow::login_cb(Fl_Widget* w, void* data) {
    LoginWindow* win = static_cast<LoginWindow*>(data);
    win->checkLogin();
}

void LoginWindow::checkLogin() {
    std::string username = usernameInput->value();
    std::string password = passwordInput->value();
    int role_idx = roleChoice->value();
    const char* roles[] = {"Doctor", "Nurse", "Admin"};
    std::string role = roles[role_idx];

    for (const User& u : users) {
        if (u.username == username && u.password == password && u.role == role) {
            messageBox->labelcolor(FL_DARK_GREEN);
            messageBox->copy_label("Login successful!");
            hide();

            if (role == "Doctor")
                new DoctorMainWindow();
            else if (role == "Nurse")
                new NurseMainWindow();
            else if (role == "Admin")
                new AdminMainWindow();
            return;
        }
    }

    messageBox->labelcolor(FL_RED);
    messageBox->copy_label("Invalid credentials or role!");
}
