#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fftw3.h>

#define LINE_MAX 1000
#define CUT_FREQ 30
#define PEAK_NUM 5

// linear interpolation between real data points
struct Interp
{
    float x1,x2,y1,y2;
    float m, c;
};

int main (int argc, char *argv[])
{
    if (argc < 2) {
        printf("usage: %s <infile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int line_count, line, fits_line;
    double mag[LINE_MAX][2], synth_mag[LINE_MAX][2]; // day mag
    double magleg, mag_max, mag_min, mag_diff;
    double time_spacing;
    fftw_plan plan;
    struct Interp *fits = calloc(LINE_MAX, sizeof(struct Interp));
    double *f_in = calloc(LINE_MAX, sizeof(double));
    double *f_out = calloc(LINE_MAX, sizeof(double));
    double fpeak[PEAK_NUM+2][2]; // first 5 fourier peaks from high to low apm in format: freq amp 
    double mag_mean, max_mean, min_mean, mag_sum;
    double fsum, fmean, f_min, f_max fmaxmean, fminmean;
    

    // planning fourier transform

    plan = fftw_plan_r2r_1d(LINE_MAX, f_in, f_out, FFTW_R2HC, FFTW_ESTIMATE);

    // null initialization
    for(line=0;line<LINE_MAX;line++){
        mag[line][0]=0;
        mag[line][1]=0;
        synth_mag[line][0]=0;
        synth_mag[line][1]=0;
    }

    FILE *file_in = fopen(argv[1], "r");

    magleg = 0;

    for(line=0; line < LINE_MAX; line++){
        fscanf(file_in, "%lf %lf\n", &mag[line][0], &mag[line][1]);
        if(magleg == mag[line][1]){
            line_count = line - 1;
        //    printf("%d\n", line_count);
           // return 0;
            break;
        }
        magleg = mag[line][0];

        if(line<LINE_MAX-1){
            fits[line].x1 = mag[line][0];
            fits[line].x2 = mag[line+1][0];
            fits[line].y1 = mag[line][1];
            fits[line].y2 = mag[line+1][1];
            fits[line].m = (fits[line].y2-fits[line].y1)/(fits[line].x2-fits[line].x1);
            fits[line].c = fits[line].y1-fits[line].m*fits[line].x1;
        }
    }


    // setting even spacing of synthetic data
    time_spacing = (mag[line_count - 1][0] - mag[0][0]) / (line_count + 1);
    for (line = 0; line < line_count; line++) {
        synth_mag[line][0] = mag[0][0] + line * time_spacing;
    }

    // setting magnitude values of synthetic data
    for (line = 0; line < LINE_MAX; line++){
        for (fits_line = 0; fits_line < LINE_MAX; fits_line++){
            if ((synth_mag[line][0] >= mag[fits_line][0])
                && (synth_mag[line][0] < mag[fits_line+1][0]))
            {
                synth_mag[line][1] = fits[fits_line].m * synth_mag[line][0] + fits[fits_line].c;
            }
        }
        f_in[line] = synth_mag[line][1];
    }

    fftw_execute(plan);



    // initializing fourier peak initialization
    for(int peak = 1; peak < PEAK_NUM+1; peak++){
        fpeak[peak][0] = 0;
        fpeak[peak][1] = 0;
    }
    fpeak[0][0] = 1000;
    fpeak[0][1] = 1000;

    for(int peak = 1; peak < PEAK_NUM; peak++){
        for(line = 1; line < CUT_FREQ; line++){
            if((f_out[line]>fpeak[peak][1])
            &&(f_out[line]>=f_out[line-1])
            &&(f_out[line]>=f_out[line+1])
            &&(f_out[line]<fpeak[peak-1][1])){
                fpeak[peak][0] = line * time_spacing;
                fpeak[peak][1] = f_out[line];
            }
        }
    }
    
    for(int peak = 1; peak < PEAK_NUM+1; peak++){
        printf("Peak %i has frequency %lf and amplitude %lf\n", peak, fpeak[peak][0], fpeak[peak][1]);
    }
    
    
    
    mag_max = -100;
    f_max = -10000
    mag_min = 100;
    f_min = 10000
    for (line = 0; line < line_count; line++){
        if(f_out[line][1]>f_max){
            f_max = f_out[line][1];
        }
        if(f_out[line][1]<f_min){
            f_min = f_out[line][1];
        }
        
        if(mag[line][0]>mag_max){
            mag_max = mag[line][0];
        }
        if(mag[line][0]<mag_min){
            mag_min = mag[line][0];
        }
    }
    
    mag_sum = 0;
    fsum = 0;
    mag_diff = mag_max-mag_min;
    for(line = 0; line < line_count; line++){
        mag_sum = mag_sum + mag[line][1];
        fsum = fsum + f_out[line][1];
    }
    mag_mean = mag_sum / (line_count + 1);
    max_mean = mag_max - mag_mean;
    min_mean = mag_mean - mag_min;
    
    fmean = fsum / (line_count + 1);
    fmaxmean = f_max - fmean;
    fminmean = fmean - f_min;
    
    //MAGMEAN MAGMIN MAGMAX MAGDIFF MAXMEAN MINMEAN 
    printf("lf %lf %lf %lf %lf %lf",mag_mean, mag_min, mag_max, mag_diff, max_mean, min_mean);
    //F1 F2 F3 F4 F5 
    printf("%lf %lf %lf %lf %lf", fpeak[1][0], fpeak[2][0], fpeak[3][0], fpeak[4][0], fpeak[5][0]);
    //A1 A2 A3 A4 A5
    printf("%lf %lf %lf %lf %lf", fpeak[1][1], fpeak[2][1], fpeak[3][1], fpeak[4][1], fpeak[5][1]);
    //FSUM FMEAN FMIN FMAX FMAXMEAN FMINMEAN
    printf("%lf %lf %lf %lf %lf %lf", fsum, fmean, f_min, f_max, fmaxmean, fminmean);
    
    return 0;
}

// vim: set et sw=4 sts=4 ts=4 :
