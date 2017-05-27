#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <set>
#include <list>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:

    struct estado{
      int cols;
      int fils;
      int orientacion;
      estado() : cols(0), fils(0), orientacion(0) {};
      estado(int y, int x, int NSEO) : cols(x), fils(y), orientacion(NSEO) {}

      bool operator != (const estado e2) const {
        return !((cols == e2.cols) && (fils == e2.fils)); }

      bool operator == (const estado e2) const {
        return (cols == e2.cols) && (fils == e2.fils); }

      bool operator<(const estado e1) const {
        if (fils < e1.fils && cols < e1.cols && orientacion < e1.orientacion) {
          return true;
        }else
          return false;
      }

      int distanciaManhattanEstados(const estado &eFin)const { return std::abs(cols-eFin.cols) + std::abs(fils-eFin.fils); }

    };

    struct Nodo{
      estado st;
      std::list<Action> lista_acciones;
      int G, H, F;

      Nodo() : st(), G(0) , H(0), F(0) {}

      Nodo(estado e) : st(e.fils,e.cols,e.orientacion), G(0), H(0), F(0) {}

      Nodo(estado e, list<Action> acciones) :
          st(e.fils, e.cols, e.orientacion), lista_acciones(acciones), G(0), H(0), F(0) { }


      void setF() {F = G+H;}
      int  getF() {return F;}
      int  distanciaManhattan( Nodo &nodoFin){ return std::abs(this->st.cols-nodoFin.st.cols) + std::abs(this->st.fils-nodoFin.st.fils); }

    };

    struct CompararQueue{
      bool operator()(const Nodo &a, const Nodo &b){
        return a.F > b.F;
      }
    };

    struct CompararSet{
      bool operator()(const estado &a, const estado &b){
        if (a.fils < b.fils) {
          return true;
        }else if (a.fils == b.fils && a.cols < b.cols) {
          return true;
        }else if (a.fils == b.fils && a.cols == b.cols && a.orientacion < b.orientacion){
          return true;
        }else
          return false;
      }
    };

    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      pasos = 0;
      orientacion = 0;
      num_objetos_mochila = 0;
      fil = 99;
      col = 99;
      copiadoAnterior = false;
      darRey = darPerro = false;
      num_regalos_ahora = num_regalos_antes = 0;
      num_Rey = 0;
      ejecutando_plan = false;
      error_plan = false;

      // Control mochila
      num_objetos_mochila = 0;
      tengo_Llave = false;
      tengo_Bikini = false;
      tengo_Hueso = false;
      tengo_Zapatillas = false;
      tengo_Regalo = false;

      // Orientación
      ubicado = false;
      orientacion = 0;
      fil = 99;
      col = 99;

      for (int i = 0; i < 200; i++){
        for(int j = 0; j < 200; j++){
          aux_matrix[i][j].veces = 0;
          aux_matrix[i][j].tipo_terreno='?';
          aux_matrix[i][j].tipo_superficie='?';
        }
      }

      rey_visto = false;

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
    void comprobarCasillas(Sensores sensores);
    int	 hayObstaculo(Sensores sensores, int x, int y, char down);
    void controlDeMochila(Sensores sensores);
    void mostrarDecision();
    estado getInicio();
    estado getBestObjetivo(Sensores sensores, char t);

    void buscarPlanificarEjecutar(Sensores sensores);

    int porcentageMapaSolucion();
    bool tengoObjetosParaTravesia();


    char getSuperficie(int y, int x){return aux_matrix[y][x].tipo_superficie;}
    char getTerreno(int y, int x){return aux_matrix[y][x].tipo_terreno;}
    bool getHueso(){ return tengo_Hueso ;}
    bool getBikini(){ return tengo_Bikini ;}
    bool getZapatillas(){ return tengo_Zapatillas ;}
    bool getLlave(){return tengo_Llave;}

    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////


    // IDA ESTRELLA
    // Camino manjatan

    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    void Reiniciado();
    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    void insertarAbiertos(int y, int x, int nseo, int nuevoCoste, list<Action>acciones, list<Action> accionesFuturas);
    int  ValorCasilla(int y, int x);

    const static int TAM = 200;

    // Control de acciones
    Action ultima_accion;
    int decision, ultima_decision;

    // Control mochila y manos
    int num_objetos_mochila;
    bool tengo_Llave, tengo_Bikini, tengo_Hueso, tengo_Zapatillas, tengo_Regalo;
    bool darRey, darPerro;
    int num_regalos_ahora, num_regalos_antes;

    // Orientación
    bool ubicado;
    int orientacion, fil, col;
    int real_fil, real_col;

    // Recuerdos para comportamiento reactivo.
    struct Casilla{
      int veces;
      unsigned char tipo_superficie;
      unsigned char tipo_terreno;
    };

    int pasos;
    bool copiadoAnterior;
    Casilla aux_matrix[TAM][TAM];
    Casilla aux_muerto[TAM][TAM];

    // Estados para comportamiento deliverativo
    list<Action> mi_plan;
    bool ejecutando_plan, error_plan, rey_visto;
    int num_Rey;
    std::vector<estado> reyes;

    Nodo goal;
    Nodo start;
    std::set<estado, CompararSet> CERRADOS;
    std::priority_queue<Nodo, std::vector<Nodo>, CompararQueue> ABIERTOS;
    list <Action> plan;
    Action decision_futura;

};


#endif
