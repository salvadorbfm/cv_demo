#include "data_sem.h"

using namespace elements;

Data::Data()
{
    this->widthstep = 0;
    this->ptr_data = NULL;
    this->ptr_orig = NULL;
    this->ptr_double_data = NULL;

}

void Data::set_size(int rows, int cols, int type)
{

            /*
                 type:
                 * 1: For Video and some Source
                 * 0: For Normal rgb Vector with one channel
                 * -1: For Normal rgb Vector with three channels
                    */
            this->rows = rows;
            this->cols = cols;
            if(this->ptr_double_data != NULL)
            {
                this->release_memory();
            }
            if(type==1)
            {
                int n = this->rows*this->cols*3;
                this->ptr_double_data = new double[n];
                for(register int i=0; i<n ;i++){
                    this->ptr_double_data[i] = 0.0;
                }
            }else if(type == 0)
            {
                int n = this->rows*this->cols;
                this->ptr_double_data = new double[n];
                this->widthstep = this->cols*sizeof(double);
                this->ptr_data = (uchar*)&(this->ptr_double_data[0]);
                this->ptr_orig = (uchar*)&(this->ptr_double_data[0]);
                for(register int i=0; i<n ;i++){
                    this->ptr_double_data[i] = 0.0;
                }
            }else if(type == -1)
            {
                int n = this->rows*this->cols*3;
                this->ptr_double_data = new double[n];
                this->widthstep = 3*this->cols*sizeof(double);
                this->ptr_data = (uchar*)&(this->ptr_double_data[0]);
                this->ptr_orig = (uchar*)&(this->ptr_double_data[0]);
                for(register int i=0; i<n ;i++){
                    this->ptr_double_data[i] = 0.0;
                }
            }

}

Data::~Data()
 {
    if(this->ptr_double_data != NULL)
       delete [] this->ptr_double_data;
}


void Data::release_memory()
{
    delete [] this->ptr_double_data;
    this->ptr_double_data = NULL;
    this->ptr_data = NULL;
    this->ptr_orig = NULL;

}