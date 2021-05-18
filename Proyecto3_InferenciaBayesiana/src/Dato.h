#pragma once

#include <iostream>
#include <map>

using namespace std;

class Dato {
private:
	map<string, string> valores;
public:
	Dato();
	~Dato();
	string buscarDato(string col);
	void ingresarDato(string col, string dato);
	void imprimirValores();
	map<string, string> darValores();
	int tam();
};








