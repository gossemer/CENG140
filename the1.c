#include <stdio.h>
#include <math.h>

int main()
{
    double delta_t, el_time, boat_v, boat_vx;
    int i,n;

    double pos = 0;
    double active_current;

    scanf("%lf %lf\n",&delta_t,&boat_v);
    scanf("%d\n",&n);

    double dist_curr[n+1][2];
    double a_coef[n+1];
    for(i=0;i<n+1;i++)
    {
        scanf("%lf %lf\n",&dist_curr[i][0],&dist_curr[i][1]);
    }
    

    for(i = 1; i<n+1; i++)
    {
        a_coef[i] =-a_coef[i-1]+2*(dist_curr[i][1]-dist_curr[i-1][1])/(dist_curr[i][0]-dist_curr[i-1][0]);
    }

    while(pos< dist_curr[n][0])
    {
        for(i = 0; i<n+1;i++)
        {
            if(pos == dist_curr[i][0])
            {
                active_current = dist_curr[i][1];
                break;
            }
            else if(pos > dist_curr[i][0] && pos < dist_curr[i+1][0])
            {
                double var = pos-dist_curr[i][0];

                active_current = dist_curr[i][1]+a_coef[i]*(var)+(a_coef[i+1]-a_coef[i])*pow(var,2)/(2*(dist_curr[i+1][0]-dist_curr[i][0]));
                break;
            }
            else
            {
                continue;
            }
        }
        boat_vx = sqrt(pow(boat_v,2)-pow(active_current,2));
        el_time+= delta_t;
        pos += delta_t*boat_vx;
    }
    printf("%f",el_time);
    return 0;
}