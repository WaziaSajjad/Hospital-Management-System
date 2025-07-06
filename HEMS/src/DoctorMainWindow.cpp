#include "DoctorMainWindow.h"
#include "LoginWindow.h"
#include <FL/fl_ask.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <cstring>
//-------------------
#include "Patient.h"
#include "Queue.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <vector>
#include <sstream>
#include <FL/Fl_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <FL/Fl_Multiline_Input.H>

static PriorityQueue patientQueue;
static std::vector<std::string> MedData;
static std::vector<std::string> TestsData;
static std::vector<std::string> vitalsData;
static std::vector<std::string> appointmentData;
static std::vector<std::string> messageData;

DoctorMainWindow::DoctorMainWindow() : Fl_Window(600, 500, "Doctor Dashboard") {
    initQueue(patientQueue);
    loadQueueFromFile(patientQueue, "files/patients.txt");
    // savePatientsToFile();
    DoctorMainWindow::saveQueueToFile(patientQueue, "files/patients.txt");

    // Background image
    Fl_PNG_Image* bgImg = new Fl_PNG_Image("images/doctor.png");
    if (!bgImg->fail()) {
        Fl_Image* scaledBg = bgImg->copy(w(), h());
        Fl_Box* bgBox = new Fl_Box(0, 0, w(), h());
        bgBox->image(scaledBg);
        bgBox->box(FL_NO_BOX);
    } else {
        fl_alert("Failed to load background image.");
    }

    // Buttons
    int btn_w = 200, btn_h = 50;
    int gap_x = 30, gap_y = 20;
    int start_x = 55, start_y = 30;

    viewPatientsBtn = new Fl_Button(start_x, start_y, btn_w, btn_h, "View Patients");
    updateDiagnosisBtn = new Fl_Button(start_x, start_y + btn_h + gap_y, btn_w, btn_h, "Update Diagnosis");
    prescribeMedBtn = new Fl_Button(start_x, start_y + 2 * (btn_h + gap_y), btn_w, btn_h, "Prescribe Medication");
    PrescribeTestBtn = new Fl_Button(start_x, start_y + 3 * (btn_h + gap_y), btn_w, btn_h, "Prescribe Tests");
    dischargeBtn = new Fl_Button(start_x, start_y + 4 * (btn_h + gap_y), btn_w, btn_h, "Discharge Patient");
    logoutBtn = new Fl_Button(start_x, start_y + 5 * (btn_h + gap_y), btn_w, btn_h, "Logout");

    viewPatientsBtn->callback(on_button_click, this);
    updateDiagnosisBtn->callback(on_button_click, this);
    prescribeMedBtn->callback(on_button_click, this);
    PrescribeTestBtn->callback(on_button_click, this);
    dischargeBtn->callback(on_button_click, this);
    logoutBtn->callback(on_button_click, this);

    viewPatientsBtn->color(FL_DARK_BLUE);
    updateDiagnosisBtn->color(FL_DARK_BLUE);
    prescribeMedBtn->color(FL_DARK_BLUE);
    PrescribeTestBtn->color(FL_DARK_BLUE);
    dischargeBtn->color(FL_DARK_BLUE);
    logoutBtn->color(FL_DARK_BLUE);
    viewPatientsBtn->labelcolor(FL_WHITE);
    updateDiagnosisBtn->labelcolor(FL_WHITE);
    prescribeMedBtn->labelcolor(FL_WHITE);
    PrescribeTestBtn->labelcolor(FL_WHITE);
    dischargeBtn->labelcolor(FL_WHITE);
    logoutBtn->labelcolor(FL_WHITE);

    end();

    savePatientsToFile();
    loadVitalsFromFile();
    loadMessagesFromFile();
    DoctorMainWindow::saveQueueToFile(patientQueue, "files/patients.txt");
    DoctorMainWindow::loadQueueFromFile(patientQueue, "files/patients.txt");
    
    show();
}

void DoctorMainWindow::on_button_click(Fl_Widget* w, void* data) {
    DoctorMainWindow* win = static_cast<DoctorMainWindow*>(data);
    if (w == win->viewPatientsBtn) {
        win->show_patients_window();
    } else if (w == win->updateDiagnosisBtn) {
        win->show_update_diagnosis_window();
    } else if (w == win->prescribeMedBtn) {
        win->show_prescribe_med_form();
    } else if (w == win->PrescribeTestBtn) {
        win->show_prescribe_tests_window();
    } else if (w == win->dischargeBtn) {
        win->show_discharge_patients_window();
    } else if (w == win->logoutBtn) {
        LoginWindow* login = new LoginWindow(1000, 500, "Hospital Emergency Management System - Login"); 
        login->show();
        win->hide();
        
}

}

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

class UpdateDiagnosisWindow : public Fl_Window {
private:
    Fl_Multiline_Input* input;
    Fl_Button* saveButton;
    std::vector<std::string>& messageData;

public:
    UpdateDiagnosisWindow(std::vector<std::string>& data)
        : Fl_Window(600, 340, "Diagnosis Editor"), messageData(data) {
        
        input = new Fl_Multiline_Input(20, 20, 560, 260);

        std::ostringstream oss;
        for (const auto& msg : messageData) {
            oss << msg << "\n";
        }
        input->value(oss.str().c_str());

        saveButton = new Fl_Button(500, 290, 80, 30, "Save");
        saveButton->callback([](Fl_Widget*, void* userdata) {
            auto* win = static_cast<UpdateDiagnosisWindow*>(userdata);
            std::istringstream iss(win->input->value());
            std::vector<std::string> newData;
            std::string line;
            while (std::getline(iss, line)) {
                newData.push_back(line);
            }
            win->messageData = newData;
            win->hide();
        }, this);
        saveButton->color(FL_DARK_BLUE);
        saveButton->labelcolor(FL_WHITE);

        end();
    }
};

// prescribe meds Window --------------------------
class PrescribeMedWindow : public Fl_Window {
private:
    Fl_Input* patientNameInput;
    Fl_Input* Med1;
    Fl_Input* Dose1;
    Fl_Input* times;
    Fl_Button* submitBtn;

    static void on_submit(Fl_Widget*, void* userdata) {
        PrescribeMedWindow* form = static_cast<PrescribeMedWindow*>(userdata);
        std::string name = form->patientNameInput->value();
        std::string med = form->Med1->value();
        std::string dose = form->Dose1->value();
        std::string time = form->times->value();

        MedData.push_back(name + ": " + med + ", only " + dose + " take " + time + " a day.");
        DoctorMainWindow::saveMedsToFile();  
        fl_message("Medicines prescribed successfully!");
        form->hide();
    }

public:
    PrescribeMedWindow() : Fl_Window(350, 300, "Prescribe Medicines") {
        patientNameInput = new Fl_Input(150, 50, 160, 25, "Patient Name:");
        Med1 = new Fl_Input(150, 90, 160, 25, "Medicine:");
        Dose1 = new Fl_Input(150, 130, 160, 25, "Dose:");
        times = new Fl_Input(150, 170, 160, 25, "Times to take:");
        submitBtn = new Fl_Button(100, 220, 100, 30, "Submit");
        submitBtn->callback(on_submit, this);
        submitBtn->color(FL_DARK_BLUE);
        submitBtn->labelcolor(FL_WHITE);
        end();
    }
};

// Discharge Patients
class DischargePatientsWindow : public Fl_Window {
private:
    DoctorMainWindow* mainWindow;
    Fl_Multiline_Output* display;
    Fl_Button* disBtn;

    static void on_discharge(Fl_Widget*, void* userdata) {
        DischargePatientsWindow* win = static_cast<DischargePatientsWindow*>(userdata);
        Patient discharged;
        if (treatNextPatient(patientQueue, discharged)) {
            fl_message(("Discharged: " + discharged.name).c_str());
            saveQueueToFile(patientQueue, "files/patients.txt");
        } else {
            fl_message("No patients to discharge.");
        }

        // Refresh display
        std::ostringstream oss;
        Node* current = patientQueue.front;
        while (current != nullptr) {
            oss << current->data.name << " (" << current->data.age << ", " << current->data.condition << ", Priority: " << current->data.priority << ")\n";
            current = current->next;
        }
        win->display->value(oss.str().c_str());
    }

public:
    DischargePatientsWindow(DoctorMainWindow* mainWin) : Fl_Window(500, 300, "Discharge Patients") {
        loadQueueFromFile(patientQueue, "files/patients.txt");
        display = new Fl_Multiline_Output(80, 20, 360, 200, "Queue:");
        disBtn = new Fl_Button(200, 240, 100, 30, "Discharge");

        // Initial display
        std::ostringstream oss;
        Node* current = patientQueue.front;
        while (current != nullptr) {
            oss << current->data.name << " (" << current->data.age << ", " << current->data.condition << ", Priority: " << current->data.priority << ")\n";
            current = current->next;
        }
        display->value(oss.str().c_str());

        disBtn->callback(on_discharge, this);
        disBtn->color(FL_DARK_BLUE);
        disBtn->labelcolor(FL_WHITE);
        end();
    }
};


// Prescribe Tests Window -------------------
class PrescribeTestsWindow : public Fl_Window {
private:
    Fl_Input* patientNameInput;
    Fl_Input* test1Input;
    Fl_Input* test2Input;
    Fl_Input* test3Input;
    Fl_Button* submitBtn;

    static void on_submit(Fl_Widget*, void* userdata) {
        PrescribeTestsWindow* form = static_cast<PrescribeTestsWindow*>(userdata);
        std::string name = form->patientNameInput->value();
        std::string test1 = form->test1Input->value();
        std::string test2 = form->test2Input->value();
        std::string test3 = form->test3Input->value();

        TestsData.push_back(name + ": " + test1 + " " + test2 + " " + test3);
        DoctorMainWindow::saveTestsToFile();  
        fl_message("Tests prescribed successfully!");
        form->hide();
    }

public:
    PrescribeTestsWindow() : Fl_Window(350, 300, "Prescribe Tests") {
        patientNameInput = new Fl_Input(150, 50, 160, 25, "Patient Name:");
        test1Input = new Fl_Input(150, 90, 160, 25, "Test 1:");
        test2Input = new Fl_Input(150, 130, 160, 25, "Test 2:");
        test3Input = new Fl_Input(150, 170, 160, 25, "Test 3:");
        submitBtn = new Fl_Button(100, 220, 100, 30, "Submit");
        submitBtn->callback(on_submit, this);
        submitBtn->color(FL_DARK_BLUE);
        submitBtn->labelcolor(FL_WHITE);
        end();
    }
};


// Show Patients Window
void DoctorMainWindow::show_patients_window() {
    PriorityQueue tempQueue;
    initQueue(tempQueue);

    // Load the latest patient data saved by the Nurse
    loadQueueFromFile(tempQueue, "files/patients.txt");

    // Now show the ViewPatientsWindow with the updated queue
    ViewPatientsWindow* win = new ViewPatientsWindow(tempQueue);
    win->show();
    win->color(fl_rgb_color(245, 245, 245));
    // win->labelcolor(FL_WHITE);
}
// Assist Doctor Window
void DoctorMainWindow::show_update_diagnosis_window() {
    UpdateDiagnosisWindow* update = new UpdateDiagnosisWindow(messageData);
    update->show();
    update->color(fl_rgb_color(245, 245, 245));
    // update->labelcolor(FL_WHITE);
}
// Show Record Vitals Form
void DoctorMainWindow::show_prescribe_med_form() {
    PrescribeMedWindow* form = new PrescribeMedWindow();
    form->show();
    form->color(fl_rgb_color(245, 245, 245));
    // form->labelcolor(FL_WHITE);
}
// Discharge Patients Window
void DoctorMainWindow::show_discharge_patients_window() {
    DischargePatientsWindow* dischargeWin = new DischargePatientsWindow(this);
    dischargeWin->show();
    dischargeWin->color(fl_rgb_color(245, 245, 245));
    // dischargeBtn->labelcolor(FL_WHITE);
}
// Prescribe Tests Window
void DoctorMainWindow::show_prescribe_tests_window() {
    PrescribeTestsWindow* assist = new PrescribeTestsWindow();
    assist->show();
    assist->color(fl_rgb_color(245, 245, 245));
//     assist->labelcolor(FL_WHITE);
}



//--------------------------------------------
// void DoctorMainWindow::loadAppointmentsFromFile() {
//     appointmentData.clear();
//     std::ifstream file("files/appointments.txt");
//     std::string line;
//     while (std::getline(file, line)) {
//         if (!line.empty())
//             appointmentData.push_back(line);
//     }
// }
//----------------------------------------------------------
void DoctorMainWindow::loadMessagesFromFile() {
    messageData.clear();
    std::ifstream file("files/messages.txt");
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            messageData.push_back(line);
    }
}
// void DoctorMainWindow::saveQueueToFile() {
//     saveQueueToFile(patientQueue, "files/patients.txt");
// }
void DoctorMainWindow::saveQueueToFile(PriorityQueue& queue, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    Node* current = queue.front;
    while (current != nullptr) {
        file << current->data.to_string() << '\n';  // ✔ Optional: use to_string() for full data
        current = current->next;
    }
}


void DoctorMainWindow::savePatientsToFile() {
    std::ofstream file("files/patients.txt");
    if (!file) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }

    Node* current = patientQueue.front;
    while (current != nullptr) {
        file << current->data.to_string() << '\n';
        current = current->next;
    }
}


void DoctorMainWindow::saveMedsToFile() {
    std::ofstream file("files/medications.txt");
    if (!file) {
            std::cerr << "Failed to open vitals.txt for reading.\n";
            return;
        }
    for (const auto& a : MedData) {
        file << a << "\n";
    }
}
void DoctorMainWindow::saveTestsToFile() {
    std::ofstream file("files/Tests.txt");
    if (!file) {
            std::cerr << "Failed to open vitals.txt for reading.\n";
            return;
        }
    for (const auto& a : TestsData) {
        file << a << "\n";
    }
}
void DoctorMainWindow::loadVitalsFromFile() {
    vitalsData.clear();
    std::ifstream file("files/vitals.txt");
    if (!file) {
            std::cerr << "Failed to open vitals.txt for reading.\n";
            return;
        }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            vitalsData.push_back(line);
    }
}
// void DoctorMainWindow::loadQueueFromFile() {
//     loadQueueFromFile(patientQueue, "files/patients.txt");
// }
void DoctorMainWindow::loadQueueFromFile(PriorityQueue& queue, const std::string& filename) {
    freeQueue(queue);  // Correctly clear existing queue

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        if (pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos)
            continue;

        std::string name = line.substr(0, pos1);
        int age = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        std::string condition = line.substr(pos2 + 1, pos3 - pos2 - 1);
        int priority = std::stoi(line.substr(pos3 + 1));

        addPatient(queue, name, age, condition, priority);
    }

    std::cout << "Queue loaded from file: " << filename << std::endl;
}

