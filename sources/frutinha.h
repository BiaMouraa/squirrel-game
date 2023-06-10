#pragma once

class Frutinha{
    private:
        int eixoX;
        int eixoY;
        int comeu;
    public:
        Frutinha();
        void move();

        int getX();
        int getY();
        int getComeu();
        void setComeu(int comeu);
};