#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fftw3.h>

#define line_max 1000

// linear interpolation between real data points
struct Interp
{
    float x1,x2,y1,y2;
    float m, c;

} interp;


int main (int argc, char *argv[]){
    char file_in_name[512], file_out_name[512];
    char in_line[512];
    int line_count, line, fits_line;
    double mag[line_max][2], synth_mag[line_max][2]; // day mag
    double magleg, mag_max, mag_min, mag_diff;
    struct Interp fits[1000];
    double time_spacing;


    fftw_plan plan;
    double f_in[1000], f_out[1000];
    // planning fourier transform
    plan = fftw_plan_dft_r2hc_1d(1000, in, out, -1, FFTW_ESTIMATE);



    // null initialization
    for(line=0;line<line_max;line++){
        fits[line].x1=0;
        fits[line].x2=0;
        fits[line].y1=0;
        fits[line].c=0;
        fits[line].m=0;
        mag[line][0]=0;
        mag[line][1]=0;
        synth_mag[line][0]=0;
        synth_mag[line][1]=0;
        f_in[line];
        f_out[line];
    }

    FILE *file_in, *file_out;
    strcpy(file_in_name,argv[1]);
    strcpy(file_out_name,argv[2]);
    file_in = fopen(file_in_name, "r");
    file_out = fopen(file_out_name, "w");

    magleg = 0;

    for(line=0; line < line_max; line++){
        fgets(in_line, 512, file_in);

        sscanf(in_line, "%lf %lf\n", &mag[line][0], &mag[line][1]);
        if(magleg == mag[line][1]){
            line_count = line+1;
            line = line_max;
        }
        magleg = mag[line][0];

        if(line<999){
            fits[line].x1 = mag[line][0];
            fits[line].x2 = mag[line+1][0];
            fits[line].y1 = mag[line][1];
            fits[line].y2 = mag[line+1][1];
            fits[line].m = (fits[line].y2-fits[line].y1)/(fits[line].x2-fits[line].x1);
            fits[line].c = fits[line].y1-fits[line].m*fits[line].x1;
        }
    }

    // setting even spacing of synthetic data
    time_spacing = (mag[line_count-1][0]-mag[0][0])/line_count;
    for(line=0;line<line_max;line++){
        synth_mag[line][0] = mag[0][0]+line*time_spacing;
    }

    // setting magnitude values of synthetic data
    for(line=0;line<line_max;line++){
        for(fits_line=0; fits_line<line_max; fits_line++){
            if((synth_mag[line][0]>=mag[fits_line][0])&&(synth_mag[line][0]<mag[fits_line+1][0])){
                synth_mag[line][1] = fits[fits_line].m * synth_mag[line][0] + fits[fits_line].c;
            }
        }
    }


    execute(plan);


    mag_max = 0;
    mag_min = 100;
    for(line=0; line < line_max; line++){
        if(mag[line][0]>mag_max){
            mag_max = mag[line][0];
        }
        if(mag[line][0]<mag_min){
            mag_min = mag[line][0];
        }
    }

    mag_diff = mag_max-mag_min;

    fprintf(file_out, "%f %f %f", mag_max, mag_min, mag_diff);



    return 0;
}
