Descripción

En esta parte del proyecto se implementa un sistema de gestión de memoria virtual para el simulador de kernel. La memoria física es simulada y se utiliza un mecanismo de traducción de direcciones mediante tablas de páginas. El Process Generator ha sido reemplazado por un Loader que carga programas desde archivos .elf.
Avances Realizados

    Memoria física simulada: Se ha creado una memoria de 64 MB con regiones para el Kernel, .text y .data.
    Loader: Reemplaza al Process Generator, cargando segmentos .text y .data desde archivos .elf.
    Ampliación de la máquina de cómputo: Se ha agregado soporte para la traducción de direcciones (MMU, TLB, PTBR).
    Tablas de páginas: Cada proceso tiene su propia tabla de páginas en memoria.

Trabajo Pendiente

    Corregir errores en el sistema de memoria.
    Ejecutar instrucciones de los programas cargados desde .elf.
