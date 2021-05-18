#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include "Dato.h"
#include "Tabla.h"

using namespace std;

vector<string> menuDinamico(vector<vector<string>> miniTabla, string columna) {

    int decision;
    bool encuenta = false;
    vector<string> consulta;

    cout << endl;
    cout << "Para realizar una consulta por favor indique: " << endl;
    string input;
    for (int i = 1; i < miniTabla.size(); i++) {
        if (columna != miniTabla[0][i - 1]) {
            
            while ((cout << "\nDesea tener en cuenta la columna " << miniTabla[0][i - 1] << "?\n1. Si\n2. No\n-> ") //make sure the user knows what you expect
                && getline(cin, input)
                && !(istringstream{ input } >> decision) //construct a stringstream from `line` and read it into `num`
                || !(decision >= 1 && decision <= 2)
                ) //this loop continues on bad input and ends on good input
            {
                std::cerr << "\nInput invalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
            }
            if (decision == 1) {
                encuenta = true;
            }
            else if (decision == 2) {
                encuenta = false;
                consulta.push_back("Nulo");
            }

            if (encuenta) {
                cout << endl;
                cout << "En la columna " << miniTabla[0][i - 1] << " escoja: " << endl;
                for (int j = 0; j < miniTabla[i].size(); j++) {
                    cout << j + 1 << ". " << miniTabla[i][j] << endl;
                }
                cout << "-> ";
                while (getline(cin, input)
                    && !(istringstream{ input } >> decision) //construct a stringstream from `line` and read it into `num`
                    || !(decision >= 1 && decision <= miniTabla[i].size())
                    )//this loop continues on bad input and ends on good input
                {
                    std::cerr << "\nInput invalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
                    for (int j = 0; j < miniTabla[i].size(); j++) {
                        cout << j + 1 << ". " << miniTabla[i][j] << endl;
                    }
                    cout << "-> ";
                }
                consulta.push_back(miniTabla[i][decision - 1]);
            }
        }
        else {
            consulta.push_back("Nulo");
        }
    }
    return consulta;
}

int main() {
    cout << ".::BIENVENIDO::." << endl;

    while (true) //Loop de consultas, cada vez con un nuevo archivo
    {
        Tabla* t = new Tabla();
        vector<string> consulta;
        string archivo, columna;
        int decision;
        string input;
        bool abrio = false;
        while (!abrio)
        {
            while ((cout << "Ingrese el nombre y ubicacion del archivo: ") //make sure the user knows what you expect
                && getline(cin, input)
                && !(istringstream{ input } >> archivo) //construct a stringstream from `line` and read it into `num`
                ) //this loop continues on bad input and ends on good input
            {
                if (abrio)
                    std::cerr << "\nInput invalido. Intente nuevamente.\n"; //let the user know they made a mistake
                else
                    std::cerr << "Archivo no encontrado. Intente nuevamente.\n";
            }
            abrio = t->leerDeArchivo(archivo);
            if (!abrio)
                cout << "Intente nuevamente\n";
        }
        

        //t->leerDeArchivo("Datos_Ejemplo.csv");
        //t->imprimirDatos(); // Ya lee y guarda datos desde un archivo .csv 
        t->contadorTabla();
        //t->imprimirCantidadDeOcurrencias();

        // El usuario ingresa la consulta
        vector<string> colsConsulta;
        for (int i = 1; i < t->getMiniTabla().size(); i++) {
            colsConsulta.push_back(t->getMiniTabla()[0][i - 1]);
        }
        while (true) //Loop de nuevas consultas con el mismo archivo
        {
            cout << "Escoja la columna con la cual se hara el calculo:\n";
            for (int i = 0; i < colsConsulta.size(); i++) {
                cout << i + 1 << ". " << colsConsulta[i] << endl;
            }
            cout << "-> ";
            while ( getline(cin, input)
                && !(istringstream{ input } >> decision) //construct a stringstream from `input` and read it into `decision`
                || !(decision >= 1 && decision <= colsConsulta.size())
                ) //this loop continues on bad input and ends on good input
            {
                std::cerr << "\nInput invalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
                for (int i = 0; i < colsConsulta.size(); i++) {
                    cout << i + 1 << ". " << colsConsulta[i] << endl;
                }
                cout << "-> ";
            }

            columna = colsConsulta[decision - 1];
            consulta = menuDinamico(t->getMiniTabla(), columna);
            consulta[decision - 1] = "Buscada";
            cout << endl;



            cout << "Su consulta es: P(" << columna << "|";
            bool primero = true;
            for (int i = 0; i < consulta.size(); i++)
            {
                if (i != decision - 1)
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


            //Se calcula
            string s = t->calcularProbabilidad(consulta, colsConsulta);
            cout << "\nLa respuesta de su consulta es: " << columna << "::" << s << "\n";


            while ((cout << "Desea hacer una nueva consulta? \n 1. Si\n 2. No\n ->") //make sure the user knows what you expect
                && getline(cin, input)
                && !(istringstream{ input } >> decision) //construct a stringstream from `line` and read it into `num`
                || !(decision >= 1 && decision <= 2)
                ) //this loop continues on bad input and ends on good input
            {
                std::cerr << "\nInput invalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
            }

            if (decision == 2)
                break;
            else if (decision == 1)
            {
                while ((cout << "Desea hacer la nueva consulta con un archivo diferente? \n 1. Si\n 2. No\n ->") //make sure the user knows what you expect
                    && getline(cin, input)
                    && !(istringstream{ input } >> decision) //construct a stringstream from `line` and read it into `num`
                    || !(decision >= 1 && decision <= 2)
                    ) //this loop continues on bad input and ends on good input
                {
                    std::cerr << "\nInput invalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
                }

                if (decision == 1)
                    break;
            }
        }

        if (decision == 2)
            break;
    }

    return 0;
}