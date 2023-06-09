#include "frutinha.h"
#include <cstdlib>

Frutinha::Frutinha(){
    eixoX = 50;
    eixoY = 300;
    comeu = 0;
}

void Frutinha::move(){
    eixoX = rand() % 460 + 10;
    eixoY = rand() % 360 + 10;
    comeu = 0;
}

int Frutinha::getX(){ return eixoX; }

int Frutinha::getY(){ return eixoY; }

int Frutinha::getComeu(){ return comeu; }

void Frutinha::setComeu(int comeu){ this->comeu = comeu; }