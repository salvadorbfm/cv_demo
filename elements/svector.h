#ifndef _SVECTOR_H
#define _SVECTOR_H
#include <stdio.h>
#include <cstring>
#include <iostream>
using namespace std;
namespace elements
{
    class SVector
    {
    public:
            SVector();
            SVector(int longitud);
            SVector(const SVector &init);
            ~SVector();

            bool set_longitud(int longitud);
            int  get_longitud();
            void set(int posicion, double valor);
            double get(int posicion);
            double& operator[](long posicion);
            void release(void);
            int get_size(void);
            double get_max(void);
            double sum(int indexA, int indexB);
            double mean();
            double normalized(double value);

    // ________________________________________________operadores
            SVector& operator=(const SVector& init);
            bool operator==(const SVector& init)const;
            SVector operator+(SVector init);

    // ________________________________________________


            double *datos;

    private:

            int longitud;
    };
}
#endif