#pragma warning(disable:4996)
#include "ConectorServer.h"
#include "GarbageCollector.h"

#include <iostream>
#include <WinSock2.h>
#include <string>
#include <string.h>
#include <time.h>
#include <iomanip>
#include <WS2tcpip.h>
#include <io.h>

#define PORT 54000

void ConectorServer::CrearConector() {
	int opt = true;
	int master_socket, addrlen, newsocket, client_socket[30], max_clients = 30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address {};
	fd_set readfds;

	for (i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
	}

	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("El conector fallido");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("Fallo de enlace");
		exit(EXIT_FAILURE);
	}
	std::cout << "Puerto en : %d \n", PORT;

	if (listen(master_socket, 3) < 0) {
		perror("Listen");
		exit(EXIT_FAILURE);
	}

	addrlen = sizeof(address);
	puts("Esperando por conexiones ...");
	while (true) {
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];
			if (sd > 0) {
				FD_SET(sd, &readfds);
			}
			else if (sd > max_sd) {
				max_sd = sd;
			}
		}

		activity = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
		if ((activity < 0) && (errno != EINTR)) {
			std::cout << "Seleccionar error \n";
		}

		if (FD_ISSET(master_socket, &readfds)) {
			if ((newsocket = accept(master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
				perror("accpet");
				exit(EXIT_FAILURE);
			}
			CurrentSocket = newsocket;
			std::cout << "New connection, socket fd is% d, ip is : % s, port : % \n", newsocket, ntohs(address.sin_port);
			char mensaje[100];
			if (send(newsocket, mensaje, strlen(mensaje), 0) != strlen(mensaje)) {
				perror("Enviar");
			}
			puts("Mensaje de bienvenida enviado con exito");

			for (i = 0; i < max_clients; i++) {
				if (client_socket[i] == 0) {
					client_socket[i] = newsocket;
					std::cout << "Agergando a la lista de conectores como %d\n", i;
					break;
				}
			}
		}

		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds)) {
				char buf[4096];
				valread = _read(sd, buf, 1024);
				std::string data = std::string(buf, 0, valread);
				std::cout << data << std::endl;

				if (data.compare("Conexion hecha")) {
					std::cout << "\n";
				}

				if (valread == 0) {
					getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
					std::cout << "Servidor desconectado, ip %s, port %d \n", ntohs(address.sin_port);
					_close(sd);
					client_socket[i] = 0;
				}
				else {
					Json::Reader reader;
					Json::Value obj;
					reader.parse(data, obj);
					EvaluarJson(obj);
				}
			}
		}
	}
}

void ConectorServer::EnviarMensaje(int ServidorCliente, std::string& buffer) {
	const char* buf = buffer.c_str();
	send(ServidorCliente, buf, strlen(buf) + 1, 0);
}

void ConectorServer::Aumentar(std::string ID) {
	GarbageCollector::ObtenerGCInstance()->AumentarContador(ID);
}

void ConectorServer::Disminuir(std::string ID) {
	GarbageCollector::ObtenerGCInstance()->ReducirContador(ID);
}

void CreadorInstancias(std::string& type, std::string& id) {
	if (type == "i") {
		int* temp = new int{};
		GarbageCollector::ObtenerGCInstance()->AgregarInstance(temp, id);
	}
	else if (type == typeid(std::string).name()) {
		auto* temp = new std::string{};
		GarbageCollector::ObtenerGCInstance()->AgregarInstance(temp, id);
	}
	else if (type == "f") {
		float* temp = new float{};
		GarbageCollector::ObtenerGCInstance()->AgregarInstance(temp, id);
	}
	else if (type == "b") {
		bool* temp = new bool{};
		GarbageCollector::ObtenerGCInstance()->AgregarInstance(temp, id);
	}
	else if (type == "l") {
		long* temp = new long{};
		GarbageCollector::ObtenerGCInstance()->AgregarInstance(temp, id);
	}
	else if (type == "c") {
		char* temp = new char{};
		GarbageCollector::ObtenerGCInstance()->AgregarInstance(temp, id);
	}
	else if (type == "d") {
		double* temp = new double{};
		GarbageCollector::ObtenerGCInstance()->AgregarInstance(temp, id);
	}
}

void ConectorServer::EvaluarJson(Json::Value& info) {
	Json::StreamWriterBuilder builder;
	Json::FastWriter fastWriter;
	Json::Value obj = info["VSPointerInfo"];

	std::cout << "COMMAND" << info["COMMAND"] << "\n";
	if (info["COMMAND"] == "ADD") {
		std::cout << "Agregando al GC";
		std::string type = fastWriter.write(obj["type"]);
		std::string ID = fastWriter.write(obj["ID"]);
		type = type.substr(1, 1);
		CreadorInstancias(type, ID);
		GarbageCollector::ObtenerGCInstance()->PrintGCInfo();
		std::string mensaje = "Instancia agergada";
		EnviarMensaje(CurrentSocket, mensaje);
	}
	if (info["COMMAND"]) {
		//ndad
	}
}