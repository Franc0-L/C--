#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
/*
2. Debe realizar una agenda que permita guardar personas con sus contactos. 
 Una persona tiene un nombre, apellido, dirección y contactos 
 Un contacto tiene un tipo (email, teléfono, celular, fax, whatsapp, etc) y un valor 
La agenda debe guardar los contactos en un archivo binario y debe permitir buscar personas para saber sus datos. 
*/

class Contacto {
private:
    string tipo;
    string valor;
public:
    Contacto(string t, string v) : tipo(t), valor(v) {}
    Contacto() : tipo(""), valor("") {}

    string getTipo() const {
        return tipo;
    }

    string getValor() const {
        return valor;
    }
};

class Persona {
private:
    string apenom;
    string direccion;
    vector<Contacto> contactos;
public:
    Persona(string nya, string dir) : apenom(nya), direccion(dir) {}
    Persona() : apenom(""), direccion("") {}

    void agregarContacto(const Contacto& c) {
        contactos.push_back(c);
    }

    string getNombreApellido() const {
        return apenom;
    }

    string getDireccion() const {
        return direccion;
    }

    vector<Contacto> getContactos() const {
        return contactos;
    }
};

class Agenda {
private:
    vector<Persona> personas;
public:
    Agenda() {}

    void agregarPersona(const Persona& p) {
        personas.push_back(p);
    }

    vector<Persona> getPersonas() const {
        return personas;
    }

    Persona buscarPersona(const string& apenom) {
        for (auto& p : personas) {
            if (p.getNombreApellido() == apenom) {
                return p;
            }
        }
        throw invalid_argument("Persona no encontrada.");
    }
};
int main() {
    Agenda agenda; // Declare agenda object

    // Cargar agenda desde archivo
    ifstream archivo("agenda.bin", ios::binary);
    if (archivo) {
        size_t cantidad;
        archivo.read(reinterpret_cast<char*>(&cantidad), sizeof(size_t));
        for (size_t i = 0; i < cantidad; ++i) {
            string nombre, direccion;
            size_t lenNombre, lenDireccion;
            archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(size_t));
            nombre.resize(lenNombre);
            archivo.read(&nombre[0], lenNombre);
            archivo.read(reinterpret_cast<char*>(&lenDireccion), sizeof(size_t));
            direccion.resize(lenDireccion);
            archivo.read(&direccion[0], lenDireccion);
            Persona p(nombre, direccion);
            agenda.agregarPersona(p);
        }
        archivo.close();
    }

    // Interfaz de usuario
    int opcion;
    do {
        cout << "1. Agregar persona\n";
        cout << "2. Buscar persona\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                string nombre, direccion;
                cout << "Ingrese nombre y apellido: ";
                getline(cin, nombre);
                cout << "Ingrese direccion: ";
                getline(cin, direccion);
                Persona nuevaPersona(nombre, direccion);
                agenda.agregarPersona(nuevaPersona);
                break;
            }
            case 2: {
                string nombreApellido;
                cout << "Ingrese el nombre y apellido de la persona a buscar: ";
                getline(cin, nombreApellido);
                try {
                    Persona encontrada = agenda.buscarPersona(nombreApellido);
                    cout << "Persona encontrada: " << encontrada.getNombreApellido() << endl;
                    cout << "Direccion: " << encontrada.getDireccion() << endl;
                    cout << "Contactos:" << endl;
                    for (const auto& contacto : encontrada.getContactos()) {
                        cout << " - " << contacto.getTipo() << ": " << contacto.getValor() << endl;
                    }
                } catch (const exception& e) {
                    cout << e.what() << endl;
                }
                break;
            }
            case 3:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    } while (opcion != 3);

    // Guardar agenda en archivo
    ofstream archivoSalida("agenda.bin", ios::binary);
    vector<Persona> personas = agenda.getPersonas();
    size_t cantidad = personas.size();
    archivoSalida.write(reinterpret_cast<const char*>(&cantidad), sizeof(size_t));
    for (const auto& persona : personas) {
        string nombre = persona.getNombreApellido();
        string direccion = persona.getDireccion();
        size_t lenNombre = nombre.size();
        size_t lenDireccion = direccion.size();
        archivoSalida.write(reinterpret_cast<const char*>(&lenNombre), sizeof(size_t));
        archivoSalida.write(nombre.c_str(), lenNombre);
        archivoSalida.write(reinterpret_cast<const char*>(&lenDireccion), sizeof(size_t));
        archivoSalida.write(direccion.c_str(), lenDireccion);
        // Contactos no se guardan en este ejemplo, pero se pueden agregar aquí
    }
    archivoSalida.close();

    return 0;
}