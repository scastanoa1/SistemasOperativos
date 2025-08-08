#include "CargaInstrucciones.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::string> cargarInstruccionesDesdeArchivo(int pid, const std::string& rutaCarpeta) {
    std::vector<std::string> instrucciones;
    std::ostringstream oss;
    oss << rutaCarpeta << "/" << pid << ".txt";
    std::string nombreArchivo = oss.str();

    std::ifstream file(nombreArchivo);
    if (!file) {
        std::cerr << "Error: no se pudo abrir " << nombreArchivo << "\n";
        return instrucciones;
    }

    std::string linea;
    while (std::getline(file, linea)) {
        // Ignorar líneas vacías
        if (linea.find_first_not_of(" \t\r\n") == std::string::npos) 
            continue;
        instrucciones.push_back(linea);
    }
    return instrucciones;
}
