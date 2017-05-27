#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void ComportamientoJugador::actualizarInformacion(Sensores sensores) {
	cout << "/////// Actualizo info ///////" << endl;
	if (sensores.reset) {

		pasos = 0;
		orientacion = 0;
		num_objetos_mochila = 0;
		ubicado = false;
		orientacion = 0;
		real_fil = -1;
		real_col = -1;

		personaje_delante = false;

		// Como pierdo los ojetros
		tengo_Llave = false;
		tengo_Bikini = false;
		tengo_Hueso = false;
		tengo_Zapatillas = false;
		num_objetos_mochila = 0;

		// Restauro filas
		fil = col = 99;
		// Restauro Matrix

		for (int i = 0; i < TAM; i++) {
			for (int j = 0; j < TAM; j++) {
				aux_matrix[i][j].veces = 0;
				aux_matrix[i][j].tipo_superficie='?';
	      aux_matrix[i][j].tipo_terreno='?';
			}
		}
	}else{
		cout << "Sigo Vivo. " << endl;
		switch (ultima_accion) {
			case actFORWARD:
			if (!sensores.colision) {
				switch (orientacion) {
					case 0:
						fil--;
						if (ubicado) real_fil--;
						break;
					case 1:
						col++;
						if (ubicado) real_col++;
						break;
					case 2:
						fil++;
						if (ubicado) real_fil++;
						break;
					case 3:
						col--;
						if (ubicado) real_col--;
						break;
				}
			}
			break;

			case actTURN_L:
				orientacion = (orientacion+3)%4;
				break;

			case actTURN_R:
				orientacion = (orientacion+1)%4;
				break;
		}
		cout << "Fila: " << fil << endl << "Cols: " << col << endl << "Paso: " << pasos << endl;
		guardarQueso(sensores.terreno, 't');
		guardarQueso(sensores.superficie, 's');
		if (ubicado)
			guardarQueso(sensores.terreno, 'r');
	}

	cout << "/////// Fin actualizacion ///////" << endl;
}

//tipo

void ComportamientoJugador::guardarQueso(std::vector< unsigned char> vector, char tipo){
	if (tipo =='t') {
		aux_matrix[fil][col].tipo_terreno = vector[0];
		switch (orientacion) {
			case 0:
				for (int i=0; i<3; i++) aux_matrix[fil-1][col-1+i].tipo_terreno = vector[i+1];
				for (int i=0; i<5; i++) aux_matrix[fil-2][col-2+i].tipo_terreno = vector[i+4];
				for (int i=0; i<7; i++) aux_matrix[fil-3][col-3+i].tipo_terreno = vector[i+9];
			break;

			case 1:
				for (int i=0; i<3; i++) aux_matrix[fil-1+i][col+1].tipo_terreno = vector[i+1];
				for (int i=0; i<5; i++) aux_matrix[fil-2+i][col+2].tipo_terreno = vector[i+4];
				for (int i=0; i<7; i++) aux_matrix[fil-3+i][col+3].tipo_terreno = vector[i+9];
			break;

			case 2:
				for (int i=0; i<3; i++) aux_matrix[fil+1][col+1-i].tipo_terreno = vector[i+1];
				for (int i=0; i<5; i++) aux_matrix[fil+2][col+2-i].tipo_terreno = vector[i+4];
				for (int i=0; i<7; i++) aux_matrix[fil+3][col+3-i].tipo_terreno = vector[i+9];
			break;

			case 3:
				for (int i=0; i<3; i++)aux_matrix[fil-i+1][col-1].tipo_terreno = vector[i+1];
				for (int i=0; i<5; i++)aux_matrix[fil-i+2][col-2].tipo_terreno = vector[i+4];
				for (int i=0; i<7; i++)aux_matrix[fil-i+3][col-3].tipo_terreno = vector[i+9];
			break;
		}
	}else if (tipo == 's') {
		switch (orientacion) {
			case 0:
				aux_matrix[fil-1][col].tipo_superficie = vector[2];
				break;
			case 1:
				aux_matrix[fil][col+1].tipo_superficie = vector[2];
				break;
			case 2:
				aux_matrix[fil+1][col].tipo_superficie = vector[2];
				break;
			case 3:
				aux_matrix[fil][col-1].tipo_superficie = vector[2];
				break;
		}
	}else{
		mapaResultado[real_fil][real_col] = vector[0];
		switch (orientacion) {
			case 0:
				for (int i=0; i<3; i++) mapaResultado[real_fil-1][real_col-1+i] = vector[i+1];
				for (int i=0; i<5; i++) mapaResultado[real_fil-2][real_col-2+i] = vector[i+4];
				for (int i=0; i<7; i++) mapaResultado[real_fil-3][real_col-3+i] = vector[i+9];
			break;

			case 1:
				for (int i=0; i<3; i++) mapaResultado[real_fil-1+i][real_col+1] = vector[i+1];
				for (int i=0; i<5; i++) mapaResultado[real_fil-2+i][real_col+2] = vector[i+4];
				for (int i=0; i<7; i++) mapaResultado[real_fil-3+i][real_col+3] = vector[i+9];
			break;

			case 2:
				for (int i=0; i<3; i++) mapaResultado[real_fil+1][real_col+1-i] = vector[i+1];
				for (int i=0; i<5; i++) mapaResultado[real_fil+2][real_col+2-i] = vector[i+4];
				for (int i=0; i<7; i++) mapaResultado[real_fil+3][real_col+3-i] = vector[i+9];
			break;

			case 3:
				for (int i=0; i<3; i++) mapaResultado[real_fil-i+1][real_col-1] = vector[i+1];
				for (int i=0; i<5; i++) mapaResultado[real_fil-i+2][real_col-2] = vector[i+4];
				for (int i=0; i<7; i++) mapaResultado[real_fil-i+3][real_col-3] = vector[i+9];
			break;
		}
	}
}

void ComportamientoJugador::buscarDelante(Sensores sensores) {
	buscarPersonajes(sensores);
	buscarOjetos(sensores);
	buscarGPS(sensores);
}

void ComportamientoJugador::buscarOjetos(Sensores sensores) {
	if (num_objetos_mochila < 4) { // Preguntar como saber si tengo la mochila llena
		for (int i = 1; i < sensores.superficie.size(); i++) {
			if (sensores.superficie[i] >= '0' && sensores.superficie[i] <= '4') {
				decision = 1;
				destino = i;
			}
		}
	}
}

void ComportamientoJugador::buscarGPS(Sensores sensores) {
	if (!ubicado){
		for (int i = 1; i < sensores.terreno.size(); i++) {
			if (sensores.terreno[i] == 'K') {
				decision = 1;
				destino = i;
			}
		}
	}
}

void ComportamientoJugador::buscarPersonajes(Sensores sensores) {
	for (int i = 1; i < sensores.superficie.size(); i++) {
		if (sensores.superficie[i] == 'l' && tengo_Hueso) {
			decision = 1;
			destino = i;
		}
	}
}

// Solo sucedera uno de ellos, o hay un objto, personaje o puerta
void ComportamientoJugador::comprobarJustoDelante(Sensores sensores) {
	mirarSupeficie(sensores);
	if (decision == 1)
		mirarTerreno(sensores);
}

void ComportamientoJugador::mirarTerreno(Sensores sensores) {
	char terreno, objeto_necesario;
	bool es_puerta = false;
	bool tengo;

	switch (sensores.terreno[2]) {
		case 'D':
			es_puerta = true;
			break;
		case 'A':
			terreno = 'A';
			objeto_necesario = '1';
			tengo = tengo_Bikini;
			break;
		case 'B':
			terreno = 'B';
			objeto_necesario = '2';
			tengo = tengo_Zapatillas;
			break;
		case 'T':
			decision = 1;
			break;
		case 'S':
			decision = 1;
			break;
	}

	if (!es_puerta) {
		if (sensores.terreno[2] == terreno && sensores.objetoActivo == objeto_necesario) {
			decision = 1;
		}else if (sensores.terreno[2] == terreno && sensores.objetoActivo == '_' && tengo) {
			decision = 8; // Pop
		}else if (sensores.terreno[2] == terreno && sensores.objetoActivo != objeto_necesario && tengo) {
			decision = 7; // Push
		}else{
			decision = 1;
		}
	}else{
		darPosibleObjeto(sensores, 'd');
	}
	cout << "Decision tras mirarTerreno: " << decision << endl;
}

// En la superficie hay objetos o personajes
void ComportamientoJugador::mirarSupeficie(Sensores sensores) {
	bool es_objeto = false, es_personaje = false, necesito = false, lobo = false;
	bool tengo;
	bool vacia;

	switch (sensores.superficie[2]) {
		case 'a':
			es_personaje = true;
			break;
		case 'l':
			es_personaje = true;
			lobo = true;
			break;
		case '0':
			es_objeto = true;
			necesito = !tengo_Hueso;
			break;
		case '1':
			es_objeto = true;
			necesito = !tengo_Bikini;
			break;
		case '2':
			es_objeto = true;
			necesito = !tengo_Zapatillas;
			break;
		case '3':
			es_objeto = true;
			necesito = !tengo_Llave ;
			break;
		case '_':
			decision = 1;
			break;
	}

	if (es_objeto && necesito && num_objetos_mochila < 4)
		recogerPosibleObjeto(sensores);
	else if (!es_objeto && es_personaje && lobo)
		darPosibleObjeto(sensores,'l');


	cout << "Decision tras mirarSupeficie: " << decision << endl;
}

// Pre: Sé que lo necesito antes de llamar a la función por eso Solo
// mete y saca de la mochila para encontrarlo.
void ComportamientoJugador::recogerPosibleObjeto(Sensores sensores) {
	cout << "/////// RECOGER OBJETO /////////////// " << endl;
	cout << "hueso: " << tengo_Hueso << endl << "bikini: " << tengo_Bikini << endl
			 << "zapas: " << tengo_Zapatillas << endl << "llaves: " << tengo_Llave << endl;
	if (sensores.objetoActivo != '_' && num_objetos_mochila < 4) {
		decision = 7;
	}else if (sensores.objetoActivo == '_' ) {
		decision = 5;
	}else
		decision = 1;

	cout << "N. objetos mochila " << num_objetos_mochila << endl;
	std::cout << "recogerPosibleObjeto, decision: " << decision << '\n';
}

void ComportamientoJugador::darPosibleObjeto(Sensores sensores, char entidad) {

	char tipo; // 0 es terreno, 1 es superficie
	char objeto_necesario;
	bool tengo;

	if (entidad == 'l')	{ objeto_necesario = '0'; tengo = tengo_Hueso; }
	else								{ objeto_necesario = '3'; tengo = tengo_Llave; }


	if (sensores.superficie[2] == entidad && sensores.objetoActivo == objeto_necesario) {
		decision = 9; // Give
	}else if (sensores.superficie[2] == entidad && sensores.objetoActivo == '_' && tengo) {
		decision = 8; // Pop
	}else if (sensores.superficie[2] == entidad && sensores.objetoActivo != objeto_necesario && tengo) {
		decision = 7; // Push
	}else{
		decision = 1;
	}

  cout << "Entro en darPosibleObjeto, decision: " << decision << endl;
}

void ComportamientoJugador::comprobarJustoDebajo(Sensores sensores) {
	if (sensores.terreno[0] == 'K' && !ubicado) {
		real_fil = sensores.mensajeF;
		real_col = sensores.mensajeC;
		ubicado = true;
		calculoTrasUbicarme();
	}
}

void ComportamientoJugador::calculoTrasUbicarme() {
	int diferencia_f = real_fil -  fil ;
	int diferencia_c = real_col - col;

	for (int i = 0; i < TAM; i++) {
		for (int j = 0; j < TAM; j++) {
			if (aux_matrix[i][j].tipo_terreno != '?') {
				mapaResultado[i+diferencia_f][j+diferencia_c] = aux_matrix[i][j].tipo_terreno;
			}
		}
	}

}

void ComportamientoJugador::comprobarMovimiento(Sensores sensores) {
	int valor_arriba, valor_derecha, valor_abajo, valor_izquierda;

	switch (orientacion) {
		case 0:
			valor_arriba			= hayObstaculo(sensores, fil-1, col);
			valor_derecha			= hayObstaculo(sensores, fil, col+1);
			valor_izquierda		= hayObstaculo(sensores, fil, col-1);
			break;
		case 1:
			valor_arriba			= hayObstaculo(sensores, fil, col+1);
			valor_derecha			= hayObstaculo(sensores, fil+1, col);
			valor_izquierda		= hayObstaculo(sensores, fil-1, col);
			break;
		case 2:
			valor_arriba			= hayObstaculo(sensores, fil+1, col);
			valor_derecha			= hayObstaculo(sensores, fil, col-1);
			valor_izquierda		= hayObstaculo(sensores, fil, col+1);
			break;
		case 3:
			valor_arriba			= hayObstaculo(sensores, fil, col-1);
			valor_derecha			= hayObstaculo(sensores, fil-1, col);
			valor_izquierda		= hayObstaculo(sensores, fil+1, col);
			break;
	}

	std::cout << "IZQ: " << valor_izquierda << "\nDER: " << valor_derecha << "\nTOP: " << valor_arriba <<'\n';

	int minimo = valor_arriba;

	if (valor_derecha < minimo)
		minimo = valor_derecha;

	if (valor_izquierda < minimo)
		minimo = valor_izquierda;


	if (minimo == valor_arriba && minimo < 99999) {
		decision = 1;
	}else if (minimo == valor_izquierda && minimo < 99999) {
		decision = 2;
	}else if (minimo == valor_derecha && minimo < 99999){
		decision = 3;
	}else{
		decision = 3;
	}

/*
	if (valor_arriba < valor_derecha && valor_arriba < valor_izquierda && valor_arriba < 9999) {
		decision = 1; // ARRIBA MEJOR
	}else if (valor_derecha < valor_izquierda && valor_derecha < valor_arriba && valor_derecha < 9999) {
		decision = 3; // DER MEJOR
	}else if (valor_izquierda < valor_arriba && valor_izquierda < valor_derecha && valor_derecha < 9999) {
		decision = 2; // IZQ MEJOR
	}else if(valor_izquierda == valor_arriba && valor_izquierda == valor_derecha && valor_arriba < 9999){
		decision = (rand())%2+2;
	}else{
		std::cout << "ni puta idea de porque" << '\n';
	}*/

	std::cout << "Entro en comprobarMovimiento, decision: " << decision << '\n';
}

int	ComportamientoJugador::hayObstaculo(Sensores sensores, int x, int y){

	int valor_casilla = 0;
	bool valoro = false;

	// Si hay un objeto en la casilla puede convertirse en un obstaculo o en algo
	// beneficioso

	cout << "Valor matrix T: " << aux_matrix[x][y].tipo_terreno << endl;


	if (aux_matrix[x][y].tipo_terreno == 'P' || aux_matrix[x][y].tipo_terreno == 'M') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_terreno == 'M' && aux_matrix[x-1][y+1].tipo_terreno == 'D') {
		cout << "guay" << endl;
		valor_casilla = valor_casilla - 100000;
	}else if (aux_matrix[x][y].tipo_terreno == 'A' && !tengo_Bikini) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_terreno == 'B' && !tengo_Zapatillas) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_terreno == 'D' && !tengo_Llave) {
		valor_casilla = valor_casilla + 100000;
	}else{
		valor_casilla = valor_casilla + aux_matrix[x][y].veces;
	}

	if (aux_matrix[x][y].tipo_superficie == '0' && tengo_Hueso) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_superficie == '1' && tengo_Bikini) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_superficie == '2' && tengo_Zapatillas) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_superficie == '3' && tengo_Llave) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_superficie == 'l' && !tengo_Hueso) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_superficie == 'a') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[x][y].tipo_superficie == 'd' && aux_matrix[x][y].tipo_terreno == 'M') {
		valor_casilla = valor_casilla - 100000;
	}else{
		valor_casilla = valor_casilla + aux_matrix[x][y].veces;
	}


/*
	if (aux_matrix[x][y].tipo_terreno == '?') {
		valor_casilla = valor_casilla + aux_matrix[x][y].veces;
	}else if (aux_matrix[x][y].tipo_terreno == 'S' || aux_matrix[x][y].tipo_terreno == 'T') {
		valor_casilla = valor_casilla + aux_matrix[x][y].veces;
	}else if (aux_matrix[x][y].tipo_terreno == 'P' || aux_matrix[x][y].tipo_terreno == 'M') {
		valor_casilla = valor_casilla + 100000;
	}else if ((aux_matrix[x][y].tipo_terreno == 'K' || aux_matrix[x][y].tipo_terreno == 'D') && aux_matrix[x][y].veces == 0) {
		valor_casilla = valor_casilla - 10000;
	}else if ((aux_matrix[x][y].tipo_terreno == 'K' || aux_matrix[x][y].tipo_terreno == 'D') && aux_matrix[x][y].veces != 0) {
		valor_casilla = valor_casilla + aux_matrix[x][y].veces;
	}else if (aux_matrix[x][y].tipo_terreno == 'A' && !tengo_Bikini) {
		valor_casilla = valor_casilla + 10000;
	}else if (aux_matrix[x][y].tipo_terreno == 'A' && tengo_Bikini) {
		valor_casilla = valor_casilla + aux_matrix[x][y].veces;
	}else if (aux_matrix[x][y].tipo_terreno == 'B' && !tengo_Zapatillas) {
		valor_casilla = valor_casilla + 10000;
	}else if (aux_matrix[x][y].tipo_terreno == 'B' && tengo_Zapatillas) {
		valor_casilla = valor_casilla + aux_matrix[x][y].veces;
	}else if (aux_matrix[x][y].tipo_terreno == 'D' && tengo_Llave) {
		valor_casilla = valor_casilla + aux_matrix[x][y].veces;
	}else
		std::cout << "hayObstaculo: No he complido condicion del terreno " << '\n';
*/
	return valor_casilla;
}

void ComportamientoJugador::controlDeMochila(Sensores sensores) {
	switch (decision) {
		case 5:
			switch (sensores.superficie[2]) {
				case '0':
					tengo_Hueso = true;
					break;
				case '1':
					tengo_Bikini = true;
					break;
				case '2':
					tengo_Zapatillas = true;
					break;
				case '3':
					tengo_Llave = true;
					break;
			}
			break;
		case 7:
			num_objetos_mochila++;
			cout << "Control de mochila, nMoch: " << num_objetos_mochila << endl;
			break;
		case 8:
			num_objetos_mochila--;
			break;
		case 9:
			if (sensores.objetoActivo == '0') tengo_Hueso = false;
			break;
	}
}

void ComportamientoJugador::mostrarDecision() {
	switch (decision) {
		case 1:
			cout << "¡Sigo recto!" << endl;
			break;
		case 2:
			cout << "¡Giro izquierda" << endl;
			break;
		case 3:
			cout << "¡Giro derecha!" << endl;
			break;
		case 4:
			cout << "Me quedo quieto" << endl;
			break;
		case 5:
			cout << "¡Un objeto! Lo cojo" << endl;
			break;
		case 6:
			cout << "Voy a dejar lo que tengo en las manos en el suelo" << endl;
			break;
		case 7:
			cout << "Guardaré en la bolsa el objeto" << endl;
			break;
		case 8:
			cout << "Sacaré algo, creo que es necesario" << endl;
			break;
		case 9:
			cout << "Voy a darle el objeto a la entidad" << endl;
			break;
		case 10:
			cout << "Tiro lo que tengo en las manos." << endl;
			break;
	}
}

Action ComportamientoJugador::think(Sensores sensores){

	aux_matrix[fil][col].veces = pasos;

	pasos++;
	decision = 1;
	Action accion;

	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Mochila: " << sensores.mochila << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << "Fila: " << sensores.mensajeF << endl;
	cout << "Columna: " << sensores.mensajeC << endl;
	cout << "objetoActivo: " << sensores.objetoActivo << endl;
	cout << endl;


	actualizarInformacion(sensores);

	// 1) Veo lo lejano
	buscarDelante(sensores);

	// 2) Analizo lo que tengo delante
	comprobarJustoDelante(sensores);

	// 3) Analizo lo que tengo debajo
	comprobarJustoDebajo(sensores);

	// 4) Si tengo que avanzar compruebo la casilla, o si me bloqueo por:
	// no tener objeto, por ser un aldeano, por tener todos los objetos
	if (decision == 1)
		comprobarMovimiento(sensores);

	// 5) Si la acción es hacer cosas con los objetos:
	if (decision >= 5 && decision <= 9)
		controlDeMochila(sensores);

	// 6) Muestro la decisión final, asigno las acciones y guardo la última
	mostrarDecision();

	switch (decision) {
		case 1:
 			accion = actFORWARD;
			break;
		case 2:
			accion = actTURN_L;
			break;
		case 3:
			accion = actTURN_R;
			break;
		case 4:
			accion = actIDLE;
			break;
		case 5:
			accion = actPICKUP;
			break;
		case 6:
			accion = actPUTDOWN;
			break;
		case 7:
			accion = actPUSH;
			break;
		case 8:
			accion = actPOP;
			break;
		case 9:
			accion = actGIVE;
			break;
		case 10:
			accion = actTHROW;
			break;
	}

	ultima_accion = accion;
	ultima_decision = decision;

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}


/*

void ComportamientoJugador::colisionObjeto(Sensores sensores) {

	bool tengo = false;

	if (sensores.superficie[2] != '_') {
		if (sensores.superficie[2] == '0' && tengo_Bikini) {
			tengo = true;
		}else if (sensores.superficie[2] == '1' && tengo_Zapatillas) {
			tengo = true;
		}else if (sensores.superficie[2] == '2' && tengo_Llave) {
			tengo = true;
		}else if (sensores.superficie[2] == '3' && tengo_Hueso) {
			tengo = true;
		}

		if (!no_tengo) {
			if (sensores.superficie[2] != '_' && objetoActivo == '_') {
				decision = 5; // No lo tengo y manos vacías -> lo cojo
			}else if (sensores.superficie[2] != '_' &&
			sensores.objetoActivo != '_' &&
			num_objetos_mochila < 5) {
				decision = 7; // Guardo lo que tengo en las manos
			}else{
				decision = -1;	// No puedo dejar manos libres -> pasar de él
			}
		}else
			decision = -1; // Como lo tengo, paso de él
	}

}

void ComportamientoJugador::colisionPersonaje(Sensores sensores) {
	int guardar_x = fil, guardar_y = col;
	// Guardo la posicion en la que se encuentra el lobo o el
	if (sensores.superficie[2] == 'l') {
		switch (orientacion) {
			case 0:
				guardar_x-- ;
				break;
			case 1:
				guardar_y++ ;
				break;
			case 2:
				guardar_x++ ;
				break;
			case 3:
				guardar_y-- ;
				break;
		}
		recordar('l', guardar_x, guardar_y);
		darPosibleObjeto(sensores, 'l');
	}else if (sensores.superficie[2] == 'a') {
		decision = -1;
	}
}
void ComportamientoJugador::colisionPuerta(Sensores sensores) {
	int guardar_x = fil,
	guardar_y = col;

	if (sensores.terreno[2] == 'D') {
		switch (orientacion) {
		case 0:
			guardar_x-- ;
			break;
		case 1:
			guardar_y++ ;
			break;
		case 2:
			guardar_x++ ;
			break;
		case 3:
			guardar_y-- ;
			break;
		}
		recordar('D', guardar_x, guardar_y);
		darPosibleObjeto(sensores,'D');

		cout << "Decision tras colisionPuerta: " << decision << endl;
	}
}

void ComportamientoJugador::pasandoPorPuerta(Sensores sensores) {
	if (sensores.superficie[0] == 'D') {
		for (int i = 0; i < localizaciones.size(); i++) {
			if (localizaciones[i].tipo_terreno == 'D' && localizaciones[i].x == fil && localizaciones[i].y == col) {
				localizaciones[i].veces_puerta++;
				if (localizaciones[i].veces_puerta%2 == 0) {
					soltar_llave = true; // Donde se suelta la llave?
				}
			}
		}
	}
}

void ComportamientoJugador::recordar(unsigned char p, int x, int y) {
	bool existe = false;
	for (int i = 0; i < localizaciones.size() && !existe; i++) {
		if (localizaciones[i].tipo_terreno = p && localizaciones[i].x == x && localizaciones[i].y == y)
			existe = true;
	}

	if (!existe) {
		Casilla nueva_ubicacion;
		nueva_ubicacion.x = fil;
		nueva_ubicacion.y = col;
		nueva_ubicacion.tipo_terreno = p;
		nueva_ubicacion.tipo_superficie = '_';
		nueva_ubicacion.veces = 0;
		localizaciones.push_back(nueva_ubicacion);
	}
}


void ComportamientoJugador::elegirCaminoMasInteresante(Sensores sensores) {
  // Me giraré al terreno en el que pueda seguir recto y sea además
  // el menos visitado
	cout << "Entro en elegirCaminoMasInteresante: " << endl;
  switch (orientacion) {
			// Norte:
			// Casilla arriba: 			aux_matrix[fil-1][col]
			// Casilla derecha: 		aux_matrix[fil][col+1]
			// Casilla abajo: 			aux_matrix[fil+1][col]
			// Casilla izquierda: 	aux_matrix[fil][col-1]
    case 0:
			// La de ariba es la menor y puedo ir.
			if ((aux_matrix[fil-1][col].veces < aux_matrix[fil][col+1].veces && aux_matrix[fil-1][col].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil-1][col].veces < aux_matrix[fil][col-1].veces)
			&& (aux_matrix[fil-1][col].tipo_terreno != ('M' || 'P') || (aux_matrix[fil-1][col].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil-1][col].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 1;
			}
			// El de la derecha es la menor y puedo ir.
			else if ((aux_matrix[fil][col+1].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil][col+1].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil][col+1].veces < aux_matrix[fil][col-1].veces)
			&& (aux_matrix[fil][col+1].tipo_terreno != ('M' || 'P') || (aux_matrix[fil][col+1].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil][col+1].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 3;
			}
			else if ((aux_matrix[fil][col-1].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil][col-1].veces < aux_matrix[fil-1][col].veces && aux_matrix[fil][col-1].veces < aux_matrix[fil][col+1].veces)
			&& (aux_matrix[fil][col-1].tipo_terreno != ('M' || 'P') || (aux_matrix[fil][col-1].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil][col-1].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 2;
			}else{
				decision = ((rand()%2)+2);
			}
			cout << "Arriba: " << aux_matrix[fil-1][col].veces << endl;
			cout << "Derecha: " << aux_matrix[fil][col+1].veces << endl;
			cout << "Abajo: " << aux_matrix[fil+1][col].veces << endl;
			cout << "Izquierda: " << aux_matrix[fil][col-1].veces << endl;
      break;

			// Este:
			// Casilla arriba: 			aux_matrix[fil][col+1]
			// Casilla derecha: 		aux_matrix[fil+1][col]
			// Casilla abajo: 			aux_matrix[fil][col-1]
			// Casilll izquierda: 	aux_matrix[fil-1][col]

    case 1:
			if ((aux_matrix[fil][col+1].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil][col+1].veces < aux_matrix[fil][col-1].veces && aux_matrix[fil][col+1].veces < aux_matrix[fil-1][col].veces)
			&& (aux_matrix[fil][col+1].tipo_terreno != ('M' || 'P') || (aux_matrix[fil][col+1].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil][col+1].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 1;
			}
			// El de la derecha es la menor y puedo ir.
			else if ((aux_matrix[fil][col+1].veces < aux_matrix[fil-1][col].veces && aux_matrix[fil][col+1].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil][col+1].veces < aux_matrix[fil][col-1].veces)
			&& (aux_matrix[fil][col+1].tipo_terreno != ('M' || 'P') || (aux_matrix[fil][col+1].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil][col+1].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 3;
			}
			else if ((aux_matrix[fil-1][col].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil-1][col].veces < aux_matrix[fil-1][col].veces && aux_matrix[fil-1][col].veces < aux_matrix[fil][col+1].veces)
			&& (aux_matrix[fil-1][col].tipo_terreno != ('M' || 'P') || (aux_matrix[fil-1][col].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil-1][col].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 2;
			}else{
				decision = ((rand()%2)+2);
			}
			cout << "Arriba: " << aux_matrix[fil][col+1].veces << endl;
			cout << "Derecha: " << aux_matrix[fil+1][col].veces << endl;
			cout << "Abajo: " << aux_matrix[fil][col-1].veces << endl;
			cout << "Izquierda: " << aux_matrix[fil-1][col].veces << endl;
      break;

			// Sur
			// Casilla arriba 			aux_matrix[fil+1][col]
			// Casilla derecha 			aux_matrix[fil][col-1]
			// Casilla abajo 				aux_matrix[fil-1][col]
			// Casilla izquierda		aux_matrix[fil][col+1]

    case 2:
			if ((aux_matrix[fil+1][col].veces < aux_matrix[fil][col-1].veces && aux_matrix[fil+1][col].veces < aux_matrix[fil-1][col].veces && aux_matrix[fil+1][col].veces < aux_matrix[fil][col+1].veces)
			&& (aux_matrix[fil+1][col].tipo_terreno != ('M' || 'P') || (aux_matrix[fil+1][col].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil+1][col].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 1;
			}
			// El de la derecha es la menor y puedo ir.
			else if ((aux_matrix[fil][col-1].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil][col-1].veces < aux_matrix[fil-1][col].veces && aux_matrix[fil][col-1].veces < aux_matrix[fil][col+1].veces)
			&& (aux_matrix[fil][col-1].tipo_terreno != ('M' || 'P') || (aux_matrix[fil][col-1].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil][col-1].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 3;
			}
			else if ((aux_matrix[fil][col+1].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil][col+1].veces < aux_matrix[fil][col-1].veces && aux_matrix[fil][col+1].veces < aux_matrix[fil-1][col].veces)
			&& (aux_matrix[fil][col+1].tipo_terreno != ('M' || 'P') || (aux_matrix[fil][col+1].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil][col+1].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 2;
			}else{
				decision = ((rand()%2)+2);
			}
			cout << "Arriba: " << aux_matrix[fil+1][col].veces << endl;
			cout << "Derecha: " << aux_matrix[fil][col-1].veces << endl;
			cout << "Abajo: " << aux_matrix[fil-1][col].veces << endl;
			cout << "Izquierda: " << aux_matrix[fil][col+1].veces << endl;
    	break;

			// Oeste:
			// Casilla arriba: 			aux_matrix[fil][col-1]
			// Casilll derecha: 		aux_matrix[fil-1][col]
			// Casilla abajo: 			aux_matrix[fil][col+1]
			// Casilla izquierda: 	aux_matrix[fil+1][col]

    case 3:
			if ((aux_matrix[fil][col-1].veces < aux_matrix[fil-1][col].veces && aux_matrix[fil][col-1].veces < aux_matrix[fil][col+1].veces && aux_matrix[fil][col-1].veces < aux_matrix[fil+1][col].veces)
			&& (aux_matrix[fil][col-1].tipo_terreno != ('M' || 'P') || (aux_matrix[fil][col-1].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil][col-1].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 1;
			}
			// El de la derecha es la menor y puedo ir.
			else if ((aux_matrix[fil-1][col].veces < aux_matrix[fil][col+1].veces && aux_matrix[fil-1][col].veces < aux_matrix[fil+1][col].veces && aux_matrix[fil-1][col].veces < aux_matrix[fil][col-1].veces)
			&& (aux_matrix[fil-1][col].tipo_terreno != ('M' || 'P') || (aux_matrix[fil-1][col].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil-1][col].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 3;
			}
			else if ((aux_matrix[fil+1][col].veces < aux_matrix[fil][col-1].veces && aux_matrix[fil+1][col].veces < aux_matrix[fil-1][col].veces && aux_matrix[fil+1][col].veces < aux_matrix[fil][col+1].veces)
			&& (aux_matrix[fil+1][col].tipo_terreno != ('M' || 'P') || (aux_matrix[fil+1][col].tipo_terreno == 'A' && sensores.objetoActivo == '1') || (aux_matrix[fil+1][col].tipo_terreno == 'B' && sensores.objetoActivo == '2'))) {
				decision = 2;
			}else{
				decision = ((rand()%2)+2);
			}
			cout << "Arriba: " << aux_matrix[fil][col-1].veces << endl;
			cout << "Derecha: " << aux_matrix[fil-1][col].veces << endl;
			cout << "Abajo: " << aux_matrix[fil][col+1].veces << endl;
			cout << "Izquierda: " << aux_matrix[fil+1][col].veces << endl;
      break;
  }

	cout << "Orientacion: " << orientacion << endl << "Decision: " << decision << endl << endl;
}
*/
