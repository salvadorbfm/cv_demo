#include <stdlib.h>
#include <iostream>
#include <math.h>
typedef unsigned char uchar;
#ifndef _DATA_SEM_H
#define _DATA_SEM_H
namespace elements
{
    enum Semsem { BLA, BLA2, BLA3};
    enum Component { GRAY=0, RED=1, GREEN=2, BLUE=3 };
    class Data {
    public:
        Data();

        virtual ~Data();

        void set_size(int rows, int cols, int type);
        void release_memory();
        inline double sum(Component comp, bool is_double)
        {
            double sum=0.0;
            int channel_index = 0;
            unsigned int N3 = rows*cols*3;
            register unsigned int index=0;
            switch(comp)
            {
                case GRAY:
                    channel_index = 1;
                    break;
                case RED:
                    channel_index = 2;
                    break;

                case GREEN:
                    channel_index = 1;
                    break;

                case BLUE:
                    channel_index = 0;
                    break;
            }
            if(is_double==true)
            {
                while(index < N3)
                {
                    double current_value = ptr_double_data[ index +channel_index];
                    sum += current_value;
                    index = index + 3;
                }
            }else
            {
                for(register int i=0; i<rows; i++)
                {
                    this->ptr_data = (uchar*)(this->ptr_orig + i* this->widthstep);
                    for(register int j=0; j<cols ; j++)
                    {
                        double current_value = (double)ptr_data[3*j+channel_index];
                        sum += current_value;
                    }
                }
            }
            return sum;

        }
        inline double get_standard_deviation(Component comp, bool is_double)
        {
            int channel_index=0;
            unsigned int N3 = rows*cols*3, N = rows*cols;
            register unsigned int index=0;
            double sum = 0.0;

            if(N<1) return 0;
            switch(comp)
            {
                case GRAY:
                    channel_index = 1;
                    break;
                case RED:
                    channel_index = 2;
                    break;

                case GREEN:
                    channel_index = 1;
                    break;

                case BLUE:
                    channel_index = 0;
                    break;
            }
            double mean = this->get_mean(comp, 0, is_double);
            if(is_double==true)
            {
                while(index < N3)
                {
                    double current_value = ptr_double_data[ index +channel_index];
                    sum += (current_value - mean)*(current_value - mean);
                    index = index + 3;
                }
            }else
            {
                for(register int i=0; i<rows; i++)
                {
                    this->ptr_data = (uchar*)(this->ptr_orig + i* this->widthstep);
                    for(register int j=0; j<cols ; j++)
                    {
                        double current_value = (double)ptr_data[3*j+channel_index];
                        sum += (current_value-mean)*(current_value-mean);
                    }
                }
            }
            return sqrt(sum/N);
        }
        inline double get_max(Component comp, bool is_double)
        {
            int channel_index=0;
            unsigned int N3 = rows*cols*3;
            register unsigned int index=0;
            double max = -99999;
            switch(comp)
            {
                case GRAY:
                    channel_index = 1;
                    break;
                case RED:
                    channel_index = 2;
                    break;
                case GREEN:
                    channel_index = 1;
                    break;
                case BLUE:
                    channel_index = 0;
                    break;
            }
            if(is_double==true)
            {
                while(index < N3)
                {
                    double current_value = ptr_double_data[ index +channel_index];
                    if(current_value>max)
                    {
                        max = current_value;
                    }
                    index = index + 3;
                }
            }else
            {
                for(register int i=0; i<rows; i++)
                {
                    this->ptr_data = (uchar*)(this->ptr_orig + i* this->widthstep);
                    for(register int j=0; j<cols ; j++)
                    {
                        double current_value = (double)ptr_data[3*j+channel_index];
                        if(current_value>max)
                        {
                            max = current_value;
                        }
                    }
                }
            }

            return max;
        }
        inline double get_min(Component comp, bool is_double)
        {
            int channel_index=0;
            unsigned int N3 = rows*cols*3;
            register unsigned int index=0;
            double min = 99999;
            switch(comp)
            {
            case GRAY:
                channel_index = 1;
                break;
            case RED:
                channel_index = 2;
                break;

            case GREEN:
                channel_index = 1;
                break;

            case BLUE:
                channel_index = 0;
                break;
            }
            if(is_double==true)
            {
                while(index < N3)
                {
                    double current_value = ptr_double_data[ index +channel_index];
                    if(current_value<min)
                    {
                        min = current_value;
                    }
                    index = index + 3;
                }
            }else
            {
                for(register int i=0; i<rows; i++)
                {
                    this->ptr_data = (uchar*)(this->ptr_orig + i* this->widthstep);
                    for(register int j=0; j<cols ; j++)
                    {
                        double current_value = (double)ptr_data[3*j+channel_index];
                        if(current_value<min)
                        {
                            min = current_value;
                        }
                    }
                }
            }

            return min;
        }

        inline double get_mean(Component comp, int jump, bool is_double)
        {
            int channel_index=0;
            register unsigned int index=0;
            unsigned int N3 = rows*cols*3;
            double mean = 0.0;
            jump++;
            switch(comp)
            {
                case GRAY:
                    channel_index = 1;
                    break;
                case RED:
                    channel_index = 2;
                    break;

                case GREEN:
                    channel_index = 1;
                    break;

                case BLUE:
                    channel_index = 0;
                    break;
            }
            if(is_double==true)
            {
                while(index < N3)
                {
                    double current_value = ptr_double_data[ index +channel_index];
                    mean += current_value;
                    index = index + 3;
                }
            }else
            {
                for(register int i=0; i<rows; i = i + jump)
                {
                    this->ptr_data = (uchar*)(this->ptr_orig + i* this->widthstep);
                    for(register int j=0; j<cols ; j = j + jump)
                    {
                        double current_value = (double)ptr_data[3*j+channel_index];
                        mean += current_value;
                    }
                }
            }

            return mean/((rows/jump)*(cols/jump));
        }
        inline double get_mean(Component comp, int jump, bool is_double, int x0, int y0, int x1, int y1)
        {
            int channel_index=0;
            register unsigned int index=0;
            double mean = 0.0;
            jump++;
            if(y0>rows || y1>rows) return -1;
            if(x0>cols || x1>cols) return -1;
            switch(comp){
                case GRAY:
                    channel_index = 1;
                    break;
                case RED:
                    channel_index = 2;
                    break;
                case GREEN:
                    channel_index = 1;
                    break;
                case BLUE:
                    channel_index = 0;
                    break;
            }

            for(register int i=y0; i<y1; i = i + jump){
                this->ptr_data = (uchar*)(this->ptr_orig + i* this->widthstep);
                for(register int j=x0; j<x1 ; j = j + jump){
                    if(is_double == true){
                        index = i*3*this->cols + 3*j;
                        double current_value = ptr_double_data[ index +channel_index];
                        mean += current_value;
                    }else{
                        double current_value = (double)ptr_data[3*j+channel_index];
                        mean += current_value;
                    }
                }
            }

            return mean/(((double)(y1-y0)/jump)*((double)(x1-x0)/jump));
        }
        inline double get_standard_deviation(Component comp, bool is_double, int x0, int y0, int x1, int y1)
        {
            int channel_index=0;
            register unsigned int index=0;
            double sum = 0.0;

            if(y0>rows || y1>rows) return -1;
            if(x0>cols || x1>cols) return -1;
            switch(comp){
            case GRAY:
                channel_index = 1;
                break;
            case RED:
                channel_index = 2;
                break;
            case GREEN:
                channel_index = 1;
                break;
            case BLUE:
                channel_index = 0;
                break;
            }
            double mean = this->get_mean(comp, 0, is_double,x0,y0, x1,y1);
            for(register int i=y0; i<y1; i++){
                this->ptr_data = (uchar*)(this->ptr_orig + i* this->widthstep);
                for(register int j=x0; j<x1 ; j++){
                    if(is_double == true){
                        index = i*3*this->cols + 3*j;
                        double current_value = ptr_double_data[ index +channel_index];
                        sum += (current_value-mean)*(current_value-mean);
                    }else{
                        double current_value = (double)ptr_data[3*j+channel_index];
                        sum += (current_value-mean)*(current_value-mean);
                    }
                }
            }

            return sqrt(sum/((double)(y1-y0)*(y1-y0)));
        }


        uchar *ptr_data;
        uchar *ptr_orig;
        double *ptr_double_data;
        int rows;
        int cols;
        int widthstep;
    private:

    };
}
#endif