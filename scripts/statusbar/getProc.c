#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *strremove(char *str, const char *sub) {
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}

int main(int argc, char** argv)
{
    long double a[4], b[4], loadavg;
    FILE *fp;
    char dump[50];
    char* path;

    if(argc > 1) path = argv[1];
    else path=strremove(argv[0], "procper");
    
    char* lastStat = strcat(path, "/status/lastStat");

    fp = fopen("/proc/stat","r");
    fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3]);
    fclose(fp);

    if (access(lastStat, F_OK) != 0) {
        fp = fopen(lastStat,"w");
        fprintf(fp, "%Lf %Lf %Lf %Lf", a[0],a[1],a[2],a[3]);
        fclose(fp);
        printf("0\n");
        return(0);
    }

    fp = fopen(lastStat,"r");
    fscanf(fp,"%Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3]);
    fclose(fp);

    loadavg = (((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]))) * 100;
    printf("%Lf\n",loadavg);

    fp = fopen(lastStat,"w");
    fprintf(fp, "%Lf %Lf %Lf %Lf", a[0],a[1],a[2],a[3]);
    fclose(fp);

    return(0);
}
