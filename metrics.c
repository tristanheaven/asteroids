#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fftw3.h>

#define LINE_MAX 1000

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
    time_spacing = (mag[line_count -2][0] - mag[0][0]) / (line_count + 1);
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

    mag_max = 0;
    mag_min = 100;
    for (line = 0; line < LINE_MAX; line++){
        if(mag[line][0]>mag_max){
            mag_max = mag[line][0];
        }
        if(mag[line][0]<mag_min){
            mag_min = mag[line][0];
        }

        printf("%lf\n", f_out[line]);
    }

    mag_diff = mag_max-mag_min;

    //printf("%f %f %f\n", mag_max, mag_min, mag_diff);

    return 0;
}

// vim: set et sw=4 sts=4 ts=4 :
