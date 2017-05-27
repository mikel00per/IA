#include "../Comportamientos_Jugador/jugador.hpp"
#include <set>
#include <queue>

using namespace std;

void ComportamientoJugador::actualizarInformacion(Sensores sensores) {
	cout << "::::::::::::::: Actualizo memoria :::::::::::::::" << endl;
	if (sensores.reset) {
		Reiniciado();
	}else{
		cout << "Sigo Vivo. " << endl;
		// Borro los elementos que se movian.
		for (int i = fil-1; i < fil + 1; i++)
			for (int j = col-1; j < col + 1; j++)
				if (aux_matrix[i][j].tipo_superficie == 'l' && i > 0 && j > 0 && i < TAM && j < TAM)
					aux_matrix[i][j].tipo_superficie == '?';
		cout << "Accion anterior: ";
		switch (ultima_accion) {
			case actFORWARD:
				cout << "¡Sigo recto!" << endl;
				if (!sensores.colision) {
					switch (orientacion) {
						case 0: fil--; if (ubicado) real_fil--; break;
						case 1: col++; if (ubicado) real_col++; break;
						case 2: fil++; if (ubicado) real_fil++; break;
						case 3: col--; if (ubicado) real_col--; break;
					}
				}
				break;

			case actTURN_L:
				cout << "¡Giro izquierda" << endl;
				orientacion = (orientacion+3)%4;
				break;

			case actTURN_R:
				cout << "¡Giro derecha!" << endl;
				orientacion = (orientacion+1)%4;
				break;

			case actPICKUP:
				cout << "¡Un objeto! Lo cojo" << endl;
				switch (sensores.objetoActivo) {
					case '0': tengo_Hueso = true;	break;
					case '1': tengo_Bikini = true;	break;
					case '2': tengo_Zapatillas = true;	break;
					case '3': tengo_Llave = true;	break;
					case '4': tengo_Regalo = true;
						ejecutando_plan = false;
						plan.clear();
					break;
				}
				break;
				case actPUSH:
					cout << "Guardaré en la bolsa el objeto" << endl;
					num_objetos_mochila++;
					cout << "Control de mochila, nMoch: " << num_objetos_mochila << endl;
				break;

				case actPOP:
					cout << "Sacaré algo, creo que es necesario" << endl;
					num_objetos_mochila--;
				break;

				case actGIVE:
					cout << "Voy a darle el objeto a la entidad" << endl;
					if (darPerro) {tengo_Hueso = false; darPerro = false;}
					if (darRey) {
						tengo_Regalo = false;
						darRey = false;
						ejecutando_plan = false;
						plan.clear();
					}

				break;
		}

		cout << "Fila: " << fil << endl << "Cols: " << col << endl << "Paso: " << pasos << endl;

		guardarQueso(sensores.terreno, 't');
		guardarQueso(sensores.superficie, 's');
		if (ubicado)
			guardarQueso(sensores.terreno, 'r'); // r de real, cuando me ubico pinto

	}
	cout << ":::::::::::::: Fin actualizacion :::::::::::::::" << endl;
}

void ComportamientoJugador::Reiniciado() {

	cout << "	[REINICIADO]: ";
	if (ubicado) {
		cout << "	¡Guardo lo que he visto hasta ahora, hasta ubicarme!" << endl;
		copiadoAnterior = true;
		for (int i = 0; i < TAM; i++)
			for (int j = 0; j < TAM; j++)
				if (aux_matrix[i][j].tipo_terreno != '?' && aux_muerto[i][j].tipo_terreno =='?')
					aux_muerto[i][j].tipo_terreno = aux_matrix[i][j].tipo_terreno;

	}

	cout << "	Borro matriz auxiliar " << endl;
	for (int i = 0; i < TAM; i++) {
		for (int j = 0; j < TAM; j++) {
			aux_matrix[i][j].veces = 0;
			aux_matrix[i][j].tipo_superficie='?';
			aux_matrix[i][j].tipo_terreno='?';
		}
	}

	pasos = 0;
	real_fil = -1;
	real_col = -1;
	orientacion = 0;
	ubicado = false;
	num_objetos_mochila = 0;

	// Como pierdo los ojetros
	tengo_Hueso = false;
	tengo_Llave = false;
	tengo_Bikini = false;
	tengo_Regalo = false;
	tengo_Zapatillas = false;


	ejecutando_plan = false;
	error_plan = false;
	// Restauro filas
	fil = col = 99;

}

bool ComportamientoJugador::tengoObjetosParaTravesia(){
	return tengo_Bikini && tengo_Zapatillas;
}

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
		estado rey;
		switch (orientacion) {
			case 0:
				if (vector[2] == 'r') {
					num_Rey++;
					rey.fils = fil - 1;
					rey.cols = col;

					bool repetido = false;
					for (int i = 0; i < reyes.size() ; i++){
						if (reyes[i].fils == rey.fils && reyes[i].cols == rey.cols){
							repetido = true;
							cout << "¡¡¡¡¡¡¡¡¡¡¡¡¡¡no repetido!!!!!!!!!!!!!!!!1!!! " << endl;
						}
					}
					if (!repetido) reyes.push_back(rey);
				}
				aux_matrix[fil-1][col].tipo_superficie = vector[2];
				break;
			case 1:
				if (vector[2] == 'r') {
					num_Rey++;
					rey.fils = fil;
					rey.cols = col + 1;

					bool repetido = false;
					for (int i = 0; i < reyes.size(); i++){
						if (reyes[i].fils == rey.fils && reyes[i].cols == rey.cols){
							repetido = true;
							cout << "no repetido!!!!! " << endl;
						}
					}
					if (!repetido) reyes.push_back(rey);

				}
				aux_matrix[fil][col+1].tipo_superficie = vector[2];
				break;
			case 2:
				if (vector[2] == 'r') {
					num_Rey++;
					rey.fils = fil + 1;
					rey.cols = col;
					bool repetido = false;
					for (int i = 0; i < reyes.size(); i++){
						if (reyes[i].fils == rey.fils && reyes[i].cols == rey.cols){
							repetido = true;
							cout << "no repe!!!!! " << endl;
						}
					}
					if (!repetido) reyes.push_back(rey);
				}
				aux_matrix[fil+1][col].tipo_superficie = vector[2];
				break;
			case 3:
				if (vector[2] == 'r'){
					num_Rey++;
					rey.fils = fil;
					rey.cols = col - 1;
					bool repetido = false;
					for (int i = 0; i < reyes.size(); i++){
						if (reyes[i].fils == rey.fils && reyes[i].cols == rey.cols){
							repetido = true;
							cout << "no repe!!!!! " << endl;

						}
					}
					if (!repetido) reyes.push_back(rey);
				}
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
		case 'D': es_puerta = true; break;
		case 'A': terreno = 'A'; objeto_necesario = '1'; tengo = tengo_Bikini; break;
		case 'B': terreno = 'B'; objeto_necesario = '2'; tengo = tengo_Zapatillas; break;

		// NOTE Añadir case T y case S de la version anterior
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

	cout << "Decision tras mirarTerreno 1: " << decision << endl;


	// Controla si, al cambiar de objeto me voy a morir:
	if (sensores.terreno[0] == 'A' &&  (decision == 7 || decision == 9)) {
		decision = 1;
	}else if (sensores.terreno[0] == 'B' && (decision == 7 || decision == 9)) {
		decision = 1;
	}else
		decision = decision; // Es estúpido pero para que se entienda conceptualment
												 // que la decisión previa no cambiaría.

	cout << "Decision tras mirarTerreno 2: " << decision << endl;
}

// En la superficie hay objetos o personajes
void ComportamientoJugador::mirarSupeficie(Sensores sensores) {
	bool necesito = false;

	//
	//  En caso de haber una puerta avanzo, hay que tener en cuenta que despues
	// mirarTerreno le da la llave si es necesario. despues avanza automaticamente
	// debido a darPosibleObjeto y el valor de la casilla
	// NOTE: La puerta desaparece de la vision cuando se la das.
	//
	switch (sensores.superficie[2]) {
		case '0': necesito = false; 											break;
		case '1': necesito = !tengo_Bikini;								break;
		case '2': necesito = !tengo_Zapatillas;						break;
		case '3': necesito = !tengo_Llave ;								break;
		case '4': necesito = !tengo_Regalo;	break;
	}

	if (sensores.superficie[2] >= '0' && sensores.superficie[2] <= '4' && necesito) {
		recogerPosibleObjeto(sensores);
	}else if (sensores.superficie[2] == 'l') {
		darPosibleObjeto(sensores, 'l');
		if (decision == 9) darPerro = true;
	}else if (sensores.superficie[2] == 'r') {
		darPosibleObjeto(sensores, 'r');
		if (decision == 9) darRey = true;
	}else
		decision = 1;

		if (sensores.terreno[0] == 'A' &&  (decision == 7 || decision == 9)) {
			decision = 1;
			darPerro = darRey = false;
		}else if (sensores.terreno[0] == 'B' && (decision == 7 || decision == 9)) {
			decision = 1;
			darPerro = darRey = false;
		}else
			decision = decision; // Es estúpido pero para que se entienda conceptualment


	cout << "Decision tras mirarSupeficie: " << decision << endl;
}

// Pre: Sé que lo necesito antes de llamar a la función por eso Solo
// mete y saca de la mochila para encontrarlo.
void ComportamientoJugador::recogerPosibleObjeto(Sensores sensores) {

	if (sensores.objetoActivo != '_' && num_objetos_mochila < 4) {
		decision = 7;	// Guardar manos
	}else if (sensores.objetoActivo == '_' ) {
		decision = 5;	// Cogerlo
	}else
		decision = 1;	// Seguir

	cout << "N. objetos mochila " << num_objetos_mochila << endl;
	std::cout << "recogerPosibleObjeto, decision: " << decision << '\n';
}

void ComportamientoJugador::darPosibleObjeto(Sensores sensores, char entidad) {

	char tipo; // 0 es terreno, 1 es superficie
	char objeto_necesario;
	bool tengo;

	if 			(entidad == 'l') { objeto_necesario = '0'; tengo = tengo_Hueso; }
	else if (entidad == 'd') { objeto_necesario = '3'; tengo = tengo_Llave; }
	else if	(entidad == 'r') { objeto_necesario = '4'; tengo = tengo_Regalo; }

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
		ubicado = true;
		real_fil = sensores.mensajeF;
		real_col = sensores.mensajeC;
		calculoTrasUbicarme();
	}
	/*
	else if (sensores.terreno[0] == 'K' && ubicado) {
	for (size_t i = 0; i < TAM; i++) {
	for (size_t j = 0; j < TAM; j++) {
	if (aux_matrix[i][j].tipo_superficie != '?') {
	aux_matrix[i][j].veces += 1000;
}
}
}
}

 */

}

void ComportamientoJugador::calculoTrasUbicarme() {
	int diferencia_f = real_fil - fil ;
	int diferencia_c = real_col - col;

	for (int i = 0; i < TAM; i++)
		for (int j = 0; j < TAM; j++)
			if (aux_matrix[i][j].tipo_terreno != '?')
				mapaResultado[i+diferencia_f][j+diferencia_c] = aux_matrix[i][j].tipo_terreno;

	if (copiadoAnterior) {
		for (int i = 0; i < TAM; i++) {
			for (int j = 0; j < TAM; j++) {
				if (aux_matrix[i][j].tipo_terreno == '?' && aux_muerto[i][j].tipo_terreno != '?')
					aux_matrix[i][j].tipo_terreno = aux_muerto[i][j].tipo_terreno;

				if (aux_matrix[i][j].tipo_terreno != '?') {
					aux_matrix[i][j].veces = pasos;
				}
			}
		}
		copiadoAnterior = false;
	}

}

void ComportamientoJugador::comprobarCasillas(Sensores sensores) {
	int valor_arriba, valor_derecha, valor_abajo, valor_izquierda;
	cout << "Compruebo casillas: " << endl;

	switch (orientacion) {
		case 0:
			valor_arriba		= hayObstaculo(sensores, fil-1,	col, sensores.terreno[0]);
			valor_derecha		= hayObstaculo(sensores, fil, col+1, sensores.terreno[0]);
			valor_izquierda	= hayObstaculo(sensores, fil, col-1, sensores.terreno[0]);
			break;
		case 1:
			valor_arriba		= hayObstaculo(sensores, fil, col+1, sensores.terreno[0]);
			valor_derecha		= hayObstaculo(sensores, fil+1, col, sensores.terreno[0]);
			valor_izquierda	= hayObstaculo(sensores, fil-1, col, sensores.terreno[0]);
			break;
		case 2:
			valor_arriba		= hayObstaculo(sensores, fil+1, col, sensores.terreno[0]);
			valor_derecha		= hayObstaculo(sensores, fil, col-1, sensores.terreno[0]);
			valor_izquierda	= hayObstaculo(sensores, fil, col+1, sensores.terreno[0]);
			break;
		case 3:
			valor_arriba		= hayObstaculo(sensores, fil, col-1, sensores.terreno[0]);
			valor_derecha		= hayObstaculo(sensores, fil-1, col, sensores.terreno[0]);
			valor_izquierda	= hayObstaculo(sensores, fil+1, col, sensores.terreno[0]);
			break;
	}
	cout << "----------------" << endl;
	cout << "      " << valor_arriba << endl;
	cout << valor_izquierda << "            " << valor_derecha << endl;
	cout << "----------------" << endl;

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

	std::cout << "decision: " << decision << '\n';
}

int	ComportamientoJugador::hayObstaculo(Sensores sensores, int y, int x, char down){

	int valor_casilla = 0;
	bool valoro = false;

	// Si hay un objeto en la casilla puede convertirse en un obstaculo o en algo
	// beneficioso

	cout << "Valor matrix T: " << aux_matrix[y][x].tipo_terreno;;

	if (aux_matrix[y][x].tipo_terreno == 'P' || aux_matrix[y][x].tipo_terreno == 'M') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_terreno == 'A' && !tengo_Bikini) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_terreno == 'B' && !tengo_Zapatillas) {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_terreno == 'A' && tengo_Bikini && down == 'B') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_terreno == 'B' && tengo_Zapatillas && down == 'A') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_terreno == 'D' && !tengo_Llave) {
		valor_casilla = valor_casilla + 100000;
	}else{
		valor_casilla = valor_casilla + aux_matrix[y][x].veces;
	}

	if (aux_matrix[y][x].tipo_superficie >= '0' && aux_matrix[y][x].tipo_superficie <= '4') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_superficie == 'l') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_superficie == 'a') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_superficie == 'r') {
		valor_casilla = valor_casilla + 100000;
	}else if (aux_matrix[y][x].tipo_superficie == 'd' && aux_matrix[y][x].tipo_terreno == 'M') {
		valor_casilla = valor_casilla - 102000;
	}else{
		valor_casilla = valor_casilla + aux_matrix[y][x].veces;
	}

	cout << " " << valor_casilla  << endl;
	return valor_casilla;
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan){
	bool seguir = true, solucion = false;

	start.st = origen;
	goal.st = destino;

	int h =  origen.distanciaManhattanEstados(destino);
	start.G = 0;
	start.H = h;
	start.setF();


	while (!ABIERTOS.empty())
		ABIERTOS.pop();

	CERRADOS.clear();

	ABIERTOS.push(start);

	cout << "~~ Buscando Camino ~~" << endl;
	cout << "Padre: (" << start.st.fils << "," << start.st.cols << ")" << endl;

	while (!ABIERTOS.empty() && seguir) {

		cout << "	> Nodo siguiente ABIERTO ";

		Nodo nodoActual = ABIERTOS.top();
		CERRADOS.insert(nodoActual.st);
		ABIERTOS.pop();

		cout << "F[" << nodoActual.st.fils << "] C[" << nodoActual.st.cols << "] O[" << nodoActual.st.orientacion << "] "<< endl;


		// Si es nodo final termino de epandir y recupero camino
		if ((nodoActual.st.cols == goal.st.cols) && (nodoActual.st.fils == goal.st.fils)) {
			cout << "		> Nodo encontrado " << endl;
			goal.lista_acciones = nodoActual.lista_acciones;
			plan = goal.lista_acciones;
			seguir = false; solucion = true;
		}else if ((nodoActual.st.distanciaManhattanEstados(destino) > (origen.distanciaManhattanEstados(destino) *2)) || (nodoActual.F > origen.distanciaManhattanEstados(destino)*3)) {
			cout << "		> Nodo mejor es demasiado malo, paro !" << endl;
			seguir = false; solucion = false;
		}else{

			// Simulo la brujula y añado las nuevas coordenadas a los nodos
	    list<Action> listaArriba,listaDerecha, listaIzquierda;
	    // un hijo que esté en uan casilla colindante supone dos acciones
	    listaDerecha.push_back(actTURN_R);
	    listaDerecha.push_back(actFORWARD);
	    listaIzquierda.push_back(actTURN_L);
	    listaIzquierda.push_back(actFORWARD);
			listaArriba.push_back(actFORWARD);

			cout << "		> Inserto Hijos " << endl;

			int x = nodoActual.st.cols;
			int y = nodoActual.st.fils;
			int i;
			int j;

			switch (nodoActual.st.orientacion) {
	      case 0:
					i = nodoActual.st.fils-1;
					j = nodoActual.st.cols;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i, j,  nodoActual.st.orientacion,      nodoActual.G + 1, nodoActual.lista_acciones, listaArriba);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;
					i = nodoActual.st.fils;
					j = nodoActual.st.cols+1;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i, j, (nodoActual.st.orientacion+1)%4, nodoActual.G + 2, nodoActual.lista_acciones, listaDerecha);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;

					i = nodoActual.st.fils;
					j = nodoActual.st.cols-1;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i, j, (nodoActual.st.orientacion+3)%4, nodoActual.G + 2, nodoActual.lista_acciones, listaIzquierda);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;
	      break;

	      case 1:
					i = nodoActual.st.fils;
					j = nodoActual.st.cols+1;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i,j, nodoActual.st.orientacion,      nodoActual.G + 1, nodoActual.lista_acciones, listaArriba);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;

					i = nodoActual.st.fils+1;
					j = nodoActual.st.cols;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i,j, (nodoActual.st.orientacion+1)%4, nodoActual.G + 2, nodoActual.lista_acciones, listaDerecha);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;

					i = nodoActual.st.fils-1;
					j = nodoActual.st.cols;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i,j, (nodoActual.st.orientacion+3)%4, nodoActual.G + 2, nodoActual.lista_acciones, listaIzquierda);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;
				break;

	      case 2:
					i = nodoActual.st.fils+1;
					j = nodoActual.st.cols;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i, j,  nodoActual.st.orientacion,      nodoActual.G + 1, nodoActual.lista_acciones, listaArriba);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;

					i = nodoActual.st.fils;
					j = nodoActual.st.cols-1;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i, j, (nodoActual.st.orientacion+1)%4, nodoActual.G + 2, nodoActual.lista_acciones, listaDerecha);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;

					i = nodoActual.st.fils;
					j = nodoActual.st.cols+1;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i, j, (nodoActual.st.orientacion+3)%4, nodoActual.G + 2, nodoActual.lista_acciones, listaIzquierda);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;
	      break;

	      case 3:
					i = nodoActual.st.fils;
					j = nodoActual.st.cols-1;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i,j, nodoActual.st.orientacion,      nodoActual.G + 1, nodoActual.lista_acciones, listaArriba);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;

					i = nodoActual.st.fils-1;
					j = nodoActual.st.cols;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i,j, (nodoActual.st.orientacion+1)%4, nodoActual.G + 2, nodoActual.lista_acciones, listaDerecha);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;

					i = nodoActual.st.fils+1;
					j = nodoActual.st.cols;
					if (!(aux_matrix[i][j].tipo_terreno == 'A' && aux_matrix[y][x].tipo_terreno == 'B') && !(aux_matrix[i][j].tipo_terreno == 'B' && aux_matrix[y][x].tipo_terreno == 'A') && i<TAM && i>0 && j<TAM && j>0)
						insertarAbiertos(i,j, (nodoActual.st.orientacion+3)%4, nodoActual.G + 2, nodoActual.lista_acciones, listaIzquierda);
					else
						cout << "		>>> HIJO PELIGROSO AGUA/BOSQUE" << endl;
				break;
	    }
		}
	}

	if (ABIERTOS.empty()) {
		solucion = false;
	}

	return solucion;
}

void ComportamientoJugador::insertarAbiertos(int y, int x, int nseo, int nuevoCoste, list<Action>acciones, list<Action> accionesF) {

	if (ValorCasilla(y,x) > 0) {
		cout<< "				> No inserto hijo, obstaculo " << endl;
		return;
	}


  // Cuando se considera que un nodo es igual a otro?
  // Mismas coordenadas, padre y coste?

	estado nuevoEstado = estado(y, x, nseo);

	std::set<estado>::iterator it = CERRADOS.find(nuevoEstado);
	if (*it == nuevoEstado){
		cout << "				> Hijo existe"<< endl;
		return;
	}

	cout << "				> Hijo no existe" <<  endl;

  Nodo nodoHijo = Nodo(nuevoEstado, acciones);
  list<Action> accionesFuturas = accionesF;

	cout << "				NodoHIJO: " <<
	"F[" << nodoHijo.st.fils << "] C[" << nodoHijo.st.cols << "] O[" << nodoHijo.st.orientacion << "] "<< endl;

  // Meto las nuevas acciones, pueden ser dos
  while (!accionesFuturas.empty()) {
		cout<< "				> Nueva acción para nodo Hijo" << endl;
    nodoHijo.lista_acciones.push_back(accionesFuturas.front());
    accionesFuturas.pop_front();
  }

  nodoHijo.G = nuevoCoste;
  nodoHijo.H = nodoHijo.distanciaManhattan(goal);
  nodoHijo.setF(); // -> calcula F, func. del struct

  // Si la cola con prioridad guarda o no repetidos
  // NOTE creo que hay que imlemntar el metodo comp(),
  // es como comparar los objetosde una lista.

  ABIERTOS.push(nodoHijo);
	cout << "				> Nodo Hijo insertado " << endl;
}

int ComportamientoJugador::ValorCasilla(int y, int x) {
  int valor = 0;
	cout << "			>> Valoro casilla/nodo: ";

  char superficie = aux_matrix[y][x].tipo_superficie,
       terreno    = aux_matrix[y][x].tipo_terreno;

	cout << terreno << superficie << "-";
  if (terreno == 'M' || terreno == 'P' || terreno == '?') {
		valor += 10000;
  }

	if (superficie != '?' && superficie != 'r' && superficie != '_') {
		valor += 10000;
	}

	if (superficie == 'r' && !tengo_Regalo) {
		valor += 10000;
	}

	cout << "[" << valor << "]" << endl;

	return valor;
}


ComportamientoJugador::estado ComportamientoJugador::getInicio() {
	estado inicial;

	inicial.fils = real_fil;
	inicial.cols = real_col;
	inicial.orientacion = orientacion;

	return inicial;
}

ComportamientoJugador::estado ComportamientoJugador::getBestObjetivo(Sensores sensores, char tipo){
	estado 	inicio;
					inicio.fils = fil;
					inicio.cols = col;
					inicio.orientacion = orientacion;

	estado fin;
	estado aux;

	int diferencia_f = real_fil - fil;
	int diferencia_c = real_col - col;

	bool existePlan = false;
	switch (tipo) {
		case 'r':
			cout << "Selecionando rey. " << endl;
			for (int i = 0; i < reyes.size() && !existePlan; i++){
				aux = reyes[i];
				aux.orientacion = orientacion;
				plan.clear();
				existePlan = pathFinding(inicio, aux, plan);
			}
		break;

		case 'g':
			cout << "Seleciono regalo posible " << endl;
			pair<int, int> auxPair;
			for (int i = 0; i < sensores.regalos.size() && !existePlan; i++) {
				auxPair = sensores.regalos[i];
				aux.fils = auxPair.first-diferencia_f;
				aux.cols = auxPair.second-diferencia_c;
				aux.orientacion = orientacion;
				if (sensores.terreno[0] == 'B' && aux_matrix[aux.fils][aux.cols].tipo_terreno == 'A') {
					existePlan = false;
				}else if (sensores.terreno[0] == 'A' && aux_matrix[aux.fils][aux.cols].tipo_terreno == 'B') {
					existePlan = false;
				}else if (aux_matrix[aux.fils][aux.cols].tipo_terreno != '?' && (sensores.terreno[2] == 'S' || sensores.terreno[2] == 'T')){
					plan.clear();
					existePlan = pathFinding(inicio, aux, plan);
					cout << "	~ existe plan: "<< existePlan << endl;
				}
			}
		break;
	}

	if (existePlan) {
		fin.fils = aux.fils;
		fin.cols = aux.cols;
		fin.orientacion = orientacion;
		cout << "~ Existe camino al objetivo." << endl;
		cout << "~ Mejor Objetivo: [" << fin.fils << "," << fin.cols << "]" << endl;
	}else{
		fin.orientacion = 10000;
		cout << "~ No existe camino al objetivo." << endl;
	}

	return fin;
}

void ComportamientoJugador::buscarPlanificarEjecutar(Sensores sensores) {
	if (!ejecutando_plan && ubicado && tengoObjetosParaTravesia() && sensores.tiempo < 250) {
		cout << "Planifico para ir a un destino" << endl;
		estado inicio, fin;
					 inicio.fils = fil;
					 inicio.cols = col;
					 inicio.orientacion = orientacion;

			plan.clear();
			if (!tengo_Regalo && error_plan) {
				cout << "Necesito un regalo" << endl;
				fin = getBestObjetivo(sensores, 'g');
			}else if (!tengo_Regalo && !error_plan) {
				cout << "Recalculando ruta para regalo" << endl;
				fin = getBestObjetivo(sensores, 'g');
			}else if (tengo_Regalo && !error_plan) {
				cout << "Necesito a quien darle el regalo" << endl;
				fin = getBestObjetivo(sensores, 'r');
			}else if (tengo_Regalo && error_plan) {
				cout << "Recalculando a que rey ir" << endl;
				fin = getBestObjetivo(sensores, 'r');
			}

		// Indica que ningún plan es factible
		int tiempoNecesario;
		if (fin.orientacion == 10000) tiempoNecesario = 10000;
		else tiempoNecesario = inicio.distanciaManhattanEstados(fin);

		cout << "	~ Tiempo estimado: " << tiempoNecesario << endl;

		if (tiempoNecesario < 200) {
			cout << "	~ Es posible ejecutar el plan" << endl;
			error_plan = false;
			ejecutando_plan = true;
			decision_futura = plan.front();
		}else
			cout << "	~ Es imposible ejecutar el plan" << endl;

			if (ejecutando_plan == true) {
				cout << "Acciones necesarias: ";
				while (!goal.lista_acciones.empty()){ cout << goal.lista_acciones.front() << " "; goal.lista_acciones.pop_front();}
			}
	}
}

Action ComportamientoJugador::think(Sensores sensores){
	cout << "///////// INICIO THINK" << endl;
	pasos++;
	decision = 1;
	Action accion;
	aux_matrix[fil][col].veces = pasos;

	actualizarInformacion(sensores);

	if (ejecutando_plan) {
		if (!plan.empty())
			decision_futura = plan.front();
		decision = (int)decision_futura + 1;
		cout << " ~~ Ejecutando plan. Misma acción necesaria ! : " << decision_futura << " aka: " << decision << endl;
	}

	cout << endl;

	cout << "::::::::::::::: Sensores :::::::::::::::" << endl;
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
	pair<int, int> casilla;
	for (int i = 0; i < sensores.regalos.size(); i++) {
		casilla = sensores.regalos[i];
		cout << "Regalo [" << i << "] = " << "(" << casilla.first << "," << casilla.second << ")" << endl;
	}
	cout << endl;
	for (int i = 0; i < reyes.size(); i++)
		cout << "Rey " << i << ": (" << reyes[i].fils << "," << reyes[i].cols << ")" << endl;
	cout << "Tengo H[" << tengo_Hueso << "] LL[" << tengo_Llave << "] R["
				<< tengo_Regalo << "]" << " B[" << tengo_Bikini << "] Z["
				<< tengo_Zapatillas << "] " << endl;
	cout << "::::::::::::::: Fin Sensores :::::::::::::::" << endl;

	cout << endl;


	cout << "::::::::::::::: Mensajes en el camino :::::::::::::::" << endl;

	// 2) Analizo lo que tengo delante
	comprobarJustoDelante(sensores);

	// 3) Analizo lo que tengo debajo
	comprobarJustoDebajo(sensores);

	// 4) Compuebo si puedo ejecutar el plan de ir a por un regalo
	buscarPlanificarEjecutar(sensores);

	// 4) Si tengo que avanzar compruebo la casilla, o si me bloqueo por:
	// no tener objeto, por ser un aldeano, por tener todos los objetos
	if (decision == 1 && !ejecutando_plan)
		comprobarCasillas(sensores);

	switch (decision) {
		case 1: accion = actFORWARD;	break;
		case 2: accion = actTURN_L;		break;
		case 3: accion = actTURN_R;		break;
		case 4: accion = actIDLE;			break;
		case 5: accion = actPICKUP; 	break;
		case 6: accion = actPUTDOWN; 	break;
		case 7: accion = actPUSH; 		break;
		case 8: accion = actPOP; 			break;
		case 9: accion = actGIVE; 		break;
		case 10: accion = actTHROW; 	break;
	}

	if (ejecutando_plan) {
		if (decision_futura == accion) {
			cout << "~~ Reactivo y delativo misma acción ! " << endl;
			cout << "decision_futura 1: " << (int)decision_futura +1 << endl;
			cout << "accion 1: " << (int)accion +1 << endl;

			if (accion == actFORWARD && sensores.superficie[2] != '_') {
				accion = actIDLE;
			}

			if (sensores.colision != 0) {
				cout << "Colisioné antes, arreglo y espero" << endl;
				plan.push_front(decision_futura);
				accion = actIDLE;
			}

			cout << "decision_futura 2: " << (int)decision_futura +1 << endl;
			cout << "accion 2: " << (int)accion +1 << endl;

			if (accion == actIDLE && sensores.superficie[2] >= '0' && sensores.superficie[2] <= '4' ) {
				cout << "	~Error en el plan" << endl;
				error_plan = true;
				ejecutando_plan = false;
			}

			if (!plan.empty() && accion != actIDLE)
				plan.pop_front();
		}else{
			cout << "~~ Realizao OTRA accion ! " << endl;
			cout << "decision_futura: " << (int)decision_futura +1 << endl;
			cout << "accion: " << (int)accion +1 << endl;

			if (accion == actFORWARD || accion == actTURN_R || accion == actTURN_L) {
				accion = decision_futura;
			}

			if (accion == actFORWARD && sensores.superficie[2] != '_') {
				accion = actIDLE;
			}
			if (!plan.empty() && accion == decision_futura)
				plan.pop_front();
		}
	}

	cout << "Ultima Accion: " << ultima_accion << endl << "Accion: " << accion << endl;
	ultima_accion = accion;

	cout << "::::::::::::::: Fin Mensajes en el camino :::::::::::::::" << endl;


	cout << endl << endl;

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
