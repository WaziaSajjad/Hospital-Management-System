#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <sstream>
#include <vector>

struct Patient {
    std::string name;
    int age;
    std::string condition;
    int priority;  // 1 = Critical, 2 = Moderate, 3 = Mild
    std::vector<std::string> medicines;

    // Default constructor (needed for file reads, vectors)
    Patient() : name(""), age(0), condition(""), priority(3) {}

    // Param constructor
    Patient(const std::string& n, int a, const std::string& c, int p)
        : name(n), age(a), condition(c), priority(p) {}

    // Convert patient to CSV-style line
    
    std::string to_string() const {
        std::ostringstream oss;
        oss << name << "," << age << "," << condition << "," << priority;
        for (const auto& med : medicines) {
            oss << "," << med;
        }
        return oss.str();
    }


    // Parse patient from CSV-style line
    static Patient from_string(const std::string& line) {
        std::istringstream iss(line);
        std::string token;
        Patient p;

        std::getline(iss, p.name, ',');
        std::getline(iss, token, ',');
        p.age = std::stoi(token);
        std::getline(iss, p.condition, ',');
        std::getline(iss, token, ',');
        p.priority = std::stoi(token);

        while (std::getline(iss, token, ',')) {
            p.medicines.push_back(token);
        }

        return p;
    }

};

#endif // PATIENT_H
