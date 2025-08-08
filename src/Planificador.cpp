#include "Planificador.h"
#include <iostream>
#include <sstream>

Planificador::Planificador(std::vector<Proceso>& procesos)
    : procesos_(procesos) {}

void Planificador::imprimirContextSwitch(const Proceso& anterior, const Proceso& siguiente) {
    std::cout << "Context switch: PID " << anterior.pid
              << " -> PID " << siguiente.pid << "\n";
}

void Planificador::imprimirEstado(const Proceso& p, const std::string& instr) {
    std::cout << "  PID " << p.pid
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

    std::string linea = p.instrucciones[p.pc];
    std::istringstream iss(linea);
    std::string opcode;
    iss >> opcode;

    if (opcode == "INC") {
        std::string reg; iss >> reg;
        if      (reg == "AX") ++p.ax;
        else if (reg == "BX") ++p.bx;
        else if (reg == "CX") ++p.cx;

    } else if (opcode == "ADD" || opcode == "SUB") {
        std::string dest, src;
        iss >> dest; dest.pop_back();  
        iss >> src;
        int* pd = nullptr;
        if      (dest == "AX") pd = &p.ax;
        else if (dest == "BX") pd = &p.bx;
        else if (dest == "CX") pd = &p.cx;
        int val = (src=="AX"?p.ax: src=="BX"?p.bx: p.cx);
        if (pd) {
            if (opcode == "ADD") *pd += val;
            else                 *pd -= val;
        }

    } else if (opcode == "NOP") {

    } else {
        p.estado = Estado::Terminado;
        return;
    }

    ++p.pc;
    imprimirEstado(p, linea); 
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
