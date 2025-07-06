#include "AdminMainWindow.h"
#include "LoginWindow.h"
#include <FL/fl_ask.H>
#include <cstring>
#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_PNG_Image.H>


// Local class for displaying system information
class SystemInfoWindow : public Fl_Window {
public:
    SystemInfoWindow() : Fl_Window(600, 400, "System Info") {
        Fl_Multiline_Output* output = new Fl_Multiline_Output(10, 10, 580, 380);
        output->value(
            "{\n"
            "  \"configurations\": [\n"
            "    {\n"
            "      \"name\": \"Win32\",\n"
            "      \"includePath\": [\n"
            "        \"${workspaceFolder}/include\",\n"
            "        \"${workspaceFolder}/**\",\n"
            "        \"C:/dev/fltk\",\n"
            "        \"C:/dev/fltk/build/FL\",\n"
            "        \"C:/dev/fltk/build\",\n"
            "        \"C:/dev\"\n"
            "      ],\n"
            "      \"browse\": {\n"
            "        \"path\": [\n"
            "          \"${workspaceFolder}/include\",\n"
            "          \"C:/dev/fltk\",\n"
            "          \"C:/dev/fltk/build/FL\"\n"
            "        ],\n"
            "        \"limitSymbolsToIncludedHeaders\": true,\n"
            "        \"databaseFilename\": \"\"\n"
            "      },\n"
            "      \"defines\": [],\n"
            "      \"compilerPath\": \"C:/MinGW/bin/g++.exe\",\n"
            "      \"cStandard\": \"c11\",\n"
            "      \"cppStandard\": \"c++17\",\n"
            "      \"intelliSenseMode\": \"windows-gcc-x64\"\n"
            "    }\n"
            "  ],\n"
            "  \"version\": 4\n"
            "}"
        );
        output->textfont(FL_COURIER); // Optional: make it look like code
        output->textsize(12);
        end();
        show();
    }
};

AdminMainWindow::AdminMainWindow() : Fl_Window(550, 300, "Admin Dashboard") {
    int start_x = 70;
    int start_y = 80;
    int btn_w = 180;
    int btn_h = 40;
    int gap_x = 30;
    int gap_y = 30;

    // Background Image
    Fl_PNG_Image* bgImg = new Fl_PNG_Image("images/admin.png");
    if (!bgImg->fail()) {
        Fl_Image* scaledBg = bgImg->copy(w(), h());
        Fl_Box* bgBox = new Fl_Box(0, 0, w(), h());
        bgBox->image(scaledBg);
        bgBox->box(FL_NO_BOX);
    } else {
        fl_alert("Failed to load background image.");
    }


    viewUsersBtn = new Fl_Button(start_x, start_y, btn_w, btn_h, "View Users");
    systemInfoBtn = new Fl_Button(start_x + btn_w + gap_x, start_y, btn_w, btn_h, "System Info");
    logoutBtn = new Fl_Button(start_x, start_y + btn_h + gap_y, btn_w+btn_w+10, btn_h, "Logout");

    viewUsersBtn->callback(on_button_click, (void*)"View Users");
    systemInfoBtn->callback(on_button_click, (void*)"System Info");
    logoutBtn->callback(on_button_click, (void*)"Logout");

    viewUsersBtn->color(FL_DARK_BLUE);
    systemInfoBtn->color(FL_DARK_BLUE);
    logoutBtn->color(FL_DARK_BLUE);

    viewUsersBtn->labelcolor(FL_WHITE);
    systemInfoBtn->labelcolor(FL_WHITE);
    logoutBtn->labelcolor(FL_WHITE);

    end();
    show();
}
//view user

// Local class for displaying hardcoded users
class UserManagementWindow : public Fl_Window {
public:
    UserManagementWindow() : Fl_Window(400, 200, "User Management") {
        int row_h = 30;

        // Headers
        new Fl_Box(20, 10, 100, row_h, "Username");
        new Fl_Box(130, 10, 100, row_h, "Password");
        new Fl_Box(240, 10, 100, row_h, "Role");

        // User 1
        new Fl_Box(20, 50, 100, row_h, "doctor1");
        new Fl_Box(130, 50, 100, row_h, "pass123");
        new Fl_Box(240, 50, 100, row_h, "Doctor");

        // User 2
        new Fl_Box(20, 90, 100, row_h, "nurse1");
        new Fl_Box(130, 90, 100, row_h, "nursepass");
        new Fl_Box(240, 90, 100, row_h, "Nurse");

        // User 3
        new Fl_Box(20, 130, 100, row_h, "admin");
        new Fl_Box(130, 130, 100, row_h, "adminpass");
        new Fl_Box(240, 130, 100, row_h, "Admin");

        end();
        show();
    }
};

void AdminMainWindow::on_button_click(Fl_Widget* w, void* data) {
    const char* action = static_cast<const char*>(data);

    if (std::strcmp(action, "Logout") == 0) {
        LoginWindow* login = new LoginWindow(1000, 500, "Hospital Emergency Management System - Login"); 
        login->show();
        Fl_Window* win = (Fl_Window*)w->window();
        win->hide();
    }
    else if (std::strcmp(action, "View Users") == 0) {
        new UserManagementWindow();
    }
    else if (std::strcmp(action, "System Info") == 0) {
        new SystemInfoWindow();
    }
    else {
        fl_message("Unknown action: %s", action);
    }
}
