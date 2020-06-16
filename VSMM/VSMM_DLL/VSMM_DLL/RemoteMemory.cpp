#include <sstream>
#include "RemoteMemory.h"

RemoteMemory* RemoteMemory::instance = nullptr;

void RemoteMemory::RAumentarInstancia(std::string& id) {
	Json::Value root;
	root["COMMAND"] = "AUMENTAR";
	root["ID"] = id;
	POST(root);
}

void RemoteMemory::RDisminuirInstancia(std::string& id) {
	Json::Value root;
	root["COMMAND"] = "DISMINUIR";
	root["ID"] = id;
	POST(root);
}

std::string RemoteMemory::POST(Json::Value& root) {
	std::string Mensaje = root.toStyledString();
	std::string ValorRecibido = conectorcliente.EnviarInfo(const_cast<char *>(Mensaje.c_str()));
	return ValorRecibido;
}

void RemoteMemory::RAgregarInstancia(const char* type, std::string& id) {
	Json::Value root;
	root["COMMAND"] = "ADD";
	Json::Value Info;
	Info["ID"] = id;
	Info["type"] = type;
	root["VSPtrInfo"] = Info;
	POST(root);
}

std::string RemoteMemory::getValor(std::string& id) {
	Json::Value root;
	root["COMMAND"] = "GET";
	root["id"] = id;
	std::string Valor = POST(root);
	return Valor;
}