# SistemasOperativos
## Simulador de Planificación Round-Robin (C++)

Guía rápida para **compilar** y **ejecutar** el proyecto.

## Requisitos
- Linux o WSL 
- g++ con soporte C++17

## Compilación
g++ -std=c++17 -pthread src/*.cpp -o simulador

## Ejecución
### Con carpeta de instrucciones específica
./simulador -f procesos.txt instrucciones

### Si los <pid>.txt están en la carpeta actual
./simulador -f procesos.txt
    