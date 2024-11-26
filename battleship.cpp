#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Clase Nave
class Nave {
public:
    string tipo;
    int longitud;
    vector<pair<int, int>> coordenadas;
    vector<pair<int, int>> impactos;

    Nave(string tipo, int longitud) : tipo(tipo), longitud(longitud) {}

    void establecerCoordenadas(int x, int y, char orientacion) {
        coordenadas.clear();
        for (int i = 0; i < longitud; i++) {
            if (orientacion == 'H')
                coordenadas.emplace_back(x, y + i);
            else
                coordenadas.emplace_back(x + i, y);
        }
    }

    bool estaHundido() const {
        return coordenadas.size() == impactos.size();
    }

    void registrarImpacto(int x, int y) {
        for (auto& coord : coordenadas) {
            if (coord.first == x && coord.second == y) {
                impactos.emplace_back(x, y);
                return;
            }
        }
    }
};

// Clase Tablero
class Tablero {
public:
    int size;
    vector<vector<char>> mapa;
    vector<Nave> barcos;

    Tablero(int size = 5) : size(size), mapa(size, vector<char>(size, '~')) {}

    bool colocarBarco(Nave barco, int x, int y, char orientacion) {
        barco.establecerCoordenadas(x, y, orientacion);
        if (!validarPosicion(barco)) return false;

        barcos.push_back(barco);
        for (auto& coord : barco.coordenadas)
            mapa[coord.first][coord.second] = 'B';
        return true;
    }

    bool validarPosicion(const Nave& barco) const {
        for (auto& coord : barco.coordenadas) {
            if (coord.first < 0 || coord.first >= size || coord.second < 0 || coord.second >= size)
                return false;
            if (mapa[coord.first][coord.second] == 'B')
                return false;
        }
        return true;
    }

    string recibirAtaque(int x, int y) {
        if (mapa[x][y] == 'B') {
            mapa[x][y] = 'X';
            for (auto& barco : barcos) {
                barco.registrarImpacto(x, y);
                if (barco.estaHundido())
                    return "Barco hundido";
            }
            return "Impacto";
        } else if (mapa[x][y] == '~') {
            mapa[x][y] = 'O';
            return "Agua";
        }
        return "Posición ya atacada";
    }

    void mostrarTablero(bool ocultarBarcos = true) const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (ocultarBarcos && mapa[i][j] == 'B')
                    cout << "~ ";
                else
                    cout << mapa[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool todosBarcosHundidos() const {
        for (const auto& barco : barcos) {
            if (!barco.estaHundido()) return false;
        }
        return true;
    }
};

// Clase Jugador
class Jugador {
public:
    string nombre;
    Tablero tablero;

    Jugador(string nombre, int size) : nombre(nombre), tablero(size) {}

    string atacar(Jugador& enemigo, int x, int y) {
        return enemigo.tablero.recibirAtaque(x, y);
    }
};

// Clase Referi
class Referi {
public:
    static bool verificarGanador(const Jugador& jugador) {
        return jugador.tablero.todosBarcosHundidos();
    }
};

// Clase Juego
class Juego {
private:
    Jugador jugador1;
    Jugador jugador2;
    Jugador* turnoActual;

public:
    Juego(string nombre1, string nombre2, int size = 5)
        : jugador1(nombre1, size), jugador2(nombre2, size), turnoActual(&jugador1) {}

    void colocarBarcos(Jugador& jugador) {
        cout << "Colocación de barcos para " << jugador.nombre << endl;
        for (int i = 0; i < 3; i++) {
            string tipo = (i == 0) ? "Acorazado" : (i == 1) ? "Submarino" : "Lancha";
            int longitud = (i == 0) ? 3 : (i == 1) ? 2 : 1;
            Nave barco(tipo, longitud);
            int x, y;
            char orientacion;

            do {
                cout << "Coloca tu " << tipo << " (longitud: " << longitud << ") [x y orientacion(H/V)]: ";
                cin >> x >> y >> orientacion;
            } while (!jugador.tablero.colocarBarco(barco, x, y, orientacion));
        }
    }

    void jugar() {
        colocarBarcos(jugador1);
        colocarBarcos(jugador2);

        while (true) {
            cout << "\nTurno de " << turnoActual->nombre << endl;
            Jugador& enemigo = (turnoActual == &jugador1) ? jugador2 : jugador1;

            enemigo.tablero.mostrarTablero(true);
            int x, y;
            cout << "Introduce coordenadas para atacar (x y): ";
            cin >> x >> y;

            string resultado = turnoActual->atacar(enemigo, x, y);
            cout << resultado << endl;

            if (Referi::verificarGanador(enemigo)) {
                cout << "\n¡" << turnoActual->nombre << " ha ganado el juego!" << endl;
                break;
            }
            turnoActual = (turnoActual == &jugador1) ? &jugador2 : &jugador1;
        }
    }
};

int main() {
    Juego juego("Jugador 1", "Jugador 2", 5);
    juego.jugar();
    return 0;
}
