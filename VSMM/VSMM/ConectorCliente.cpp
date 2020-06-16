#include "ConectorCliente.h"
#include <fstream>
#include <cstring>
#include <iomanip>

void ConectorCliente::CrearSocket() {
	try {
		//ConectorCliente::cliente = socket(AF_INET, SOCK_STREAM, 0);
		if (!ClienteCreado()) {
			throw std::exception();
		}
		ClienteConectado();
	}
	catch (std::exception err) {
		std::cout << "El cliente no se pudo crear:" << err.what() << "\n";
	}
}


bool ConectorCliente::ClienteCreado() const {
	return cliente != -1;
}

bool ConectorCliente::ClienteConectado() const {
	return CONEXION != -1;
}

void ConectorCliente::ConectarClienteServidor() {
	hint.sin_family = AF_INET;
	//hint.sin_port = htons(PORT);
	try {
		//inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);
		//CONEXION = connect(cliente, (sockaddr*)&hint, sizeof(hint));
		if (!ClienteCreado()) {
			throw std::exception();
		}
		std::cout << "Cliente conectado!";
	}
	catch (std::exception& err) {
		std::cout << "Error al conectar el cliente" << err.what() << "\n";
	}
}

std::string ConectorCliente::EnviarInfo(char* mensaje) {
	CrearSocket();
	//Mensaje_Enviado = send(cliente, mensaje, strlen(mensaje) + 1, 0);
	try {
		if (!MensajeEnviado()) {
			throw std::exception();
		}
		return MensajeRecibido(); 
	}
	catch (std::exception& err) {
		std::cout << "Error al enviar información" << err.what() << "\n";
	}
	return std::string("");
}

std::string ConectorCliente::MensajeRecibido() const {
	try {
		char bufferRecibido[1024];
		memset(bufferRecibido, 0, 1024);
		//int bytesRecibidos = recv(cliente, bufferRecibido, 1024, 0);
		_close(cliente);
		//return std::string(bufferRecibido, bytesRecibidos);
	}
	catch (std::exception& err) {
		std::cout << "Error al recibir el mensaje del servidor: " << err.what() << "\n";
	}
	return std::string("0", "0");
}

bool ConectorCliente::MensajeEnviado() const {
	return Mensaje_Enviado != -1;
}

void ConectorCliente::setIP(std::string& ip) {
	this->IP = IP;
}

void ConectorCliente::setPORT(int PORT) {
	this->PORT = PORT;
}

void ConectorCliente::setContra(std::string& contra) {
	//definir
}

/*
ConectorCliente::ConectorCliente() {

}
*/