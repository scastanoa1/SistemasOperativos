#include "CargaInstrucciones.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Carga las instrucciones de un proceso desde un archivo txt
std::vector<std::string> cargarInstruccionesDesdeArchivo(int pid, const std::string& rutaCarpeta) {
    std::vector<std::string> instrucciones;

    // Construimos la ruta completa al archivo de instrucciones 
    std::ostringstream oss;
    oss << rutaCarpeta << "/" << pid << ".txt";
    std::string nombreArchivo = oss.str();

    // Abre el archivo
    std::ifstream file(nombreArchivo);
    if (!file) {
        std::cerr << "Error: no se pudo abrir " << nombreArchivo << "\n";
        return instrucciones;
    }

    std::string linea;
    // Lee línea por línea
    while (std::getline(file, linea)) {
        // Salta líneas vacías o con solo espacios
        if (linea.find_first_not_of(" \t\r\n") == std::string::npos) 
            continue;
        instrucciones.push_back(linea);  // Guardamos cada instrucción
    }

    return instrucciones; 
}
