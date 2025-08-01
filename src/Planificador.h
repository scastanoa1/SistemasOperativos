#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include "Proceso.h"
#include <vector>

class Planificador {
public:
    Planificador(std::vector<Proceso>& procesos);
    void ejecutarRoundRobin();

private:
    std::vector<Proceso>& procesos_;
    void imprimirContextSwitch(const Proceso& anterior, const Proceso& siguiente);
    void ejecutarInstruccion(Proceso& p);
};

#endif 
