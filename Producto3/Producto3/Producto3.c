#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> // Proporciona las funciones básicas de entrada y salida y permite trabajar con archivos de texto.
#include <stdlib.h> // Proporciona funciones de utilidad general, como asignaciones o conversiones.
#include <string.h> // Proporciona funciones para trabajar con cadenas de caracteres.
#include <stdbool.h> // Define el tipo de datos bool.
#include <locale.h> // Proporciona funciones para configurar la localización de la aplicación, como el idioma y la configuración regional.
#include "funciones.h" // Librería personalizada que definimos para agrupar todas las funciones que se han creado en el archivo funciones.c

int main() {
    char nombreArchivo[100];

    printf("Escriba el nombre y la ruta del archivo que contiene las IPs:\n");
    scanf("%s", &nombreArchivo);
    mostrarArchivo(nombreArchivo); // Llamada para mostrar el archivo

    printf("Seleccione el adaptador que desea modificar: \n");
    mostrarAdaptador(); // LLamada a la función para mostrar adaptadores
    
    printf("Se va a comprobar la validez de las direcciones IPs del archivo \n\n");
    comprobarIPs(nombreArchivo); // Llamada a la función para chekear las IPs del archivo

    printf("Se va a testear la velocidad de las IPs...\n");
    checkearVelocidad(); // Llamada a la funcion para testear la velocidad y reconfigurar si es necesario

    system("cls");
    remove(ARCHIVO_DNS);
    remove(ARCHIVO_ADAPTADORES);
    remove(ARCHIVO_TEMPORAL);
    printf("Saliendo...");
    return 0;
}