#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Conjunto.h"

using namespace std;

//Estructura tag
struct s_tag {
    string tag;
    float miu;
    s_tag(string n, float v)
    {
        tag = n;
        miu = v;
    }
};

//Función para encontrar el punto central de una línea AB
s_punto midpoint(s_punto A,s_punto B)
{
    return s_punto((float)((A.y + B.y)/2), (float)((A.x + B.x)/2));
}

//Función para calcular el punto de intersección de dos líneas AB y CD -- Si son paralelas devuelve un punto con FLT_MAX, FLT_MAX
s_punto lineLineIntersection(s_punto A, s_punto B, s_punto C, s_punto D)
{
    // Line AB represented as a1x + b1y = c1
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1 * (A.x) + b1 * (A.y);

    // Line CD represented as a2x + b2y = c2
    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2 * (C.x) + b2 * (C.y);

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return s_punto(FLT_MAX, FLT_MAX);
    }
    else
    {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        return s_punto(y, x);
    }
}

//Función para calcular el centro de masa de un triángulo dada la información de sus tres vértices
s_punto centroideTriangulo(s_punto A, s_punto B, s_punto C) //Centro de gravedad de un triangulo
{
    return  lineLineIntersection(A, midpoint(B, C), B, midpoint(A, C));
}

//Función para sacar el centro de gravedad de un cuadrilátero teniendo información de sus cuatro vértices
s_punto centroideQuad(s_punto A, s_punto B, s_punto C, s_punto D) //Centro de gravedad de una figura de 4 lados 
{
    //Los puntos A, B, C, D deben ser los vertices de la figura de manera que se lean en contra de las manecillas dle reloj en la figura.
    //Si A es el vertice superior izquierdo, B sera el vertice superior derecho, C el vertice inferior izquierdo, D el vertice inferior derecho.

    //Se divide la figura en dos triangulos, por la linea BD y se saca el centroide de cada triangulo
    s_punto T1C1 = centroideTriangulo(A, B, D);
    s_punto T1C2 = centroideTriangulo(A, C, D);
    
    //Se divide la figura en dos triangulos, por la linea AC y se saca el centroide de cada triangulo
    s_punto T2C1 = centroideTriangulo(C, A, B);
    s_punto T2C2 = centroideTriangulo(C, D, B);


    //La interseccion de las lineas formadas por los dos pares de centroides, dara el centro de gravedad de la figura
    return lineLineIntersection(T1C1, T2C1, T1C2, T2C2);
}

//Funcion para calcular el centro de gravedad según las acciones obtenidas en la inferencia (Defuzzificación) 
s_punto centroDeGravedad(vector<s_tag>* tags, vector<Conjunto>* conj)
{
    vector<s_punto>* centros =  new vector<s_punto>();
    for (int i = 0; i < tags->size(); i++)
    {
        for (int j = 0; j < conj->size(); j++)
        {
            if ((*tags)[i].tag == (*conj)[j].nombre)
            {
                s_punto A(0, (*conj)[j].puntos[0].x);
                s_punto D(0, (*conj)[j].puntos[2].x);

                float Bx = 0.0f;
                
                float By = (*tags)[i].miu;
                float Cx = 0.0f;
                float Cy =(*tags)[i].miu;

                //Si el primer punto no empieza en y = 0
                if ((*conj)[j].puntos[0].y > 0) {
                    Bx = (*conj)[j].puntos[0].x;
                }
                else {
                    float y = (*tags)[i].miu;
                    Bx = float(((y - A.y) * ((*conj)[j].puntos[1].x - A.x) / ( (*conj)[j].puntos[1].y - A.y)) + A.x);
                }
                //Si el ultimo punto no empieza en y = 0
                if ((*conj)[j].puntos[2].y > 0) {
                    Cx = (*conj)[j].puntos[2].x;
                }
                else {
                    float y = (*tags)[i].miu;
                    Cx = float(((D.y - y) * (D.x - (*conj)[j].puntos[1].x) / (D.y - (*conj)[j].puntos[1].y)) + (*conj)[j].puntos[1].x);
                }
                centros->push_back(centroideQuad(A, s_punto(By, Bx), s_punto(Cy, Cx), D));
            }
        }
    }
    float x = 0;
    float y = 0;
    int count = 0;
    for (int i = 0; i < centros->size(); i++)
    {
        x += (*centros)[i].x;
        y += (*centros)[i].y;
        count++;
    }

    return s_punto(float(y/count), float(x / count));
}

//Creacion de la matriz 
void llenarMatriz(vector<vector<string>>* matriz) {

    (*matriz)[0].push_back("NO");
    (*matriz)[0].push_back("NO");
    (*matriz)[0].push_back("MP");
    (*matriz)[0].push_back("PP");
    (*matriz)[0].push_back("PS");
    (*matriz)[0].push_back("PS");

    (*matriz)[1].push_back("NO");
    (*matriz)[1].push_back("MP");
    (*matriz)[1].push_back("PP");
    (*matriz)[1].push_back("PS");
    (*matriz)[1].push_back("PS");
    (*matriz)[1].push_back("PS");

    (*matriz)[2].push_back("MP");
    (*matriz)[2].push_back("MP");
    (*matriz)[2].push_back("PP");
    (*matriz)[2].push_back("PS");
    (*matriz)[2].push_back("PS");
    (*matriz)[2].push_back("MS");

    (*matriz)[3].push_back("MP");
    (*matriz)[3].push_back("PP");
    (*matriz)[3].push_back("PP");
    (*matriz)[3].push_back("PS");
    (*matriz)[3].push_back("MS");
    (*matriz)[3].push_back("MS");

    (*matriz)[4].push_back("PP");
    (*matriz)[4].push_back("PS");
    (*matriz)[4].push_back("PS");
    (*matriz)[4].push_back("MS");
    (*matriz)[4].push_back("MS");
    (*matriz)[4].push_back("SI");

    (*matriz)[5].push_back("PS");
    (*matriz)[5].push_back("PS");
    (*matriz)[5].push_back("MS");
    (*matriz)[5].push_back("MS");
    (*matriz)[5].push_back("SI");
    (*matriz)[5].push_back("SI");

}

//Creacion del conjunto de presupuesto
vector<Conjunto>* crearConjuntoPresupuesto() {
    vector<Conjunto>* resp = new vector<Conjunto>();

    vector<s_punto>* puntos1 = new vector<s_punto>();
    //Conjutno "Muy bajo"
    puntos1->push_back(s_punto(1.0f, 10.0f));
    puntos1->push_back(s_punto(1.0f, 25.0f));
    puntos1->push_back(s_punto(0.0f, 50.0f));
    Conjunto* conj = new Conjunto("Muy bajo", puntos1);
    resp->push_back(*conj);
    //Conjunto "Bajo"
    vector<s_punto>* puntos2 = new vector<s_punto>();
    puntos2->push_back(s_punto(0.0f, 25.0f));
    puntos2->push_back(s_punto(1.0f, 50.0f));
    puntos2->push_back(s_punto(0.0f, 75.0f));
    conj = new Conjunto("Bajo", puntos2);
    resp->push_back(*conj);
    //Conjunto "Medio bajo"
    vector<s_punto>* puntos3 = new vector<s_punto>();
    puntos3->push_back(s_punto(0.0f, 50.0f));
    puntos3->push_back(s_punto(1.0f, 75.0f));
    puntos3->push_back(s_punto(0.0f, 100.0f));
    conj = new Conjunto("Medio bajo", puntos3);
    resp->push_back(*conj);
    //Conjunto "Medio alto"

    vector<s_punto>* puntos4 = new vector<s_punto>();
    puntos4->push_back(s_punto(0.0f, 75.0f));
    puntos4->push_back(s_punto(1.0f, 100.0f));
    puntos4->push_back(s_punto(0.0f, 150.0f));
    conj = new Conjunto("Medio alto", puntos4);
    resp->push_back(*conj);
    //Conjutno "Alto"
    vector<s_punto>* puntos5 = new vector<s_punto>();
    puntos5->push_back(s_punto(0.0f, 100.0f));
    puntos5->push_back(s_punto(1.0f, 150.0f));
    puntos5->push_back(s_punto(0.0f, 250.0f));
    conj = new Conjunto("Alto", puntos5);
    resp->push_back(*conj);
    //Conjunto "Muy alto"
    vector<s_punto>* puntos6 = new vector<s_punto>();
    puntos6->push_back(s_punto(0.0f, 150.0f));
    puntos6->push_back(s_punto(1.0f, 250.0f));
    puntos6->push_back(s_punto(1.0f, 400.0f));
    conj = new Conjunto("Muy alto", puntos6);
    resp->push_back(*conj);

    return resp;
}

//Creacion del conjunto de calidad
vector<Conjunto>* crearConjuntoCalidad() {
    vector<Conjunto>* resp = new vector<Conjunto>();

    vector<s_punto>* puntos1 = new vector<s_punto>();
    //Conjutno "Pesima"
    puntos1->push_back(s_punto(1.0f, 0.0f));
    puntos1->push_back(s_punto(1.0f, 0.5f));
    puntos1->push_back(s_punto(0.0f, 1.5f));
    Conjunto* conj = new Conjunto("Pesima", puntos1);
    resp->push_back(*conj);
    //Conjunto "Mala"
    vector<s_punto>* puntos2 = new vector<s_punto>();
    puntos2->push_back(s_punto(0.0f, 0.5f));
    puntos2->push_back(s_punto(1.0f, 1.5f));
    puntos2->push_back(s_punto(0.0f, 2.0f));
    conj = new Conjunto("Mala", puntos2);
    resp->push_back(*conj);
    //Conjunto "Mediocre"
    vector<s_punto>* puntos3 = new vector<s_punto>();
    puntos3->push_back(s_punto(0.0f, 1.5f));
    puntos3->push_back(s_punto(1.0f, 2.0f));
    puntos3->push_back(s_punto(0.0f, 3.0f));
    conj = new Conjunto("Mediocre", puntos3);
    resp->push_back(*conj);
    //Conjunto "Aceptable"

    vector<s_punto>* puntos4 = new vector<s_punto>();
    puntos4->push_back(s_punto(0.0f, 2.0f));
    puntos4->push_back(s_punto(1.0f, 3.0f));
    puntos4->push_back(s_punto(0.0f, 3.5f));
    conj = new Conjunto("Aceptable", puntos4);
    resp->push_back(*conj);
    //Conjutno "Buena"
    vector<s_punto>* puntos5 = new vector<s_punto>();
    puntos5->push_back(s_punto(0.0f, 3.0f));
    puntos5->push_back(s_punto(1.0f, 3.5f));
    puntos5->push_back(s_punto(0.0f, 4.5f));
    conj = new Conjunto("Buena", puntos5);
    resp->push_back(*conj);
    //Conjunto "Excelente"
    vector<s_punto>* puntos6 = new vector<s_punto>();
    puntos6->push_back(s_punto(0.0f, 3.5f));
    puntos6->push_back(s_punto(1.0f, 4.5f));
    puntos6->push_back(s_punto(1.0f, 5.0f));
    conj = new Conjunto("Excelente", puntos6);
    resp->push_back(*conj);

    return resp;
}

//Creacion del conjunto de acciones
vector<Conjunto>* crearConjuntoAccion() {
    vector<Conjunto>* resp = new vector<Conjunto>();

    vector<s_punto>* puntos1 = new vector<s_punto>();
    //Conjutno "NO"
    puntos1->push_back(s_punto(1.0f, 0.0f));
    puntos1->push_back(s_punto(1.0f, 10.0f));
    puntos1->push_back(s_punto(0.0f, 25.0f));
    Conjunto* conj = new Conjunto("NO", puntos1);
    resp->push_back(*conj);
    //Conjunto "MP"
    vector<s_punto>* puntos2 = new vector<s_punto>();
    puntos2->push_back(s_punto(0.0f, 10.0f));
    puntos2->push_back(s_punto(1.0f, 25.0f));
    puntos2->push_back(s_punto(0.0f, 40.0f));
    conj = new Conjunto("MP", puntos2);
    resp->push_back(*conj);
    //Conjunto "PP"
    vector<s_punto>* puntos3 = new vector<s_punto>();
    puntos3->push_back(s_punto(0.0f, 25.0f));
    puntos3->push_back(s_punto(1.0f, 40.0f));
    puntos3->push_back(s_punto(0.0f, 60.0f));
    conj = new Conjunto("PP", puntos3);
    resp->push_back(*conj);
    //Conjunto "PS"
    vector<s_punto>* puntos4 = new vector<s_punto>();
    puntos4->push_back(s_punto(0.0f, 40.0f));
    puntos4->push_back(s_punto(1.0f, 60.0f));
    puntos4->push_back(s_punto(0.0f, 75.0f));
    conj = new Conjunto("PS", puntos4);
    resp->push_back(*conj);
    //Conjutno "MS"
    vector<s_punto>* puntos5 = new vector<s_punto>();
    puntos5->push_back(s_punto(0.0f, 60.0f));
    puntos5->push_back(s_punto(1.0f, 75.0f));
    puntos5->push_back(s_punto(0.0f, 90.0f));
    conj = new Conjunto("MS", puntos5);
    resp->push_back(*conj);
    //Conjunto "SI"
    vector<s_punto>* puntos6 = new vector<s_punto>();
    puntos6->push_back(s_punto(0.0f, 75.0f));
    puntos6->push_back(s_punto(1.0f, 90.0f));
    puntos6->push_back(s_punto(1.0f, 100.0f));
    conj = new Conjunto("SI", puntos6);
    resp->push_back(*conj);

    return resp;
}



//Imprime la matriz de mapeo de los conjuntos de calidad y presupuesto hacia el conjunto de acciones
void imprimirMatriz(vector<vector<string>>* matriz) {

    cout << "\nMatriz de relacion conjunto calidad - conjunto presupuesto:";

    cout << endl << endl;
    cout << "Pesima = Pe " << endl;
    cout << "Mala = Ma" << endl;
    cout << "Mediocre = Me" << endl;
    cout << "Aceptable = Ac" << endl;
    cout << "Buena = Bu" << endl;
    cout << "Excelente = Ex" << endl;
    cout << endl << endl;
    cout << "Pe Ma Me Ac Bu Ex | Estrellas / Presupuesto" << endl;
    cout << "___________________________" << endl;
    for (int i = 0; i < (*matriz).size(); i++) {
        for (int j = 0; j < (*matriz)[i].size(); j++) {

            cout << (*matriz)[i][j] << " ";
        }
        if (i == 0) {
            cout << "| Muy Bajo";
        }
        else if (i == 1) {
            cout << "| Bajo";
        }
        else if (i == 2) {
            cout << "| Medio Bajo";
        }
        else if (i == 3) {
            cout << "| Medio Alto";
        }
        else if (i == 4) {
            cout << "| Alto";
        }
        else if (i == 5) {
            cout << "| Muy Alto";
        }
        cout << endl;
    }

    cout << endl << "NO = 'No'" << endl;
    cout << "MP = 'Muy poca probabilidad'" << endl;
    cout << "PP = 'Poco probable'" << endl;
    cout << "PS = 'Probablemente si'" << endl;
    cout << "MS = 'Muy probablemente si'" << endl;
    cout << "SI = 'Si'" << endl;
    cout << "\n";

}

//Dado un valor x y un grupo de conjuntos, se evalúa el valor y (miu) para cada uno de los conjuntos y se guarda en un tag
vector<s_tag>* evaluarConjunto(float valor, vector<Conjunto>* conj)
{
    vector<s_tag>* tags = new vector<s_tag>();
    for (int i = 0; i < conj->size(); i++)
    {
        tags->push_back(s_tag((*conj)[i].nombre, (*conj)[i].Pertenencia(valor)));
    }

    return tags;
}

//Dado el nombre de conjunto de presupuesto y el nombre del conjunto de calidad, encuentra la correspondiente acción en la matriz
string verificarMatriz(vector<vector<string>>* matriz, string pres, string cal)
{
    int calidad = -1, presupuesto = -1;
    //Buscar la accion correspondiente en la matriz para los "valores" de pres y cal 
    if (pres == "Muy bajo") {
        presupuesto = 0;
    }
    else if (pres == "Bajo") {
        presupuesto = 1;
    }
    else if (pres == "Medio bajo") {
        presupuesto = 2;
    }
    else if (pres == "Medio alto") {
        presupuesto = 3;
    }
    else if (pres == "Alto") {
        presupuesto = 4;
    }
    else if (pres == "Muy alto") {
        presupuesto = 5;
    }
    else {
        cout << "Error al verificar matriz";
    }

    if (cal == "Pesima") {
        calidad = 0;
    }
    else if (cal == "Mala") {
        calidad = 1;
    }
    else if (cal == "Mediocre") {
        calidad = 2;
    }
    else if (cal == "Aceptable") {
        calidad = 3;
    }
    else if (cal == "Buena") {
        calidad = 4;
    }
    else if (cal == "Excelente") {
        calidad = 5;
    }
    else {
        cout << "Error al verificar matriz";
    }
    return (*matriz)[calidad][presupuesto];
}

//Proceso de inferencia dfusa, dado un vector de tags del conjunto presupuesto, un vector de tags del conjunto calidad y la matriz que relaciona a ambos
vector<s_tag>* inferenciaDifusa(vector<s_tag>* tags_Pres, vector<s_tag>* tags_Cal, vector<vector<string>>* matriz) {
    vector<s_tag>* tags = new vector<s_tag>();
    for (int i = 0; i < tags_Pres->size(); i++) {
        if ((*tags_Pres)[i].miu > 0) {
            for (int j = 0; j < tags_Cal->size(); j++) {
                if ((*tags_Cal)[j].miu > 0) {
                    //"Bajo"
                    string accion = verificarMatriz(matriz, (*tags_Pres)[i].tag, (*tags_Cal)[j].tag);
                    float miu_accion = min((*tags_Pres)[i].miu, (*tags_Cal)[j].miu); //Aqui se hace el min
                    bool yaPresente = false;
                    for (int k = 0; k < tags->size(); k++) {
                        if ((*tags)[k].tag == accion) {
                            yaPresente = true;
                            if ((*tags)[k].miu < miu_accion) { //Aqui se hace el max
                                (*tags)[k].miu = miu_accion;
                                break;
                            }
                        }
                    }
                    if (!yaPresente) {
                        tags->push_back(s_tag(accion, miu_accion));
                    }

                }
            }
        }

    }
    return tags;
}

int main() {
    float presupuesto;
    float estrellas;
    vector<vector<string>>* matriz = new vector<vector<string>>;

    vector<Conjunto>* c_Pres = crearConjuntoPresupuesto();
    vector<Conjunto>* c_Cal = crearConjuntoCalidad();
    vector<Conjunto>* c_Acc = crearConjuntoAccion();

    for (int i = 0; i < 6; i++)
    {
        vector<string>* v = new vector<string>();
        matriz->push_back(*v);
    }
    // Llena la matriz Calidad vs Presupuesto
    llenarMatriz(matriz);//

    cout << "   .::Bienvenido::.   " << endl;
    string input;
    while ((cout << "\n- Ingrese su presupuesto  (Rango de 10 a 400): ") //make sure the user knows what you expect
        && getline(cin, input)
        && (!(istringstream{ input } >> presupuesto) //construct a stringstream from `line` and read it into `float`
            || !(presupuesto > 0))
        ) //this loop continues on bad input and ends on good input
    {
        std::cerr << "\nInput invalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
    }

    vector<s_tag>* tags_pres = evaluarConjunto(presupuesto, c_Pres);

    cout << "Valores de pertenencia de presupuesto = " << presupuesto << "\n";
    for (int i = 0; i < tags_pres->size(); i++)
    {
        cout << (*tags_pres)[i].tag << ": " << (*tags_pres)[i].miu << "\n";
    }
    cout << "\n";

    while ((cout << "\n- Ingrese el numero de estrellas segun la calidad del producto  (Rango de 0 a 5): ") //make sure the user knows what you expect
        && getline(cin, input)
        && (!(istringstream{ input } >> estrellas) //construct a stringstream from `line` and read it into `float`
            || (estrellas < 0 || estrellas>5))
        ) //this loop continues on bad input and ends on good input
    {
        std::cerr << "\nInput invalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
    }

    vector<s_tag>* tags_cal = evaluarConjunto(estrellas, c_Cal);

    cout << "\n\n";

    cout << "Valores de pertenencia de calidad = " << estrellas << "\n";
    for (int i = 0; i < tags_cal->size(); i++)
    {
        cout << (*tags_cal)[i].tag << ": " << (*tags_cal)[i].miu << "\n";
    }

    imprimirMatriz(matriz);

    vector<s_tag>* tags_acc = inferenciaDifusa(tags_pres, tags_cal, matriz);

    cout << "Valores de pertenencia de acciones inferidas\n";
    for (int i = 0; i < tags_acc->size(); i++)
    {
        cout << (*tags_acc)[i].tag << ": " << (*tags_acc)[i].miu << "\n";
    }

    s_punto centro = centroDeGravedad(tags_acc, c_Acc);
    cout << "Probabilidad de compra: " << centro.x << "%\n";
    return 0;
}
