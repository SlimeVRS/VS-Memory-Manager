#include <iostream>
#include <sstream>

class VSPtrInfo {
public:
	std::string ID;
	int count_ref;
	std::string type;

public:
	virtual void* ObtenerID() = 0;
	virtual std::string ObtenerValor() = 0;
	virtual void DefinirValor(std::string NuevoValor) = 0;
	virtual ~VSPtrInfo() {
	};
};

template<typename T>
class VSPointerInstance : public VSPtrInfo {
public:
	T instance;

public:
	explicit VSPointerInstance(T instance, std::string& ID);
	void* ObtenerID() override {
		return instance;
	};

	std::string ObtenerValor() override {
		std::stringstream str;
		str << (*instance);
		return str.str();
	}

	void DefinirValor(std::string NuevoValor) {
		std::cout << "Valor antiguo" << NuevoValor << "\n";
		std::stringstream str(NuevoValor);
		str >> (*instance);
		std::cout << "Nuevo Valor" << *instance << "\n";
	}
	std::string ObtenerTipo(T& type);
	~VSPointerInstance();
};

template<typename T>
VSPointerInstance<T>::~VSPointerInstance() {
	delete instance;
}

template<typename T>
VSPointerInstance<T>::VSPointerInstance(T instance, std::string& ID){
	VSPtrInfo::count_ref = 1;
	VSPtrInfo::ID = ID;
	VSPtrInfo::type = ObtenerTipo(instance);
	this->instance = instance;
}
template<typename T>
std::string VSPointerInstance<T>::ObtenerTipo(T& type) {
	std::string tipo = typeid(type).name();

	if (tipo == "Pv") {
		return "void";
	}
	else if (tipo == "Pi") {
		return "int";
	}
	else if (tipo == "Pb") {
		return "bool";
	}
	else if (tipo == "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE") {
		return "string";
	}
	else if (tipo == "Pc") {
		return "char";
	}
	else if (tipo == "Pf") {
		return "float";
	}
	else if (tipo == "Pl") {
		return "long";
	}
	else if (tipo == "Pd") {
		return "double";
	}
	else {
		return tipo;
	}
}