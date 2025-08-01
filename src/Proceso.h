// src/Proceso.h

#ifndef PROCESO_H
#define PROCESO_H

#include <string>
#include <vector>

/**
 * @brief Estado del proceso en el planificador.
 */
enum class Estado {
    Listo,        ///< Proceso está listo para ejecutarse.
    Ejecutando,   ///< Proceso actualmente en ejecución.
    Terminado     ///< Proceso ha finalizado.
};

/**
 * @brief Representación de un proceso en la simulación.
 */
struct Proceso {
    int pid;                         ///< Identificador único del proceso
    int pc;                          ///< Contador de programa (índice de instrucción)
    int ax;                          ///< Registro AX
    int bx;                          ///< Registro BX
    int cx;                          ///< Registro CX
    int quantum;                     ///< Duración de tiempo asignada
    Estado estado;                   ///< Estado actual del proceso
    std::vector<std::string> instrucciones; ///< Memoria de instrucciones

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
