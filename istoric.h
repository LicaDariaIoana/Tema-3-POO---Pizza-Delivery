#ifndef ISTORIC_H
#define ISTORIC_H
#include "exceptii.h"
#include <vector>
#include <iostream>
#include <string>

template <typename T>
class Istoric {
private:
    std::vector<T> ist;
    T ultimaOp;
	Istoric() = default;

public:

    Istoric(const Istoric&) = delete;
    Istoric& operator=(const Istoric&) = delete;

	static Istoric<T>& getIstoric() {
        static Istoric<T> i;
        return i;}

    void arhActiune(T aux) {
        ist.push_back(aux);
        ultimaOp = aux; }

    void afisUltima() const {
		if(ist.empty()) std::cout << "Istoricul este gol.\n";
        std::cout << "Ultima inregistrare din sistem: " << ultimaOp << "\n";}

	void afisIstoric() const {
		if(ist.empty()) std::cout << "Istoricul este gol.\n";
		else{
			std::cout << "\n--- ISTORICUL DE AZI ---\n";
			for (const auto& actiune : ist) {
			std::cout << " >>" << actiune << "\n";}}
    }

    template <typename U>
    friend void resetare();};

template <typename U>
void resetare() {
    Istoric<U>& aux = Istoric<U>::getIstoric();
    aux.ist.clear();
    aux.ultimaOp = U{};
    std::cout << "Istoricul a fost resetat complet.\n";
}

template <typename T>
class IstoricProxy {
private:
    const std::string parolaManager = "1234";
public:
     void afiseazaUltima() {
        Istoric<T>::getIstoric().afisUltima();
    }

    void arhiveazaActiunea(T aux) {
        Istoric<T>::getIstoric().arhActiune(aux);
    }

    void afiseazaIntregIstoric(const std::string& p) {
        if (p == parolaManager) {
            Istoric<T>::getIstoric().afisIstoric();
        } else throw parola_invalida();
    }

    void resetareIstoric(const std::string& p) {
        if (p == parolaManager) {
            resetare<T>();
        } else {throw parola_invalida();}}
};


#endif
