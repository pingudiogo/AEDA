/*
 * Destino.cpp
 *
 *  Created on: 21/12/2015
 *      Author: Diogo
 */

#include "Destino.h"

Destino::Destino():cidade(){

}

Destino::Destino(Cidade* c): cidade(c) {
	// TODO Auto-generated constructor stub

}

Destino::~Destino() {
	// TODO Auto-generated destructor stub
}

bool Destino::operator<(const Destino &d1) const{
	if(cidade->getNome()!=d1.cidade->getNome())
		return cidade->getNome()<d1.cidade->getNome();
	else if(cidade->getNome()==d1.cidade->getNome())
		if(alojamento<d1.alojamento)
			return true;
	return false;
}
