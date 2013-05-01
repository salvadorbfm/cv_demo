/*
      ### Computer Vision Demo ###

    - github.com/salvadorbfm/cv_demo
    - Author: Salvador Elizarrarás Montenegro
    - Licensed under the MIT license
*/
#include "util.h"

using namespace elements;
Util::Util() {
}

Util::Util(const Util& orig) {
}

Util::~Util() {
}

 double Util::max(double *vec, int size)
{
    double max = -99999;
    for(int i=0; i<size; i++)
    {
        if(vec[i]>max)
        {
            max = vec[i];
        }
    }
    return max;
}

 double Util::max(double A, double B)
 {
     //double eps = 0.00001;
     if((A)>B )
         return A;
     else
         return B;
 }
 ulint Util::max(ulint A, ulint B)
  {
      //double eps = 0.00001;
      if((A)>B )
          return A;
      else
          return B;
  }
 int Util::max(int A, int B)
 {
     if(A>B)
         return A;
     else
         return B;
 }

 double Util::abs(double number)
 {
     if(number < 0)
     {
         return -1.0*number;
     }else
         return number;


 }

 void Util::create_filter_sigmoid(double A, double phase, double slope, double offset_x, double offset_y, int rows, int cols, double *matrix)
 {
    int Cx = 0, Cy = 0;

    if(matrix==NULL)
    {
        //////cout << endl <<"Error al asignar memoria en matrix"<<endl;
        return ;
    }

    Cx = cols/2;
    Cy = rows/2;
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            matrix[i*cols + j] = 0.0;
        }
    }

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            double radio= sqrt((i-Cy)*(i-Cy) + (j-Cx)*(j-Cx));
            double expPart = exp(-slope*(radio-offset_x));
            matrix[i*cols + j] = (A)/(1.0+ phase * expPart) + offset_y;

        }

    }



 }

 double *Util::create_filter_bandpass(int filter_type, double gain ,int radius, int rows, int cols)
 {
    int Cx = 0, Cy = 0;
    double *matrix = NULL;
    matrix = new double[rows*cols];
    if(matrix==NULL)
    {
        //////cout << endl <<"Error al asignar memoria en matrix"<<endl;
        return NULL;
    }

    Cx = cols/2;
    Cy = rows/2;
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            matrix[i*cols + j] = 0.0;
        }
    }
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            double radio= sqrt((i-Cy)*(i-Cy) + (j-Cx)*(j-Cx));
            if(filter_type == 0)
            {
                if(radio<=radius)
                    matrix[i*cols + j] = gain;
                else
                    matrix[i*cols + j] = 0.0;

            }else
            {
                if(radio<=radius)
                    matrix[i*cols + j] = 0.0;
                else
                    matrix[i*cols + j] = gain;
            }


            //Evaluando en sigmoide
            //matrix[i][j] = (1.5)/(1.0+2.0*exp(-0.17*radio));
            //out << matrix[i][j] << " ";
            //matrix[i*cols + j] = (A)/(1.0+phase*exp(-slope*(radio-offset_x))) + offset_y;
        }
        //out << endl;
    }

//    if(matrix!=NULL)
//    delete matrix;

     return matrix;
 }
  double *Util::create_filter(int filter_type, int rows, int cols)
 {
      /*
       *  filter_type:
       *      0: LAP1
       *      1: LAP2
       *      2: LAP3
       *      3: LAP4
       *      4: SOB_W
       *      5: SOB_N
       *      6: SOB_NW
       *      7: PRE_W
       *      8: PRE_N
       *      9: PRE_WN
       * */
      int cx = 0, cy = 0;
      double *matrix = new double[rows*cols];
      double sum=0.0, value=0.0;
      cx = cols/2;
      cy = rows/2;

      if(filter_type == 0)
      {
           for(int i=0; i<rows; i++)
          {
              for(int j=0; j<cols; j++)
              {

                  sum = (i-cy)*(i-cy) + (j-cx)*(j-cx);
                  value = exp(-(sum/(2*10*10)));  //((1)/(2*3.141598*gama*gama))*
                  matrix[i*cols+j] = value;
                  ////printf(" %.0f",value);
              }
              //////cout << endl;
          }
  //        //////cout << " ";
      }

      return matrix;
 }
 double *Util::create_filter(int filter_type, double gama, int rows, int cols)
 {
    /*
     *  filter_type:
     *      0: gaussian
     *      1: exponential
     *
     *
     * */
    int Cx = 0, Cy = 0, incX = 0.0, incY = 0.0;
    double sum=0.0, value=0.0;

    //rows = (int)(2*k+1);
    //cols = (int)(2*k+1);
    //int radius = (int)(2*k+1);
    double *matrix = new double[rows*cols];

    //matrix = new double[rows*cols];

    incX = 200/(cols-1);
    incY = 200/(rows-1);
    Cx = cols/2;
    Cy = rows/2;


    if(filter_type == 0)
    {
         for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                //sumR = sqrt( (i-Cy)*(i-Cy) + (j-Cx)*(j-Cx) );
//              if(sumR<k+1) //sqrt(sumR)
                sum = (i-Cy)*(i-Cy) + (j-Cx)*(j-Cx);
                value = exp(-(sum/(2*gama*gama)));  //((1)/(2*3.141598*gama*gama))*
                matrix[i*cols+j] = value;
                ////printf(" %.0f",value);
            }
            //////cout << endl;
        }
//        //////cout << " ";
    }else if(filter_type == 1)
    {
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                sum = (i-Cy)*(i-Cy) + (j-Cx)*(j-Cx);
                value = exp(-(sqrt(sum)/(gama)));  //((1)/(2*3.141598*gama*gama))*
                matrix[i*cols+j] = value;
                ////printf(" %.2f",matrix[i*cols+j]);
            }
            ////////cout << endl;
        }
    }
    return matrix;
 }


 void Util::create_filter_data(Data *matrix,int filter_type, double gama)
 {
    /*
     *  filter_type:
     *      0: gaussian
     *      1: exponential
     *      2: median 3x3
     *      3: PRE_NW
     * */
    int Cx = 0, Cy = 0, incX = 0.0, incY = 0.0;
    double sum=0.0,value=0.0;
    int k = (int)gama;
    int rows = (int)(2*k+1);
    int cols = (int)(2*k+1);
    //Data matrix = Data();
    matrix->set_size(rows,cols,0);
    //matrix = new double[rows*cols];
    incX = 200/(cols-1);
    incY = 200/(rows-1);
    Cx = cols/2;
    Cy = rows/2;

    if(filter_type == 0)
    {
         for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                sum = (i-k)*(i-k) + (j-k)*(j-k);
                value = exp(-(sum/(2*gama*gama)));  //((1)/(2*3.141598*gama*gama))*
                matrix->ptr_double_data[i*matrix->cols+j] = value;
                matrix->ptr_data = (uchar *) (matrix->ptr_orig + i*matrix->widthstep + j*sizeof(double));
 //               //printf("  %.4f",matrix.ptr_double_data[i*matrix.cols+j]);
            }
//            //////cout << endl;
        }
//        //////cout << " ";
    }else if(filter_type == 1)
    {
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                sum = (i-k)*(i-k) + (j-k)*(j-k);
                value = exp(-(sqrt(sum)/(gama)));
                matrix->ptr_double_data[i*matrix->cols+j] = value;
                matrix->ptr_data = (uchar *) (matrix->ptr_orig + i*matrix->widthstep + j*sizeof(double));
                ////////cout << " " << matrix.ptr_double_data[i*matrix.cols+j];
            }
            ////////cout << endl;
        }
    }
    else if(filter_type == 2)
    {
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {

                matrix->ptr_double_data[i*matrix->cols+j] = 1.0/9.0;
                matrix->ptr_data = (uchar *) (matrix->ptr_orig + i*matrix->widthstep + j*sizeof(double));

            }

        }
    }else if(filter_type == 3){
        matrix->ptr_double_data[0] = -2; matrix->ptr_double_data[1] = -1; matrix->ptr_double_data[2] = 0;
        matrix->ptr_double_data[3] = -1; matrix->ptr_double_data[4] =  0; matrix->ptr_double_data[5] = 1;
        matrix->ptr_double_data[6] =  0; matrix->ptr_double_data[7] =  1; matrix->ptr_double_data[8] = 2;
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {


                matrix->ptr_data = (uchar *) (matrix->ptr_orig + i*matrix->widthstep + j*sizeof(double));

            }

        }
    }


 }


 void Util::format_a2(const char* string)
 {
     cout << endl;
     cout << "------------------------------------------";
     cout << string;
     cout << endl;
     cout << "------------------------------------------";
     cout << endl;
 }

 void Util::format_a3(const char* string)
 {
     cout << endl;
     //cout << "------------------------------------------";
     cout << string;
     cout << endl;
     //cout << "------------------------------------------";
     //cout << endl;
 }

 void Util::format_a4(const char* string)
 {
     cout << endl;
     cout << "\t\t" << string;
     cout << endl;
 }

 int Util::get_option_enum( char* string)
 {
     //IMAGE, VIDEO,CAMERA, IMAGES_ALGORITHMS, TEST, EXIT
     Util::str_to_upper(string);
     if(strcmp(string,"IMAGE")==0)
         return 0;
     if(strcmp(string,"AUTO")==0)
         return 1;
     else if(strcmp(string,"VIDEO")==0)
         return 2;
     else if(strcmp(string,"CAMERA")==0)
         return 3;
     else if(strcmp(string,"IMAGES_ALGORITHMS")==0)
         return 4;
     else if(strcmp(string,"TEST")==0)
         return 5;
     else if(strcmp(string,"DEMO")==0)
              return 6;
     else if(strcmp(string,"EXIT")==0)
         return 20;
     else
         return 6;

 }


 int Util::get_algorithm_enum( char* string)
 {
     //WPR, WPR_I, GW, HORN_W, HORN_GW, HOMOMORPHIC, HOMOMORPHIC_HSV, LOCALCOLOR_1, EXIT_ALGORITHM

     Util::str_to_upper(string);
     if(strcmp(string,"WPR")==0)
         return 0;
     else if(strcmp(string,"WPR_I")==0)
         return 1;
     else if(strcmp(string,"GW")==0)
         return 2;
     else if(strcmp(string,"HORN_W")==0)
         return 3;
     else if(strcmp(string,"HORN_GW")==0)
         return 4;
     else if(strcmp(string,"HOMOMORPHIC")==0)
         return 5;
     else if(strcmp(string,"HOMOMORPHIC_HSV")==0)
         return 6;
     else if(strcmp(string,"LOCALCOLOR_1")==0)
         return 7;
     else if(strcmp(string,"LOCALCOLOR_2") == 0)
         return 8;
     else if(strcmp(string,"EXIT_ALG")==0)
         return 20;
     else
         return 9;

 }
 int Util::get_demo_algorithm_enum( char* string)
 {
     //GRIS, ROJO, VERDE, AZUL, AMARILLO, UMBRAL, BORDES, DIFUSO, EXIT_ALG

     Util::str_to_upper(string);
     if(strcmp(string,"GRIS")==0)
         return 0;
     else if(strcmp(string,"ROJO")==0)
         return 1;
     else if(strcmp(string,"VERDE")==0)
         return 2;
     else if(strcmp(string,"AZUL")==0)
         return 3;
     else if(strcmp(string,"AMARILLO")==0)
         return 4;
     else if(strcmp(string,"COLORES")==0)
         return 5;
     else if(strcmp(string,"UMBRAL")==0)
         return 6;
     else if(strcmp(string,"BORDES")==0)
         return 7;
     else if(strcmp(string,"DIFUSO")==0)
         return 8;
     else if(strcmp(string,"INVERSO")==0)
              return 9;
    else if(strcmp(string,"EXIT_DEMO")==0)
         return 20;
     else
         return 9;

 }

 char *Util::get_algorithm_name(int algorithm){
     switch(algorithm){
     case 0:
         return "WPR";
         break;
     case 1:
         return "WPR_I";
         break;
     case 2:
         return "GW";
         break;
     case 3:
         return "HORN";
         break;
     case 4:
         return "HORN";
         break;
     case 5:
         return "HOMOMORPHIC";
         break;
     case 6:
         return "HOMOMORPHIC_HSV";
         break;
     case 7:
         return "LOCAL COLOR";
         break;
     }
     return "NA";
 }

void Util::str_to_upper(char* string)
{
    int length = strlen(string);

    for(int i=0; i<length; i++)
    {
        *string = toupper(*string);
        *string++;
    }
}