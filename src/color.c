#include "color.h"

int dist_to_color(double dist)
{

	const int color_max   = 160;
	const int color_min   = 30;
	const double thresh_low  = 0.5;
	const double thresh_high = 2.0;
	const int color_delta = color_max - color_min;
	const double thresh_delta = thresh_high - thresh_low;


	int color;
	double fraction;
	if(dist <= thresh_low)
	{
		color = color_max;
	}
    else if(dist >= thresh_high)
    {
    	color=  color_min;
    }
    else
    {
    	fraction = (dist - thresh_low) / thresh_delta;
    	color = (int) (color_max - color_delta * fraction);
    }
    	 

    return color;
}