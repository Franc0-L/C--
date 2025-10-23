#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;
/*
La UADER lo contrata para realizar un sistema que permita saber si un alumno aprobó o no un curso. Para lo cual
cuenta con dos archivos binarios donde se encuentra la información.
Alumnos.dat
legajo: int
Nombre: char[300]
tipo: char //R regular o L libre

Notas.dat
legajo: int
int Nota: int

Los Alumnos del curso pueden ser regulares, que aprueban con promedio 60 o mayor y ninguna nota menor a 55 o pueden ser
libres que se les toma un solo examen que tiene que tener nota mayor a 70.

1. (30) Realice el diseño y programe las clases que permitan saber qué alumnos aprobaron el curso.

2. (20) Escriba un archivo de texto que permita saber los alumnos que aprobaron y desaprobaron con el siguiente formato:
“Nombre ………. Aprobó o no aprobó”.

3. (35) Realice funciones usando stl :
• (10) Que obtenga el o los alumnos con mayor promedio
• (10) Que obtenga el promedio total discriminados por si aprobó o no.
• (15) Que obtenga los alumnos regulares que tienen menos cantidad de notas que la mayoría.
*/

struct strAlumnos{
    int legajo;
    char nombre[300];
    char tipo; // R: regular, L: libre
};

struct strNotas{
    int legajo;
    int nota;
};

class Nota{
    public:
    Nota() = default;
    Nota(int legajo, int puntaje) : legajo(legajo), puntaje(puntaje) {}

    int getLegajo() const{ return legajo; }
    int getPuntaje() const{ return puntaje;}

    private:
    int legajo;
    int puntaje;
};

class Alumno{
    public:
    Alumno() = default;
    Alumno(int legajo, const string &nombre, char tipo = 'L') : legajo(legajo), nombre(nombre), tipo(tipo) {}

    int getLegajo() const { return legajo; }
    const string getNombre() const{ return nombre; }
    char getTipo() const{ return tipo; }
    const vector<Nota>& getNotas() const { return notas; }

    void agregarNota(Nota &n){ notas.push_back(n); }
    
    float calcularPromedio() const{
        float prome = 0.0f;
        if (notas.empty()) return prome;
        for (auto &n : notas) prome += n.getPuntaje();
        return prome / notas.size();
    }

    bool tieneNotaMenorQue(int num) const{
        for (auto &n : notas) if (n.getPuntaje() < num) return true;
        return false;
    }

    bool tieneNotaMayorQue(int num) const{
        for (auto &n : notas) if (n.getPuntaje() > num) return true;
        return false;
    }

    void mostrarAlumno() const{
        cout<< legajo<< " "<< nombre<< " "<< tipo<< endl;
        if (notas.size() > 0){
            cout<< "Notas:\n"; 
            for (auto &n : notas) cout<< n.getPuntaje()<< endl;
        }
    }

    private:
    int legajo;
    string nombre;
    char tipo;
    vector<Nota> notas;
};

class Curso{
    public:
    Curso() = default;

    void agregarAlumno(Alumno &a){ alumnos.push_back(a); }

    void mostrarCurso(){ for (auto &a : alumnos) a.mostrarAlumno(); }

    vector<Alumno> getAprobados(){
        vector<Alumno> aprobados;
        copy_if(alumnos.begin(), alumnos.end(), back_inserter(aprobados), [](const Alumno &a)->bool{
                if (a.getTipo() == 'R') {
                    return a.calcularPromedio() >= 60.0f && !a.tieneNotaMenorQue(55);
                } else if (a.getTipo() == 'L') {
                    return a.tieneNotaMayorQue(70);
                }
                return false;
            }
        );
        return aprobados;
    }

    bool estaAprobado(int legajo){
        bool aprobado;
        auto aprobados = getAprobados();
        for (size_t i = 0; aprobados.size(); ++i){
            if (aprobados[i].getLegajo() == legajo) return true;
        }
        return false;
    }

    void mostrarAprobados(){
        cout<< "Nombre ....... Aprobó o no aprobó\n";
        for (auto &a : getAprobados()){
            cout<< a.getNombre()<< "     ";
            if (estaAprobado(a.getLegajo())) cout<< "Si\n";
            else cout<< "No\n";
        }
    }

    void leerAlumnos(const string &archivo){
        ifstream leer(archivo, ios::binary);
        if (!leer.is_open()) return;

        strAlumnos al;
        while(leer.read((char*)&al, sizeof(strAlumnos))){
            Alumno a(al.legajo, al.nombre, al.tipo);
            alumnos.push_back(a);
        }
        leer.close();
    }
    void leerNotas(const string &archivo){
        ifstream leer(archivo, ios::binary);
        if (!leer.is_open()) return;

        strNotas nt;
        while(leer.read((char*)&nt, sizeof(strNotas))){
            Nota n(nt.legajo, nt.nota);
            for (auto &a : alumnos) if(a.getLegajo() == n.getLegajo()) a.agregarNota(n);
        }
        leer.close();
    }

    bool guardarAlumnos(const string &archivo){
        ofstream guardar(archivo);
        if (!guardar.is_open()) return false;
        guardar<< "Nombre ..... Aprobó o no aprobó\n";
        for (auto &a : alumnos){
            guardar<< a.getNombre()<< "  ";
            if (estaAprobado(a.getLegajo())) guardar<< "Si";
            else guardar<< "No";
        }

        guardar.close();
        return true;
    }
  
    vector<Alumno> mayorPromedio(){
        vector<Alumno> res;
        if (alumnos.empty()) return res;

        float maxProm = max_element(alumnos.begin(), alumnos.end(), [](const Alumno &a, const Alumno &b){
            return a.calcularPromedio() < b.calcularPromedio();
        })->calcularPromedio();

        copy_if(alumnos.begin(), alumnos.end(), back_inserter(res), [maxProm](const Alumno &a){
            return a.calcularPromedio() == maxProm;
        });

        return res;
    }
    void mostrarMayorPromedio(){
        for (auto &p : mayorPromedio()) p.mostrarAlumno(); cout<< endl;
    }

    pair<float,float> promedioPorTipo(){
        float sumaAprob = 0.0f, sumaDesaprob = 0.0f;
        int cantAprob = 0, cantDesaprob = 0;

        for (auto &a : alumnos){
            if (estaAprobado(a.getLegajo())){
                sumaAprob += a.calcularPromedio();
                cantAprob++;
            } else{
                sumaDesaprob += a.calcularPromedio();
                cantDesaprob;
            }
        }
        float promAprob = cantAprob ? sumaAprob / cantAprob : 0.0f;
        float promDesaprob =  cantDesaprob ? sumaDesaprob / cantDesaprob : 0.0f;

        return {promAprob, promDesaprob};
    }
    void mostrarPorTipo(){
        cout<< "\nPromedio por tipo\n";
        auto promedios = promedioPorTipo();
        cout<< "Promedio aprobados: "<< promedios.first<< endl;
        cout<< "Promedio no aprobados: "<< promedios.second<< endl;
    }

    vector<Alumno> regularMenorCantNotas(){
        vector<Alumno> regulares;
        vector<int> cantNotas;

        for (auto &a : alumnos){
            if (a.getTipo() == 'R'){
                regulares.push_back(a);
                cantNotas.push_back(a.getNotas().size());
            }
        }
        
        map<int,int> freq;
        for (int c : cantNotas) freq[c]++;
        int maxFreq = 0, moda = 0;
        for (auto &p : freq){
            if (p.second > maxFreq){
                maxFreq = p.second;
                moda = p.first;
            }
        }

        vector<Alumno> res;
        for (size_t i = 0; i < regulares.size(); ++i){
            if (cantNotas[i] < moda) res.push_back(regulares[i]);
        }

        return res;
    }
    void mostrarRegularMenorCantNotas(){
        cout<< "\nRegular con menor cantidad de notas\n";
        for (auto &r : regularMenorCantNotas()) r.mostrarAlumno();
    }

    private:
    vector<Alumno> alumnos;
};

int main() {
    Curso UADER;

    UADER.leerAlumnos("Alumnos.dat");
    UADER.leerNotas("Notas.dat");
    UADER.guardarAlumnos("alumnos.txt");
    
    UADER.mostrarCurso();
    UADER.mostrarAprobados();

    UADER.mostrarMayorPromedio();
    UADER.mostrarPorTipo();
    UADER.mostrarRegularMenorCantNotas();

    return 0;
}