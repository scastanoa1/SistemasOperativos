#include "CargaProcesos.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<Proceso> cargarProcesosDesdeArchivo(const std::string& rutaArchivo) {
    std::vector<Proceso> procesos;
    std::ifstream file(rutaArchivo);
    if (!file) {
        std::cerr << "Error al abrir archivo: " << rutaArchivo << "\n";
        return procesos;
    }
    int pid, quantum;
    while (file >> pid >> quantum) {
        procesos.emplace_back(pid, quantum);
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
