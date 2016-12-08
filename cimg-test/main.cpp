#include <iostream>
#include <vector>
#include <algorithm>
#include "CImg.h"

using namespace cimg_library;
void sortPixels(CImg<unsigned char>* img, float avg_min, float avg_max);
float rgbAvg(int r, int g, int b);

enum SortState { SEEKING, SORTING };

class RGBData
{
	public:
		int r, g, b;
		float rgbAvg;

	RGBData(int p_r, int p_g, int p_b)
	{
		r = p_r;
		g = p_g;
		b = p_b;
		rgbAvg = (r + g + b) / 3.0f;
	}

	RGBData(int p_r, int p_g, int p_b, float p_rgbAvg)
	{
		r = p_r;
		g = p_g;
		b = p_b;
		rgbAvg = p_rgbAvg;
	}

};

bool compAvg(RGBData d1, RGBData d2);
void sortByAvg(std::vector<RGBData>* v);
void swapEls(std::vector<RGBData>* v, int left, int right);

int main(int numArgs, char* args[])
{
	if(numArgs != 5)
	{
		std::cout << "Usage : pxsort <bmp-in> <min-range> <max-range> <bmp-out>\n";
		return 1;
	}
	try
	{
		CImg<unsigned char> image(args[1]);

		sortPixels(&image, std::atof(args[2]), ::atof(args[3]));

		image.save(args[4]);
	}
	catch (const std::exception e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	//std::cin.get();

	return 0;
}

void sortPixels(CImg<unsigned char>* img, float avg_min, float avg_max)
{
	std::vector<RGBData> colSegment;
	int segmentStart;
	SortState sortState;
	for (int x = 0; x < (*img).width(); x++)
	{
		segmentStart = -1;
		sortState = SEEKING;
		colSegment.clear();
		for (int y = 0; y < (*img).height(); y++)
		{
			int r = (*img)(x, y, 0),
				g = (*img)(x, y, 1),
				b = (*img)(x, y, 2);
			float avg = rgbAvg(r, g, b) / 255.0f;

			switch(sortState)
			{
			case SEEKING:
				if (avg <= avg_max && avg >= avg_min && y + 1 < (*img).height())
				{
					if(segmentStart == -1)
					{
						//std::cout << "starting segment at " << y << std::endl;
						//segmentStart = x * (*img).height() + y;
							segmentStart = y;
					}

					colSegment.push_back(RGBData(r,g,b,avg));

					//std::cout << "Found pixel within range " << avg << " at " << x << ", " << y << std::endl;
					break;
				}
				else if(segmentStart == -1)
				{
					break;
				}
			case SORTING:
				// sort vector, manipulate pixels in image
				sortByAvg(&colSegment);
				for(unsigned int i = 0; i < colSegment.size(); i++)
				{
					//std::cout << colSegment[i].rgbAvg << std::endl;
					(*img)(x, segmentStart + i, 0) = colSegment[i].r;
					(*img)(x, segmentStart + i, 1) = colSegment[i].g;
					(*img)(x, segmentStart + i, 2) = colSegment[i].b;
				}
				colSegment.clear();
				sortState = SEEKING;
				segmentStart = -1;
				break;
			}
			// this loop needs several states.
			// 1) first, seek a pixel within the threshold and record that first pixel's offset (x * height + y). go to state (2).
			// 2) store that pixel, and each consecutive pixel within the threshold in a vector.
			// when a pixel is found outside the threshold, stop storing pixels and stop traversing. Go to state (3).
			// 3) sort the pixels in the vector by their avg. use each pixels index in the vector plus the offset from (1) to arrange the pixels in the image.
			// return to state (1).

			
		}
	}
}

float rgbAvg(int r, int g, int b)
{
	return (r + g + b) / 3.0f;
}

bool compAvg(RGBData d1, RGBData d2)
{
	return d1.rgbAvg < d2.rgbAvg;
}

void sortByAvg(std::vector<RGBData>* v)
{
	std::sort ((*v).begin(), (*v).end(), compAvg);
	//std::sort((*v).begin(), (*v).end(), [] (RGBData d1, RGBData d2) { return d1.rgbAvg < d2.rgbAvg; });
}

void swapEls(std::vector<RGBData>* v, int left, int right)
{
	RGBData temp = (*v)[right];
	(*v)[right] = (*v)[left];
	(*v)[left] = temp;
}
