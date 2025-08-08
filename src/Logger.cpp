#include "Logger.h"
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>

// Archivo de salida para el log
std::ofstream Logger::ofs;
static std::mutex logger_mutex;

// Inicializa el logger
void Logger::init(const std::string& filename) {
    ofs.open(filename, std::ios::app);
}

// Escribe un mensaje en el log, junto con la fecha y hora actual
void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logger_mutex);

    auto now     = std::chrono::system_clock::now();          // Obtenemos tiempo actual
    auto in_time = std::chrono::system_clock::to_time_t(now); // Lo convierte a formato legible

    // Escribe timestamp y mensaje
    ofs << std::put_time(std::localtime(&in_time), "%Y-%m-%d %H:%M:%S")
        << " " << message;

    // Si no termina en salto de línea, lo agregamos
    if (message.empty() || message.back() != '\n')
        ofs << "\n";
}
