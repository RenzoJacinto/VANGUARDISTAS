#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

class Temporizador
{

    public:

        void iniciar()
        {
            iniciarTemporizador = std::chrono::system_clock::now();
            sigueContabilizando = true;
        }

        void finalizar()
        {
            finalizarTemporizador = std::chrono::system_clock::now();
            sigueContabilizando = false;
        }

        double transcurridoEnMilisegundos()
        {
            std::chrono::time_point<std::chrono::system_clock> medidor;

            if(sigueContabilizando)
            {
                medidor = std::chrono::system_clock::now();
            }
            else
            {
                medidor = finalizarTemporizador;
            }

            return std::chrono::duration_cast<std::chrono::milliseconds>(medidor - iniciarTemporizador).count();
        }

        double transcurridoEnSegundos()
        {
            return transcurridoEnMilisegundos() / 1000.0;
        }

    private:

        std::chrono::time_point<std::chrono::system_clock> iniciarTemporizador;
        std::chrono::time_point<std::chrono::system_clock> finalizarTemporizador;
        bool sigueContabilizando = false;

};

#endif
