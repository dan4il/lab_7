#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

void taylor(double h, double* y, double* dy)
{
    int n = (2 + h) / h;
    double x[n];
    for (int i = 0; i < n; i++)
        x[i] = i * h;

    y[0] = 1;
    dy[0] = 0;
    double d2y = 16.;
    double d3y = 4.-16.*d2y;
    for (int i = 1; i < n; i++) 
    {
        y[i] = y[i - 1] + h * dy[i - 1] + h * h * d2y / 2. + h * h * h * d3y / 6.;
        dy[i] = dy[i - 1] + h * d2y + h * h * d3y / 2.;
        d2y = sin(4*x[i])*exp(x[i])-16.*dy[i]+16*y[i];
        d3y = 4.*cos(4*x[i])*exp(x[i])+sin(4*x[i])*exp(x[i])-16*d2y+16*dy[i];
    }
}

double find_shag(double eps = 0.01)
{
    double h = 1;
    bool conv = true;
    do {
        int n = (2 + h) / h;
        double y[n], dy[n];
        taylor(h, y, dy);
        h /= 2;
        double y1[2 * n], dy1[2 * n];
        taylor(h, y1, dy1);

        double delta_y = abs(y[0] - y1[0]);
        double delta_dy = abs(dy[0] - dy1[0]);

        for (int i = 0; i < n; i++)
        {
            if (delta_y < abs(y[i] - y1[2 * i])) 
                delta_y = abs(y[i] - y1[2 * i]);
            if (delta_dy < abs(dy[i] - dy1[2 * i])) 
                delta_dy = abs(dy[i] - dy1[2 * i]);
        }
        if (conv = delta_y >= eps || delta_dy >= eps)
            conv = true;
        else
            conv = false;
    } while (conv);
    return h;
}

int main()
{
    double h = find_shag();
    int n = (2 + h) / h;
    double y[n], dy[n];
    y[0] = 1;
    dy[0] = 0;
    taylor(h, y, dy);

    ofstream fout("data.dat", ios_base::out);

    for (int i = 0; i < n; i++)
        fout << y[i] << "," << dy[i] << "," << i * h << endl;
    fout.close();
    return 0;
}
