#ifndef REMOTEMEMORY_H
#define REMOTEMEMORY_H

#include "ConectorCliente.h"
#include "jsoncpp/json/json.h"
#include <cstring>

class RemoteMemory {
private:
	ConectorCliente conectorcliente;
	static RemoteMemory* instance;

	RemoteMemory() = default;

public:
	static RemoteMemory* ObtenerInstance() {
		if (instance == nullptr) {
			instance = new RemoteMemory();
			return instance;
		}
		return instance;
	}

	void RAgregarInstancia(const char* type, std::string& id);
	void RAumentarInstancia(std::string& id);
	void RDisminuirInstancia(std::string& id);

	template<typename T>
	void setValor(T& NuevoValor, std::string& id);
	std::string getValor(std::string& id);
	std::string POST(Json::Value& root);

	template<typename T>
	T ConvertirValor(std::string & valor);
};

template<typename T>
void RemoteMemory::setValor(T& NuevoValor, std::string& id) {
	std::cout << "Cambiando valor remoto" << "\n";
	Json::Value root;
	root["COMMAND"] = "SET";
	root["id"] = id;
	root["Nuevo Valor"] = NuevoValor;
	std::cout << root.get("COMMAND", "DEFAULT").asString() << "\n";
	POST(root);
}

template<typename T>
T RemoteMemory::ConvertirValor(std::string& valor) {
	try {
		if (typeid(T) == typeid(int)) {
			return std::atoi(valor.c_str());
		}
		else if (typeid(T) == typeid(float)) {
			return std::stof(valor.c_str());
		}
		else if (typeid(T) == typeid(double)) {
			return std::stof(valor.c_str());
		}
		else if (typeid(T) == typeid(long)) {
			return std::atol(valor.c_str());
		}
		else if (typeid(T) == typeid(bool)) {
			return std::atoi(valor.c_str());
		}
		else if (typeid(T) == typeid(char)) {
			return *valor.c_str();
		}
	}
	catch (std::exception& err) {
		std::cout << "Valor no compatible";
	}
}
#endif // !REMOTEMEMORY_H