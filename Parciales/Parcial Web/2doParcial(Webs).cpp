#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <string>

using namespace std;

struct DatosWeb{
    char url[20];
    int permanencia;
    char origen; // B:buscador, D:directo, O:otro
};

class Visita{
    public:
    Visita(int permanencia, char origen) : permanencia(permanencia), origen(origen) {}
    Visita(DatosWeb data) : permanencia(data.permanencia), origen(data.origen) {}
    
    virtual int getPuntacion() = 0;
    
    int getPermanencia() { return permanencia; }
    char getOrigen() { return origen; }

    protected:
    int permanencia;
    char origen;
};

class Buscador : public Visita{
    public:
    Buscador(DatosWeb data) : Visita(data) {}

    int getPuntacion() { return this->permanencia * 10; }
};

class Directo : public Visita{
    public:
    Directo(DatosWeb data) : Visita(data) {}

    int getPuntacion() { return this->permanencia * 15; }
};

class Otro : public Visita{
    public:
    Otro(DatosWeb data) : Visita(data) {}

    int getPuntacion() { return this->permanencia * 1; }
};

class Web{
    public:
    Web() = default;
    Web(const string &url) : url(url) {}
    
    void agregarVisita(Visita* v){ visitas.push_back(v); }
    bool revisarAcceso(char tipo){
        bool accedido;
        accedido = any_of(visitas.begin(), visitas.end(), [tipo](Visita* v){
            return v->getOrigen() == tipo;
        });
        return accedido;
    }
    int getPuntaje(){
        return accumulate(visitas.begin(), visitas.end(), 0, [](int sumatoria, Visita* v){
            return sumatoria + v->getPuntacion();
        });
    }

    string getUrl() { return url; }
    vector<Visita*> getVisitas(){ return visitas; }

    private:
    string url;
    vector<Visita*> visitas;
};

class GestorWeb{
    public:
    GestorWeb(const string &archivo){
        ifstream in(archivo, ios::binary);
        if (!in.is_open()) return;

        DatosWeb dat;
        map<string, vector<DatosWeb>> mapLectura;
        while(in.read((char*)&dat, sizeof(DatosWeb))){
            string url = dat.url;
            mapLectura[url].push_back(dat);
        }
        in.close();

        for (auto &p : mapLectura){
            Web pagina(p.first);
            for (auto &v : p.second){
                switch (v.origen){
                    case 'B': pagina.agregarVisita(new Buscador(v));
                    break;
                    case 'D': pagina.agregarVisita(new Directo(v));
                    break;
                    case 'O': pagina.agregarVisita(new Otro(v)); 
                    break;
                }
            }
            this->webs.push_back(pagina);
        }
    }

    Web getWeb(string url){
        for (auto &w : webs){
            if (w.getUrl() == url) return w;
        }
        return Web(url);
    }

    bool guardarPuntajes(const string &archivo){
        ofstream out(archivo);
        if (!out.is_open()) return false;
        
        for (auto &w : webs){
        out<< "url: "<< w.getUrl()<< " puntaje: "<< w.getPuntaje()<< endl;
        }

        out.close();
        return true;
    }

    vector<Web> getTop5(){
        auto top5 = webs;
        sort(top5.begin(), top5.end(), [](Web &w1, Web &w2){
            return w1.getPuntaje() > w2.getPuntaje();
        });
        if (top5.size() > 5) top5.resize(5);
        
        return top5;
    }

    vector<Web> getAccesosDirectos(){
        vector<Web> res;
        copy_if(webs.begin(), webs.end(), back_inserter(res), [](Web &w){
            return w.revisarAcceso('D');
        });
        return res;
    }

    private:
    vector<Web> webs;
};

int main() {
   
    return 0;
}