#include "Planificador.h"
#include <iostream>
#include <sstream>

Planificador::Planificador(std::vector<Proceso>& procesos)
    : procesos_(procesos) {}

void Planificador::imprimirContextSwitch(const Proceso& anterior,
                                         const Proceso& siguiente) {
    if (anterior.pid != 0) {
        std::cout << "Guardando estado de Proceso " << anterior.pid
                  << ": PC=" << anterior.pc
                  << ", AX=" << anterior.ax
                  << ", BX=" << anterior.bx
                  << ", CX=" << anterior.cx << "\n";
    }
    std::cout << "Cargando estado de Proceso " << siguiente.pid
              << ": PC=" << siguiente.pc
              << ", AX=" << siguiente.ax
              << ", BX=" << siguiente.bx
              << ", CX=" << siguiente.cx << "\n";
}

void Planificador::imprimirEstado(const Proceso& p,
                                  const std::string& instr) {
    std::cout << "PID " << p.pid
              << " ejecuta: " << instr
              << " | AX=" << p.ax
              << " BX=" << p.bx
              << " CX=" << p.cx
              << " PC=" << p.pc
              << "\n";
}

void Planificador::ejecutarInstruccion(Proceso& p) {
    if (p.pc < 0 || p.pc >= static_cast<int>(p.instrucciones.size())) {
        p.estado = Estado::Terminado;
        return;
    }

    const std::string instr = p.instrucciones[p.pc];
    std::istringstream iss(instr);
    std::string opcode;
    iss >> opcode;

    if (opcode == "INC") {
        std::string reg; iss >> reg;
        if      (reg == "AX") ++p.ax;
        else if (reg == "BX") ++p.bx;
        else if (reg == "CX") ++p.cx;
        ++p.pc;

    } else if (opcode == "JMP") {
        int target; iss >> target;
        if (0 <= target && target < static_cast<int>(p.instrucciones.size()))
            p.pc = target;
        else
            p.estado = Estado::Terminado;

    } else if (opcode == "ADD" || opcode == "SUB" || opcode == "MUL") {
        std::string dest, src;
        iss >> dest;
        if (!dest.empty() && dest.back()==',') dest.pop_back();
        iss >> src;

        int* pd = (dest=="AX" ? &p.ax
                 : dest=="BX" ? &p.bx
                 : dest=="CX" ? &p.cx
                 : nullptr);
        int val = (src=="AX" ? p.ax
                 : src=="BX" ? p.bx
                 : src=="CX" ? p.cx
                 : std::stoi(src));
        if (pd) {
            if      (opcode=="ADD") *pd += val;
            else if (opcode=="SUB") *pd -= val;
            else                    *pd *= val;
        }
        ++p.pc;

    } else if (opcode == "NOP") {
        ++p.pc;

    } else {
        p.estado = Estado::Terminado;
        return;
    }

    imprimirEstado(p, instr);
}

void Planificador::ejecutarRoundRobin() {
    bool quedaTrabajo = true;
    Proceso dummy(0, 0);
    dummy.estado = Estado::Terminado;
    Proceso* anterior = &dummy;

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
                if (p.estado != Estado::Terminado)
                    p.estado = Estado::Listo;
                anterior = &p;
            }
        }
    }
    std::cout << "Todos los procesos han terminado.\n";
}
