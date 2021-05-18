#pragma once

#include <vector>
#include "Dato.h"

class Tabla
{
private:
	vector<Dato> filas;
	vector<string> nombreCols;
	map<string, map<string, int>> clasesColumnas; //Se guarda los posibles valores para cada columna y se guarda la probabilidad de cada una. Esto ultimo se calcula despues de llenar toda la tabla
public:
	Tabla();
	~Tabla();
	void imprimirDatos();
	void imprimirCantidadDeOcurrencias();
	void limpiarTabla();
	//Metodos para llenar la tabla (Como lo hacemos? seria super si podemos leer de un archivo .csv )
	bool leerDeArchivo(string filename);
	//Entrenamiento -> aqui se calcula la probabilidad (o el conteo) de cada una de los posibles valores de las clasesColumnas

	void contadorTabla();
	void mostrarResultados();

	//Cálculo de probabilidad de una clase dados ciertos eventos
	float probabilidadDe(string nomX, string valorX, string nomY, string valorY);
	string calcularProbabilidad(vector<string> consulta, vector<string> colsConsulta);
	float comparaStrings(vector<string>);

	// Retorna el atributo
	vector<string> getNombreCols();
	vector<vector<string>> getMiniTabla();
};