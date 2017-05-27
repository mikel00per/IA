#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <vector>
using namespace std;


class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      pasos = 0;
      orientacion = 0;
      num_objetos_mochila = 0;
      fil = 99;
      col = 99;

      // Control mochila
      num_objetos_mochila = 0;
      tengo_Llave = false;
      tengo_Bikini = false;
      tengo_Hueso = false;
      tengo_Zapatillas = false;
      
      // Orientación
      ubicado = false;
      orientacion = 0;
      fil = 99;
      col = 99;
      real_fil = -1;
      real_col = -1;

      for (int i = 0; i < 200; i++){
        for(int j = 0; j < 200; j++){
          aux_matrix[i][j].veces = 0;
          aux_matrix[i][j].tipo_terreno='?';
          aux_matrix[i][j].tipo_superficie='?';
        }
      }
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}
    Action think(Sensores sensores);
    int interact(Action accion, int valor);

    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    void actualizarInformacion(Sensores sensores);
    void guardarQueso(std::vector< unsigned char> vector, char tipo);
    void buscarDelante(Sensores sensores);
    void buscarOjetos(Sensores sensores);
    void buscarGPS(Sensores sensores);
    void buscarPersonajes(Sensores sensores);
    void comprobarJustoDelante(Sensores sensores);
    void mirarTerreno(Sensores sensores);
    void mirarSupeficie(Sensores sensores);
    void recogerPosibleObjeto(Sensores sensores);
    void darPosibleObjeto(Sensores sensores, char entidad);
    void comprobarJustoDebajo(Sensores sensores);
    void calculoTrasUbicarme();
    void comprobarMovimiento(Sensores sensores);
    int	 hayObstaculo(Sensores sensores, int x, int y);
    void controlDeMochila(Sensores sensores);
    void mostrarDecision();


    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////


    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:

    const static int TAM = 200;

    // Control de acciones
    Action ultima_accion;
    int decision, destino, ultima_decision;

    // Control mochila
    int num_objetos_mochila;
    bool tengo_Llave, tengo_Bikini, tengo_Hueso, tengo_Zapatillas;
    bool personaje_delante;

    // Orientación
    bool ubicado;
    int orientacion, fil, col;
    int real_fil, real_col;

    // "Memoria" del agente:
    struct Casilla{
      int veces;
      unsigned char tipo_superficie;
      unsigned char tipo_terreno;
    };

    int pasos;
    Casilla aux_matrix[TAM][TAM];

};


#endif
