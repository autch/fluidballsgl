
/* This is derived from fluidballs.c, xscreensaver-5.05 */

/* fluidballs, Copyright (c) 2000 by Peter Birtles <peter@bqdesign.com.au>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 * Ported to X11 and xscreensaver by jwz, 27-Feb-2002.
 *
 * http://astronomy.swin.edu.au/~pbourke/modelling/fluid/
 *
 * Some physics improvements by Steven Barker <steve@blckknght.org>
 */

#ifndef FLUIDBALLS_H
#define FLUIDBALLS_h

#include <math.h>
#include <stdlib.h>
#include "FixedReal.h"

class FluidBalls
{
private:
	static const double PI = 3.14159265358979;
	static const double WEIGHT_FACTOR = 0.1;
	static const double MAX_VELOCITY = 20;

	typedef FixedReal<int, long long int, 8> fixed;

	int count; /* number of balls */
	int xmin; /* rectangle of window, relative to root */
	int ymin;
	int xmax, ymax;

	fixed tc; /* time constant (time-warp multiplier) */
	fixed accx; /* horizontal acceleration (wind) */
	fixed accy; /* vertical acceleration (gravity) */

	fixed* vx;
	fixed* vy; /* current ball velocities */
	fixed* px;
	fixed* py; /* current ball positions */
	fixed* r; /* ball radiuses */
	fixed* m; /* ball mass, pre-calculated */

	fixed e; /* coefficient of elasticity */
	int max_radius; /* largest radius of any ball */

	static fixed hypot(fixed x, fixed y)
	{
		fixed l, l1, l2;

		x = (x < 0.0) ? -x : x;
		y = (y < 0.0) ? -y : y;
		l1 = x;
		l2 = y;

		if (x < y)
		{
			l1 = y;
			l2 = x;
		}

		l = l1 * 0.9604 + l2 * 0.3978;

		return l;
	}

	static double drand(double max)
	{
		return (double)rand() * max / (double)RAND_MAX;
	}

	template<class T, class E>
	static T abs_max(T& x, E max)
	{
		if(x < 0 && -x > max)
			return -max;
		else if(x > 0 && x > max)
			return max;
		return x;
	}

public:
	FluidBalls(int count, int width, int height)
	{
		this->count = count;

		xmin = 0;
		xmax = width;
		ymin = 0;
		ymax = height;

		max_radius = 25 / 2;
		if (max_radius < 1)
			max_radius = 1;

		adjust_count(width, height);

		m = new fixed[count];
		r = new fixed[count];
		vx = new fixed[count];
		vy = new fixed[count];
		px = new fixed[count];
		py = new fixed[count];

		init(width, height);
	}

	~FluidBalls()
	{
		delete[] m;
		delete[] r;
		delete[] vx;
		delete[] vy;
		delete[] px;
		delete[] py;
	}

	/*
	 * If the initial window size is too small to hold all these balls, make
	 * fewer of them...
	 */
	void adjust_count(int width, int height)
	{
		double r = max_radius * 0.7;
		double ball_area = (PI * r * r);
		double balls_area = count * ball_area;
		double window_area = width * height;
		window_area *= 0.75; /* don't pack it completely full */
		if (balls_area > window_area)
			count = (int) (window_area / ball_area);
	}

	void init(int width, int height)
	{
		accx = 0; // Wind
		if (accx < -1.0 || accx > 1.0)
			accx = 0;

		accy = 0.0075; // Gravity
		if (accy < -1.0 || accy > 1.0)
			accy = 0.01;

		e = 0.95; // Elasticity
		if (e < 0.2 || e > 1.0)
			e = 0.95;

		tc = 1.0; // TimeScale
		if (tc <= 0.0 || tc > 10.0)
			tc = 1.0;

		for(int i = 0; i < count; i++)
		{
			px[i] = drand(width) + xmin;
			py[i] = drand(height) + ymin;
			vx[i] = drand(0.2) - 0.1;
			vy[i] = drand(0.2) - 0.1;

			r[i] = (0.3 + drand(0.7)) * max_radius;
			m[i] = pow(r[i], 3.0) * PI * 1.3333 * WEIGHT_FACTOR;
		}
	}

	/*
	 * Implements the laws of physics: move balls to their new positions.
	 */
	void update_balls()
	{
		fixed xmins(xmin);
		fixed ymins(ymin);
		fixed xmaxs(xmax);
		fixed ymaxs(ymax);

		/* For each ball, compute the influence of every other ball. */
		for (int a = 0; a < count - 1; a++)
		{
			for (int b = a + 1; b < count; b++)
			{
				fixed pxa = px[a], pxb = px[b], pya = py[a], pyb = py[b];
				fixed ra = r[a], rb = r[b];

				fixed d = hypot(pxa - pxb, pya - pyb);

				if (d < ra + rb)
				{
					fixed dd = ra + rb - d;
					fixed cdx = (pxb - pxa) / d;
					fixed cdy = (pyb - pya) / d;

					/*
					 * Move each ball apart from the other by half the
					 * 'collision' distance.
					 */
					pxa -= dd * cdx * 0.5;
					pya -= dd * cdy * 0.5;
					pxb += dd * cdx * 0.5;
					pyb += dd * cdy * 0.5;

					px[a] = pxa;
					py[a] = pya;
					px[b] = pxb;
					py[b] = pyb;

					fixed ma = m[a], mb = m[b];
					fixed vxa = vx[a], vya = vy[a], vxb = vx[b], vyb = vy[b];
					fixed vca = vxa * cdx + vya * cdy; /* the component of each velocity */
					fixed vcb = vxb * cdx + vyb * cdy; /* along the axis of the collision */

					/* elastic collision */
					fixed dva = (vca * (ma - mb) + vcb * mb * 2.0) / (ma + mb) - vca;
					fixed dvb = (vcb * (mb - ma) + vca * ma * 2.0) / (ma + mb) - vcb;

					dva *= e; /* some energy lost to inelasticity */
					dvb *= e;

					vxa += dva * cdx;
					vya += dva * cdy;
					vxb += dvb * cdx;
					vyb += dvb * cdy;

					vx[a] = vxa;
					vy[a] = vya;
					vx[b] = vxb;
					vy[b] = vyb;
				}
			}
		}

		/*
		 * Force all balls to be on screen.
		 */
		for (int a = 0; a < count; a++)
		{
			if (px[a] <= (xmins + r[a]))
			{
				px[a] = xmins + r[a];
				vx[a] *= -e;
			}
			if (px[a] >= (xmaxs - r[a]))
			{
				px[a] = xmaxs - r[a];
				vx[a] *= -e;
			}
			if (py[a] <= (ymins + r[a]))
			{
				py[a] = ymins + r[a];
				vy[a] *= -e;
			}
			if (py[a] >= (ymaxs - r[a]))
			{
				py[a] = ymaxs - r[a];
				vy[a] *= -e;
			}
		}

		/*
		 * Apply gravity to all balls.
		 */
		for (int a = 0; a < count; a++)
		{
			vx[a] += accx * tc;
			vy[a] += accy * tc;
			// vx[a] = abs_max(vx[a], MAX_VELOCITY);
			// vy[a] = abs_max(vy[a], MAX_VELOCITY);
			px[a] += vx[a] * tc;
			py[a] += vy[a] * tc;
		}
	}

	void setAccel(double ax, double ay)
	{
		accx = ax;
		accy = ay;
	}

	int getCount() const { return count; }
	double getPX(int i) const { return px[i]; }
	double getPY(int i) const { return py[i]; }
	double getR(int i) const { return r[i]; }
};

#endif // !FLUIDBALLS_H
