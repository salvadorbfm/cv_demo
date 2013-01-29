#ifndef _IMAGE_ANALYSER_H
#define _IMAGE_ANALYSER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv2/highgui/highgui.hpp"
#include "image.h"
#include "svector.h"

namespace elements
{
    class ImageAnalyser {
    public:
        ImageAnalyser();
        ImageAnalyser(const ImageAnalyser& orig);
        virtual ~ImageAnalyser();
        bool different_colors(Image *cielab_image, double positive_threshold_a, double negative_threshold_a, double positive_threshold_b,double negative_threshold_b, double p );
        // TODO: Better data structures for this
        static void compute_features_lch(Image *lch, SVector *histoBins, SVector *histo_perc,double *mean_l,double *mean_c,double *mean_h,double *stdDevL,double *std_dev_c,double *stdDevH,double *maxL,double *maxC,double *maxH,double *minL,double *minC,double *minH);
        static bool is_uniform(SVector *histo_perc, double L_Mean, double C_StdDev);
        static bool are_many_colors(SVector *histo_perc, double threshold_perc, int perc_times, int *counter);
        static bool is_specular(SVector *histo_perc, double L_Max, double L_Mean);
        static bool has_dominant_color(SVector *histo_perc, double perc_adj_threshold, double mean_l, double mean_c, double *value);
        static bool is_a_dark_image(double L_Mean);
        static bool is_exterior(double std_dev_c, double stdDevH, double mean_c);
        static int select_algorithm(bool u, bool m, bool s, bool d, bool dI, bool e);
        static bool is_uniform_by_zone(Image *lch, double *std_dev_a, double *std_dev_b, double *std_dev_c, double *std_dev_d);


    private:

    };
}
#endif