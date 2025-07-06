#ifndef NURSEMAINWINDOW_H
#define NURSEMAINWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <string>
#include "Queue.h"

class NurseMainWindow : public Fl_Window {
private:
    Fl_Button *viewPatientsBtn, *recordVitalsBtn, *assistDoctorBtn, *manageAppointmentsBtn, *addPatientBtn, *logoutBtn;

    static void on_button_click(Fl_Widget*, void* userdata);

public:
    NurseMainWindow();

    void show_add_patient_form();   //--1
    void show_patient_list();    //---2
    void openRecordVitalsWindow();  //--3
    void openMessagesWindow();      //---4
    void openAssistDoctorWindow();  //--5
    void openManageAppointmentsWindow();    //--6

    static void savePatientsToFile();   //   
    static void saveVitalsToFile();     //
    static void saveAppointmentsToFile();
    static void loadAppointmentsFromFile();     //
    static void saveMessagesToFile();
    static void loadMessagesFromFile();     //
};

#endif // NURSEMAINWINDOW_H
