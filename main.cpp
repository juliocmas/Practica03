#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <stdexcept>

using namespace std;

// Prototipos de funciones
string invertir(const string &binario);
string codificarBloque(const string &bloque);
string aplicarReglas(const string &bloque, const string &bloqueAnterior);
string decodificar(const string &binario, int n);

// Función para codificar un archivo de texto
void codificarArchivo(const string &rutaArchivo, int n, int opcion) {
    ifstream archivoEntrada(rutaArchivo, ios::binary);
    if (!archivoEntrada.is_open()) {
        throw runtime_error("Error al abrir el archivo.");
    }

    vector<char> contenido((istreambuf_iterator<char>(archivoEntrada)), istreambuf_iterator<char>());
    archivoEntrada.close();

    string contenidoBinario;
    for (char c : contenido) {
        contenidoBinario += bitset<8>(c).to_string();
    }

    string contenidoProcesado;
    string bloqueAnterior;

    for (size_t i = 0; i < contenidoBinario.length(); i += n) {
        string bloque = contenidoBinario.substr(i, n);

        if (opcion == 1) {
            // Método 01: Invertir cada bloque
            contenidoProcesado += invertir(bloque);
        } else if (opcion == 2) {
            // Método 02: Aplicar reglas según la cantidad de 0s y 1s
            if (i == 0) {
                contenidoProcesado += codificarBloque(bloque);
            } else {
                contenidoProcesado += aplicarReglas(bloque, bloqueAnterior);
            }
        }
        bloqueAnterior = bloque;
    }

    ofstream archivoSalida("file_codificado.txt", ios::binary);
    if (!archivoSalida.is_open()) {
        throw runtime_error("Error al abrir el archivo de salida.");
    }
    archivoSalida.write(contenidoProcesado.c_str(), contenidoProcesado.length());
    archivoSalida.close();

    cout << "Archivo procesado exitosamente. Se ha guardado como 'file_codificado.txt'" << endl;
}

// Función para decodificar un archivo de texto
void decodificarArchivo(const string &rutaArchivo, int n, int opcion) {
    ifstream archivoEntrada(rutaArchivo, ios::binary);
    if (!archivoEntrada.is_open()) {
        throw runtime_error("Error al abrir el archivo.");
    }

    vector<char> contenido((istreambuf_iterator<char>(archivoEntrada)), istreambuf_iterator<char>());
    archivoEntrada.close();

    string contenidoBinario;
    for (char c : contenido) {
        contenidoBinario += bitset<8>(c).to_string();
    }

    string contenidoProcesado;
    string bloqueAnterior;

    if (opcion == 1) {
        // Método 01: Invertir cada bloque
        contenidoProcesado = invertir(contenidoBinario);
    } else if (opcion == 2) {
        // Método 02: Decodificar según las reglas
        contenidoProcesado = decodificar(contenidoBinario, n);
    }

    vector<char> caracteres;
    for (size_t i = 0; i < contenidoProcesado.length(); i += 8) {
        caracteres.push_back(bitset<8>(contenidoProcesado.substr(i, 8)).to_ulong());
    }

    ofstream archivoSalida("file_decodificado.txt", ios::binary);
    if (!archivoSalida.is_open()) {
        throw runtime_error("Error al abrir el archivo de salida.");
    }
    archivoSalida.write(caracteres.data(), caracteres.size());
    archivoSalida.close();

    cout << "Proceso de decodificación completado. Se ha guardado como 'file_decodificado.txt'" << endl;
}

// Función para invertir los bits de una cadena binaria
string invertir(const string &binario) {
    string invertido;
    for (char bit : binario) {
        invertido += (bit == '0') ? '1' : '0';
    }
    return invertido;
}

// Función para codificar un bloque según las reglas especificadas
string codificarBloque(const string &bloque) {
    string resultado = bloque;

    for (char &bit : resultado) {
        bit = (bit == '0') ? '1' : '0';
    }

    return resultado;
}

// Función para aplicar las reglas de codificación a un bloque dado el bloque anterior
string aplicarReglas(const string &bloque, const string &bloqueAnterior) {
    string resultado = bloque;

    int cantidad0s = 0;
    int cantidad1s = 0;

    for (char bit : bloqueAnterior) {
        if (bit == '0') {
            cantidad0s++;
        } else {
            cantidad1s++;
        }
    }

    if (cantidad0s == cantidad1s) {
        // Igual cantidad de 1s y 0s: invertir cada bit del grupo
        for (char &bit : resultado) {
            bit = (bit == '0') ? '1' : '0';
        }
    } else if (cantidad0s > cantidad1s) {
        // Mayor cantidad de 0s: invertir cada 2 bits
        for (int i = 1; i < resultado.length(); i += 2) {
            resultado[i] = (resultado[i] == '0') ? '1' : '0';
        }
    } else {
        // Mayor cantidad de 1s: invertir cada 3 bits
        for (int i = 2; i < resultado.length(); i += 3) {
            resultado[i] = (resultado[i] == '0') ? '1' : '0';
        }
    }

    return resultado;
}

// Función para decodificar un bloque según las reglas especificadas
string decodificar(const string &binario, int n) {
    string binario_decodificado;

    binario_decodificado += invertir(binario.substr(0, n));

    string bloqueAnterior;

    for (int i = n; i < binario.length(); i += n) {
        string bloque = binario.substr(i, n);

        int cantidad_unos = 0;
        int cantidad_ceros = 0;

        for (char bit : bloqueAnterior) {
            if (bit == '0') {
                cantidad_ceros++;
            } else {
                cantidad_unos++;
            }
        }

        switch (cantidad_unos - cantidad_ceros) {
        case 0:
            binario_decodificado += invertir(bloque);
            break;
        case 1:
            binario_decodificado += bloque;
            break;
        case 2:
            binario_decodificado += invertir(bloque.substr(0, n / 2));
            binario_decodificado += invertir(bloque.substr(n / 2, n / 2));
            break;
        default:
            throw invalid_argument("Cantidad de 1s y 0s no válida");
        }

        bloqueAnterior = bloque;
    }

    return binario_decodificado;
}

int main() {
    cout << "CODIFICADOR Y DECODIFICADOR:\n";
    cout << "Seleccione una opcion:\n";
    cout << "1. Codificar con metodo_01\n";
    cout << "2. Codificar con metodo_02\n";
    cout << "3. Decodificar\n";
    cout << "4. Salir\n";
    cout << "Elija una opcion (1, 2, 3 o 4): ";

    int opcion;
    cin >> opcion;

    if (opcion == 1 || opcion == 2) {
        int n;
        cout << "Ingrese la semilla n (numero entero): ";
        cin >> n;

        if (opcion == 1 || opcion == 2) {
            string rutaArchivo;
            cout << "Ingrese la ruta del archivo: ";
            cin >> rutaArchivo;

            try {
                if (opcion == 1) {
                    codificarArchivo(rutaArchivo, n, opcion);
                } else if (opcion == 2) {
                    codificarArchivo(rutaArchivo, n, opcion);
                }
            } catch (const exception &e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
    } else if (opcion == 3) {
        int n;
        cout << "Ingrese la semilla n (numero entero): ";
        cin >> n;

        string rutaArchivo;
        cout << "Ingrese la ruta del archivo a decodificar: ";
        cin >> rutaArchivo;

        try {
            decodificarArchivo(rutaArchivo, n, opcion);
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else if (opcion == 4) {
        cout << "Saliendo del programa..." << endl;
    } else {
        cerr << "Opción no válida. Seleccione 1, 2, 3 o 4." << endl;
    }

    return 0;
}
