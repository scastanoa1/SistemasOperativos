// src/Proceso.h

#ifndef PROCESO_H
#define PROCESO_H

#include <string>
#include <vector>

/**
 * @brief Estado del proceso en el planificador.
 */
enum class Estado {
    Listo,        
    Ejecutando,   
    Terminado     
};

/**
 * @brief Representación de un proceso en la simulación.
 */
struct Proceso {
    int pid;                         
    int pc;                          
    int ax;                          
    int bx;                         
    int cx;                          
    int quantum;                     
    Estado estado;                   
    std::vector<std::string> instrucciones; 

    /**
     * @brief Constructor que inicializa un proceso sin instrucciones.
     * @param pid_ Identificador del proceso
     * @param quantum_ Quantum asignado al proceso
     */
    Proceso(int pid_, int quantum_)
        : pid(pid_), pc(0), ax(0), bx(0), cx(0),
          quantum(quantum_), estado(Estado::Listo) {}

    /**
     * @brief Asigna la lista de instrucciones al proceso.
     * @param instr Vector de líneas de instrucción (p.ej. "INC AX")
     */
    void setInstrucciones(const std::vector<std::string>& instr) {
        instrucciones = instr;
    }
};

#endif // PROCESO_H
