#include "NurseMainWindow.h"
#include "LoginWindow.h"
#include "Patient.h"
#include "Queue.h"
#include <FL/fl_ask.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Multiline_Output.H>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <FL/Fl_Button.H>
#include <FL/fl_message.H>
#include <functional>
#include <string>
#include <FL/Fl.H>



// Static vector holding patient data
static PriorityQueue patientQueue;

// static std::vector<Patient> patientQueue;
static std::vector<std::string> vitalsData;
static std::vector<std::string> appointmentData;
static std::vector<std::string> messageData;

class AddPatientForm : public Fl_Window {
private:
    Fl_Input* nameInput;
    Fl_Input* ageInput;
    Fl_Input* conditionInput;
    Fl_Input* priorityInput;
    Fl_Button* submitBtn;
    PriorityQueue& queue;  // Reference to the main queue

    static void on_submit(Fl_Widget*, void* userdata) {
        AddPatientForm* form = static_cast<AddPatientForm*>(userdata);
        std::string name = form->nameInput->value();
        int age = std::stoi(form->ageInput->value());
        std::string condition = form->conditionInput->value();
        int priority = std::stoi(form->priorityInput->value());

        if (priority < 1 || priority > 3) {
            fl_alert("Priority must be between 1 (high) and 3 (low).");
            return;
        }

        addPatient(form->queue, name, age, condition, priority);
        saveQueueToFile(form->queue, "files/patients.txt");
        fl_message("Patient added successfully!");
        form->hide();
        
    }

public:
    AddPatientForm(PriorityQueue& q) : Fl_Window(400, 350, "Add Patient"), queue(q) {
        nameInput = new Fl_Input(150, 50, 160, 25, "Name:");
        ageInput = new Fl_Input(150, 95, 160, 25, "Age:");
        conditionInput = new Fl_Input(150, 135, 160, 25, "Condition:");
        priorityInput = new Fl_Input(150, 180, 160, 25, "Priority (1-3):");
        submitBtn = new Fl_Button(160, 250, 100, 30, "Submit");
        submitBtn->callback(on_submit, this);
        submitBtn->color(FL_DARK_BLUE);
        submitBtn->labelcolor(FL_WHITE);
        end();
    }
};

// --------------
// Inside NurseMainWindow
void NurseMainWindow::show_add_patient_form() {
    AddPatientForm* form = new AddPatientForm(patientQueue);
    form->show();
    form->color(fl_rgb_color(245, 245, 245)); 
}

// ---------------
// View Patients Window
#include <FL/Fl_Multiline_Output.H>
#include <sstream>
#include "Queue.h"  // for Node, PriorityQueue, Patient

class ViewPatientsWindow : public Fl_Window {
private:
    Fl_Multiline_Output* display;

public:
    ViewPatientsWindow(const PriorityQueue& queue) : Fl_Window(400, 300, "Patient Queue") {
        display = new Fl_Multiline_Output(20, 20, 360, 260);

        std::ostringstream oss;
        Node* current = queue.front;
        while (current != nullptr) {
            const Patient& p = current->data;
            oss << p.name << ", "
                << p.age << ", "
                << p.condition << ", Priority: ";

            switch (p.priority) {
                case 1: oss << "Critical"; break;
                case 2: oss << "Moderate"; break;
                case 3: oss << "Mild"; break;
                default: oss << "Unknown"; break;
            }
            oss << "\n";
            current = current->next;
        }

        display->value(oss.str().c_str());
        end();
    }
};

// --------------
void NurseMainWindow::show_patient_list() {
    ViewPatientsWindow* win = new ViewPatientsWindow(patientQueue);
    win->color(fl_rgb_color(245, 245, 245)); 
    win->show();
}
// ---------------

// Record Vitals Window
class RecordVitalsWindow : public Fl_Window {
private:
    Fl_Input* patientNameInput;
    Fl_Input* vitalsInput;
    Fl_Input* bodyTempInput;
    Fl_Input* bloodPressInput;
    Fl_Button* submitBtn;

    std::vector<std::string>& vitalsData;
    std::function<void()> saveCallback;

    static void on_submit(Fl_Widget*, void* userdata) {
        RecordVitalsWindow* form = static_cast<RecordVitalsWindow*>(userdata);
        std::string name = form->patientNameInput->value();
        std::string vitals = form->vitalsInput->value();
        std::string bt = form->bodyTempInput->value();
        std::string bp = form->bloodPressInput->value();

        form->vitalsData.push_back(name + ": " + vitals + ": " + bt + ": " + bp);
        if (form->saveCallback) {
            form->saveCallback();
        }
        fl_message("Vitals recorded successfully!");
        form->hide();
    }

public:
    RecordVitalsWindow(std::vector<std::string>& vitalsRef, std::function<void()> onSave)
        : Fl_Window(350, 300, "Record Vitals"), vitalsData(vitalsRef), saveCallback(onSave) {

        patientNameInput = new Fl_Input(150, 50, 160, 25, "Patient Name:");
        vitalsInput = new Fl_Input(150, 90, 160, 25, "Vitals:");
        bodyTempInput = new Fl_Input(150, 130, 160, 25, "Body Temperature:");
        bloodPressInput = new Fl_Input(150, 170, 160, 25, "Blood Pressure:");
        submitBtn = new Fl_Button(100, 220, 100, 30, "Submit");
        submitBtn->callback(on_submit, this);
        submitBtn->color(FL_DARK_BLUE);
        submitBtn->labelcolor(FL_WHITE);

        end();
    }
};
// --------------
void NurseMainWindow::openRecordVitalsWindow() {
    RecordVitalsWindow* win = new RecordVitalsWindow(
        vitalsData,
        []() { NurseMainWindow::saveVitalsToFile(); }
    );
    win->color(fl_rgb_color(245, 245, 245));
    win->show();
}
// ---------------


// View Messages Window


class ViewMessagesWindow : public Fl_Window {
private:
    Fl_Multiline_Output* display;

public:
    ViewMessagesWindow(const std::vector<std::string>& messages)
        : Fl_Window(600, 300, "Messages") {

        display = new Fl_Multiline_Output(20, 20, 560, 260);  // width adjusted
        std::ostringstream oss;

        for (const auto& msg : messages) {
            oss << msg << "\n";
        }

        display->value(oss.str().c_str());
        end();
    }
};
// --------------
void NurseMainWindow::openMessagesWindow() {
    auto* win = new ViewMessagesWindow(messageData);  // assuming messageData is accessible here
    win->show();
    win->color(fl_rgb_color(245, 245, 245));
}
// ---------------

NurseMainWindow::NurseMainWindow() : Fl_Window(750, 400, "Nurse Dashboard") {
    begin();

    // Background Image
    Fl_PNG_Image* bgImg = new Fl_PNG_Image("images/nurse.png");
    if (!bgImg->fail()) {
        Fl_Image* scaledBg = bgImg->copy(w(), h());
        Fl_Box* bgBox = new Fl_Box(0, 0, w(), h());
        bgBox->image(scaledBg);
        bgBox->box(FL_NO_BOX);
    } else {
        fl_alert("Failed to load background image.");
    }

    // Buttons layout
    int btn_w = 250, btn_h = 55;
    int gap_x = 50, gap_y = 15;
    int start_x = 100, start_y = 50;

    viewPatientsBtn = new Fl_Button(start_x, start_y, btn_w, btn_h, "View Patients");
    recordVitalsBtn = new Fl_Button(start_x + btn_w + gap_x, start_y, btn_w, btn_h, "Record Vitals");
    assistDoctorBtn = new Fl_Button(start_x, start_y + btn_h + gap_y, btn_w, btn_h, "Assist Doctor");
    manageAppointmentsBtn = new Fl_Button(start_x + btn_w + gap_x, start_y + btn_h + gap_y, btn_w, btn_h, "Manage Appointments");
    addPatientBtn = new Fl_Button(start_x, start_y + 2 * (btn_h + gap_y), btn_w, btn_h, "Add Patient");
    logoutBtn = new Fl_Button(start_x + btn_w + gap_x, start_y + 2 * (btn_h + gap_y), btn_w, btn_h, "Logout");

    // Button callbacks
    viewPatientsBtn->callback(on_button_click, this);
    addPatientBtn->callback(on_button_click, this);
    recordVitalsBtn->callback(on_button_click, this);
    manageAppointmentsBtn->callback(on_button_click, this);
    assistDoctorBtn->callback(on_button_click, this);
    logoutBtn->callback(on_button_click, this);

    viewPatientsBtn->color(FL_DARK_BLUE);
    addPatientBtn->color(FL_DARK_BLUE);
    recordVitalsBtn->color(FL_DARK_BLUE);
    manageAppointmentsBtn->color(FL_DARK_BLUE);
    assistDoctorBtn->color(FL_DARK_BLUE);
    logoutBtn->color(FL_DARK_BLUE);

    viewPatientsBtn->labelcolor(FL_WHITE);
    addPatientBtn->labelcolor(FL_WHITE);
    recordVitalsBtn->labelcolor(FL_WHITE);
    manageAppointmentsBtn->labelcolor(FL_WHITE);
    assistDoctorBtn->labelcolor(FL_WHITE);
    logoutBtn->labelcolor(FL_WHITE);

    end();

    // Load persistent data
    loadQueueFromFile(patientQueue, "files/patients.txt");  // ✅ FIXED
    // loadVitalsFromFile();
    loadAppointmentsFromFile();
    loadMessagesFromFile();

    show();
}


//Assist Doctor
class AssistDoctorWindow : public Fl_Window {
private:
    Fl_Input* messageInput;
    Fl_Button* sendBtn;
    std::vector<std::string>& messageDataRef;

    static void on_send(Fl_Widget*, void* userdata) {
        AssistDoctorWindow* assist = static_cast<AssistDoctorWindow*>(userdata);
        std::string msg = assist->messageInput->value();

        if (!msg.empty()) {
            assist->messageDataRef.push_back("Nurse: " + msg);
            NurseMainWindow::saveMessagesToFile();  // Assumes static file save logic remains
            fl_message("Message sent to Doctor.");
            assist->hide();
        }
    }

public:
    AssistDoctorWindow(std::vector<std::string>& messages)
        : Fl_Window(400, 200, "Assist Doctor"), messageDataRef(messages) {

        messageInput = new Fl_Input(80, 40, 260, 25, "Message:");
        sendBtn = new Fl_Button(140, 100, 100, 30, "Send");
        sendBtn->callback(on_send, this);
        sendBtn->color(FL_DARK_BLUE);
        sendBtn->labelcolor(FL_WHITE);
        end();
    }
};
// --------------
void NurseMainWindow::openAssistDoctorWindow() {
    auto* win = new AssistDoctorWindow(messageData);
    win->color(fl_rgb_color(245, 245, 245));
    win->show();
}
// ---------------

// Manage appointmemnts
class ManageAppointmentsWindow : public Fl_Window {
private:
    Fl_Multiline_Output* display;
    Fl_Input* input;
    Fl_Button* addBtn;
    std::vector<std::string>& appointmentDataRef;

    static void on_add(Fl_Widget*, void* userdata) {
        ManageAppointmentsWindow* win = static_cast<ManageAppointmentsWindow*>(userdata);
        std::string appt = win->input->value();
        if (!appt.empty()) {
            win->appointmentDataRef.push_back(appt);
            NurseMainWindow::saveAppointmentsToFile();  // Keep your static file-saving method
            fl_message("Appointment added.");
            win->color(fl_rgb_color(245, 245, 245));
            win->hide();
        }
    }

public:
    ManageAppointmentsWindow(std::vector<std::string>& appointments)
        : Fl_Window(500, 300, "Manage Appointments"), appointmentDataRef(appointments) {

        display = new Fl_Multiline_Output(80, 20, 360, 180, "Existing:");
        input = new Fl_Input(130, 240, 250, 25, "New Appointment:");
        addBtn = new Fl_Button(400, 240, 80, 25, "Add");

        std::ostringstream oss;
        for (const std::string& appt : appointmentDataRef) {
            oss << appt << "\n";
        }
        display->value(oss.str().c_str());

        addBtn->callback(on_add, this);
        addBtn->color(FL_DARK_BLUE);
        addBtn->labelcolor(FL_WHITE);
        end();
    }
};
// --------------
void NurseMainWindow::openManageAppointmentsWindow() {
    auto* win = new ManageAppointmentsWindow(appointmentData);
    win->color(fl_rgb_color(245, 245, 245));
    win->show();
}

// ---------------


// Button click handler
void NurseMainWindow::on_button_click(Fl_Widget* w, void* data) {
    NurseMainWindow* win = static_cast<NurseMainWindow*>(data);
    if (w == win->viewPatientsBtn) {
        win->show_patient_list();
    } else if (w == win->addPatientBtn) {
        win->show_add_patient_form();
    } else if (w == win->recordVitalsBtn) {
        win->openRecordVitalsWindow();
    } else if (w == win->manageAppointmentsBtn) {
        win->openManageAppointmentsWindow();
    } else if (w == win->assistDoctorBtn) {
        win->openAssistDoctorWindow();
    } else if (w == win->logoutBtn) {
        LoginWindow* login = new LoginWindow(1000, 500, "Hospital Emergency Management System - Login"); 
        login->show();
        win->hide();
    }
}

void NurseMainWindow::saveVitalsToFile() {
    std::ofstream file("files/vitals.txt");
    for (const auto& vitals : vitalsData) {
        file << vitals << "\n";
    }
}
void NurseMainWindow::saveMessagesToFile() {
    std::ofstream file("files/messages.txt");
    for (const auto& m : messageData) {
        file << m << "\n";
    }
}
void NurseMainWindow::saveAppointmentsToFile() {
    std::ofstream file("files/appointments.txt");
    for (const auto& a : appointmentData) {
        file << a << "\n";
    }
}


void NurseMainWindow::loadAppointmentsFromFile() {
    appointmentData.clear();
    std::ifstream file("files/appointments.txt");
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            appointmentData.push_back(line);
    }
}

void NurseMainWindow::loadMessagesFromFile() {
    messageData.clear();
    std::ifstream file("files/messages.txt");
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            messageData.push_back(line);
    }
}
