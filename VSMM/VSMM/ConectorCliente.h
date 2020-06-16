#ifndef CONECTORCLIENTE_H
#define CONECTORCLIENTE_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <WinSock2.h>
#include <io.h>
#include <sys/types.h>
#include <WS2tcpip.h>
#include <string>

class ConectorCliente {
private:
	unsigned int PORT = 54000;
	std::string IP = "127.0.0.1";
	std::string password;
	int cliente;
	int CONEXION;
	int Mensaje_Enviado;
	sockaddr_in hint;

	bool ClienteCreado() const;
	void ConectarClienteServidor();
	bool ClienteConectado() const;
	bool MensajeEnviado() const;
	std::string MensajeRecibido() const;

public:
	//ConectorCliente();
	~ConectorCliente() = default;
	void CrearSocket();
	std::string EnviarInfo(char* mensaje);
	void setPORT(int PORT);
	void setIP(std::string& IP);
	void setContra(std::string& contra);
};

#endif // !CONECTORCLIENTE_H