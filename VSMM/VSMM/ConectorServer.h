#ifndef CONECTORSERVER_H
#define CONECTORSERVER_H

#include <iostream>
#include "jsoncpp/json/json.h"

class ConectorServer {
private:
	int CurrentSocket;
	std::string contra = "GCSERVER";
	static void EnviarMensaje(int ServidorCliente, std::string& buffer);

public:
	ConectorServer() = default;
	~ConectorServer() = default;

	void CrearConector();
	void Aumentar(std::string id);
	void Disminuir(std::string id);
	void EvaluarJson(Json::Value& info);
};

#endif // !CONECTORSERVER_H
