#include <iostream>
#include <vector>
#include "Conjunto.h"

using namespace std;

Conjunto::
Conjunto(string nombre, vector<s_punto>* puntos)
{
    this->nombre = nombre;
    this->puntos = *puntos;
}

float Conjunto::
Pertenencia(float valor)
{
    float resp = 0;
    if (this->puntos.size() < 2)
    {
        return -1;
    }

    for (int i = 0; i < this->puntos.size() - 1; i++)
    {
        float x1 = this->puntos[i].x;
        float y1 = this->puntos[i].y;

        float x2 = this->puntos[i + 1].x;
        float y2 = this->puntos[i + 1].y;

        if (valor >= x1 && valor <= x2)
        {
            resp = (((y2 - y1) / (x2 - x1)) * (valor - x1)) + y1;
            break;
        }
    }


    return resp;
}

