///home/dhanashree/Dhanashree/RTOS/UpdatedSample.xlsx#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int main()
{
    FILE *fpr, *fpw;
    fpr = fopen("/home/dhanashree/Dhanashree/RTOS/UpdatedSample.csv","r");
    fpw = fopen("/home/dhanashree/Dhanashree/RTOS/filteredSample.dat","w");
    char s[10];
    char ch;
    double sampleTime, sampleValue;
    //sampling frenquency = (no. of samples)/(60/75)-->(98/.8)==122.5==150
    double fs = 150;
    double fc = 30;

    //calculation of coefficients
    double pi = 3.14;
    double b0,b1,b2,a1,a2;
    double ff = fc/fs;//0.2
    double ita = 1/ tan(pi*ff);//1.377
    double q = sqrt(2);//1.4142
    b0 = 1.0 / (1.0 + (q*ita) + (ita*ita));//0.206408
    b1 = 2*b0;//0.412816
    b2= b0;//0.206408
    a1 = 2.0 * (ita*ita - 1.0) * b0;//0.370283
    a2 = -(1.0 - q*ita + ita*ita) * b0;//-0.195915

    //final caculation
    double x0,x1,x2,y1,y2,y;
    fscanf(fpr,"%s",&s);
    //fprintf(fpw,"%s", s);
    //fputc("\n",fpw);
    x1=0;
    x2=0;
    y1=0;
    y2=0;
    int n=0;

        while(ch != EOF)
        {
            fscanf(fpr,"%lf",&sampleTime);
            fprintf(fpw,"%lf",sampleTime);
            ch=fgetc(fpr);
            fputc(ch,fpw);
            fscanf(fpr,"%lf",&sampleValue);
            x0 = sampleValue;
            y = (a1*y1)+(a2*y2)+(b0*x0)+(b1*x1)+(b2*x2);
            printf("%lf",y);
            fprintf(fpw,"%lf", y);
                if(n==1)
                {
                    x2=0;
                    x1=x0;
                    y2=0;
                    y1=y;
                }
                else
                {
                    x2=x1;
                    x1=x0;
                    y2=y1;
                    y1=y;
                }
            ch=fgetc(fpr);
            fputc(ch,fpw);
            printf("\n");
            n = n+1;
        }
    fclose(fpr);
    fclose(fpw);
    //printf("b0= %lf",b0);
    //printf("b1= %lf",b1);
    //printf("b2= %lf",b2);
    //printf("a1= %lf",a1);
    //printf("a2= %lf",a2);
    return 0;
}

/*To plot: use gnuplot

set datafile separator ","
set title "ECG"
set xlabel "time"
set ylabel "data"
set xrange [0:2000]
set yrange [400:600]
plot "filteredSample.dat" with l
pause 50

cmd to plot from gnuplot file: gnuplot ECGPlot.gnuplot

*/
