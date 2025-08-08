#include "CargaProcesos.h"
#include "CargaInstrucciones.h"
#include "Planificador.h"
#include <unistd.h>    
#include <iostream>
#include <vector>
#include <string>

void mostrarUso(const char* prog) {
    std::cout << "Uso:\n  " << prog
              << " -f <procesos.txt> [<ruta_instrucciones>]\n";
}

int main(int argc, char* argv[]) {
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
            return 1;
        }
    }
    if (rutaProcesos.empty()) {
        std::cerr << "Error: debe especificar el fichero de procesos con -f\n";
        mostrarUso(argv[0]);
        return 1;
    }

    if (optind < argc) {
        rutaInstrucciones = argv[optind];
    }

    auto procesos = cargarProcesosDesdeArchivo(rutaProcesos);
    if (procesos.empty()) {
        std::cerr << "No se cargaron procesos.\n";
        return 1;
    }

    for (const auto& p : procesos) {
        std::cout << "Cargando estado de Proceso " << p.pid
                  << ": PC=" << p.pc
                  << ", AX=" << p.ax
                  << ", BX=" << p.bx
                  << ", CX=" << p.cx
                  << ", Quantum=" << p.quantum
                  << "\n";
    }

    for (auto& p : procesos) {
        auto instr = cargarInstruccionesDesdeArchivo(p.pid, rutaInstrucciones);
        p.setInstrucciones(instr);
    }

    Planificador sched(procesos);
    sched.ejecutarRoundRobin();

    return 0;
}
