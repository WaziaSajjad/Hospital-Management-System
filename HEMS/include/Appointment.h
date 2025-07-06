// #ifndef APPOINTMENT_H
// #define APPOINTMENT_H

// #include <string>
// #include <sstream>

// struct Appointment {
//     std::string name;
//     int age;
//     std::string condition;

//     // Default constructor (needed for file reads, vectors)
//     Appointment() : name(""), age(0), condition("") {}

//     // Param constructor
//     Appointment(const std::string& n, int a, const std::string& c)
//         : name(n), age(a), condition(c) {}

//     // Convert Appointment to CSV-style line
//     std::string to_string() const {
//         std::ostringstream oss;
//         oss << name << "," << age << "," << condition;
//         return oss.str();
//     }

//     // Parse Appointment from CSV-style line
//     static Appointment from_string(const std::string& line) {
//         std::istringstream iss(line);
//         std::string token;
//         Appointment p;

//         std::getline(iss, p.name, ',');
//         std::getline(iss, token, ',');
//         p.age = std::stoi(token);
//         std::getline(iss, p.condition, ',');
//         // std::getline(iss, token, ',');
//         // p.priority = std::stoi(token);

//         return p;
//     }
// };

// #endif // APPOINTMENT.H


#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>
#include <sstream>

struct Appointment {
    std::string name;
    int age;
    std::string condition;

    // Default constructor
    Appointment() : name(""), age(0), condition("") {}

    // Param constructor - fixed: removed unused parameter
    Appointment(const std::string& n, int a, const std::string& c)
        : name(n), age(a), condition(c) {}

    // Convert Appointment to CSV-style line
    std::string to_string() const {
        std::ostringstream oss;
        oss << name << "," << age << "," << condition;
        return oss.str();
    }

    // Parse Appointment from CSV-style line
    static Appointment from_string(const std::string& line) {
        std::istringstream iss(line);
        std::string token;
        Appointment p;

        std::getline(iss, p.name, ',');
        std::getline(iss, token, ',');
        p.age = std::stoi(token);
        std::getline(iss, p.condition, ',');

        return p;
    }
};

#endif // APPOINTMENT_H
