#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include "Proceso.h"
#include <vector>
#include <string>


class Planificador {
public:
    Planificador(std::vector<Proceso>& procesos);
    void ejecutarRoundRobin();

private:
    std::vector<Proceso>& procesos_;
    void imprimirContextSwitch(const Proceso& anterior, const Proceso& siguiente);
    void imprimirEstado(const Proceso& p, const std::string& instr);
    void ejecutarInstruccion(Proceso& p);
};

#endif 
