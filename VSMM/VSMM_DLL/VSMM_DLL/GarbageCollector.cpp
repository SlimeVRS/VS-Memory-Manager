#include "GarbageCollector.h"

GarbageCollector* GarbageCollector::garbageCollector = nullptr;

std::thread GarbageCollector::THREAD() {
	return std::thread(&GarbageCollector::EjecutarGC, this);
}

GarbageCollector::GarbageCollector() {
	GCThread = THREAD();
}

GarbageCollector* GarbageCollector::ObtenerGCInstance() {
	if (garbageCollector == nullptr) {
		printf("Se ha creado la instancia del GC");
		garbageCollector = new GarbageCollector();
	}
	printf("me la pelas");
	return  garbageCollector;
}

std::string GarbageCollector::GenerarID() const {
	return UU_ID::GenerarUUID();
}

void GarbageCollector::PrintGCInfo() {
	printf("\n________________________________\n");
	printf(" LISTA DEL GC \n");
	printf("|\t id \t |\t Count Ref \t | \t address \t| \t type \t | \t value \t | \n");
	for (std::map<std::string, VSPtrInfo*>::iterator i = mapGC.begin(); i != mapGC.end(); i++) 
	{
		printf("|\t %s \t|\t %d \t|\t %p \t|\t %s \t|\t %s \t|\n", i->second->ID.c_str(), i->second->count_ref, i->second->ObtenerID(), i->second->type.c_str(), i->second->ObtenerValor().c_str());
	}
	printf("\n________________________________\n");
}
void GarbageCollector::AumentarContador(const std::string& id) {
	std::unique_lock<std::mutex> lock(mutex_);
	if (!(mapGC.find(id) == mapGC.end())) {
		VSPtrInfo* current = mapGC[id];
		current->count_ref++;
		printf("+1 al contador: id: %s count_ref: %d \n", current->ID.c_str(), current->count_ref);
	}
}
void GarbageCollector::ReducirContador(const std::string& id) {
	std::unique_lock<std::mutex> lock(mutex_);
	if (!(mapGC.find(id) == mapGC.end())) {
		VSPtrInfo* current = mapGC[id];
		current->count_ref--;
		printf("Disminuye el contador: id: %s contador: %d \n", current->ID.c_str(), current->count_ref);
	}
}
void GarbageCollector::EjecutarGC() {
	try {
		while (true) {
			std::this_thread::sleep_for(std::chrono::seconds(3));
			PrintGCInfo();
			std::unique_lock<std::mutex>locker(mutex_);
			printf("Ejecutando hilo 1 \n");
			for (std::map<std::string, VSPtrInfo*>::iterator i = mapGC.begin(); i != mapGC.end(); i++) {
				if (i->second->count_ref == 0) {
					VSPtrInfo* vsPtrInfo = i->second;
					printf("Eliminadno... %s", vsPtrInfo->ID.c_str());
					mapGC.erase(i->second->ID);
					delete vsPtrInfo;
				}
			}
			GenerarJSON();
			printf("Termiando... \n");
			printf("________________________");
			locker.unlock();
		}
	}
	catch (const std::exception& err) {
		printf("Error %s", err.what());
		GCThread = THREAD();
	}
}
/*
void GarbageCollector::GenerarJSON() {
	Json::StyledStreamWriter writer;
	Json::Value Mi_lista;
	Json::Value vec(Json::arrayValue);
	for (std::map<std::string, VSPtrInfo*>::iterator i = mapGC.begin(); i != mapGC.end(); i++) {
		VSPtrInfo* current = i->second;
		Json::Value obj;

		std::ostringstream Direccion;
		Direccion << current->ObtenerID();
		obj["id"] = current->ID;
		obj["address"] = Direccion.str();
		obj["type"] = current->type;
		obj["valor"] = current->ObtenerValor();
		obj["contador"] = std::to_string(current->count_ref);
		vec.append(obj);
	}
	Mi_lista["VSPointer"] = vec;
	std::ofstream file("./ArchivosPrueba/VSPtr.json");
	writer.write(file, Mi_lista);
	file.close();
}
*/
void GarbageCollector::GenerarJSON() {
	Json::Value Mi_lista;
	Json::Value vec(Json::arrayValue);
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "     ";
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	for (std::map<std::string, VSPtrInfo*>::iterator i = mapGC.begin(); i!=mapGC.end(); i++) {
		VSPtrInfo* current = i->second;
		Json::Value obj;
		
		std::ostringstream Direccion;
		Direccion << current->ObtenerID();
		obj["id"] = current->ID;
		obj["address"] = Direccion.str();
		obj["type"] = current->type;
		obj["valor"] = current->ObtenerValor();
		obj["contador"] = std::to_string(current->count_ref);
		vec.append(obj);
	}
	Mi_lista["VSPointer"] = vec;
	std::ofstream file("./ArchivosPrueba/VSPtr.json");
	writer->write(Mi_lista, &std::cout);
	std::cout << std::endl;
	file.close();
}


void GarbageCollector::setValor(std::string newValue, std::string& id) {
	std::cout << "DefinirValor" << newValue << "\n";
	//mapGC[id]->DefinirValor(newValue);
}