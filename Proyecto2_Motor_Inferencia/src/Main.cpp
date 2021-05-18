#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;


struct REGLA {
    string regla;
    bool estado;
    vector<string> componentes;
};

void listarExpresiones(vector<vector<REGLA>> expresiones);
void agregarRegla(vector<vector<REGLA>>* exp);
void getValidateRule(REGLA* pRegla);
void searchRuleMatch(vector<vector<REGLA>>& pVector, REGLA* pRegla, int& posx);
vector<vector<REGLA*>>* unificarVariables(vector<vector<REGLA>>& exp);
void inferencia(vector<vector<REGLA>>& exp, vector<vector<REGLA*>>* sist);

int main() {
    cout << "MOTOR DE INFERECIA LOGICA\n\n";
    int select = 1;

    vector<vector<REGLA>> expresiones;
    vector<vector<REGLA*>>* sist;
    REGLA expValid;
    while (select != 0)
    {
        select = 0;
        cout << "\nMenu principal\n 0.Salir\n 1.Agregar regla\n 2.Listar reglas\n 3.Validar condicion\n Su opcion:";
        cin >> select;
        switch (select)
        {
        case 1:
            agregarRegla(&expresiones);
            break;

        case 2:
            cout << "Las reglas en sistema son :\n";
            listarExpresiones(expresiones);
            break;

        case 3:
            cout << "Validar expresion";
            getValidateRule(&expValid);
            sist = unificarVariables(expresiones);
            cout << "\nLas variables han sido unificadas \n";
            listarExpresiones(expresiones);
            inferencia(expresiones, sist);
            cout << "\nResultado de inferencia: \n";
            cout << expresiones[0][0].componentes[0] << (expresiones[0][0].estado ? "" : " No ")<<" es " << expresiones[0][0].regla;
            cout << " se cancela con " << expValid.componentes[0] << (expValid.estado ? "" : " No ")<< "es " << expValid.regla;
            expresiones.erase(expresiones.begin());
            listarExpresiones(expresiones);


            break;

        case 0:
            cout << "Saliendo del programa\n";
            break;

        default:
            break;
        }
    }

    return 0;
}

//Realiza el proceso de inferencia, dejando al final solo el resultado de inferencia dentro del sistema
void inferencia(vector<vector<REGLA>> &exp, vector<vector<REGLA*>>* sistema)
{
    vector<string> elim;
    for (int i = 0; i < sistema->size(); i++){
        for (int j = 0; j < (*sistema)[i].size(); j++){
            bool marcada = false;
            for (int k = 0; k < (*sistema)[i].size(); k++)
            {
                bool enc = false;
                if (!enc && (*sistema)[i][j]->regla == (*sistema)[i][k]->regla && (*sistema)[i][j]->estado != (*sistema)[i][k]->estado && (*sistema)[i][j]->componentes == (*sistema)[i][k]->componentes) {
                    cout << "\n";
                    if ((*sistema)[i][j]->componentes.size() == 1) {
                        cout << (*sistema)[i][j]->componentes[0] << ((*sistema)[i][j]->estado ? "" : " No ") << " es " << (*sistema)[i][j]->regla << " se cancela con ";
                        cout << (*sistema)[i][k]->componentes[0] << ((*sistema)[i][k]->estado ? "" : " No ") << " es " << (*sistema)[i][k]->regla;
                    }
                    else if ((*sistema)[i][j]->componentes.size() == 2)
                    {
                        cout << (*sistema)[i][j]->componentes[0] << ((*sistema)[i][j]->estado ? " " : " No ") << (*sistema)[i][j]->regla << " " << (*sistema)[i][j]->componentes[1] << " se cancela con ";
                        cout << (*sistema)[i][k]->componentes[0] << ((*sistema)[i][k]->estado ? " " : " No ") << (*sistema)[i][k]->regla << " " << (*sistema)[i][k]->componentes[1];
                    }
                    else if ((*sistema)[i][j]->componentes.size() == 3) {
                        cout << (*sistema)[i][j]->componentes[0] << ((*sistema)[i][j]->estado ? " " : " No ") << (*sistema)[i][j]->regla << " " << (*sistema)[i][j]->componentes[1] << ", " << (*sistema)[i][j]->componentes[2] << " se cancela con ";
                        cout << (*sistema)[i][k]->componentes[0] << ((*sistema)[i][k]->estado ? " " : " No ") << (*sistema)[i][k]->regla << " " << (*sistema)[i][k]->componentes[1] << ", " << (*sistema)[i][k]->componentes[2];
                    }

                }
                if ((*sistema)[i][j]->regla == (*sistema)[i][k]->regla && (*sistema)[i][j]->estado != (*sistema)[i][k]->estado && (*sistema)[i][j]->componentes == (*sistema)[i][k]->componentes)
                {
                    elim.push_back((*sistema)[i][j]->regla);
                    
                    marcada = true;
                    break;
                }
                for (int l = 0; l < elim.size(); l++){
                    if (elim[l] == (*sistema)[i][j]->regla)
                    {
                        enc = true;
                    }
                }
               
            }
            if (marcada)
                break;
        }
    }

    while (elim.size() > 0){
        vector<int> indE;
        for (int i = 0; i < exp.size(); i++){
            vector<int> indR;
            for (int j = 0; j < exp[i].size(); j++){
                for (int k = 0; k < elim.size(); k++){
                    if (j < exp[i].size() && elim[k] == exp[i][j].regla){
                        bool yaEsta = false;
                        for (int l = 0; l < indE.size(); l++){
                            if (indE[l] == k){
                                yaEsta = true; 
                                break;
                            }
                        }
                        if (!yaEsta){
                            indE.push_back(k);
                        }
                        bool yaEstaR = false;
                        for (int l = 0; l < indR.size(); l++) {
                            if (indR[l] == j) {
                                yaEstaR = true;
                                break;
                            }
                        }
                        if (!yaEstaR) {
                            indR.push_back(j);
                        }

                    }
                }
            }
            sort(indR.begin(), indR.end());
            for (int r = indR.size()-1; r >= 0 ; r--)
            {
                exp[i].erase(exp[i].begin() + indR[r]);
            }
        }

        sort(indE.begin(), indE.end());
        for (int e = indE.size() - 1; e >= 0; e--)
        {
            elim.erase(elim.begin() + indE[e]);
        }
    }

    for (int i = exp.size() - 1; i >= 0; i--)
    {
        if (exp[i].size() == 0)
        {
            exp.erase(exp.begin() + i);
        }
    }

}

//Realiza el proceso para unificar todas las variables que se puedan
vector<vector<REGLA*>>* unificarVariables(vector<vector<REGLA>>& exp)
{
    vector<vector<REGLA*>>* resp = new vector<vector<REGLA*>>();
    vector<vector<REGLA*>> sistema;
    vector<string> reglasUnicas;
    for (int i = 0; i < exp.size(); i++) {
        for (int j = 0; j < exp[i].size(); j++) {
            bool yaExiste = false;
            for (int k = 0; k < reglasUnicas.size(); k++) {
                if (reglasUnicas[k] == exp[i][j].regla) {
                    yaExiste = true;
                    break;
                }
            }
            if (!yaExiste) {
                reglasUnicas.push_back(exp[i][j].regla);
                vector<REGLA*>* vec = new vector<REGLA*>();
                sistema.push_back(*vec);
            }
        }
    } //Aqui ya tenemos un vector con los nombres de todas las reglas unicas del sistema (y la cantidad que hay)


    for (int i = 0; i < exp.size(); i++) {
        for (int j = 0; j < exp[i].size(); j++) {
            for (int k = 0; k < reglasUnicas.size(); k++) {
                if (reglasUnicas[k] == exp[i][j].regla)
                {
                    sistema[k].push_back(&exp[i][j]);
                }
            }
        }
    }

    while (sistema.size() > 0)
    {
        vector<string> var;
        vector<string> reem;
        for (int i = 0; i < sistema.size(); i++) { //Reglas
            int indiceI = -1;
            int indiceC = -1;
            for (int j = 0; j < sistema[i].size(); j++) { //Instancias
                for (int k = 0; k < sistema[i][j]->componentes.size(); k++) {
                    char c;
                    c = sistema[i][j]->componentes[k][0];
                    if (isupper(c)) {//No es variable
                        // guardo el indice k, vuelvo a recorrer este arreglo de componentes buscando las variables para reemplazar
                        indiceC = k;
                        indiceI = j;
                        for (int l = 0; l < sistema[i].size(); l++)
                        {
                            c = sistema[i][l]->componentes[indiceC][0];
                            if (islower(c)) { //es variable, hey que reemplazarla
                                if (!(find(var.begin(), var.end(), sistema[i][l]->componentes[indiceC]) != var.end())) //Si no existe ya el reemplazo de esta variable
                                {
                                    var.push_back(sistema[i][l]->componentes[indiceC]);
                                    reem.push_back(sistema[i][indiceI]->componentes[indiceC]);
                                }
                                sistema[i][j]->componentes[k] = sistema[i][indiceI]->componentes[indiceC];
                            }
                            sistema[i][l]->componentes[indiceC] = sistema[i][indiceI]->componentes[indiceC];
                        }
                    }
                }
            }
        }

        //Volver a recorrer toddo reemplazando las variables en var por los valores en reem
        for (int i = 0; i < sistema.size(); i++) { //Reglas
            for (int j = 0; j < sistema[i].size(); j++) { //Instancias
                for (int k = 0; k < sistema[i][j]->componentes.size(); k++) { //Componentes
                    for (int l = 0; l < var.size(); l++) {
                        if (sistema[i][j]->componentes[k] == var[l])
                        {
                            sistema[i][j]->componentes[k] = reem[l];
                        }
                    }
                }
            }
        }

        //Eliminar de sistema las reglas que ya no tengan variables en sus instancias
        stack<int> ind; //indices a eliminar;
        for (int i = 0; i < sistema.size(); i++) { //Reglas
            bool hayVariables = false;
            for (int j = 0; j < sistema[i].size(); j++) { //Instancias
                for (int k = 0; k < sistema[i][j]->componentes.size(); k++) { //Componentes
                    char c;
                    c = sistema[i][j]->componentes[k][0];
                    if (islower(c)) { //es variable, existen variables en las instancias de esta regla
                        hayVariables = true;
                        break;
                    }
                }
                if (hayVariables)
                    break;
            }
            if (!hayVariables)
                ind.push(i);
        }
        

        while (ind.size() > 0)
        {
            resp->push_back(sistema[ind.top()]);
            sistema.erase(sistema.begin() + ind.top());
            ind.pop();
        }

    }
    return resp;
}


void searchRuleMatch(vector<vector<REGLA>>& pVector, REGLA* pRegla, int& posx) {
    vector<string> variablesSolucionadas;
    for (int i = 0; i < pVector.size(); ++i) {
        for (int j = 0; j < pVector[i].size(); ++j) {
            if (pVector[i][j].regla == pRegla->regla) {
                variablesSolucionadas = pVector[i][j].componentes;
            }
        }
    }

    for (int i = 0; i < pVector.size(); ++i) {
        for (int j = 0; j < pVector[i].size(); ++j) {
            for (int k = 0; k < variablesSolucionadas.size(); ++k) {
                for (int l = 0; l < pVector[i][j].componentes.size(); ++l) {
                    if (pVector[i][j].componentes[l] == variablesSolucionadas[k]) {
                        pVector[i][j].componentes[l] = pRegla->componentes[k];
                    }
                }
            }
        }
    }

}

//obtiene la información de la regla a validar
void getValidateRule(REGLA* pRegla) {
    cout << "\nDe el nombre de la regla a validar: ";
    string input;
    cin.ignore();
    getline(cin, input);
    pRegla->regla = input;
    char resp = 'n';
    while ((cout << "\nLa regla estará negada? (s/n): ") //make sure the user knows what you expect
        && getline(cin, input)
        && !(istringstream{ input } >> resp) //construct a stringstream from `line` and read it into `num`
        && (resp == 's' || resp == 'n')
        ) //this loop continues on bad input and ends on good input
    {
        std::cerr << "\nInput nvalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
    }
    resp == 's' ? pRegla->estado = false : pRegla->estado = true;
    int num;
    cout << "Cuantos componentes tiene su regla: ";
    cin >> num;


    cin.ignore();
    for (int i = 0; i < num; ++i) {
        cout << "Digite el " << i + 1 << "componente: ";
        getline(cin, input);
        pRegla->componentes.push_back(input);
    }
}

//Lista todas las reglas de todas las expresiones en el sistema
void listarExpresiones(vector<vector<REGLA>> exp) {
    for (int i = 0; i < exp.size(); ++i) {
        cout << "Expresion " << i + 1 << ": \n";
        for (int j = 0; j < exp[i].size(); j++)
        {
            if (!exp[i][j].estado) {
                cout << "¬";
            }
            cout << exp[i][j].regla << "(";
            for (int k = 0; k < exp[i][j].componentes.size(); ++k) {
                if (k == 0) {
                    cout << exp[i][j].componentes[k];
                }
                else {
                    cout << "," << exp[i][j].componentes[k];
                }
            }
            cout << ")";
            if (!(exp[i].size() == j + 1)) {
                cout << " V ";
            }
        }
        cout << "\n";
    }
}


//Agrega una nueva expresión compuesta de reglas al vector de expresiones 
void agregarRegla(vector<vector<REGLA>>* exp)
{
    vector<REGLA>* reglas = new vector<REGLA>();
    string input;
    int num = 0;
    cin.ignore(1);
    while ((cout << "\nDigite el numero de reglas en su expresion: ") //make sure the user knows what you expect
        && getline(cin, input)
        && !(istringstream{ input } >> num) //construct a stringstream from `line` and read it into `num`
        ) //this loop continues on bad input and ends on good input
    {
        std::cerr << "\nInput invalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
    }

    for (int i = 0; i < num; i++)
    {
        REGLA r;
        cout << "\nDigite el nombre de la regla: ";
        getline(cin, input);
        r.regla = input;
        char resp = 'n';
        while ((cout << "\nLa regla estará negada? (s/n): ") //make sure the user knows what you expect
            && getline(cin, input)
            && !(istringstream{ input } >> resp) //construct a stringstream from `line` and read it into `num`
            && (resp == 's' || resp == 'n')
            ) //this loop continues on bad input and ends on good input
        {
            std::cerr << "\nInput nvalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
        }
        resp == 's' ? r.estado = false : r.estado = true;
        int numC = -1;
        while (numC == -1)
        {
            while ((cout << "\nDigite el numero de componentes de su regla: ") //make sure the user knows what you expect
                && getline(cin, input)
                && !(istringstream{ input } >> numC) //construct a stringstream from `line` and read it into `num`
                ) //this loop continues on bad input and ends on good input
            {
                std::cerr << "\nInput nvalido. Intente nuevamente." << std::endl; //let the user know they made a mistake
            }
            switch (numC)
            {
            case 1:

                cout << "\nDigite el componente: ";
                getline(cin, input);
                r.componentes.push_back(input);
                reglas->push_back(r);
                break;
            case 2:
                cout << "\nDigite el componente C1 (C1" << (r.estado ? " " : " no ") << r.regla << " C2): ";
                getline(cin, input);
                r.componentes.push_back(input);

                cout << "\nDigite el componente C2 (" << r.componentes[0] << (r.estado ? " " : " no ") << r.regla << " C2): ";
                getline(cin, input);
                r.componentes.push_back(input);
                reglas->push_back(r);
                break;

            case 3:
                cout << "\nDigite el componente C1 (C1" << (r.estado ? " " : " no ") << r.regla << " C2, C3): ";
                getline(cin, input);
                r.componentes.push_back(input);

                cout << "\nDigite el componente C2 (" << r.componentes[0] << (r.estado ? " " : " no ") << r.regla << " C2, C3): ";
                getline(cin, input);
                r.componentes.push_back(input);

                cout << "\nDigite el componente C3 (" << r.componentes[0] << (r.estado ? " " : " no ") << r.regla << " " << r.componentes[1]<<", C3): ";
                getline(cin, input);
                r.componentes.push_back(input);
                reglas->push_back(r);
                break;
            default:
                if (numC > 3 && numC <= 10) //maximo 10 componentes por que paila mas :v
                {
                    for (int i = 0; i < numC; i++)
                    {
                        cout << "\nDigite el componente " << i+1 <<":";
                        getline(cin, input);
                        r.componentes.push_back(input);
                    }
                    reglas->push_back(r);
                }
                else
                {
                    cout << "\nNumero de componentes invalido. intente nuevamente.";
                    numC = -1;

                }
            }
        }
    }
    exp->push_back(*reglas);
}