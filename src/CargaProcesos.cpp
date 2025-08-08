#include "CargaProcesos.h"
#include "Proceso.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string& s) {
    auto left = std::find_if_not(s.begin(), s.end(), ::isspace);
    auto right = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
    return (left < right ? std::string(left, right) : std::string());
}

std::vector<Proceso> cargarProcesosDesdeArchivo(const std::string& rutaArchivo) {
    std::vector<Proceso> procesos;
    std::ifstream file(rutaArchivo);
    if (!file) {
        std::cerr << "Error al abrir archivo: " << rutaArchivo << "\n";
        return procesos;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        int pid = 0, quantum = 0, ax = 0, bx = 0, cx = 0;
        std::stringstream ss(line);
        std::string token;

        // Partir por comas
        while (std::getline(ss, token, ',')) {
            token = trim(token);
            if (token.rfind("PID:", 0) == 0) {
                pid = std::stoi(trim(token.substr(4)));
            } else if (token.rfind("AX=", 0) == 0) {
                ax = std::stoi(token.substr(3));
            } else if (token.rfind("BX=", 0) == 0) {
                bx = std::stoi(token.substr(3));
            } else if (token.rfind("CX=", 0) == 0) {
                cx = std::stoi(token.substr(3));
            } else if (token.rfind("Quantum=", 0) == 0) {
                quantum = std::stoi(token.substr(8));
            }
        }

        Proceso p(pid, quantum);
        p.ax = ax;
        p.bx = bx;
        p.cx = cx;
        procesos.push_back(p);
    }

    return procesos;
}

std::vector<Proceso> cargarProcesosDesdeConsola() {
    std::vector<Proceso> procesos;
    int n;
    std::cout << "¿Cuántos procesos va a ingresar? ";
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int pid, quantum;
        std::cout << "Proceso " << i+1 << " - PID y Quantum: ";
        std::cin >> pid >> quantum;
        procesos.emplace_back(pid, quantum);
    }
    return procesos;
}