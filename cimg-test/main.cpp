#include <iostream>
#include <vector>
#include "CImg.h"

using namespace cimg_library;

void sortPixels(CImg<unsigned char>* img, float avg_min, float avg_max);

float rgbAvg(int r, int g, int b);


int main(int numArgs, char* args[])
{
	try
	{
		CImg<unsigned char> image("satlumtest.bmp");

		sortPixels(&image, 0.5f, 0.8f);

		//image.save("airplane_out.bmp");
	}
	catch (const std::exception e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}


	return 0;
}

void sortPixels(CImg<unsigned char>* img, float avg_min, float avg_max)
{
	std::vector<float> colSegment;
	for (int x = 0; x < (*img).width(); x++)
	{
		for (int y = 0; y < (*img).height(); y++)
		{
			float avg = rgbAvg((*img)(x, y, 0), (*img)(x, y, 1), (*img)(x, y, 2));

			// this loop needs several states.
			// 1) first, seek a pixel within the threshold and record that first pixel's offset (x * height + y). go to state (2).
			// 2) store that pixel, and each consecutive pixel within the threshold in a vector.
			// when a pixel is found outside the threshold, stop storing pixels and stop traversing. Go to state (3).
			// 3) sort the pixels in the vector by their avg. use each pixels index in the vector plus the offset from (1) to arrange the pixels in the image.
			// return to state (1).

			if (avg <= avg_max && avg >= avg_min)
			{
				
			}
		}
	}
}

float rgbAvg(int r, int g, int b)
{
	return (r + g + b) / 3.0f;
}