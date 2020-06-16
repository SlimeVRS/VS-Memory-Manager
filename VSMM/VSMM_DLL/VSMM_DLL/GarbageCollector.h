#ifndef GARBAGECOLLECTOR_HEADER
#define GARBAGECOLLECTOR_HEADER

#include <thread>
#include <mutex>
#include <map>
#include "VSPointerInstance.h"
#include "UU_ID.h"
#include <fstream>
#include <any>
#include <iostream>
#include <stdio.h>
#include "jsoncpp/json/json.h"


class GarbageCollector {
private:
	static GarbageCollector* garbageCollector;
	std::thread GCThread;
	mutable std::mutex mutex_;
	std::map <std::string, VSPtrInfo*> mapGC;
	GarbageCollector();

public:
	static GarbageCollector* ObtenerGCInstance();
	std::string GenerarID() const;
	template<typename T>
	void AgregarInstance(const T& valor, std::string ID);
	void AumentarContador(const std::string& id);
	void ReducirContador(const std::string& id);
	void PrintGCInfo();
	void EjecutarGC();
	void GenerarJSON();
	std::thread THREAD();
	void setValor(std::string NuevoValor, std::string& id);
	std::string getValor(std::string& id);
};
template<typename T>

void AgregarInstance(const T& valor, std::string ID) {
	VSPointerInstance<T> VSPointerI = new VSPointerInstance<T>(valor, ID);
}

#endif // !1