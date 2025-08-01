// src/Planificador.cpp

#include "Planificador.h"
#include <iostream>
#include <sstream>

Planificador::Planificador(std::vector<Proceso>& procesos)
    : procesos_(procesos) {}

void Planificador::imprimirContextSwitch(const Proceso& anterior, const Proceso& siguiente) {
    std::cout << "Context switch: PID " << anterior.pid
              << " -> PID " << siguiente.pid << '\n';
}

void Planificador::ejecutarInstruccion(Proceso& p) {
    // Si ya no hay más instrucciones, marcamos como terminado
    if (p.pc >= static_cast<int>(p.instrucciones.size())) {
        p.estado = Estado::Terminado;
        return;
    }

    std::istringstream iss(p.instrucciones[p.pc]);
    std::string opcode;
    iss >> opcode;

    if (opcode == "INC") {
        std::string reg; iss >> reg;
        if (reg == "AX") ++p.ax;
        else if (reg == "BX") ++p.bx;
        else if (reg == "CX") ++p.cx;

    } else if (opcode == "ADD") {
        std::string r1, r2;
        iss >> r1 >> r2;
        int* dest = nullptr; int* src = nullptr;
        if (r1 == "AX") dest = &p.ax;
        else if (r1 == "BX") dest = &p.bx;
        else if (r1 == "CX") dest = &p.cx;
        if (r2 == "AX") src = &p.ax;
        else if (r2 == "BX") src = &p.bx;
        else if (r2 == "CX") src = &p.cx;
        if (dest && src) *dest += *src;

    } else if (opcode == "SUB") {
        std::string r1, r2;
        iss >> r1 >> r2;
        int* dest = nullptr; int* src = nullptr;
        if (r1 == "AX") dest = &p.ax;
        else if (r1 == "BX") dest = &p.bx;
        else if (r1 == "CX") dest = &p.cx;
        if (r2 == "AX") src = &p.ax;
        else if (r2 == "BX") src = &p.bx;
        else if (r2 == "CX") src = &p.cx;
        if (dest && src) *dest -= *src;

    } else if (opcode == "NOP") {
        // no hace nada
    }

    // Avanzar contador de programa
    ++p.pc;
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
    std::cout << "Todos los procesos han terminado." << '\n';
}
