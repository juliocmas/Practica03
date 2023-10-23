#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <string>

using namespace std;

// Func. codificar método_01
string metodo_01(const string &bloque, int semilla) {
    string codigo;
    int numUnos = 0, numCeros = 0;

    for (char c : bloque) {
        if (c == '1') {
            numUnos++;
        } else {
            numCeros++;
        }
    }

    if (semilla == 1) {
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

// Func. codificar método_02
string metodo_02(const string &bloque) {
    string codigo;

    for (int i = bloque.size() - 1; i >= 0; i--) {
        codigo += bloque[i];
    }

    return codigo;
}

// Func. para decodificar método_01
string metodo_01_decodificar(const string &bloque, int semilla) {
    string codigo;
    int numUnos = 0, numCeros = 0;

    for (char c : bloque) {
        if (c == '1') {
            numUnos++;
        } else {
            numCeros++;
        }
    }

    if (semilla == 1) {
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

// Func. para decodificar método_02
string metodo_02_decodificar(const string &bloque) {
    string codigo;

    for (int i = bloque.size() - 1; i >= 0; i--) {
        codigo += bloque[i];
    }

    return codigo;
}

// Func.  codificar archivo de  según el método y semilla selec
void codificarArchivo(const char *nombreArchivoEntrada, const char *nombreArchivoSalida, int semilla, int metodo) {
    ifstream archivoEntrada(nombreArchivoEntrada, ios::binary);
    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo de entrada." << endl;
        return;
    }

    // Lector contenido archivo en binario
    string contenido((istreambuf_iterator<char>(archivoEntrada)), (istreambuf_iterator<char>()));
    archivoEntrada.close();

    string codigo;
    for (char c : contenido) {
        string bits = bitset<8>(c).to_string();
        string bloque_codificado;

        if (metodo == 1) {
            bloque_codificado = metodo_01(bits, semilla);
        } else if (metodo == 2) {
            bloque_codificado = metodo_02(bits);
        }

        codigo += bloque_codificado;
    }

    // Guarda archivo codificado en binario
    ofstream archivoSalida(nombreArchivoSalida, ios::binary);
    archivoSalida.write(codigo.c_str(), codigo.size());
    archivoSalida.close();

    cout << "Archivo codificado y guardado como '" << nombreArchivoSalida << "'." << endl;
}

// Func decodificar archivo método y semilla selc
void decodificarArchivo(const char *nombreArchivoEntrada, const char *nombreArchivoSalida, int semilla, int metodo) {
    ifstream archivoEntrada(nombreArchivoEntrada, ios::binary);
    if (!archivoEntrada.is_open()) {
        cerr << "Error al abrir el archivo de entrada." << endl;
        return;
    }

    // Lector contenido archivo en binario
    string contenido((istreambuf_iterator<char>(archivoEntrada)), (istreambuf_iterator<char>()));
    archivoEntrada.close();

    string resultado;
    for (char c : contenido) {
        string bits = bitset<8>(c).to_string();
        string bloque_decodificado;

        if (metodo == 1) {
            bloque_decodificado = metodo_01_decodificar(bits, semilla);
        } else if (metodo == 2) {
            bloque_decodificado = metodo_02_decodificar(bits);
        }

        resultado += bloque_decodificado;
    }

    // Guarda archivo decodificado binario
    ofstream archivoSalida(nombreArchivoSalida, ios::binary);
    archivoSalida.write(resultado.c_str(), resultado.size());
    archivoSalida.close();

    cout << "Archivo decodificado y guardado como '" << nombreArchivoSalida << "'." << endl;
}

int main() {
    int opcion;
    cout << "Seleccione una opción:\n";
    cout << "1. Codificar con método_01\n";
    cout << "2. Codificar con método_02\n";
    cout << "3. Decodificar\n";
    cout << "Elija una opción (1, 2 o 3): ";
    cin >> opcion;

    int metodo, semilla;
    string rutaArchivoEntrada, rutaArchivoSalida;

    if (opcion == 1 || opcion == 2) {
        cout << "Ingrese la semilla de codificación (entero): ";
        cin >> semilla;

        // Agrega esta línea para descartar el carácter de nueva línea residual
        cin.ignore();

        cout << "Ingrese la ruta completa del archivo de entrada: ";
        getline(cin, rutaArchivoEntrada);

        cout << "Ingrese la ruta completa y el nombre del archivo de salida: ";
        getline(cin, rutaArchivoSalida);

        metodo = opcion;
        codificarArchivo(rutaArchivoEntrada.c_str(), rutaArchivoSalida.c_str(), semilla, metodo);
    } else if (opcion == 3) {
        cout << "Ingrese la semilla de decodificación (entero): ";
        cin >> semilla;

        // Agrega esta línea para descartar el carácter de nueva línea residual
        cin.ignore();

        cout << "Ingrese la ruta completa del archivo codificado de entrada: ";
        getline(cin, rutaArchivoEntrada);

        cout << "Ingrese la ruta completa y el nombre del archivo de salida (decodificado): ";
        getline(cin, rutaArchivoSalida);

        // Método de decodificación
        cout << "Seleccione el método de decodificación:\n";
        cout << "1. método_01\n";
        cout << "2. método_02\n";
        cout << "Elija un método (1 o 2): ";
        cin >> metodo;

        decodificarArchivo(rutaArchivoEntrada.c_str(), rutaArchivoSalida.c_str(), semilla, metodo);
    } else {
        cout << "Opción no válida. Seleccione 1, 2 o 3." << endl;
    }

    return 0;
}
