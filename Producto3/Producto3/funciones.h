#ifndef funciones

#define funciones
#define BUFFER_IP 16
#define TAMAÑO_BUFFER 512
#define ARCHIVO_TEMPORAL "temporal.txt"
#define ARCHIVO_DNS "DNSConf.txt"
#define ARCHIVO_ADAPTADORES "adaptadores.txt"
#define ARCHIVO_DNSIPS "DNSips.txt"

void mostrarArchivo(char nombreArchivo[TAMAÑO_BUFFER]);
void mostrarAdaptador();
void modificarDNS(char adaptador[20], char dnsMasRapido[BUFFER_IP]);
int contadorSaltos(const char* ip);
void comprobarIPs(char ips[TAMAÑO_BUFFER]);
void checkearVelocidad();


#endif