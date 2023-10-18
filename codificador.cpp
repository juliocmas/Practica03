#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <string>

using namespace std;

// Función para codificar un bloque de bits según las reglas especificadas
string codificarBloque(const string &bloque, int bloqueNum) {
    string codigo;
    int numUnos = 0, numCeros = 0;

    for (char c : bloque) {
        if (c == '1') {
            numUnos++;
        } else {
            numCeros++;
        }
    }

    if (bloqueNum == 1) {
        for (char c : bloque) {
            codigo += (c == '0') ? '1' : '0';
        }
    } else {
        if (numUnos == numCeros) {
            for (char c : bloque) {
                codigo += (c == '0') ? '1' : '0';
            }
        } else if (numCeros > numUnos) {
            for (int i = 0; i < bloque.size(); i += 2) {
                codigo += (bloque[i] == '0') ? '1' : '0';
                if (i + 1 < bloque.size()) {
                    codigo += bloque[i + 1];
                }
            }
        } else {
            for (int i = 0; i < bloque.size(); i += 3) {
                codigo += (bloque[i] == '1') ? '0' : '1';
                if (i + 1 < bloque.size()) {
                    codigo += bloque[i + 1];
                }
                if (i + 2 < bloque.size()) {
                    codigo += bloque[i + 2];
                }
            }
        }
    }

    return codigo;
}

// Función para codificar un archivo de texto según las reglas especificadas
void codificarArchivo(const char *nombreArchivo, int semilla) {
    ifstream archivoEntrada(nombreArchivo);
    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo de entrada." << endl;
        return;
    }

    string contenido;
    char c;
    while (archivoEntrada.get(c)) {
        contenido += c;
    }

    archivoEntrada.close();

    // Convierte el contenido a una secuencia de bits
    string bits;
    for (char c : contenido) {
        bits += bitset<8>(c).to_string();
    }

    string codigo;
    for (int i = 0; i < bits.size(); i += semilla) {
        string bloque = bits.substr(i, semilla);
        codigo += codificarBloque(bloque, (i / semilla) + 1);
    }

    // Guarda el archivo codificado
    ofstream archivoSalida("codificado.txt");
    if (!archivoSalida.is_open()) {
        cerr << "Error al abrir el archivo de salida." << endl;
        return;
    }

    for (int i = 0; i < codigo.size(); i += 8) {
        string byte = codigo.substr(i, 8);
        char caracter = (char)bitset<8>(byte).to_ulong();
        archivoSalida.put(caracter);
    }

    archivoSalida.close();
    cout << "Archivo codificado y guardado como 'codificado.txt'." << endl;
}

int main() {
    int semilla;
    string nombreArchivoEntrada;

    cout << "Ingrese la semilla de codificación (entero): ";
            cin >> semilla;

    // Agrega esta línea para descartar el carácter de nueva línea residual
    cin.ignore();

    cout << "Ingrese la ubicación del archivo de entrada: ";
        getline(cin, nombreArchivoEntrada);

    codificarArchivo(nombreArchivoEntrada.c_str(), semilla);

    return 0;
}
