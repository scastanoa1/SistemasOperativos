#ifndef CARGA_INSTRUCCIONES_H
#define CARGA_INSTRUCCIONES_H

#include <string>
#include <vector>

/**
 * @brief 
 * @param pid 
 * @param rutaCarpeta 
 * @return 
 */
std::vector<std::string> cargarInstruccionesDesdeArchivo(int pid, const std::string& rutaCarpeta);

#endif 
