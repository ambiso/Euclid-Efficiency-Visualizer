#include <iostream>
#include "lodepng.cpp"
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "HSVtoRGB.cpp"

using namespace std;

int gcd(int a, int b)
{
	//NEWER MOD ALG
    int c = 0;
    while(a%b != 0)
    {
        int h = a%b;
        a = b;
        b = h;
        c++;
    }
    return c;
    /*OLDER ALGORITHM
    int c = 0;
    if(a == 0)
		return 0;
	else
	{
		while(b != 0)
		{
			if(a > b)
				a -= b;
			else
				b -= a;
			c++;
		}
	}
	return c;*/
}
void encode(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
  unsigned error = lodepng::encode(filename, image, width, height);
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

int main(int argc, char* argv[])
{
    static int max = 0;
    int deg[2] = {-360,160};
    switch(argc)
    {
        case 1:
            cout << "Max: ";
            cin >> max;
            break;
        case 4:
            deg[0] = atoi(argv[2]);
            deg[1] = atoi(argv[3]);
        case 2:
            max = atoi(argv[1]);
            break;

        default:
            cout << "Argument Mismatch.";
            return 1;
    }
    int * steps = new int[max*max];
    for(int i = 1; i < max; i++)
    {
        for(int j = 1; j < max; j++)
        {
            steps[i+max*j] = gcd(i,j);
        }
    }

    int maxsteps = 0;
    for(int i = 0; i < max*max; i++)
        if(*(steps+i) > maxsteps)
            maxsteps = steps[i];

    static const unsigned int width = max;

    vector<unsigned char> img;
    img.resize(width*width*4);
    hsv px;
    px.s = 1;
    px.v = 1;
    rgb conv;
    for(unsigned y = 0; y < width; y++)
        for(unsigned x = 0; x < width; x++)
        {
            px.h = deg[0]+(double)steps[x+y*max]/(double)maxsteps*deg[1];
            conv = hsv2rgb(px);
            img[4 * width * y + 4 * x + 0] = conv.r * 255;
            img[4 * width * y + 4 * x + 1] = conv.g * 255;
            img[4 * width * y + 4 * x + 2] = conv.b * 255;
            img[4 * width * y + 4 * x + 3] = 255;
        }

    delete steps;
    encode("out.png", img, width, width);

    return 0;
}
