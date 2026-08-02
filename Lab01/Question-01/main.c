#include <stdio.h>
#include <math.h>
#include <string.h>

double value(int ch,double n){
    switch(ch){
        case 0:return n*(log(n)/log(2));
        case 1:return 12*sqrt(n);
        case 2:return 1.0/n;
        case 3:return pow(n,log(n)/log(2));
        case 4:return 100*n*n+6*n;
        case 5:return pow(n,0.51);
        case 6:return n*n-324;
        case 7:return 50*sqrt(n);
        case 8:return 2*n*n*n;
        case 9:return pow(3,n);
        case 10:return pow(2,32)*n;
        case 11:return log(n)/log(2);
    }
    return 0;
}

int main(){
    char *name[12]={"n*log2(n)","12*sqrt(n)","1/n","n^(log2 n)","100n^2+6n","n^0.51","n^2-324","50*n^0.5","2n^3","3^n","(2^32)*n","log2(n)"};
    FILE *fp=fopen("growth_values.csv","w");
    int i,j,n;
    if(fp==NULL) fp=stdout;

    fprintf(fp,"n");
    for(i=0;i<12;i++) fprintf(fp,",%s",name[i]);
    fprintf(fp,"\n");

    for(n=2;n<=60;n++){
        fprintf(fp,"%d",n);
        for(i=0;i<12;i++) fprintf(fp,",%.6g",value(i,n));
        fprintf(fp,"\n");
    }
    if(fp!=stdout) fclose(fp);

    double val[12];
    char *order[12];
    for(i=0;i<12;i++){ val[i]=value(i,60); order[i]=name[i]; }

    for(i=0;i<11;i++){
        for(j=0;j<11-i;j++){
            if(val[j]>val[j+1]){
                double t=val[j]; val[j]=val[j+1]; val[j+1]=t;
                char *s=order[j]; order[j]=order[j+1]; order[j+1]=s;
            }
        }
    }

    printf("Increasing order at n = 60\n");
    for(i=0;i<12;i++) printf("%2d. %-12s %.6g\n",i+1,order[i],val[i]);
    return 0;
}
