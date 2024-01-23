#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> // Proporciona las funciones b�sicas de entrada y salida y permite trabajar con archivos de texto.
#include <stdlib.h> // Proporciona funciones de utilidad general, como asignaciones o conversiones.
#include <string.h> // Proporciona funciones para trabajar con cadenas de caracteres.
#include <stdbool.h> // Define el tipo de datos bool.
#include <locale.h> // Proporciona funciones para configurar la localizaci�n de la aplicaci�n, como el idioma y la configuraci�n regional.
#include <ctype.h> // Proporciona funciones para trabajar con caracteres, para determinar si un car�cter es una letra o para determinar si un car�cter es un d�gito.
#include "funciones.h" // Librer�a personalizada que definimos para agrupar todas las funciones que se han creado en el archivo funciones.c

typedef struct {
    char ip[TAMA�O_BUFFER]; // Variable que guardar� la IP de la velocidad que se va a testear
    int velocidad; // Variable que guardara la velocidad de la IP testeada
} DnsInfo;// Estructura de datos para alamacenar la info necesaria
DnsInfo listaDnsInfo[TAMA�O_BUFFER]; // Lista de estructuras con todas las IPs y sus velocidades


// Aci�n para mostrar el contenido de un archivo
void mostrarArchivo(char nombreArchivo[TAMA�O_BUFFER]) {
    FILE* archivo;
    char linea[100];
    archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) { // Mensaje en caso de no poder abrir el archivo
        printf("No se pudo abrir el archivo.\n");
        return;
    }
    // Mostramos el contenido del archivo
    printf("\nLas Ips que contiene el archivo son: \n\n");
    while (fgets(linea, 100, archivo) != NULL) {
        printf("%s", linea);
    }
    fclose(archivo);
    printf("\n\n");
    system("pause");
    system("cls"); // Limpiamos consola
}

// Acci�n que muestra la informaci�n de un adaptador de red
void mostrarAdaptador() {
    char buffer[TAMA�O_BUFFER]; // Almacenamiento para las l�neas le�das
    char nombreAdaptador[TAMA�O_BUFFER]; // Variable para el nombre del adaptador 
    char* ipDNS; // Puntero para almacenar la direcci�n IP del servidor DNS actual
    int opcion, contador = 0, numOpciones = 0; // Variables para almacenar la opci�n seleccionada y el contador de adaptadores
    bool adaptadorEncontrado = false, dnsEncontrado = false; // Variable para llevar registro de si se encontr� alg�n adaptador y los DNS
    FILE* cmd; // Variable para almacenar la salida del comando ejecutado
    FILE* documento = fopen("adaptadores.txt", "w"); // Abrimos el documento en modo escritura, si no existe se crea.
    FILE* archivo = fopen("DNSconf.txt", "w");

    // Se comprueba que el documento se abre correctamente
    if (documento == NULL) {
        printf("No se ha podido abrir el documento\n"); // Texto que se muestra en caso de error
        system("pause");
        system("cls"); // Limpiamos la consola
        return; // Finaliza la funci�n
    }

    // Ejecutar el comando para obtener la configuraci�n de red completa
    cmd = _popen("ipconfig /all", "r"); /* Abre un nuevo proceso y ejecuta el comando "ipconfig /all" para obtener la adaptadores de red.
                                        La "r" en el segundo argumento indica que la salida del comando se leer� como texto.
                                        La salida del comando se almacena en una tuber�a que puede ser le�da por el programa.*/

    // Leemos el resultado del comando y almacenamos la configuraci�n de red en el archivo
    while (fgets(buffer, TAMA�O_BUFFER, cmd)) { /* Permite leer  las l�neas del resultado del comando ejecutado y almacenarlas en la variable "buffer".
                                                  El ciclo se ejecuta hasta que no haya m�s l�neas por leer o se alcance el tama�o m�ximo del buffer. */
        fprintf(documento, "%s", buffer); // Escribimos la configuraci�n de red en el archivo
    }
    _pclose(cmd);
    fclose(documento);
    printf("\n");
    fopen("adaptadores.txt", "r");

    // Mostrar la lista de adaptadores disponibles y pedir al usuario que seleccione uno
    while (fgets(buffer, TAMA�O_BUFFER, documento)) { // Leemos la liniea del archivo
        if (strstr(buffer, "Adaptador")) { /* Se utiliza la funcion buscar una subcadena en una cadena.
                                              Si se encuentra la palabra "Adaptador" en la l�nea, se trata de un adaptador de red */
            contador++; // Incrementamos el contador de adaptadores
            numOpciones++; // Incrementamos las opciones junto al contador pa utilizar posteriomente
            printf("%d. %s", contador, buffer); // Mostramos el nombre del adaptador y su n�mero de orden
            adaptadorEncontrado = true; // Indica que se encontr� al menos un adaptador
        }
    }
    if (adaptadorEncontrado == false) { // Si no se encontr� ning�n adaptador ...
        printf("No se encontraron adaptadores\n");
        fclose(documento);
        system("pause"); // Cerramos el documento
        system("cls"); // Limpiamos consola
        return; // Finaliza la funci�n y volvemos al menu
    }
    while (dnsEncontrado == false) {
        printf("\nIngrese el numero del adaptador que desea consultar: ");
        scanf_s("%d", &opcion);
        if (opcion > numOpciones || opcion == 0) { // Si el usuario selecciona una opcion con un numero mayor a los que ha disponibles o "0", muestra mensaje de error
            printf("Opcion no valida, Ingrese una opcion valida: ");
            scanf_s("%d", &opcion);
        }
        rewind(documento); // Vuelve al principio del archivo
        contador = 0; // Volvemos a inicializar el contador a 0

        // Buscar el adaptador seleccionado y mostrar su informaci�n
        while (fgets(buffer, TAMA�O_BUFFER, documento)) {
            if (strstr(buffer, "Adaptador")) { // Se utiliza la misma funci�n que anteriormente hasta encontrar el nombre del adaptador que se ha seleccionado 
                contador++; // Incrementamos el contador de adaptadores
                if (contador == opcion) { // Si el contador coincide con la opci�n seleccionada lo mostramos
                    printf("\nInformacion del adaptador:\n\n");
                    printf("%s", buffer); // Mostramos el nombre del adaptador

                    // Buscamos el nombre del adaptador en el buffer para poder guardarlo en una variable que utilizaremos mas adelantes
                    if (strstr(buffer, "Adaptador de Ethernet")) {
                        // Buscamos la posici�n del inicio del nombre del adaptador
                        char* inicioNombre = strstr(buffer, "Ethernet") + strlen("Ethernet") + 1;
                        // Buscamos la posici�n del fin del nombre del adaptador
                        char* finNombre = strstr(buffer, ":");
                        // Copiamos el nombre del adaptador a la variable nombreAdaptador
                        strncpy(nombreAdaptador, inicioNombre, finNombre - inicioNombre);
                        // Agregamos un terminador nulo al final de la cadena
                        nombreAdaptador[finNombre - inicioNombre] = '\0';
                        
                    }
                    else if (strstr(buffer, "Adaptador de LAN")) {
                        // Busca la posici�n del inicio del nombre del adaptador
                        char* inicioNombre = strstr(buffer, "LAN") + strlen("LAN") + 13;
                        // Busca la posici�n del fin del nombre del adaptador
                        char* finNombre = strstr(buffer, ":");
                        // Copia el nombre del adaptador a la variable nombreAdaptador
                        strncpy(nombreAdaptador, inicioNombre, finNombre - inicioNombre);
                        // Agrega un terminador nulo al final de la cadena
                        nombreAdaptador[finNombre - inicioNombre] = '\0';
                        ;
                    }
                    
                    // Imprime el nombre del adaptador 

                    while (fgets(buffer, TAMA�O_BUFFER, documento)) { // Leemos el resto de la informaci�n del adaptador
                        if (strstr(buffer, "Adaptador")) {
                            break; // Salimos del ciclo si se encuentra otro adaptador
                        }
                        if (strstr(buffer, "Servidores DNS")) { // Buscar la l�nea que contiene los servidores DNS
                            printf(buffer); // Mostrar solo los servidores DNS 
                            dnsEncontrado = true;
                            if (dnsEncontrado == true) { // Solo guardamos el nombre del adaptador si el Adaptador esta configurado para evitar posible errores de lectura posteriromente
                                fprintf(archivo, "%s\n", nombreAdaptador);
                                ipDNS = strtok(buffer, ":");
                                ipDNS = strtok(NULL, ": ");
                                fprintf(archivo, "%s", ipDNS);
                            }  
                        }
                    }
                    if (dnsEncontrado == false) {
                        printf("Este adaptador se encuentra actualmente desconectado o no configurado por lo tanto eliga otro adaptador");
                    }
                    break; // Salimos del ciclo
                }
            }
        }
   }

    // Cerramos los archivos y el comando y limpiamos la consola
    fclose(documento);
    fclose(archivo);
    system("pause");
    system("cls"); // Limpiamos consola 
}

// Acci�n que modifica el DNS del adaptador seleccionado utilizando el comando netsh
void modificarDNS(char adaptador[20], char dnsMasRapido[BUFFER_IP]) {
    
    char comando[100];
    printf("\nSe va a modificar la configuracion DNS...\n");
    strcpy(comando, "netsh interface ipv4 set dns name=\""); // Se utiliza el comando netsh para modificar la configuraci�n "SOLO ADMINISTRADOR"
    strcat(comando, adaptador);
    strcat(comando, "\" static ");
    strcat(comando, dnsMasRapido); // Se concatena el comando con las variables necesarias para la modificaci�n
    system(comando);
    printf("\nEl adaptador '%s' se ha modificado con exito a la configuracion DNS %s\n", adaptador, dnsMasRapido);
    return; // Nos lleva al main para finalizar
}

// Acci�n que comprueba e n�mero de salto de una IP utilizando el comando tracert
int contadorSaltos(const char* ip) {
    char comando[TAMA�O_BUFFER], palabra[100];
    int numSaltos = 0;
    printf("Comprobando saltos... \n");
    strcpy(comando, "tracert ");
    strcat(comando, ip); // Se concatena todo el comando completo
    FILE* cmd = _popen(comando, "r"); // Abrimos el flujo de datos para leer la salida del comando

    while (fgets(palabra, 100, cmd)) {
        if (isdigit(palabra[0]) || (palabra[0] == ' ' && isdigit(palabra[2]))) { // Verifica si el primer car�cter de la l�nea es un n�mero o dos espacios seguidos de un n�mero
            if (!strstr(palabra, "Tiempo de espera agotado")) { // Verifica si la l�nea contiene la cadena "Tiempo de espera agotado"
                numSaltos++;
            }
        }
    }
    printf("Saltos comprobados \n");
    _pclose(cmd); // Cerramos el flujo de datos
   
    return numSaltos; // Devuelve el numero de saltos obtenidos en el comando
}

// Acci�n que checkea las IPs del archivo y guarda las que son accesibles en un archivo temporal 
void comprobarIPs(char ips[TAMA�O_BUFFER]) {
    FILE* archivo = fopen("DNSips.txt", "r"); // Abrir archivo en modo lectura
    FILE* temporal = fopen("temporal.txt", "w"); // Abrir archivo en modo escritura
    char buffer[TAMA�O_BUFFER]; // Definir un buffer para leer l�neas del archivo
    char ip[BUFFER_IP]; // Definir una cadena de caracteres para almacenar una direcci�n IP
    char comando[TAMA�O_BUFFER] = ""; // Variable que se utiliza para almacenar el comando de ping que se va a ejecutar en el sistema
    bool ip_valida = false; // Definir una bandera para determinar si se encontr� una IP accesible
    FILE* cmd; // Variable para almacenar la salida del comando ejecutado

    // Comprobar si se puede abrir el archivo DNSips.txt en modo lectura
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo\n"); // Imprimir mensaje de error si no se pudo abrir el archivo
        return; // Salir de la funci�n
    }

    // Leer cada l�nea del archivo
    while (fgets(buffer, TAMA�O_BUFFER, archivo)) {
        sscanf(buffer, "%s", ip); // Obtener la direcci�n IP de la l�nea
        printf("Comprobando... IP: %s\n", ip); // Imprimir la direcci�n IP

        // Ejecutar comando de ping
        comando[0] = '\0'; // Reiniciar la variable en cada iteraci�n
        strcat(comando, "ping -n 1 -w 1000 ");// Se concatena a la cadena command el comando de ping,"-n 1" se env�a solo un paquete "-w 1000" tiempo de espera en milisegundos.
        strcat(comando, ip); // Se concatena a la cadena commando la direcci�n IP que se quiere comprobar
        cmd = _popen(comando, "r");
        
        // Leer la salida del comando de ping
        while (fgets(buffer, TAMA�O_BUFFER, cmd)) {}

        // Comprobar resultado del comando de ping
        if (_pclose(cmd) == 0) { // Si el comando de ping se ejecut� correctamente
            printf("La IP %s es accesible\n\n", ip); // Imprimir mensaje de IP accesible
            fprintf(temporal, "%s\n", ip); // Escribir la direcci�n IP en el archivo temporal.txt
            ip_valida = true; 
        }
        else {
            printf("La IP %s no es accesible\n\n", ip); // Imprimir mensaje de IP no accesible
        }
    }

    // Si ninguna IP es accesible
    if (!ip_valida) {
        printf("No se encontraron IPs accesibles\n\n"); // Imprimir mensaje
    }

    // Cerrar archivos
    fclose(archivo);
    fclose(temporal);

    system("pause");
    system("cls"); // Limpiamos consola
}

// Acci�n que checkea la velocidad de las IPs de DNS validas incluyendo la configurada que estan en el archivo temporal
void checkearVelocidad() {
    char buffer[TAMA�O_BUFFER]; // Variable que se utiliza para almacenar temporalmente datos que se leen desde un archivo y almacena las IP de los DNS
    char comando[TAMA�O_BUFFER] = "", velocidad[5]; // Variable que se utiliza para almacenar el comando que se va a ejecutar y variable donde se alamacena la velocidad media en caracteres
    char salidaCMD[TAMA�O_BUFFER]; // Variable para leer la salida almacenada del comando ejecutado "ping"
    char* puntero; // Variable que se utiliza para apuntar a la velocidad media del comando ping
    char* token; // Variable para separa la direcci�n IP de caracter de salto de l�nea
    char* adaptador[20] = { NULL }; // Variable para almacenar el nombre de adaptador seleccionado para reconfigurar los DNS
    FILE* archivoTemporal = fopen("temporal.txt", "r"); // Archivo donde se encuentran las IPs validas de la funcion anterior
    FILE* archivoDNS = fopen("DNSconf.txt", "r"); // Archivo donde se encuentra la IP configurada
    FILE* cmd; // Variable para almacenar la salida del comando ejecutado
    char ipDnsConf[BUFFER_IP]; // variable para almacenar la direcci�n IP
    int velocidadDnsConf = 0, empates = 0, numDns = 0; // variable para almacenar la velocidad media / Si empatan 2 velocidades / Contador de IPs que hay en la lista
    char dnsMasRapido[TAMA�O_BUFFER] = "", dnsEmpate[TAMA�O_BUFFER] = ""; // Variable para almacenar la direcci�n IP de la configuraci�n DNS m�s r�pida
    int velocidadMasRapida = 1000; // Variable para almacenar la velocidad de la configuraci�n DNS m�s r�pida
    bool pingExitoso = false; // Variable para determinar si se ha podido hacer ping a la IP configurada

    if (archivoDNS == NULL) {  // Mensaje de error en caso de no poder abrir el archivo donde se encuentra la IP del adaptador configurado
        printf("Error al abrir el archivo con la configuraci�n DNS\n");
        return;
    }
    else {
        // Leemos la direcci�n IP del DNS configurado y la almacenamos en la variable "Adaptador"
        fgets(buffer, TAMA�O_BUFFER, archivoDNS);
        token = strtok(buffer, " \t\n"); // divide la cadena en palabras usando espacios, tabuladores y saltos de l�nea como delimitadores
        if (token != NULL) {
            strcpy(adaptador, token);
        }
            fgets(buffer, TAMA�O_BUFFER, archivoDNS);
            printf("\nLa direccion IP del DNS configurado es: %s", buffer);
            printf("Comprobando velocidad media de la direccion IP... \n");

            // Ejecutamos comando ping
            
            comando[0] = '\0'; // Reiniciamos la variable en cada iteraci�n
            strcat(comando, "ping "); // Se concatena a la cadena command el comando de ping
            strcat(comando, buffer); // Se concatena a la cadena commando la direcci�n IP configurada
            cmd = _popen(comando, "r");
            while (fgets(salidaCMD, TAMA�O_BUFFER, cmd)) {
                // Buscamos la velocidad media en el resultado del comando
                puntero = strstr(salidaCMD, "Media = ");
                if (puntero != NULL) {
                    sscanf(puntero + 8, "%s", velocidad); // Movemos 8 caracteres a la derecha para obtener lo que nos interesa

                    // Almacenamos la direcci�n IP y la velocidad media en la estructura DnsInfo
                    token = strtok(buffer, "\n");
                    strcpy(ipDnsConf, buffer);
                    velocidadDnsConf = atoi(velocidad);

                    // Designamos la IP y la velocidad como la mas rapida, puesto que inicialmente est� configurado de esa manera 
                    if (velocidadDnsConf != 0) {
                        velocidadMasRapida = velocidadDnsConf;
                        strcpy(dnsMasRapido, ipDnsConf);
                    }
                    pingExitoso = true; // Marcamos que se ha podido hacer ping a la IP configurada
                }
            }

            if (!pingExitoso) {
                printf("No se ha podido hacer ping a la IP configurada: %s\n", buffer);
            }
    }// Cerramos el archivo 
    fclose(archivoDNS);

    // Leemos las direcciones IP del archivo temporal
    while (fgets(buffer, TAMA�O_BUFFER, archivoTemporal)) {
        printf("\nSe va a comprobar la velocidad de esta direccion IP: %s", buffer);
        printf("Comprobando velocidad media de la direccion IP... \n");

        // Ejecutamos el comando ping
        comando[0] = '\0'; // Reiniciamos la variable en cada iteraci�n
        strcat(comando, "ping "); // Se concatena a la cadena command el comando de ping
        strcat(comando, buffer); // Se concatena a la cadena commando la direcci�n IP configurada
        cmd = _popen(comando, "r");
        while (fgets(salidaCMD, TAMA�O_BUFFER, cmd)) {
            
            // Buscamos la velocidad media en el resultado del comando
            puntero = strstr(salidaCMD, "Media = ");
            if (puntero != NULL) {
                sscanf(puntero + 8, "%s", velocidad);
                
                // Almacenamos la direcci�n IP y la velocidad media en la estructura DnsInfo
                DnsInfo dns;
                token = strtok(buffer, "\n");
                strcpy(dns.ip, buffer);
                dns.velocidad = atoi(velocidad);
                listaDnsInfo[numDns] = dns;
                numDns++;
                
                if (dns.velocidad == velocidadMasRapida) {
                    empates++;
                    strcpy(dnsEmpate, dns.ip);
                }
                // Si se cumple la condicion de que la velocidad que se testea es menor que la velocidad mas rapida, se asigna esa velocidad como la mas rapida junto su IP
                if (dns.velocidad < velocidadMasRapida) {
                    velocidadMasRapida = dns.velocidad;
                    strcpy(dnsMasRapido, dns.ip);
                }
            }
        }
    }// Cerramos el archivo
    fclose(archivoTemporal);
    
    // Mostramos la IP i velocidad del DNS configurado en primer lugar
    if (pingExitoso == true) {
        printf("\nIP: %s\nVelocidad media: %d ms\n", ipDnsConf, velocidadDnsConf);
    }
    
    // Mostramos la informaci�n almacenada en la estructura DnsInfo
    for (int i = 0; i < numDns; i++) {
        printf("\nIP: %s\n", listaDnsInfo[i].ip);
        printf("Velocidad media: %d ms\n", listaDnsInfo[i].velocidad);
    }
    
    // Si la velocidad mas rapida es inferior a la velocidad de la IP que ya esta configurada procedemos a cambiarla
    if (velocidadMasRapida < velocidadDnsConf && velocidadDnsConf != 0 && empates == 0) {
        char respuesta[3];
        printf("\nLa velocidad actual del DNS es mas lenta. Desea cambiarla por la mas rapida? (si/no): ");
        scanf("%2s", respuesta); // lee hasta dos caracteres (el tama�o de respuesta es 3)
        while (getchar() != '\n'); // limpia el b�fer del teclado
        if (strcmp(respuesta, "si") == 0) { // compara las cadenas con strcmp
            modificarDNS(adaptador, dnsMasRapido); // se llama a la funcion para cambiar la configuraci�n
        }
        else {
            printf("Si necesitas cambiar la configuracion del servidor DNS ejecuta de nuevo la aplicacion. "); // mensaje en caso de no querer modificar los DNS
            system("pause");
            return;
        }
    } // Si hay a�lg�n empate en la velocidades, se comprobara el numero de saltos
    else if (empates > 0) {
        printf("\nHay dos DNS con la misma velocidad se va chequear el numero de saltos... los DNS empatados son: %s y %s con una velocidad de %d ms\n\n", dnsMasRapido, dnsEmpate, velocidadMasRapida);
        int saltosDnsMasRapido = 0, saltosDnsEmpate = 0;
        
        saltosDnsMasRapido = contadorSaltos(dnsMasRapido);
        printf("Numeros de salto del DNS mas rapido: %d \n", saltosDnsMasRapido);
        saltosDnsEmpate = contadorSaltos(dnsEmpate);
        printf("Numeros de salto del DNS empatado: %d \n", saltosDnsEmpate);

        if (saltosDnsMasRapido < saltosDnsEmpate) {
            
            // Si el DNS mas rapido tiene un numero de salto menor que el DNS empatado, comprobamos que sea el configurado, de lo conrtario se modifica
            printf("\nEl DNS %s tiene un numero menor de saltos, por lo tanto es el mas rapido. \n", dnsMasRapido);
            if (strcmp(ipDnsConf, dnsMasRapido) == 0) {
                printf("\nEl DNS %s ya estaba configurado inicialmente. \n", ipDnsConf);
                system("pause");
            }
            else { // En caso de que el DNS mas rapido no sea el que estaba configurado lo modificamos
                printf("\nSe va a modificar el DNS...");
                system("pause");
                modificarDNS(adaptador, dnsMasRapido);
            }
        }   
        // SI el numero de saltos es el mismo modificamos el primer DNS encontrado mas rapido
        else if (saltosDnsMasRapido == saltosDnsEmpate) {
            printf("\nLos DNS tienen el mismo numero de saltos, por lo tanto se puede considerar que la velocidad es la misma. \n");
            printf("\nSe va a configurar el primer DNS mas rapido que se ha encontrado. \n");
            system("pause");
            modificarDNS(adaptador, dnsMasRapido);
        }
        // Si el DNS empatado tiene menor numero de saltos lo modificamos
        else {
            printf("\nEl DNS %s tiene un numero menor de saltos, por lo tanto es el mas rapido. \n", dnsEmpate);
            system("pause");
            modificarDNS(adaptador, dnsEmpate);
        }
    } 
    // Si la IP del DNS configurado es diferente a la del mas rapido se modifica como son punteros se utiliza strcmp
    else if (strcmp(ipDnsConf, dnsMasRapido) != 0) {
        printf("ip dns confi: %s  ip dns mas rapido: %s", ipDnsConf, dnsMasRapido);
        printf("\nLa direccion IP %s es la mas rapida con una velocidad de %d ms, puesto que el DNS actual no puede hacer ping reconfiguramos el DNS\n\n", dnsMasRapido, velocidadMasRapida);
        system("pause");
        modificarDNS(adaptador, dnsMasRapido);
    }
    // Si la IP configurada es la mas rapida mostramos mensaje informativo
    else {
        printf("\nLa direccion IP %s es la mas rapida con una velocidad de %d ms, por lo tanto no es necesario que se modifique el servidor DNS\n\n", dnsMasRapido, velocidadMasRapida);
    }
    system("pause");
    return;
}


