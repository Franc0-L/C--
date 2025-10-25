#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

/*
La empresa Telecom lo contrata dado que desea brindar un servicio de notificaciones para empresas.
El sistema deberá poder enviar email y mensajes de texto. Con las siguientes características:
• El email tiene una dirección de email a quien va dirigido y un texto.
• El mensaje de texto un nro telefónico y un texto de no más de 150 caracteres

Los email a enviar están en un archivo binario con el siguiente formato:
int nro
char[100] email
char[500] texto

Los mensajes de texto están en un archivo binario con el siguiente formato:
int nro
int nro_telefono
char[150] texto

El sistema debe crear un archivo de texto que es utilizado para enviar las notificaciones, el cual contiene una línea por notificación.

En el caso de email se debe escribir la letra "E" seguida del email y el texto. Y en el caso del mensaje de
texto la letra "T", seguida del número de teléfono y el texto.
El archivo de texto debe ser escrito de forma ordenada por número (campo nro)

Además es necesario saber los email o teléfonos repetidos.
*/
using namespace std;

// Structs para archivos binarios
struct strEmail{
    int nro;
    char email[100];
    char texto[500];
};

struct strTxt{
    int nro;
    int nro_telefono;
    char texto[150];
};

// Clases
class Notificacion{
    public:
    Notificacion() = default;
    Notificacion(int nro, const string &texto) : nro(nro), texto(texto) {}

    int getNro() const { return nro; }
    const string getTexto() const{ return texto; }

    virtual string mostrarNotificacion(){
        return to_string(nro) + " " + texto;
    }

    protected:
    int nro = 0;
    string texto;
};

class Email : public Notificacion{
    public:
    Email() = default;
    Email(int nro, const string &texto, const string &correo) : Notificacion(nro, texto), correo(correo) {}

    const string getCorreo() const { return correo; }

    string mostrarNotificacion() override{
        return string("E ") + correo + " " + texto;
    }

    private:
    string correo;
};

class Txt : public Notificacion{
    public:
    Txt(int nro, const string &texto, string nro_telefono) : Notificacion(nro, texto), nro_telefono(nro_telefono) {}

    string getNroTelefono() const { return nro_telefono; }

    string mostrarNotificacion() override{
        return string("T ") + nro_telefono + " " + texto;
    }

    private:
    string nro_telefono;
};

class Telecom{
    public:
    Telecom() = default;

    void leerTxt(const string &archivo){
        ifstream leer(archivo, ios::binary);
        if (!leer.is_open()) return;
        strTxt st;
        while (leer.read((char*)&st, sizeof(strTxt))){
            Txt* txt = new Txt(st.nro, string(st.texto), to_string(st.nro_telefono));

            notificaciones.emplace_back(txt);
        }
        leer.close();
    }

    void leerEmail(const string &archivo){
        ifstream leer(archivo, ios::binary);
        if (!leer.is_open()) return;
        strEmail em;
        while (leer.read((char*)&em, sizeof(strEmail))){
            Email* email = new Email(em.nro, string(em.texto), string(em.email));

            notificaciones.emplace_back(email);
        }
        leer.close();
    }

    bool guardarNotificacion(const string &archivo){
        ofstream guardar(archivo, ios::binary);
        if (!guardar.is_open()) return false;

        // Ordenar por numero
        sort(notificaciones.begin(), notificaciones.end(), [](const Notificacion* a, const Notificacion* b){
            return a->getNro() > b->getNro();
        });
        // Alternativa con GREATER: sort(notificaciones.begin(), notificaciones.end(), greater<Notificacion>());
        
        for (auto &n : notificaciones){
            string noti = n->mostrarNotificacion();
            guardar<< noti<< endl;
        }

        return true;
    }

    vector<Notificacion*> getRepetidos(){
        vector<Notificacion*> repetidos;

        // Listas (clave, puntero) para emails y telefonos
        vector<pair<string, Notificacion*>> emails;
        vector<pair<string, Notificacion*>> telefonos;

        // Poblar las listas
        for (Notificacion* n : notificaciones){
            if (Email* e = dynamic_cast<Email*>(n)){
                string key = e->getCorreo();
                if (!key.empty()) emails.emplace_back(key, n);
            } else if (Txt* t = dynamic_cast<Txt*>(n)){
                string key = t->getNroTelefono();
                if (!key.empty()) telefonos.emplace_back(key, n);
            }
        }

        // Función lambda para procesar una lista ordenada y añadir los elementos repetidos
        auto procesar = [&](vector<pair<string, Notificacion*>> &lista){
            if (lista.empty()) return;

            // ordenar por clave (lexicográfico)
            sort(lista.begin(), lista.end(), [](const auto &a, const auto &b){
                return a.first < b.first;
            });

            // recorrer y detectar claves consecutivas iguales
            size_t i = 0;
            while (i < lista.size()){
                size_t j = i + 1;
                while (j < lista.size() && lista[j].first == lista[i].first) ++j;

                // si hay más de 1 con la misma clave -> son repetidos
                if (j - i > 1){
                    for (size_t k = i; k < j; ++k){
                        // evitar duplicados en el vector resultado
                        if (find(repetidos.begin(), repetidos.end(), lista[k].second) == repetidos.end()){
                            repetidos.push_back(lista[k].second);
                        }
                    }
                }

                i = j; // avanzar al siguiente bloque de claves
            }
        };

        procesar(emails);
        procesar(telefonos);

        return repetidos;
    }

    private:
    vector<Notificacion*> notificaciones;
};

// Main de prueba
int main() {
    Telecom app;

    app.leerEmail("emails.bin");
    app.leerTxt("txts.bin");
    app.getRepetidos();

    app.guardarNotificacion("notificaciones.txt");

    return 0;
}