#ifndef PROCESO_H
#define PROCESO_H

#include <string>
#include <vector>

enum class Estado { Listo, Ejecutando, Terminado };

struct Proceso {
    int pid;
    int pc;
    int ax, bx, cx;
    int quantum;
    Estado estado;
    int exit_code;                    
    std::vector<std::string> instrucciones;

    Proceso(int pid_, int quantum_)
        : pid(pid_), pc(0), ax(0), bx(0), cx(0),
          quantum(quantum_), estado(Estado::Listo),
          exit_code(0) {}              

    void setInstrucciones(const std::vector<std::string>& instr) {
        instrucciones = instr;
    }
};

#endif 
