# Hospital Emergency Management System

A C++ project simulating an emergency hospital environment using a priority queue for patient triage. Built with a linked-list-based queue and a graphical interface using FLTK, this system helps simulate how hospitals prioritize treatment for patients in real-time.

---

## Objective

The goal of this project is to simulate a hospital emergency management interface where patients are admitted and treated based on the urgency of their condition. Nurses can add patients to the system, and doctors can view or discharge them in priority order. This project demonstrates the practical use of priority queues in real-world healthcare scenarios.

---

## Core Functionalities

**Nurse Role:**
- Add patients with condition and priority
- Record vitals
- View current patient queue

**Doctor Role:**
- View the prioritized list of patients
- Discharge the next patient (highest priority)
- Assist in diagnosis and treatment

Patients are categorized into three priority levels:
- `1` – Critical  
- `2` – Moderate  
- `3` – Mild  

---

##  Data Structures Used

- **Priority Queue**: Implemented with a singly linked list (`queue.h` / `queue.cpp`).
- Each `Patient` contains: `name`, `age`, `condition`, and `priority`.
- Patients are inserted into the queue based on priority.
- Discharges always remove the front-most (highest-priority) patient.

---

##  Technologies Used

- **Language**: C++
- **GUI Library**: [FLTK (Fast Light Toolkit)](https://www.fltk.org/)
- **Data Persistence**: Text file (`patients.txt`)
- **Compiler**: g++ (via command line)
- **Platform**: Windows

---

##  File Structure

- `main.cpp`: Entry point that launches the Doctor or Nurse dashboard.
- `queue.h / queue.cpp`: Linked-list implementation of the priority queue.
- `Patient.h`: Defines patient structure and serialization methods.
- `NurseMainWindow.*`: Nurse dashboard UI and logic.
- `DoctorMainWindow.*`: Doctor dashboard UI and logic.
- `AddPatientForm.*`: Form to admit a new patient.
- `ViewPatientsWindow.*`: Window to view the current queue.
- `DischargePatientsWindow.*`: Interface to discharge patients.
- `files/patients.txt`: Stores patient data between sessions.
- `images/backgroundimg.png`: Dashboard background image.

---

##  GUI Overview

Built with **FLTK**, the application features:

- Nurse and Doctor login screens
- Input forms for patient data
- Buttons to manage patients
- Queue and discharge display windows

---

##  Why FLTK?

FLTK (Fast Light Toolkit) was chosen due to:

- **Simplicity**: Easy-to-use UI components for C++
- **Performance**: Lightweight and fast rendering
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **C++ Native**: Seamless integration with the project's logic

---

##  Observations

- Patient queue state persists using `patients.txt`.
- Shared queue logic keeps Nurse and Doctor views in sync.
- Edge cases like empty queues or invalid input are handled cleanly.
- Proper file paths are essential for UI data to load correctly.

---

##  Future Improvements

- Add user authentication for login
- Migrate to a database backend
- Implement patient history and search functionality
- Add color-coding in GUI based on priority level

---

## Conclusion

This project showcases the use of data structures (priority queues) in healthcare simulations. It integrates dynamic memory management, file I/O, and GUI development in C++. The separation of roles, use of serialization, and modular design make it extendable and educationally valuable.

---

##  License

This project is for educational purposes. 

