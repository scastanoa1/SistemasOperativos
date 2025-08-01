#include "CargaProcesos.h"
#include "Planificador.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<Proceso> procesos;

    if (argc == 2) {
        procesos = cargarProcesosDesdeArchivo(argv[1]);
    } else {
        procesos = cargarProcesosDesdeConsola();
    }

    Planificador sched(procesos);
    sched.ejecutarRoundRobin();

    return 0;
}
