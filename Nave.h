////#ifndef NAVE_H_
//#define NAVE_H_
//
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>
//#include <stdio.h>
//#include <string>
//
//class Nave
//{
//    public:
//
//        //Se inicializa la Nave en esas coordenadas
//		Nave( int x, int y );
//
//		//Mueve la Nave considerando la posicion de la Nave enemiga
//		void mover( NaveEnemiga& enemigo );
//
//		void renderizar();
//
//		Circle& getColicionador();
//
//		//Mueve el círculo de colisión en relación con el desplazamiento de la Nave
//		void desplazarColicionador();
//
//		int getPosX();
//
//		void setPosX(int n);
//
//		int getPosY();
//
//		void setPosY(int n);
//
//        int getVelX();
//
//		void setVelX(int n);
//
//        int getVelY();
//
//		void setVelY(int n);
//
//		bool checkCollision( Circle& a, Circle& b );
//
//		double distanceSquared( int x1, int y1, int x2, int y2 );
//
//    private:
//
//        //Las coordenadas X e Y de la Nave
//		int mPosX, mPosY;
//
//		//La velocidad de la Nave
//		int mVelX, mVelY;
//
//        //La Nave representada por un circula colisionador
//		Circle mColicionador;
//
//};
//
//#endif
