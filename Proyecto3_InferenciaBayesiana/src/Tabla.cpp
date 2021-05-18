
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include "Tabla.h"


Tabla::
Tabla()
{

}


Tabla::
~Tabla()
{

}

bool Tabla::
leerDeArchivo(string filename)
{
    this->limpiarTabla();
    // File pointer
    fstream fin;
    
    // Open an existing file
    fin.open(filename, ios::in);
    if (fin.is_open())
    {
        // Read the Data from the file as String Vector
        vector<string> row;
        string line, word;

        //Se lee la primera linea que contiene los nombres de las columnas
        getline(fin, line);
        stringstream s(line);
        while (getline(s, word, ';')) {
            if(word != "")
                this->nombreCols.push_back(word);
        }


        while (getline(fin, line)) {

            row.clear();
            // read every column data of a row and
            // store it in a string variable, 'word'

            stringstream s(line);
            int count = 0;
            while (std::getline(s, word, ';')) {
                // add all the column data of a row to a vector
                if (word != "")
                {
                    row.push_back(word);
                    count++;
                }
            }
            if (count < nombreCols.size())
            {
                cout << "Archivo no pudo ser leido. Verifique el formato. ";
                return false;
            }

            // Guardar la informacion leida
            Dato* d = new Dato();
            for (int i = 0; i < this->nombreCols.size(); i++)
            {
                d->ingresarDato(this->nombreCols[i], row[i]);

            }
            this->filas.push_back(*d);

        }
        
        cout << "Archivo leido y procesado!\n";
        return true;

    }
    else
    {
        cout << "Archivo no encontrado. ";
        return false;
    }


}


void Tabla::
imprimirDatos()
{
    for (int i = 0; i < this->filas.size(); i++)
    {
        this->filas[i].imprimirValores();
        cout << "\n";
    }
}
void Tabla::contadorTabla() {
    //introduce el valor del contador en el mapa
    for (int i = 0; i < this->nombreCols.size(); i++) {
        string aux = this->nombreCols[i];
        for (int j = 0; j < this->filas.size(); j++) {
            bool yaEsta = false;

            for (auto const& x : this->clasesColumnas[aux])
            {
                if (x.first == filas[j].buscarDato(aux))
                {
                    yaEsta = true;
                    break;
                }
            }
            if (yaEsta) {
                this->clasesColumnas[aux][filas[j].buscarDato(aux)] = this->clasesColumnas[aux][filas[j].buscarDato(aux)] + 1;
            }
            else
            {
                this->clasesColumnas[aux][filas[j].buscarDato(aux)] = 1;
            }
        }
    }

}

void Tabla::
imprimirCantidadDeOcurrencias()
{
    for (auto const& x : clasesColumnas)
    {
        cout << x.first << ": ";
        for (auto const& y : x.second)
        {
            cout << y.first << ":: " << y.second << "   ";
        }
        cout << "\n";
    }
}

vector<string> Tabla::getNombreCols() {
    return this->nombreCols;
}

vector<vector<string>> Tabla::getMiniTabla() {
    vector<vector<string>> miniTabla;
    vector<string> temporal, vacio;

    for (auto const& x : clasesColumnas) {
        temporal.push_back(x.first);
    }
    miniTabla.push_back(temporal);

    temporal = vacio;

    for (auto const& x : clasesColumnas) {

        for (auto const& y : x.second)
        {
            temporal.push_back(y.first);
        }
        miniTabla.push_back(temporal);
        temporal = vacio;
    }
    return miniTabla;
}

float Tabla::
comparaStrings(vector<string> vec) {
    bool aux1 = false;
    string a = "test";
    for (int i = 0; i < vec.size(); i++) {
        aux1 = false;
        while (aux1 == false) {
            for (int j = 0; j < 5; j++) {
                if (vec[i] == a) {
                    break;
                }
            }
        }

    }
    return 0;
}
//Calcula la probabilidad de X dado Y
float Tabla::
probabilidadDe(string nomX, string valorX, string nomY, string valorY) //nom -> nombre de la columna; valor -> valor de esa columna
{
    int countXconY = 0;

    for (int i = 0; i < this->filas.size(); i++)
    {
        if (filas[i].buscarDato(nomX) == valorX && filas[i].buscarDato(nomY) == valorY)
            countXconY++;
    }
    float countY = (float)this->clasesColumnas[nomY][valorY];
    return ((float)countXconY / countY);
}

string Tabla::
calcularProbabilidad(vector<string> consulta, vector<string> colsConsulta)
{
    vector<double> probabilidades;
    vector<string> nomProbabilidades;

    int indiceBuscada = 0;
    for (int i = 0; i < consulta.size(); i++)
    {
        if (consulta[i] == "Buscada")
        {
            
            indiceBuscada = i;
            break;
        }
    }

    vector<vector<string>> vec = getMiniTabla();
    for (auto x : this->clasesColumnas[colsConsulta[indiceBuscada]]) //Se recorren los posibles valores de la columna buscada
    {
        nomProbabilidades.push_back(x.first);
        double resp = (double)x.second / (double)this->filas.size();
        for (int i = 0; i < consulta.size(); i++)//Se recorre cada elemento de la consulta que no sea nulo ni la columna buscada
        {
            if (consulta[i] != "Buscada" && consulta[i] != "Nulo")
            {
                resp *= (double)this->probabilidadDe(colsConsulta[i], consulta[i], colsConsulta[indiceBuscada], x.first);
            }
        }
        probabilidades.push_back(resp);
    }


    cout << "Probabilidades encontradas para P(" << colsConsulta[indiceBuscada] << "|";
    bool primero = true;
    for (int i = 0; i < consulta.size(); i++)
    {
        if (i != indiceBuscada)
        {
            if (consulta[i] != "Nulo" && primero)
            {
                cout << colsConsulta[i] << "::" << consulta[i];
                primero = false;
            }
            else if (consulta[i] != "Nulo" && !primero)
                cout << "," << colsConsulta[i] << "::" << consulta[i];
        }
    }
    cout << ")\n";



    double max = 0;
    int indMax = 0;
    for (int i = 0; i < probabilidades.size(); i++)
    {
        cout << "\t"<<nomProbabilidades[i] << ": " << probabilidades[i]<<"\n";
        if (probabilidades[i] > max)
        {
            max = probabilidades[i];
            indMax = i;
        }
    }

    return nomProbabilidades[indMax];

}

void Tabla::
limpiarTabla()
{
    for (int i = 0; i < this->filas.size(); i++)
    {
        this->filas[i].~Dato();
    }
    this->filas.clear();
    this->clasesColumnas.clear();
    this->nombreCols.clear();
}