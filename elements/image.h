#ifndef _IMAGE_H
#define _IMAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv2/highgui/highgui.hpp"
#include "util.h"
#include "svector.h"
#include "fftw_manager.h"
#include "data_sem.h"

using namespace std;
namespace elements
{
    enum data_type_conversion{DOUBLE_DATA_2_DOUBLE_DATA, DOUBLE_DATA_2_UCHAR_DATA, UCHAR_DATA_2_UCHAR_DATA, NOTHING};
    class Image {
    public:
        Image();
        Image(const Image& orig);

        virtual ~Image();
        void initialize(int rows, int cols, int video);
        int get_rows();
        int get_cols();

        static double f(double t);

        void rgb_to_xyz(Image *XYZ);
        void xyz_to_rgb(Image *XYZ);
        void rgb_to_hsi(Image* HSI);
        static void xyz_to_lab(Image *XYZ, Image *LAB);
        static void lab_to_xyz(Image *LAB, Image *XYZ);
        static void lab_to_lch(Image *LAB, Image *lch);   //Lightness, Chroma and Hue
        static void xyz_to_lms(Image *XYZ, Image *LMS);
        static void lms_to_xyz(Image *LMS, Image *XYZ);
        //********************************************
        void create_custom_image(double r, double g, double b);

        //************** Useful Algorithms
        void transformation_tp();
        void transformation_tp_inverse();
        void to_log();
        void to_exp();
        //SVector Histogram(Component comp, int jump);
        SVector histogram_vector(Component comp, int jump, bool is_double);
        //Image  ImageOfHistogram(Component comp);
        void to_grayscale();
        void to_color(int color);
        void to_mosaic(int parts, long long int counter);
        void to_double_vector(int is_double, double *vector);
        void double_vector_to_image(double *vector, int channel_index,int is_double);
        Image convolution(Data *g);
        void  convolution(Data *g, int channel, bool isCutted);
        void normalize(bool is_double);
        void denormalize(bool is_double);
        void remove_zeros(bool is_double);
        void full_range_adjust(double rgb_min, double rgb_max, data_type_conversion data_type_conv,double *vec_max, double *vec_min, bool is_available);
        void copy_to_image(Image *out, data_type_conversion data_type_conv);
        int get_threshold_by_otsu(Component comp);
        int *get_multi_threshold_by_otsu(Component component, int numberOfThresholds);
        void threshold_umbralization(int threshold);
        void multi_threshold_umbralization(const int *thresholds, int nThresholds);
        //*****************************************

        //************** Color Constancy Algorithms
        void white_patch_retinex();
        //Image gray_world();
        void white_patch_retinex_reloaded(double p, int jump, bool doWPR);
        void gray_world(int jump);
        void white_patch_retinex_v();
        double * variant_of_horn(int assumption, bool is_available, double *last_vec);
        void homomorphic_filter(int type, FFTWManager *fftw_f, Image *XYZ, Image *LAB, double *vector_image, double *filter);
        void grayscale_filter_by_fft(FFTWManager *fftw_f, FFTWManager *fftw_g, double *vector_image);
        void local_average_color_by_fft(FFTWManager *fftw_f, FFTWManager *fftw_g, double *vector_image, Image* Z, bool is_available);
        void scaling_input_values_with_image(Image *scale_image, double factor);
        void color_shifts(int type, Image *a);
        //*****************************************


        Data rgb;
        int Type; //1: Some Source,  0:Normal Vector with One Channel, -1: Normal Vector with Three Channels

        double Xn,Yn,Zn,fx,fy,fz;  // Xn, Yn, Zn are used as triestimule and white reference depending application
    private:
        int rows;
        int cols;


    };
}
#endif