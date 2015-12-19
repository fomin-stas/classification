#include<iostream>
#include<fstream>
#include<math.h>
using namespace std;
int main()
{
    fstream F;

    double H;
    double LogN;
    double LogR_S=0;
    double R=0;
    double min_sx_X=0;
    double max_sx_X=0;
    double sx_X=0;
    double S=0;
    double X=0;
    int i=0;
    int n=0;
    //int  n = 20;
    double a[5000];
    F.open("D:\\Program Files\\Herst\\Herst.txt");
    if (F)
    {


    while (!F.eof())
    {
        F>>a[i];
        i++;
        n++;
    }
   // double a[n];

   /* for (int i=0 ; i<n; i++)
    {
        cin>>a[i];
    }*/
   //double a[20]={1.3484,1.3398,1.3264,1.3264,1.3161,1.3174,1.3364,1.3261,1.3213,1.3177,1.3023,1.2923,1.2942,1.3000,1.3132,1.3240,1.3016,1.3134,1.3249,1.3223};
 for (int i=0 ; i<n; i++)
    {
        cout<<a[i]<<endl;

    }

      for (int i=0 ; i<n; i++)
    {
        X=X+a[i];
    }
    X=X/n;

    for (int i=0 ; i<n; i++)
    {
        S=S+pow((a[i]-X),2);
    }
    S=sqrt(S/n);

     for (int i=0 ; i<n; i++)
    {
       sx_X= sx_X+ (a[i]-X);
       if (min_sx_X > sx_X) min_sx_X= sx_X;
       if (max_sx_X< sx_X) max_sx_X = sx_X;
    }

    R=max_sx_X-min_sx_X;
    LogR_S=log10(R/S);

    LogN=log10(n*1.5707963267949);

    H=LogR_S/LogN;
 LogR_S=log10((R/S)* 0.998752+1.051037);
 H=(LogR_S/LogN)*(-0.0011*log(20)+1.0136);


cout<<endl<<"R="<<R;
cout<<endl<<"S=" <<S;
cout<<endl<<"LogR/S=" <<LogR_S;
cout<<endl<<"H=" << H;
    }
    else
    {
      cout <<"Net Faila" <<endl;
    }
    return 0;
}
