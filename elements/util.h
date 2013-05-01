#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include "data_sem.h"

typedef unsigned long int ulint;
using namespace std;
namespace elements
{
class Util {
public:
    Util();
    Util(const Util& orig);
    virtual ~Util();

    static double max(double *vec, int size);
    static double max(double A, double B);
    static ulint max(ulint A, ulint B);
    static int max(int A, int B);
    static double abs(double number);
    static void create_filter_sigmoid(double A, double phase, double slope, double offset_x, double offset_y, int rows, int cols, double *matrix);
    static double *create_filter_bandpass(int filter_type, double gain ,int radius, int rows, int cols);
    static double *create_filter(int filter_type, double gama, int rows, int cols);
    static double *create_filter(int filter_type, int rows, int cols);
    static void create_filter_data(Data *matrix, int filter_type, double gama);


    static void format_a2(const char *string);
    static void format_a3(const char *string);
    static void format_a4(const char *string);

    static int get_option_enum( char *string);
    static int get_algorithm_enum( char *string);
    static int get_demo_algorithm_enum( char *string);
    static char *get_algorithm_name(int algorithm);

    static void str_to_upper(char* string);
private:

};
}
#endif