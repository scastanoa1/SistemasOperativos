#include "CargaProcesos.h"
#include "CargaInstrucciones.h"
#include "Planificador.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <procesos.txt> [<ruta_instrucciones>]\n";
        return 1;
    }

    std::string rutaProcesos    = argv[1];
    std::string rutaInstrucciones = (argc >= 3 ? argv[2] : ".");

    auto procesos = cargarProcesosDesdeArchivo(rutaProcesos);
    if (procesos.empty()) {
        std::cerr << "No se cargaron procesos.\n";
        return 1;
    }

    for (auto& p : procesos) {
        auto instr = cargarInstruccionesDesdeArchivo(p.pid, rutaInstrucciones);
        p.setInstrucciones(instr);
    }

    Planificador sched(procesos);
    sched.ejecutarRoundRobin();
    return 0;
}
