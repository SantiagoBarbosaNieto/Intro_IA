#ifndef CONJUNTO_H
#define CONJUNTO_H

using namespace std;

struct s_punto
{
    float x;
    float y;
    s_punto(float y1, float x1) {
        x = x1;
        y = y1;
    }
};

class Conjunto {

public:
    string nombre;
    vector<s_punto> puntos;
    Conjunto(string nombre, vector<s_punto>* puntos);

    float Pertenencia(float valor);
private:

};

#endif