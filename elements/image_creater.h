/*
      ### Computer Vision Demo ###

    - github.com/salvadorbfm/cv_demo
    - Author: Salvador Elizarrarás Montenegro
    - Licensed under the MIT license
*/
#ifndef _IMAGE_CREATER_H
#define _IMAGE_CREATER_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv2/highgui/highgui.hpp>
#include "image.h"
#include "svector.h"

using namespace std;

namespace elements
{
    enum mask_type{AVG3x3, AVG5x5, EXP, GAUSSIAN};
    class ImageCreater {
    public:
        ImageCreater();
        //ImageCreater(const ImageCreater& orig);
        virtual ~ImageCreater();

        //static Image     OpenCVImage2Image(IplImage* img);
        static IplImage* image_to_opencv_image(Image* sImag, bool isDouble);
        static void      opencv_image_to_image(IplImage* img, Image *myImg);
        static void      create_mask(Data* matrix, mask_type maskType );
        static void      compute_images(IplImage* img, Image *myImg);
        static int       is_null(IplImage* img);
        static void       load_image_by_opencv(IplImage** img, const char * pathname);
        static IplImage* histogram_image_by_opencv(Image *myImg);
    private:

    };
}
#endif
