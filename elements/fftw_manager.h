#ifndef _FFTW_MANAGER_H
#define _FFTW_MANAGER_H
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include "util.h"

class FFTWManager {
public:
    FFTWManager();
    FFTWManager(const FFTWManager& orig);
    virtual ~FFTWManager();

    fftw_complex *in, *out, *realOut;
    fftw_plan p, pInv;
    int N;
    bool isPlan;
    void initialize(int N);
    void Plan_1D();

    void Execute();
    void Execute_Inverse();
    void Free();

    void InitializeAndExecuteFromAVector(double *vec, int rows, int cols, int is2D);

    void FFT_Filter(double *F, int rows, int cols);
    void FFT_Filter(FFTWManager *F, int rows, int cols);
    void FFT_ToCenter(int rows, int cols);
    void FFT_ToCenter_Inverse(int rows, int cols);
    void Plan_2D(int rows, int cols);
    void to_double_vector(int indexOfVector, double *OUT);
private:
};
#endif