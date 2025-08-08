#include "CargaProcesos.h"
#include "Proceso.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string& s) {
    auto it1 = std::find_if_not(s.begin(), s.end(), ::isspace);
    auto it2 = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
    return (it1 < it2) ? std::string(it1, it2) : std::string();
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
        if (line.find_first_not_of(" \t\r\n") == std::string::npos)
            continue;

        std::unordered_map<std::string,int> kv;
        std::istringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ',')) {
            auto pos = token.find_first_of("=:"); 
            if (pos == std::string::npos) continue;
            std::string key   = trim(token.substr(0, pos));
            std::string value = trim(token.substr(pos + 1));
            kv[key] = std::stoi(value);
        }

        int pid     = kv["PID"];
        int quantum = kv["Quantum"];
        Proceso p(pid, quantum);
        p.ax = kv["AX"];
        p.bx = kv["BX"];
        p.cx = kv["CX"];

        procesos.push_back(p);
    }

    return procesos;
}
