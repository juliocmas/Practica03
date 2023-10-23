#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

// Función para codificar una cadena en binario
string codificarCadena(const string &cadena) {
    string binario;
    for (char c : cadena) {
        int valor = static_cast<int>(c);
        binario += bitset<8>(valor).to_string();
    }
    return binario;
}

// Función para decodificar una cadena desde binario
string decodificarCadena(const string &binario) {
    string cadena;
    for (size_t i = 0; i < binario.length(); i += 8) {
        string bloque = binario.substr(i, 8);
        int valor = bitset<8>(bloque).to_ulong();
        cadena += static_cast<char>(valor);
    }
    return cadena;
}

// Función para consultar el saldo y descontar una cantidad específica
int consultarSaldoYDescontar(string &saldoBinario, int cantidad) {
    int saldo = bitset<32>(saldoBinario).to_ulong();
    saldo -= cantidad;
    saldoBinario = bitset<32>(saldo).to_string();
    return saldo;
}

int main() {
    // Nombre del archivo donde se almacenarán los datos de los usuarios
    string archivo = "C:\\Users\\SONY\\Documents\\UDEA 2023-2\\Software C\\practica 3 problemas\\sudo.txt";

    int opcion;
    cout << "Seleccione una opción:\n";
    cout << "1. Registrar usuario\n";
    cout << "2. Consultar saldo\n"; // El descuento se realiza pero no se anuncia
    cout << "3. Retirar Dinero\n"; // Nueva opción para retirar dinero
    cout << "Elija una opción (1, 2 o 3): ";
    cin >> opcion;

    if (opcion == 1) {
        string cedula, clave, saldo;
        cout << "Ingrese el número de identificación (Cédula): ";
        cin >> cedula;

        cout << "Ingrese la clave (4 dígitos): ";
        cin >> clave;

        // Verificar que la clave tenga exactamente 4 dígitos
        if (clave.length() != 4) {
            cerr << "La clave debe tener exactamente 4 dígitos." << endl;
            return 1;
        }

        cout << "Ingrese el saldo en COP: ";
        cin >> saldo;

        // Codificar la cédula, clave y saldo en binario
        string cedulaCodificada = codificarCadena(cedula);
        string claveCodificada = codificarCadena(clave);
        string saldoCodificado = bitset<32>(stoi(saldo)).to_string();

        // Guardar los datos del usuario codificados en el archivo
        ofstream archivoSalida(archivo, ios::app);
        if (archivoSalida.is_open()) {
            archivoSalida << cedulaCodificada << claveCodificada << saldoCodificado << endl;
            archivoSalida.close();
            cout << "Usuario registrado correctamente." << endl;
        } else {
            cerr << "Error al abrir el archivo." << endl;
        }
    } else if (opcion == 2) {
        ifstream archivoEntrada(archivo, ios::in);
        if (!archivoEntrada.is_open()) {
            cerr << "Error al abrir el archivo." << endl;
            return 1;
        }

        string cedula, clave, saldoBinario;
        cout << "Ingrese el número de identificación (Cédula): ";
        cin >> cedula;
        cout << "Ingrese la clave (4 dígitos): ";
        cin >> clave;

        // Codificar la cédula y la clave para buscar en el archivo
        string cedulaCodificada = codificarCadena(cedula);
        string claveCodificada = codificarCadena(clave);
        string usuario;

        // Buscar al usuario en el archivo
        while (getline(archivoEntrada, usuario)) {
            if (usuario.find(cedulaCodificada) != string::npos && usuario.find(claveCodificada) != string::npos) {
                // Encontró al usuario
                saldoBinario = usuario.substr(cedulaCodificada.length() + claveCodificada.length());
                int saldo = consultarSaldoYDescontar(saldoBinario, 1000);
                cout << "Saldo actual: $" << saldo << " COP" << endl;

                // Actualizar el archivo con el nuevo saldo
                archivoEntrada.seekg(0, ios::beg);
                ofstream archivoTemp("temp.txt", ios::out);

                if (archivoTemp.is_open()) {
                    string linea;
                    while (getline(archivoEntrada, linea)) {
                        if (linea.find(cedulaCodificada) != string::npos && linea.find(claveCodificada) != string::npos) {
                            archivoTemp << cedulaCodificada << claveCodificada << saldoBinario << endl;
                        } else {
                            archivoTemp << linea << endl;
                        }
                    }

                    archivoTemp.close();
                    archivoEntrada.close();

                    remove(archivo.c_str());
                    rename("temp.txt", archivo.c_str());
                } else {
                    cerr << "Error al abrir el archivo temporal." << endl;
                }

                break;
            }
        }

        if (saldoBinario.empty()) {
            cerr << "Usuario no encontrado o clave incorrecta." << endl;
        }
    } else if (opcion == 3) {
        ifstream archivoEntrada(archivo, ios::in);
        if (!archivoEntrada.is_open()) {
            cerr << "Error al abrir el archivo." << endl;
            return 1;
        }

        string cedula, clave, saldoBinario;
        cout << "Ingrese el número de identificación (Cédula): ";
        cin >> cedula;
        cout << "Ingrese la clave (4 dígitos): ";
        cin >> clave;

        // Codificar la cédula y la clave para buscar en el archivo
        string cedulaCodificada = codificarCadena(cedula);
        string claveCodificada = codificarCadena(clave);
        string usuario;

        // Buscar al usuario en el archivo
        while (getline(archivoEntrada, usuario)) {
            if (usuario.find(cedulaCodificada) != string::npos && usuario.find(claveCodificada) != string::npos) {
                // Encontró al usuario
                saldoBinario = usuario.substr(cedulaCodificada.length() + claveCodificada.length());
                int saldo = consultarSaldoYDescontar(saldoBinario, 1000);
                cout << "Su saldo actual es: $" << saldo << " COP" << endl;

                int cantidadRetiro;
                cout << "Ingrese la cantidad que desea retirar en COP: ";
                cin >> cantidadRetiro;

                if (cantidadRetiro <= saldo) {
                    saldo = consultarSaldoYDescontar(saldoBinario, cantidadRetiro);
                    cout << "Retiro exitoso. Su nuevo saldo es: $" << saldo << " COP" << endl;

                    // Actualizar el archivo con el nuevo saldo
                    archivoEntrada.seekg(0, ios::beg);
                    ofstream archivoTemp("temp.txt", ios::out);

                    if (archivoTemp.is_open()) {
                        string linea;
                        while (getline(archivoEntrada, linea)) {
                            if (linea.find(cedulaCodificada) != string::npos && linea.find(claveCodificada) != string::npos) {
                                archivoTemp << cedulaCodificada << claveCodificada << saldoBinario << endl;
                            } else {
                                archivoTemp << linea << endl;
                            }
                        }

                        archivoTemp.close();
                        archivoEntrada.close();

                        remove(archivo.c_str());
                        rename("temp.txt", archivo.c_str());
                    } else {
                        cerr << "Error al abrir el archivo temporal." << endl;
                    }
                } else {
                    cout << "Fondos insuficientes para el retiro." << endl;
                }

                break;
            }
        }

        if (saldoBinario.empty()) {
            cerr << "Usuario no encontrado o clave incorrecta." << endl;
        }
    } else {
        cout << "Opción no válida. Seleccione 1, 2 o 3." << endl;
    }

    return 0;
}
