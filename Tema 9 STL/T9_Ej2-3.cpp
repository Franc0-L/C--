#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
/*
Realice un programa que al igual que una agenda, dado el nombre y apellido de la persona, 
retorne su numero de teléfono. Realice en el main que permita probar la agenda.

Realice un programa que al igual que una agenda de cumpleaños que dada una fecha, 
retorne las personas que nacieron dicha fecha. Realice en el main que permita probar la 
agenda.

2. Debe realizar una agenda que permita guardar personas con sus contactos. 
  Una persona tiene un nombre, apellido, dirección y contactos 
  Un contacto tiene un tipo (email, teléfono, celular, fax, whatsapp, etc) y un valor 
La agenda debe guardar los contactos en un archivo binario y debe permitir buscar personas para saber sus datos.
*/
using namespace std;

class Contacto{
    public:
    Contacto() = default;
    Contacto(const string &tipo, int valor) : tipo(tipo), valor(valor) {}

    string getTipo() const{ return tipo; }
    int getValor() const{ return valor; }

    private:
    string tipo; // email, teléfono, celular, fax, whatsapp, etc.
    int valor;
};

class Persona{
    public:
    Persona() = default;
    Persona(const string& nom, int tel, const string &fNac, const string &dir) : nombre(nom), telefono(tel), fechaNac(fNac), direccion(dir) {}

    void agregarContacto(const Contacto &c){ contactos.push_back(c); }

    void mostrarDatos(){
        cout<< nombre<< "  -  "<< telefono<< "  -  "<< direccion<< "  -  "<< fechaNac<< endl;
        if (contactos.size() > 0) mostrarContactos();
    }

    void mostrarContactos(){
        cout<< "Contacto/s: \n";
        for (auto n: contactos) cout<< n.getTipo()<< "  -  "<< n.getValor()<< endl;
    }

    string getNombre() const{ return nombre; }
    string getFechaNac() const{ return fechaNac; }
    string getDireccion() const{ return direccion; }
    int getTelefono() const{ return telefono; }
    vector<Contacto> getContactos() const{ return contactos; }
    
    private:
    string nombre;
    string direccion;
    string fechaNac;
    int telefono;
    vector<Contacto> contactos;
};


class Agenda{
    public:
    Agenda() = default;

    void agregarPersona(const Persona &p){ personas.push_back(p); }

    void mostrarAgenda(){
        cout<< "Nombre Completo - Teléfono - Dirección - Fecha de Nacimiento\n";
        for (auto &p : personas){
            p.mostrarDatos();
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

    void guardarContactos(const string &archivo){
        ofstream guardar(archivo, ios::binary);
        if (!guardar.is_open()) return;

        for (auto &p : personas){
            string nombre = p.getNombre(); size_t len = nombre.size();
            guardar.write(reinterpret_cast<const char*>(&len), sizeof(len));
            if (len) guardar.write(nombre.data(), len);

            string direccion = p.getDireccion(); len = direccion.size();
            guardar.write(reinterpret_cast<const char*>(&len), sizeof(len));
            if (len) guardar.write(direccion.data(), len);

            string fecha = p.getFechaNac(); len = fecha.size();
            guardar.write(reinterpret_cast<const char*>(&len), sizeof(len));
            if (len) guardar.write(fecha.data(), len);

            int telefono = p.getTelefono();
            guardar.write(reinterpret_cast<const char*>(&telefono), sizeof(telefono));

            auto contactos = p.getContactos();
            for (const auto &c : contactos){
                string tipo = c.getTipo(); len = tipo.size();
                guardar.write(reinterpret_cast<const char*>(&len), sizeof(len));
                if (len) guardar.write(tipo.data(), len);

                int valor = c.getValor();
                guardar.write(reinterpret_cast<const char*>(&valor), sizeof(valor));
            }
        }
        guardar.close();
    }

    private:
    vector<Persona> personas;
};

int main(){
    Agenda agenda;
    agenda.agregarPersona(Persona("Juan Perez", 123456, "1990-05-23", "Pablo Crausaz"));
    agenda.buscarPorNombre("Juan Perez")->agregarContacto(Contacto("email", 1));
    agenda.agregarPersona(Persona("Maria Gomez", 654321, "1985-12-01", "Av. Ramirez"));
    agenda.agregarPersona(Persona("Ana Lopez", 777888, "1990-05-23", "Calle Italia"));
    agenda.buscarPorNombre("Ana Lopez")->agregarContacto(Contacto("fax", 3));

    int opcion;
    do{
        cout<< "\nMenu\n";
        cout<< "1. Mostrar agenda completa.\n";
        cout<< "2. Buscar persona segun nombre.\n";
        cout<< "3. Buscar personas segun fecha.\n";
        cout<< "4. Guardar personas en archivo binario\n";
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
                    cout<< "\nNombre Completo - Teléfono - Dirección - Fecha de Nacimiento\n";
                    encontrado->mostrarDatos();
                } else cout<< "\nPersona no encontrada."<< endl;
                break;
            }

            case 3:{
                string fechaBuscada;
                cout<< "Ingrese la fecha de nacimiento a buscar (AAAA-MM-DD): ";
                cin.ignore(); getline(cin, fechaBuscada);
                vector<Persona*> cumpleaños = agenda.buscarPorFecha(fechaBuscada);
                if (!cumpleaños.empty()){
                    cout<< "Personas nacidas en "<< fechaBuscada<< ":\n";
                    cout<< "\nombre Completo - Teléfono - Dirección - Fecha de Nacimiento\n";
                    for (Persona* pp : cumpleaños){
                        pp->mostrarDatos();
                    }
                } else{
                    cout<< "\nNo se encontraron personas con esa fecha.\n";
                }
                break;
            }

            case 4:{
                agenda.guardarContactos("personas.bin");
                cout<< "Personas de la agenda guardadas correctamente...\n";
                break;
            }
        }
    } while(opcion != 0);

    return 0;
}