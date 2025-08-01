#ifndef CARGA_PROCESOS_H
#define CARGA_PROCESOS_H

#include "Proceso.h"
#include <vector>
#include <string>

/**
 * @brief 
 * @param rutaArchivo 
 * @return 
 */
std::vector<Proceso> cargarProcesosDesdeArchivo(const std::string& rutaArchivo);

/**
 * @brief 
 * @return 
 */
std::vector<Proceso> cargarProcesosDesdeConsola();

#endif 
