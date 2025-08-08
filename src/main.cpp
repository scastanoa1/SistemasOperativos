#include "CargaProcesos.h"
#include "CargaInstrucciones.h"
#include "Planificador.h"
#include "Logger.h"
#include <unistd.h>    
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>            
#include <sstream>            

void mostrarUso(const char* prog) {
    std::cout << "Uso:\n  " << prog
              << " -f <procesos.txt> [<ruta_instrucciones>]\n";
}

int main(int argc, char* argv[]) {
    Logger::init("log.txt");
    Logger::log("Programa iniciado");

    std::string rutaProcesos;
    std::string rutaInstrucciones = ".";  

    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
        case 'f':
            rutaProcesos = optarg;
            break;
        default:
            mostrarUso(argv[0]);
            return EXIT_FAILURE;
        }
    }
    if (rutaProcesos.empty()) {
        std::cerr << "Error: debe especificar el fichero de procesos con -f\n";
        mostrarUso(argv[0]);
        return EXIT_FAILURE;
    }

    if (optind < argc) {
        rutaInstrucciones = argv[optind];
    }

    auto procesos = cargarProcesosDesdeArchivo(rutaProcesos);
    if (procesos.empty()) {
        std::cerr << "No se cargaron procesos.\n";
        Logger::log("Error: No se cargaron procesos desde " + rutaProcesos);
        return EXIT_FAILURE;
    }
    Logger::log("Procesos cargados desde " + rutaProcesos);

    for (const auto& p : procesos) {
        std::ostringstream oss;
        oss << "Cargando estado de Proceso " << p.pid
            << ": PC=" << p.pc
            << ", AX=" << p.ax
            << ", BX=" << p.bx
            << ", CX=" << p.cx
            << ", Quantum=" << p.quantum;
        std::string msg = oss.str();
        std::cout << msg << "\n";
        Logger::log(msg);
    }

    for (auto& p : procesos) {
        auto instr = cargarInstruccionesDesdeArchivo(p.pid, rutaInstrucciones);
        p.setInstrucciones(instr);
    }
    Logger::log("Instrucciones asignadas a procesos desde " + rutaInstrucciones);

    Logger::log("Iniciando Round-Robin");
    Planificador sched(procesos);
    sched.ejecutarRoundRobin();
    Logger::log("Finalizó Round-Robin");

    std::cout << "\n--- Códigos de salida de procesos ---\n";
    Logger::log("--- Códigos de salida de procesos ---");
    bool huboError = false;
    for (const auto& p : procesos) {
        std::ostringstream oss;
        oss << "Proceso " << p.pid
            << " terminó con código " << p.exit_code;
        std::string msg = oss.str();
        std::cout << msg << "\n";
        Logger::log(msg);
        if (p.exit_code != 0) {
            huboError = true;
        }
    }

    Logger::log(huboError ? "Programa finalizado con errores"
                         : "Programa finalizado correctamente");
    return huboError ? EXIT_FAILURE : EXIT_SUCCESS;
}
