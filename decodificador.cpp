#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <string>

using namespace std;

// Función para decodificar un bloque de bits según las reglas especificadas
string decodificarBloque(const string &bloque, int bloqueNum) {
    string texto;
    
    if (bloqueNum == 1) {
        for (char c : bloque) {
            texto += (c == '0') ? '1' : '0';
        }
    } else {
        for (int i = 0; i < bloque.size(); i += 2) {
            texto += (bloque[i] == '0') ? '1' : '0';
        }
    }

    return texto;
}

// Función para decodificar un archivo de texto según las reglas especificadas
void decodificarArchivo(const char *nombreArchivoEntrada, const char *nombreArchivoSalida, int semilla) {
    ifstream archivoEntrada(nombreArchivoEntrada);
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

    string bits;
    for (char c : contenido) {
        bits += bitset<8>(c).to_string();
    }

    string texto;
    for (int i = 0; i < bits.size(); i += semilla) {
        string bloque = bits.substr(i, semilla);
        texto += decodificarBloque(bloque, (i / semilla) + 1);
    }

    // Guarda el archivo decodificado
    ofstream archivoSalida(nombreArchivoSalida);
    if (!archivoSalida.is_open()) {
        cerr << "Error al abrir el archivo de salida." << endl;
        return;
    }

    for (int i = 0; i < texto.size(); i += 8) {
        string byte = texto.substr(i, 8);
        char caracter = (char)bitset<8>(byte).to_ulong();
        archivoSalida.put(caracter);
    }

    archivoSalida.close();
    cout << "Archivo decodificado y guardado como '" << nombreArchivoSalida << "'." << endl;
}

int main() {
    int semilla;
    string nombreArchivoEntrada = "codificado.txt";  // Nombre del archivo codificado
    string nombreArchivoSalida = "decodificado.txt"; // Nombre del archivo de salida

    cout << "Ingrese la semilla de decodificación (entero): ";
    cin >> semilla;

    // Agrega esta línea para descartar el carácter de nueva línea residual
    cin.ignore();

    decodificarArchivo(nombreArchivoEntrada.c_str(), nombreArchivoSalida.c_str(), semilla);

    return 0;
}