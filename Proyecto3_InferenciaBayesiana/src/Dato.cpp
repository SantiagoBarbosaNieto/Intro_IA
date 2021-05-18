#include "Dato.h"
#include <iostream>
Dato::
Dato()
{

}


Dato::
~Dato()
{
    this->valores.clear();
}

string Dato::
buscarDato(string col)
{
    return this->valores[col];
}

void Dato::
ingresarDato(string col, string dato)
{
    valores[col] = dato;
}

map<string,string> Dato::
darValores()
{
    return this->valores;
}

void Dato::
imprimirValores()
{
    for (auto const& x : this->valores)
    {
        std::cout << x.first  // string (key)
            << ": "
            << x.second // string's value 
            << "\n";
    }
}


int Dato::tam() {
    return this->valores.size();
}