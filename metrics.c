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


int main (int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: %s <infile>\n", argv[0]);
		return EXIT_FAILURE;
	}

    int line_count, line, fits_line;
    double mag[line_max][2], synth_mag[line_max][2]; // day mag
    double magleg, mag_max, mag_min, mag_diff;
    struct Interp fits[1000];
    double time_spacing;


    fftw_plan plan;

	double *f_in = calloc(1000, sizeof(double));
	double *f_out = calloc(1000, sizeof(double));

    // planning fourier transform

    plan = fftw_plan_r2r_1d(1000, f_in, f_out, FFTW_R2HC, FFTW_ESTIMATE);

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
        f_in[line] = 0;
        f_out[line] = 0;
    }

    FILE *file_in = fopen(argv[1], "r");

    magleg = 0;

    for(line=0; line < line_max; line++){
        fscanf(file_in, "%lf %lf\n", &mag[line][0], &mag[line][1]);
        if(magleg == mag[line][1]){
            line_count = line+1;
			break;
        }
        magleg = mag[line][0];

        if(line<line_max-1){
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
		f_in[line] = synth_mag[line][1];
		printf("%lf:", f_in[line]);
    }


    fftw_execute(plan);


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

    printf("%f %f %f\n", mag_max, mag_min, mag_diff);

    return 0;
}
