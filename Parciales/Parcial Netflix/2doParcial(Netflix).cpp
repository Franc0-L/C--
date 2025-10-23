#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>
/*
Netflix te ha contratado para diseñar y programar un sistema de votación de sus series y películas. En esta plataforma
los usuarios pueden votar las series y películas que han visto, asignándoles una calificación del 0 al 5. Dicha empresa
tiene los datos de sus series y películas en un archivo binario con el siguiente formato: 
int nro
char[200] nombre
char tipo // s : serie, p : película. 

Y los datos de las votaciones se encuentran en otro archivo binario con el siguiente formato: 
int nro
int votación
Donde nro es el identificador de la pelicula o serie. Además la valoración de una película o serie está dada por el promedio de sus votaciones. 

1. (30) Realice el diseño que lea los archivos y permita mostrar el nombre de las películas o series y su valoración. 
2. (25) Escriba un archivo de texto que permita saber el nombre de las películas o series y su valoración con el
siguiente formato: “Nombre ………. valoración”.

3. (30) Realice funciones usando stl :
(10) Que obtenga las 5 películas o series con mayor valoración.
(10) Que obtenga el o las películas o series más votadas.
(10) Que obtenga la cantidad de votos de las series y la cantidad de votos de las películas (utilice map)
*/
using namespace std;

struct MediaBin{
    int nro;
    char nombre[200];
    char tipo;
};

struct VotacionBin{
    int nro;
    int votacion;
};

class Media{
    public:
    Media() = default;
    Media(int nro, const string &nombre, char tipo) : nro(nro), nombre(nombre), tipo(tipo) {}

    int getNro() const{ return nro; }
    string getNombre() const{ return nombre; }
    char getTipo() const { return tipo; }
    int getCantVotos() const{ return votos.size(); }

    void agregarVoto(int voto){
        if (voto >= 0 && voto <= 5) votos.push_back(voto);
        else "Rango de votacion invalido.";
    }

    float getPromedio() const{
        if (votos.empty()) return 0.0;
        float suma = 0;
        for (int v : votos) suma += v; 

        return (suma / votos.size());
    }

    void mostrarMedia(){
        cout<< nombre<< " ("<< (tipo== 's' ? "Serie)\n":"Película)\n")
        << " votos = "<< votos.size()<< endl
        << " promedio = "<< getPromedio()<< endl;
    }

    private:
    int nro;
    string nombre;
    char tipo; // S: serie, P: pelicula
    vector<int> votos;
};

class Netflix{
    public:
    Netflix() = default;

    void leerMedia(const string &archivo){
        ifstream in(archivo, ios::binary);
        if (!in.is_open()) return;
        
        MediaBin auxM;
        while(in.read((char*)&auxM, sizeof(MediaBin))){
           Media m(auxM.nro, auxM.nombre, auxM.tipo);
           catalogo.emplace_back(m);
        }
        
        in.close();
    }

    void leerVotacion(const string &archivo){
        ifstream in(archivo, ios::binary);
        if (!in.is_open()) return;

        VotacionBin auxV;
        while(in.read((char*)&auxV, sizeof(VotacionBin))){
            auto it = find_if(catalogo.begin(), catalogo.end(), [&auxV](Media &m){ return m.getNro() == auxV.nro; });
            if (it != catalogo.end() && auxV.votacion >= 0 && auxV.votacion <= 5) it->agregarVoto(auxV.votacion);
        }
    }

    bool guardarMedia(const string &archivo){
        ofstream out(archivo, ios::binary);
        if (!out.is_open()) return false;
        
        for (const auto &m : catalogo){
            string nombre = m.getNombre();
            out<< nombre<< " ";
            out<< m.getPromedio()<< "\n";
        }
        
        out.close();
        return true;
    }

    vector<Media> top5Valoracion() const{
        vector<Media> aux = catalogo;
        sort(aux.begin(), aux.end(), [](const Media &a, const Media &b){
            if (a.getPromedio() != b.getPromedio()) return a.getPromedio() > b.getPromedio();
            return a.getCantVotos() > b.getCantVotos();
        });
        if (aux.size() > 5) aux.resize(5);
        return aux;
    }

    vector<Media> masVotadas() const{
        vector<Media> aux;
        if (catalogo.empty()) return aux;
        int maxVotos = max_element(catalogo.begin(), catalogo.end(), [](const Media &a, const Media&b){
            return a.getCantVotos() < b.getCantVotos();
        })->getCantVotos();

        copy_if(catalogo.begin(), catalogo.end(), back_inserter(aux), [maxVotos](const Media &c){
            return c.getCantVotos() == maxVotos;
        });

        return aux;
    }

    map<char, int> votosPorTipo() const{
        map<char, int> cont;
        for (const auto &v : catalogo) cont[v.getTipo()] += v.getCantVotos();
        return cont;
    }

    void mostrarTop5(){
        cout<< "\nTop 5 por valoracion\n";
        for (auto &v : top5Valoracion()) v.mostrarMedia(); cout<< endl;
    }

    void mostrarMasVotadas(){
        cout<< "\nMas votadas\n";
        for (auto &v : masVotadas()) v.mostrarMedia(); cout<< endl;
    }

    void mostrarVotosPorTipo(){
        cout<< "\nVotos por tipo\n";
        auto votos = votosPorTipo();
        cout<< "Series ('s'): "<< votos['s']<< endl;
        cout<< "Peliculas ('p'): "<< votos['p']<< endl;
    }

    void mostrarCatalogo(){
        cout<< "\nCatalogo\n";
        for (const auto &c : catalogo){
            cout<< c.getNombre()
            << " Cantidad de votos: "<< c.getCantVotos()
            << " Promedio de votos: "<< c.getPromedio()<< endl;
        }
    }

    private:
    vector<Media> catalogo;
};

int main() {
    Netflix app;

    app.leerMedia("medias.bin");
    app.leerVotacion("votos.bin");

    app.mostrarCatalogo();
    app.guardarMedia("valoraciones.txt");

    app.mostrarTop5();
    app.mostrarMasVotadas();
    app.mostrarVotosPorTipo();

    return 0;
}