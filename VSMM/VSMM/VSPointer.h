#ifndef GC_VSPOINTER_H
#define GC_VSPOINTER_H

#include <iostream>
#include "GarbageCollector.h"
#include "RemoteMemory.h"
#include <typeinfo>

template<class T>
class VSPtr {
private:
    T* addr;
    std::string id;
    bool is_remote = false;
    std::string get_id();
    T* get_ptr();
    void addInstanceLocally();
    void addInstanceRemotely();
    void setVSPtrIdFromGarbageCollector();
    void decrementRefCountLocally(std::string id);
    void decrementRefCountRemotely(std::string id);
    void incrementRefCountLocally(std::string id);
    void incrementRefCountRemotely(std::string id);
public:
    explicit VSPtr(bool is_remote) {
        printf("Constructor\n");
        std::cout << (is_remote ? "Remoto" : "Local") << "\n";
        is_remote ? addInstanceRemotely() : addInstanceLocally();
    }
    VSPtr(const VSPtr& other) {
        std::cout << other.id << "\n";
        addr = other.addr;
        printf("Copia \n");
    }
    ~VSPtr() {
        this->is_remote ? decrementRefCountRemotely(id) : decrementRefCountLocally(id);
    }
    VSPtr<T>& operator=(VSPtr<T>& other) {
        printf("Pointer to Pointer %d \n", other.is_remote);
        this->is_remote ? decrementRefCountRemotely(id) : decrementRefCountLocally(id);
        addr = other.get_ptr();
        id = other.get_id();
        std::cout << "NEW ID " << id;
        other.is_remote ? incrementRefCountRemotely(id) : incrementRefCountLocally(id);
        other.is_remote ? this->is_remote = true : this->is_remote = false;
        GarbageCollector::ObtenerGCInstance()->PrintGCInfo();
        return *this;
    }
    void operator=(T element) {
        if (!is_remote) {
            *addr = element;
        }
        else {
            RemoteMemory::ObtenerInstance()->setValor(element, id);
        }
    }
    T& operator&() {
        if (!is_remote) {
            T* value = new T{};
            std::string val = "123456";
            std::stringstream str(val);
            str >> (*value);
            return *value;
        }
        else {
            T* value = new T{};
            std::string val = RemoteMemory::ObtenerInstance()->getValor(id);
            std::stringstream str(val);
            str >> (*value);
            return *value;
        }
    }
    VSPtr& operator*() {
        return *this;
    }
    static VSPtr<T> New() {
        VSPtr<T> newVSptr{ VSPtr<T>(false) };
        return newVSptr;
    }
    static VSPtr<T> NewRemote() {
        VSPtr<T> newVSptr{ VSPtr<T>(true) };
        return newVSptr;
    }
};
template<class T>
std::string VSPtr<T>::get_id() {
    return this->id;
}
template<class T>
T* VSPtr<T>::get_ptr() {
    if (is_remote) {
    }
    return this->addr;
}
template<class T>
void VSPtr<T>::addInstanceLocally() {
    this->is_remote = false;
    addr = new T{};
    setVSPtrIdFromGarbageCollector();
    GarbageCollector::ObtenerGCInstance()->AgregarInstance(addr, id);
    GarbageCollector::ObtenerGCInstance()->PrintGCInfo();
    std::cout << "DIRECCION DE MEMORIA DEL DATO QUE GUARDA EL VSPointer  " << get_id() << "   " << addr << "\n\n";
}
template<class T>
void VSPtr<T>::addInstanceRemotely() {
    this->is_remote = true;
    const char* name = typeid(T).name();
    this->id = GarbageCollector::ObtenerGCInstance()->GenerarID();
    RemoteMemory::ObtenerInstance()->RAgregarInstancia(name, id);
    std::cout << "El id es " << id << "\n";
}
template<class T>
void VSPtr<T>::setVSPtrIdFromGarbageCollector() {
    this->id = GarbageCollector::ObtenerGCInstance()->GenerarID();
}
template<class T>
void VSPtr<T>::decrementRefCountLocally(std::string id) {
    GarbageCollector::ObtenerGCInstance()->ReducirContador(id);
}
template<class T>
void VSPtr<T>::decrementRefCountRemotely(std::string id) {
    RemoteMemory::ObtenerInstance()->RDisminuirInstancia(id);
}
template<class T>
void VSPtr<T>::incrementRefCountLocally(std::string id) {
    GarbageCollector::ObtenerGCInstance()->AumentarContador(id);
}
template<class T>
void VSPtr<T>::incrementRefCountRemotely(std::string id) {
    RemoteMemory::ObtenerInstance()->RAumentarInstancia(id);
}

#endif //GC_VSPOINTER_H