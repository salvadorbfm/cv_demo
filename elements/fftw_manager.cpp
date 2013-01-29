
#include "fftw_manager.h"


using namespace elements;
FFTWManager::FFTWManager() {
    isPlan=false;
}

FFTWManager::FFTWManager(const FFTWManager& orig) {
}

FFTWManager::~FFTWManager() {
}

void FFTWManager::FFT_Filter(FFTWManager* F, int rows, int cols)
{
    int k=0;
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            this->out[k][0] = (this->out[k][0])*Util::abs(F->out[k][0]);//Gain;
            this->out[k][1] = (this->out[k][1])*Util::abs(F->out[k][0]);//Gain;
            //this->out[k][0] = Util::Abs((this->out[k][0])*(F->out[k][0]) - (this->out[k][1])*(F->out[k][1])) ;
            //this->out[k][1] = Util::Abs((this->out[k][1])*(F->out[k][0]) + (this->out[k][0])*(F->out[k][1]));//Gain;
            k++;
        }
    }
}
void FFTWManager::FFT_Filter(double *F, int rows, int cols)
{
    //double *OUT = new double[rows*cols];
    int k=0,cx=0, cy=0;

    ////cout << endl << rows << " and " << cols << endl;
    cx = cols/2;
    cy = rows/2;
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            this->out[k][0] = (this->out[k][0])*F[k];//Gain;
            this->out[k][1] = (this->out[k][1])*F[k];//Gain;
            k++;
        }
    }
}

void FFTWManager::FFT_ToCenter(int rows, int cols)
{
    int index = 0, sign = 1;
    register int i=0, j=0;
    while(i<rows)
    {
        while(j<cols)
        {
            index = i*cols + j;
            if((i+j)%2)
            {
                sign = -1;
            }else
            {
                sign = 1;
            }
            this->in[index][0] = (sign) * this->in[index][0];
            this->in[index][1] = (sign) * this->in[index][1];
            j++;
        }
        j=0;
        i++;

    }


}
void FFTWManager::FFT_ToCenter_Inverse(int rows, int cols)
{
    int index = 0, sign = 1;
    register int i=0, j=0;
    while(i<rows)
    {
        while(j<cols)
        {
            index = i*cols + j;
            if((i+j)%2)
            {
                sign = -1;
            }else
            {
                sign = 1;
            }
            this->realOut[index][0] = (sign) * this->realOut[index][0];
            this->realOut[index][1] = (sign) * this->realOut[index][1];
            j++;
        }
        j=0;
        i++;

    }


}
void FFTWManager::to_double_vector(int indexOfVector, double *OUT)
{
    /*
     *          indexOfVector:
     *          0 for in
     *          1 for out
     *          2 for realOut
     *
     */
    //double *OUT = new double[N];

    switch(indexOfVector)
    {
        case 0:
            for(int i=0; i<this->N; i++)
            {
                    OUT[i] = this->in[i][0];
            }
            break;

        case 1:
            for(int i=0; i<this->N; i++)
            {

                    OUT[i] = this->out[i][0]/N;
                    //OUT[i] = log(this->out[i][0]/N);
            }
            break;

        case 2:
        {
            int i=0;
            while(i < N)
            {
                OUT[i] = this->realOut[i][0]/N;
                i++;
            }

        }
            break;
    }

}



void FFTWManager::initialize(int N)
{
    Free();
    if(N>0)
    {
        this->N = N;
        this->in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        this->out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        this->realOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    }
}


void FFTWManager::Plan_1D()
{

    p = fftw_plan_dft_1d(this->N, this->in, this->out, FFTW_FORWARD, FFTW_ESTIMATE);
    pInv = fftw_plan_dft_1d(this->N, this->out, this->realOut, FFTW_BACKWARD, FFTW_ESTIMATE);
    isPlan=true;
}
void FFTWManager::Plan_2D(int rows, int cols)
{

    p = fftw_plan_dft_2d(rows,cols, this->in, this->out, FFTW_FORWARD, FFTW_ESTIMATE);
    pInv = fftw_plan_dft_2d(rows,cols, this->out, this->realOut, FFTW_BACKWARD, FFTW_ESTIMATE);
    isPlan=true;
}

void FFTWManager::Execute()
{
    fftw_execute(p);
}
void FFTWManager::Execute_Inverse()
{
    fftw_execute(pInv);
}
void FFTWManager::Free()
{
    if(isPlan==true)
    {
        fftw_destroy_plan(p);
        fftw_destroy_plan(pInv);
        fftw_free(in);
        fftw_free(out);
        fftw_free(realOut);
    }
    isPlan=false;

}

void FFTWManager::InitializeAndExecuteFromAVector(double* vec, int rows, int cols, int is2D)
{
    this->N = rows*cols;
    this->initialize(N);
    if(is2D == 1)
        this->Plan_2D(rows,cols);
    else
        this->Plan_1D();

    for(int i=0; i<N ; i++)
    {
        this->in[i][0]  = vec[i];
        this->in[i][1]  = 0;
    }
    this->FFT_ToCenter(rows, cols);
    this->Execute();

}
