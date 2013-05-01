#include "image_analyser.h"
using namespace elements;

ImageAnalyser::ImageAnalyser() {
}

ImageAnalyser::ImageAnalyser(const ImageAnalyser& orig) {
}

ImageAnalyser::~ImageAnalyser() {
}

bool ImageAnalyser::different_colors(Image *cielab_image, double positive_threshold_a, double negative_threshold_a, double positive_threshold_b,double negative_threshold_b, double p )
{
    int toRed = 0, toGreen = 0, toBlue = 0, toPurple = 0, manyColors=0;
    int rows = cielab_image->get_rows(), cols = cielab_image->get_cols();
    int amountOfPixels = p*cols * rows;
    int N3 = cols * rows * 3;
    double aValue = 0, bValue=0;
    register int index=0;

    while(index < N3)
    {
        bValue = cielab_image->rgb.ptr_double_data[index + 0];
        aValue = cielab_image->rgb.ptr_double_data[index + 1];
        index = index + 3;

        if(aValue > positive_threshold_a)
        {
            if(bValue > positive_threshold_b)
            {
                toRed++;
            }else if(bValue < negative_threshold_b)
            {
                toPurple++;
            }

        }else if(aValue < negative_threshold_a)
        {
            if(bValue > positive_threshold_b)
            {
                toGreen++;
            }else if(bValue < negative_threshold_b)
            {
                toBlue++;
            }
        }
    }
    if(toRed>amountOfPixels)
            manyColors++;
    if(toPurple>amountOfPixels)
            manyColors++;
    if(toGreen>amountOfPixels)
            manyColors++;
    if(toBlue>amountOfPixels)
            manyColors++;

    if(manyColors>2)
        return true;
    else
        return false;
}

void ImageAnalyser::compute_features_lch(Image *lch, SVector *histoBins, SVector *histo_perc,double *mean_l,double *mean_c,double *mean_h,double *stdDevL,double *std_dev_c,double *stdDevH,double *maxL,double *maxC,double *maxH,double *minL,double *minC,double *minH)
{
    unsigned int N3 = lch->rgb.rows * lch->rgb.cols * 3, N = lch->rgb.rows * lch->rgb.cols;
    register unsigned int index = 0;
    double L = 0.0, C=0.0, H=0.0;
    *mean_l = lch->rgb.get_mean(RED, 0, true);
    *mean_c = lch->rgb.get_mean(GREEN, 0, true);
    *mean_h = lch->rgb.get_mean(BLUE, 0, true);
    *stdDevL = lch->rgb.get_standard_deviation(RED, true);
    *std_dev_c = lch->rgb.get_standard_deviation(GREEN,true);
    *stdDevH = lch->rgb.get_standard_deviation(BLUE, true);
    *maxL  = lch->rgb.get_max(RED, true);
    *maxC  = lch->rgb.get_max(GREEN, true);
    *maxH  = lch->rgb.get_max(BLUE, true);
    *minL  = lch->rgb.get_min(RED, true);
    *minC  = lch->rgb.get_min(GREEN, true);
    *minH  = lch->rgb.get_min(BLUE, true);

    for(index=0; index<N3; index+=3){
        L = lch->rgb.ptr_double_data[index + 2];
        C = lch->rgb.ptr_double_data[index + 1];
        H = lch->rgb.ptr_double_data[index + 0];

        if(L >= 90)          { histoBins->datos[0]++;}
        if(L >= 80 && L < 90){ histoBins->datos[1]++;}
        if(L >= 70 && L < 80){ histoBins->datos[2]++;}
        if(L >= 60 && L < 70){ histoBins->datos[3]++;}
        if(L >= 50 && L < 60){ histoBins->datos[4]++;}
        if(L >= 40 && L < 50){ histoBins->datos[5]++;}
        if(L >= 30 && L < 40){ histoBins->datos[6]++;}
        if(L >= 20 && L < 30){ histoBins->datos[7]++;}
        if(L >= 10 && L < 20){ histoBins->datos[8]++;}
        if(L >= 0  && L < 10){ histoBins->datos[9]++;}

        if(C >= 90)          { histoBins->datos[10]++;}
        if(C >= 80 && C < 90){ histoBins->datos[11]++;}
        if(C >= 70 && C < 80){ histoBins->datos[12]++;}
        if(C >= 60 && C < 70){ histoBins->datos[13]++;}
        if(C >= 50 && C < 60){ histoBins->datos[14]++;}
        if(C >= 40 && C < 50){ histoBins->datos[15]++;}
        if(C >= 30 && C < 40){ histoBins->datos[16]++;}
        if(C >= 20 && C < 30){ histoBins->datos[17]++;}
        if(C >= 10 && C < 20){ histoBins->datos[18]++;}
        if(C >= 0  && C < 10){ histoBins->datos[19]++;}

        if(H >= 340)           { histoBins->datos[20]++;}
        if(H >= 320 && H < 340){ histoBins->datos[21]++;}
        if(H >= 300 && H < 320){ histoBins->datos[22]++;}
        if(H >= 280 && H < 300){ histoBins->datos[23]++;}
        if(H >= 260 && H < 280){ histoBins->datos[24]++;}
        if(H >= 240 && H < 260){ histoBins->datos[25]++;}
        if(H >= 220 && H < 240){ histoBins->datos[26]++;}
        if(H >= 200 && H < 220){ histoBins->datos[27]++;}
        if(H >= 180 && H < 200){ histoBins->datos[28]++;}
        if(H >= 160 && H < 180){ histoBins->datos[29]++;}
        if(H >= 140 && H < 160){ histoBins->datos[30]++;}
        if(H >= 120 && H < 140){ histoBins->datos[31]++;}
        if(H >= 100 && H < 120){ histoBins->datos[32]++;}
        if(H >=  80 && H < 100){ histoBins->datos[33]++;}
        if(H >=  60 && H <  80){ histoBins->datos[34]++;}
        if(H >=  40 && H <  60){ histoBins->datos[35]++;}
        if(H >=  20 && H <  40){ histoBins->datos[36]++;}
        if(H >=   0 && H <  20){ histoBins->datos[37]++;}

    }
    for(int i=0; i<10; i++){
        histo_perc->datos[i] = (100*histoBins->datos[i])/(N);
    }
    for(int i=10; i<20; i++){
        histo_perc->datos[i] = (100*histoBins->datos[i])/(N);
    }
    for(int i=20; i<38; i++){
        histo_perc->datos[i] = (100*histoBins->datos[i])/(N);
    }

}

bool ImageAnalyser::is_uniform(SVector *histo_perc, double L_Mean, double C_StdDev){
//  if(L_Mean <= 39 && C_StdDev<=25)
//      return true;
    double lastValue = 0.0, currentValue = 0.0;
    lastValue = histo_perc->datos[0];
    if(L_Mean<=39 && C_StdDev<=18){
        return true;
    }else if(C_StdDev<=18 && histo_perc->datos[11] < 0.01){
        for(int i=1; i<10;i++){
            currentValue = histo_perc->datos[i];
            if((lastValue + currentValue) >= 35.0 )
                return true;
            else
                lastValue = currentValue;
        }
    }

    return false;
}

bool ImageAnalyser::are_many_colors(SVector *histo_perc, double threshold_perc, int perc_times, int *counter){
    double currentValue = 0.0, lastValue =0.0, nextValue = 0.0;
    bool isCurrent=false, isLast=false, isNext = false, isLastCounted = false;
    //int counter = 0;
    lastValue = histo_perc->datos[20];

    for(int i=20; i<38; i++){
        if((i-1)>20){
            lastValue = histo_perc->datos[i-1];
        }
        currentValue = histo_perc->datos[i];
        if((i+1)<38){
            nextValue = histo_perc->datos[i+1];
        }else{
            nextValue = 0.0;
        }

        if(lastValue >= threshold_perc)    isLast = true;
        if(currentValue >= threshold_perc) isCurrent = true;
        if(nextValue >= threshold_perc)    isNext = true;

        if(isCurrent == true &&(isLast == false || isNext == false) && isLastCounted == false){
            (*counter)++;
            isLastCounted = true;
        }else{
            isLastCounted = false;
        }

        isCurrent = false;
        isLast = false;
        isNext = false;
    }
    //cout << "Counter = " << counter << endl;
    if(*counter>perc_times)
        return true;
    else
        return false;
}

bool ImageAnalyser::is_specular(SVector *histo_perc, double L_Max, double L_Mean){
    if(L_Max >= 80 && L_Mean <= 60){
        if((histo_perc->datos[0] < 2.0 ) && (histo_perc->datos[1] < 2.0) && (histo_perc->datos[2] < 2.0)){
            return true;
        }
    }
    return false;
}

bool ImageAnalyser::has_dominant_color(SVector *histo_perc, double perc_adj_threshold, double mean_l, double mean_c, double *value){
    double firstValue=0.0, secondValue=0.0, thirdValue=0.0;
    if(mean_l > 50 && mean_c<10) return false;
    firstValue = histo_perc->datos[20];
    secondValue = histo_perc->datos[21];
    for(int i=20+2; i<38; i++){
        thirdValue = histo_perc->datos[i];
        *value = firstValue+secondValue+thirdValue;
        if( (*value) > perc_adj_threshold)
            return true;

        firstValue=secondValue;
        secondValue =thirdValue;
    }
    return false;
}
bool ImageAnalyser::is_a_dark_image(double L_Mean){
    if(L_Mean <= 20)
        return true;
    else
        return false;
}
bool ImageAnalyser::is_exterior(double std_dev_c, double stdDevH, double mean_c){
    if((std_dev_c >= 4 && std_dev_c <= 7) && (stdDevH >= 50 && stdDevH < 54) && (mean_c >= 7 && mean_c <= 10)){
        return true;
    }else{
        return false;
    }
}

int ImageAnalyser::select_algorithm(bool u, bool m, bool s, bool d, bool dI, bool e)
{
    if(u == true){
        if(s == true)
            return 2;
        if(d == true && m == false && dI == false)
            return 1;
        if(dI == true)
            return 3; // Antes 1
        if(m == true)
            return 2;
        if(e == true)
            return 2;
        return 3;
    }else if(u == false){
        if(m==true)
            return 7;
        else
            return 5;
    }
    return 7;
}

bool ImageAnalyser::is_uniform_by_zone(Image *lch,double *std_dev_a, double *std_dev_b, double *std_dev_c, double *std_dev_d)
{
    int rows = lch->get_rows();
    int cols = lch->get_cols();
    int cont=0;

    *std_dev_a = lch->rgb.get_standard_deviation(RED,true, 0, 0, cols/2, rows/2);
    *std_dev_b = lch->rgb.get_standard_deviation(RED,true, cols/2, 0, cols, rows/2);
    *std_dev_c = lch->rgb.get_standard_deviation(RED,true, 0, rows/2, cols/2, rows);
    *std_dev_d = lch->rgb.get_standard_deviation(RED,true, cols/2, rows/2, cols, rows);

    if((*std_dev_a >= *std_dev_b - 3) && (*std_dev_a <= *std_dev_b + 3)){
        cont++;
        if(((*std_dev_d >= *std_dev_c - 3) && (*std_dev_d <= *std_dev_c + 3))){
            cont++;
        }
    }else if((*std_dev_a >= *std_dev_c - 3) && (*std_dev_a <= *std_dev_c + 3)){
        cont++;
        if(((*std_dev_d >= *std_dev_b - 3) && (*std_dev_d <= *std_dev_b + 3))){
            cont++;
        }
    }


    if(cont>=2)
        return true;
    else
        return false;


}
