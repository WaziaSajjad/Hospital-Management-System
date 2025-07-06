#ifndef DOCTORMAINWINDOW_H
#define DOCTORMAINWINDOW_H

#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <string>
#include "Queue.h"



class DoctorMainWindow : public Fl_Window {
private:
    Fl_Button *viewPatientsBtn;
    Fl_Button *updateDiagnosisBtn;
    Fl_Button *prescribeMedBtn;
    Fl_Button *PrescribeTestBtn;
    Fl_Button *dischargeBtn;
    Fl_Button *logoutBtn;

    static void on_button_click(Fl_Widget*, void* userdata);
    PriorityQueue patientQueue;


public:
    DoctorMainWindow();

    void show_patients_window();
    void show_update_diagnosis_window();
    void show_prescribe_med_form();
    void show_discharge_patients_window();
    void show_prescribe_tests_window();

    // Declarations of the file I/O functions
    // void saveQueueToFile();
    // void savePatientsToFile();
    // void loadQueueFromFile();
    void savePatientsToFile();
    static void saveQueueToFile(PriorityQueue& queue, const std::string& filename);
    static void loadQueueFromFile(PriorityQueue& queue, const std::string& filename);

    static void saveMedsToFile();     
    static void loadVitalsFromFile();   
    // static void loadAppointmentsFromFile();     
    static void loadMessagesFromFile();    
    static void saveTestsToFile();    

    // static void saveMessagesToFile();       
    // static void saveAppointmentsToFile();   

};

#endif 