#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>

class Prisma {
	
	double A[2]{ 0,10 };
	double B[2]{ 5,9 };
	double C[2]{ 7, 14 };
	double D[2]{ 7,9 };
	double E[2]{ 13,6 };
	double F[2]{ 11,1 };
	double G[2]{ 1,0 };
	double H[2]{ 4,8 };
	std::vector<double*> nach{G,H,A,B,C,D,E,F};
	int ExtraTask[2]{ 0,0 };
	double ExtraVognDot[2];
	std::vector<double*> krug;
	std::vector<double*> krug2;
	double* Add(double* DDot, double z) {
		double DDDot[3] = { DDot[0],DDot[1], z };
		return DDDot;
	}
	double kvadrouravX(double y, int vpravo) {
		double x1 = (24. + std::sqrt(576. - 4. * ((-7.25) + 144 + y * y - 7 * y + 12.25))) / 2.;
		double x2 = (24. - std::sqrt(576. - 4. * ((-7.25) + 144 + y * y - 7 * y + 12.25))) / 2.;
		if (y <= 2.5 * x1 - 26.5)
			if (vpravo==1 && x1>=12)
				return x1;
		else if (y <= 2.5 * x2 - 26.5)
				return x2;
	}
	
	double circle_vpuclost(double cx, double cy, double x) {
		double y1 = (2*cy + std::sqrt(4*cy*cy - 4 * (x*x-2*cx*x-1+2*cx))) / 2.;
		double y2 = (2 * cy - std::sqrt(4 * cy * cy - 4 * (x * x - 2 * cx * x - 1 + 2 * cx))) / 2.;
		if (y1 >= 0.1 * x - 0.1)
			return y1;
		if (y2 >= 0.1 * x - 0.1)
			return y2;

	}
	double RotatingUgol=0;
	double* newCoordinates(double* dot) {
		double x1 = dot[0] * cos(RotatingUgol) - dot[1] * sin(RotatingUgol);
		double y1 = dot[0] * sin(RotatingUgol) + dot[1] * cos(RotatingUgol);
		double coord[2]{ x1, y1 };
		return coord;
	}
	void BokovieKrug(std::vector<double*> krugi, double z) {
		glBegin(GL_TRIANGLES);
		{
			for (int i = 0; i < krugi.size()-1; i++)
			{
				glVertex3dv(Add(krugi[i], 0));
				glVertex3dv(Add(krugi[i+1], 0));
				glVertex3dv(Add(newCoordinates(krugi[i + 1]), z));
				glVertex3dv(Add(krugi[i], 0));
				glVertex3dv(Add(newCoordinates(krugi[i]), z));
				glVertex3dv(Add(newCoordinates(krugi[i + 1]), z));
			}
		}
		glEnd();
	}
	void staticpoverh(double z) {
		glBegin(GL_TRIANGLES);
		{
			for (double* i : { A,H,B,B,D,C,H,D,B,F,D,H,F,E,D }) {
				glVertex3dv(Add(i, z));
			}
		}	
		glEnd();
	}
	void imgVipucl(double z) {
			if (ExtraTask[0] == 1) {
				double Midl[2]{ 12.,3.5};
				glBegin(GL_TRIANGLE_FAN);
				{
					glVertex3dv(Add(Midl, z));
					double Last[2]{ 11.,1. };
					for (double y = 3.5 - std::sqrt(7.25); y <= 1; y += 0.01) {
						double* dot;
						dot = new double[2] {kvadrouravX(y, 0), y};
						if (z != 0.0) 
							*dot = *newCoordinates(dot);
						else
							krug.push_back(dot);//
						glVertex3dv(Add(dot, z));
					}
					krug.push_back(Last);
					glVertex3dv(Add(Last, z));
				}
				glEnd();
				std::reverse(krug.begin(), krug.end());
				krug.pop_back();

				glBegin(GL_TRIANGLE_FAN);
				{
					glVertex3dv(Add(Midl, z));
					for (double y = 3.5 - std::sqrt(7.25); y <= 6; y += 0.1) {

						double* dot;
						dot = new double[2] {kvadrouravX(y, 1), y};
						if (z != 0.0)
							*dot = *newCoordinates(dot);
						else
							krug.push_back(dot);
						glVertex3dv(Add(dot, z));

					}
					double Last[3]{ 13,6,z };
					glVertex3dv(Last);
				}
				glEnd();
			}
	}
	void imgVognut(double z) {
		if (ExtraTask[1] == 1) {
			double ma = (ExtraVognDot[1]) / (ExtraVognDot[0] - 1.);
			double mb = (1. - ExtraVognDot[1]) / (11. - ExtraVognDot[0]);
			double cx = (ma * mb * (-1) + mb * (1 + ExtraVognDot[0]) - ma * (ExtraVognDot[0] + 11)) / (2. * (mb - ma));
			double cy = ((-1 * (cx - (1 + ExtraVognDot[0]) / 2.)) / ma) + (ExtraVognDot[1] / 2.);
			glBegin(GL_TRIANGLE_FAN);
			{
				krug2.push_back(H);
				glVertex3dv(Add(H, z));
				for (double x = 1.0; x <= 11; x += 0.1) {
					double* dot;
					dot = new double[2] { x, circle_vpuclost(cx, cy, x) };
					if (z != 0)
						*dot = *newCoordinates(dot);
					else
						krug2.push_back(dot);
					glVertex3dv(Add(dot,z));
				}
			}
			glEnd();

		}
		else {
			glBegin(GL_TRIANGLES);
			{
				for (double* i : { G,F,H }) {
					if(z==0)
						krug2.push_back(i);
					glVertex3dv(Add(i, z));
				}
			}
			glEnd();
		}
	}
public:
	void poverhnost(double height) {
		for (double z : {0.0, height})
		{
			staticpoverh(z);
			BokovieKrug(nach, z);
			imgVipucl(z);
			glColor3d(0.7, 0.2, 0.7);
			std::reverse(krug.begin(), krug.end());
			krug.pop_back();
			BokovieKrug(krug,z);
			glColor3d(0.7, 0.2, 0);
			imgVognut(z);
			BokovieKrug(krug2,z);
		}
	}
	void vipucl(int razresh=1) {
		ExtraTask[0] = razresh;
	}
	void vognut(double x, double y, int razresh = 1) {
		ExtraTask[1] = razresh;
		ExtraVognDot[0] = x;
		ExtraVognDot[1] = y;
	}
	void rotate(double alpha) {//вызов до povehnost
		RotatingUgol = alpha;
	}
};
void Render(double delta_time)
{
	Prisma prism;
	prism.vipucl();
	prism.vognut(6, 2);
	//prism.rotate(0.1);
	prism.poverhnost(10);
}

