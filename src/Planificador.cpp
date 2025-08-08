#include "Planificador.h"
#include "Logger.h"
#include <iostream>
#include <sstream>

// Constructor que recibe la lista de procesos a planificar
Planificador::Planificador(std::vector<Proceso>& procesos)
    : procesos_(procesos) {}

// Muestra y registra el cambio de contexto entre dos procesos
void Planificador::imprimirContextSwitch(const Proceso& anterior,
                                         const Proceso& siguiente) {
    std::ostringstream oss;
    oss << "[Cambio de contexto]\n";

    // Guarda el estado del proceso anterior si no es dummy
    if (anterior.pid != 0) {
        oss << "Guardando estado de Proceso " << anterior.pid
            << ": PC=" << anterior.pc
            << ", AX=" << anterior.ax
            << ", BX=" << anterior.bx
            << ", CX=" << anterior.cx
            << "\n";
    }

    // Carga el estado del proceso siguiente
    oss << "Cargando estado de Proceso " << siguiente.pid
        << ": PC=" << siguiente.pc
        << ", AX=" << siguiente.ax
        << ", BX=" << siguiente.bx
        << ", CX=" << siguiente.cx
        << "\n";

    const std::string msg = oss.str();
    std::cout << msg;
    Logger::log(msg);
}

// Imprime el estado de ejecución actual de un proceso
void Planificador::imprimirEstado(const Proceso& p, const std::string& instr) {
    std::ostringstream oss;
    oss << "PID " << p.pid
        << " ejecuta: " << instr
        << " | AX=" << p.ax
        << " BX=" << p.bx
        << " CX=" << p.cx
        << " PC=" << p.pc
        << "\n";

    const std::string msg = oss.str();
    std::cout << msg;
    Logger::log(msg);
}

// Ejecuta una sola instrucción del proceso
void Planificador::ejecutarInstruccion(Proceso& p) {
    // Termina si el PC está fuera de rango
    if (p.pc < 0 || p.pc >= static_cast<int>(p.instrucciones.size())) {
        p.estado = Estado::Terminado;
        return;
    }

    const std::string instr = p.instrucciones[p.pc];
    std::istringstream iss(instr);
    std::string opcode;
    iss >> opcode;

    // Instrucción: Incrementar registro
    if (opcode == "INC") {
        std::string reg; iss >> reg;
        if      (reg == "AX") ++p.ax;
        else if (reg == "BX") ++p.bx;
        else if (reg == "CX") ++p.cx;
        ++p.pc;

    // Instrucción: Salto a línea específica
    } else if (opcode == "JMP") {
        int target; iss >> target;
        if (0 <= target && target < static_cast<int>(p.instrucciones.size())) {
            p.pc = target;
        } else if (target == static_cast<int>(p.instrucciones.size())) {
            p.estado = Estado::Terminado;  // Finaliza de forma natural
            return;
        } else {
            p.estado = Estado::Terminado;  // Salto inválido
            p.exit_code = 1;
            return;
        }

    // Instrucciones matemáticas: ADD, SUB, MUL
    } else if (opcode == "ADD" || opcode == "SUB" || opcode == "MUL") {
        std::string dest, src;
        iss >> dest;
        if (!dest.empty() && dest.back() == ',') dest.pop_back();
        iss >> src;

        int* pd = nullptr;
        if      (dest == "AX") pd = &p.ax;
        else if (dest == "BX") pd = &p.bx;
        else if (dest == "CX") pd = &p.cx;

        int val = 0;
        if      (src == "AX") val = p.ax;
        else if (src == "BX") val = p.bx;
        else if (src == "CX") val = p.cx;
        else {
            try {
                val = std::stoi(src);  // Convierte si es número
            } catch (...) {
                p.estado = Estado::Terminado; // Valor inválido
                p.exit_code = 1;
                return;
            }
        }

        if (pd) {
            if (opcode == "ADD") *pd += val;
            else if (opcode == "SUB") *pd -= val;
            else                     *pd *= val;
        } else {
            p.estado = Estado::Terminado;  // Registro inválido
            p.exit_code = 1;
            return;
        }

        ++p.pc;

    // Instrucción NOP: no hace nada
    } else if (opcode == "NOP") {
        ++p.pc;

    // Instrucción desconocida: termina con error
    } else {
        p.estado = Estado::Terminado;
        p.exit_code = 1;
        return;
    }

    imprimirEstado(p, instr);
}

// Planifica procesos con algoritmo Round Robin
void Planificador::ejecutarRoundRobin() {
    bool quedaTrabajo = true;
    Proceso dummy(0, 0);  // Proceso vacío inicial
    dummy.estado = Estado::Terminado;
    Proceso* anterior = &dummy;

    // Mientras haya procesos listos, sigue ejecutando
    while (quedaTrabajo) {
        quedaTrabajo = false;
        for (auto& p : procesos_) {
            if (p.estado == Estado::Listo) {
                quedaTrabajo = true;
                imprimirContextSwitch(*anterior, p);
                p.estado = Estado::Ejecutando;

                int t = p.quantum;
                while (t-- > 0 && p.estado != Estado::Terminado) {
                    ejecutarInstruccion(p);
                }

                if (p.estado != Estado::Terminado) {
                    p.estado = Estado::Listo;
                }

                anterior = &p;
            }
        }
    }

    const std::string endMsg = "Todos los procesos han terminado.\n";
    std::cout << endMsg;
    Logger::log(endMsg);
}
