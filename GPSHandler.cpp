/**PROGRAMA GPSHANDLER.CPP
* Programa que mediante un objeto del tipo GPS que contiene Mutex, un buffer dedicado 
* y un objeto que contiene los datos obtenidos de RTKLIB enviados mediante sockets, 
* puede entregar la información de manera ordenada para su respectivo uso.
*
* Por Alex A. Turriza Suárez - 2017
**/

#include <iostream>
#include <sys/socket.h> //Usado por socket()
//#include <unistd.h> //Usado por fork(), ya inicializado en gps.h
//#include <errno.h> //Usado por errno, ya inicializado en gps.h
#include <netinet/in.h> //Declaración de IPPROTO_TCP y de inet_aton()
#include <arpa/inet.h> //Usado por inet_aton()
//#include <cstdio> //Usado por perror, ya inicializado en gps.h
#include <thread> //Usado para manejar diferentes hilos
#include "gps.h" //Usado por objeto GPS

#ifndef _PUERTORTKLIB_
#define _PUERTORTKLIB_ 8988
#endif

#ifndef _DIRECHOST_
#define _DIRECHOST_ "127.0.0.1"
#endif

/**CONJUNTO DE FUNCIONES DEDICADAS AL MANEJO DEL SOCKET QUE CONECTARÁ A RTKLIB**/
int openSocket()
{
	/*Función que crea un socket y devuelve el descriptor*/
	int filedes;
	if((filedes = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("socket() ERROR");
		return -1;
	}
	return filedes;
}

int conecta(char * Direccion, int puerto, int sockfd)
{
	/*Función que conecta un socket a una dirección dada y un determinado puerto*/
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(puerto);
	if(!(inet_aton(Direccion, (struct in_addr *) &addr.sin_addr.s_addr)))
	{
		std::cout << "inet_aton() ERROR: Dirección no válida" << std::endl;
		return (-1);
	}
	if(connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
	{
		perror("connect() ERROR");
		return (-1);
	}
	return 0;
}
/**FIN DE CONJUNTO DE FUNCIONES PARA MANEJO DE SOCKET**/

/**FUNCIONES PARA MANEJO DE INFORMACIÓN DEL GPS MEDIANTE THREADS**/
void actualiza(GPS * ugps, int fd)
{
	while(true)
	{
		ugps->actualizaDatos(fd);
	}
}

void imprimeGPS(GPS * ugps)
{
	while(true)
	{
		if(ugps->fix())
		{
			ugps->lockMutex();
			std::cout << "Latitud: " << ugps->getLatitud() << std::endl;
			std::cout << "Longitud: " << ugps->getLongitud() << std::endl;
			std::cout << "Altura: " << ugps->getAltitud() << std::endl;
			std::cout << "Número de Satélites: " << ugps->getNumSat() << std::endl << std::endl;
			ugps->unlockMutex();
			sleep(1);
		}
		else
		{
			std::cout << "Esperando señal ..." << std::endl;
			sleep(1);
		}
	}
}

int main()
{
	GPS ubloxC94;
	int cPID, fd;
	if(!(cPID = fork()))
	{
		//Inicia rutina del programa hijo. Aquí se invoca a RTKLIB-RTKRCV.
		if(chdir("/home/alexrt07/RTKLIB-rtklib_2.4.3/app/rtkrcv/gcc"))
		{
			perror("chdir() ERROR:");
			return (-1);
		}
		else
		{
			if(execl("rtkrcv", "rtkrcv", "-s", "-o", "uuuu.conf", NULL))
			{
				perror("Execl() ERROR:");
				return (-1);
			}
		}
	}
	else
	{
		sleep(2); //Damos tiempo a que RTKLIB inicie correctamente.
		if((fd = openSocket()) >= 0)
		{
			if(!(conecta(_DIRECHOST_, _PUERTORTKLIB_, fd)))
			{
				//Conexión exitosa.
				std::cout << "Conexión exitosa" << std::endl;

				//RUTINA DE MANEJO DE GPS
				std::thread(actualiza, &ubloxC94, fd).detach();
				std::thread(imprimeGPS, &ubloxC94).detach();
			}
		}
		if(close(fd))
		{
			perror("close() socket ERROR");
			return (-1);
		}
	}
	return 0;
}
