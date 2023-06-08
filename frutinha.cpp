#include "frutinha.h"
#include <cstdlib>

Frutinha::Frutinha(){
    eixoX = 50;
    eixoY = 300;
    comeu = 0;
}

void Frutinha::move(){
    eixoX = rand() % 560 + 40;
    eixoY = rand() % 400 + 30;
    comeu = 0;
}

int Frutinha::getX(){ return eixoX; }

int Frutinha::getY(){ return eixoY; }

int Frutinha::getComeu(){ return comeu; }

void Frutinha::setComeu(int comeu){ this->comeu = comeu; }