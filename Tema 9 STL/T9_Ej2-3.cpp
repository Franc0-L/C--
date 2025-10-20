#include <iostream>
#include <vector>
#include <algorithm>
/*
Realice un programa que al igual que una agenda, dado el nombre y apellido de la persona, 
retorne su numero de teléfono. Realice en el main que permita probar la agenda.

Realice un programa que al igual que una agenda de cumpleaños que dada una fecha, 
retorne las personas que nacieron dicha fecha. Realice en el main que permita probar la 
agenda. 
*/
using namespace std;

class Persona{
public:
    Persona(const string& nombre, int telefono, const string &FechaNac) : nombre(nombre), telefono(telefono), fechaNac(FechaNac){}

    string getNombre() const{ return nombre; }
    string getFechaNac() const{ return fechaNac; }
    int getTelefono() const{ return telefono; }

private:
    string nombre;
    string fechaNac;
    int telefono;
};

class Agenda{
public:
    Agenda() = default;

    void agregarPersona(const Persona &p){ personas.push_back(p); }

    void mostrarAgenda(){
        cout<< "Nombre Completo  Telefono  Fecha de Nacimiento\n";
        for (auto &p : personas){
            cout<< p.getNombre()<< "     "<< p.getTelefono()<< "     "<< p.getFechaNac()<< endl;
        }
    }

    Persona* buscarPorNombre(const string &nombre){
        auto it = find_if(personas.begin(), personas.end(), [&](const Persona &p){
            return (p.getNombre() == nombre);
        });
        if (it != personas.end()) return &(*it);
        else return nullptr;
    }

    vector<Persona*> buscarPorFecha(const string &fecha){
        vector<Persona*> resultado;
        for (auto &p : personas){
            if (p.getFechaNac() == fecha) resultado.push_back(&p);
        }
        return resultado;
    }

private:
    vector<Persona> personas;
};

int main(){
    Agenda agenda;
    agenda.agregarPersona(Persona("Juan Perez", 123456, "1990-05-23"));
    agenda.agregarPersona(Persona("Maria Gomez", 654321, "1985-12-01"));
    agenda.agregarPersona(Persona("Ana Lopez", 777888, "1990-05-23"));

    int opcion;
    do{
        cout<< "\nMenu\n";
        cout<< "1. Mostrar agenda completa.\n";
        cout<< "2. Buscar telefono segun nombre.\n";
        cout<< "3. Buscar personas segun fecha.\n";
        cout<< "0. Salir\n";
        cout<< "Opcion: "; cin>> opcion;
        switch (opcion){
            case 0:
                break;

            case 1:{
                agenda.mostrarAgenda();
                break;
            }

            case 2:{
                string nombreBuscado;
                cout<< "Ingrese el nombre a buscar: ";
                cin.ignore(); getline(cin, nombreBuscado);
                Persona* encontrado = agenda.buscarPorNombre(nombreBuscado);

                if (encontrado){
                    cout<< nombreBuscado<< ": "<< endl;
                    cout<< "Telefono: "<< encontrado->getTelefono();
                    cout<< "\nFecha de Nacimiento: "<< encontrado->getFechaNac()<< endl;
                } else cout<< "Persona no encontrada."<< endl;
                break;
            }

            case 3:{
                string fechaBuscada;
                cout<< "Ingrese la fecha de nacimiento a buscar (AAAA-MM-DD): ";
                cin.ignore(); getline(cin, fechaBuscada);
                vector<Persona*> cumpleaños = agenda.buscarPorFecha(fechaBuscada);
                if (!cumpleaños.empty()){
                    cout<< "Personas nacidas en "<< fechaBuscada<< ":\n";
                    for (Persona* pp : cumpleaños){
                        cout<< "- "<< pp->getNombre()<< " (Tel: "<< pp->getTelefono()<< ")\n";
                    }
                } else{
                    cout<< "No se encontraron personas con esa fecha.\n";
                }
                break;
            }
        }
    } while(opcion != 0);

    return 0;
}