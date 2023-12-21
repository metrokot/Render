#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>

bool textureMode = true;
bool lightMode = true;

Vector3 GetNormal(double* start, double* end1, double* end2)
{
	Vector3 a = Vector3(start[0] - end1[0], start[1] - end1[1], start[2] - end1[2]);
	Vector3 b = Vector3(start[0] - end2[0], start[1] - end2[1], start[2] - end2[2]);
	Vector3 normal = Vector3(a.Y() * b.Z() - b.Y() * a.Z(), -a.X() * b.Z() + b.X() * a.Z(), a.X() * b.Y() - a.Y() * b.X());
	return normal;
}

void calcNormal(double A[], double B[], double C[], double* normal) {
	double vectorAB[3] = { B[0] - A[0], B[1] - A[1], B[2] - A[2] };
	double vectorAC[3] = { C[0] - A[0], C[1] - A[1], C[2] - A[2] };

	normal[0] = vectorAB[1] * vectorAC[2] - vectorAB[2] * vectorAC[1];
	normal[1] = vectorAB[2] * vectorAC[0] - vectorAB[0] * vectorAC[2];
	normal[2] = vectorAB[0] * vectorAC[1] - vectorAB[1] * vectorAC[0];
}

class Prisma {
	GLuint texId;

	
	double A[2]{ 0,10 };
	double B[2]{ 5,9 };
	double C[2]{ 7, 14 };
	double D[2]{ 7,9 };
	double E[2]{ 13,6 };
	double F[2]{ 11,1 };
	double G[2]{ 1,0 };
	double H[2]{ 4,8 };
	std::vector<double*> nach{ G,H,A,B,C,D,E,F };
	int ExtraTask[2]{ 0,0 };
	double ExtraVognDot[2];
	std::vector<double*> krug;
	std::vector<double*> krug2;
	double* Add(double* DDot, double z) {
		double* DDDot;
		DDDot = new double[3] { DDot[0],DDot[1], z };
		return DDDot;
	}
	double kvadrouravX(double y, int vpravo) {
		double x1 = (24. + std::sqrt(576. - 4. * ((-7.25) + 144 + y * y - 7 * y + 12.25))) / 2.;
		double x2 = (24. - std::sqrt(576. - 4. * ((-7.25) + 144 + y * y - 7 * y + 12.25))) / 2.;
		if (y <= 2.5 * x1 - 26.5)
			if (vpravo == 1 && x1 >= 12)
				return x1;
			else if (y <= 2.5 * x2 - 26.5)
				return x2;
	}

	double circle_vpuclost(double cx, double cy, double x) {
		double y1 = (2 * cy + std::sqrt(4 * cy * cy - 4 * (x * x - 2 * cx * x - 1 + 2 * cx))) / 2.;
		double y2 = (2 * cy - std::sqrt(4 * cy * cy - 4 * (x * x - 2 * cx * x - 1 + 2 * cx))) / 2.;
		if (y1 >= 0.1 * x - 0.1)
			return y1;
		if (y2 >= 0.1 * x - 0.1)
			return y2;

	}

	void BokovieKrug(std::vector<double*> krugi, double z, int key = 1) {
		glBindTexture(GL_TEXTURE_2D, texId);
		glBegin(GL_TRIANGLES);
		{
			for (int i = 0; i < krugi.size() - 1; i++)
			{
				Vector3 normal;
				if(key)
					normal = GetNormal(Add(krugi[i], 0), Add(krugi[i], z), Add(krugi[i + 1], 0));
				else
					normal = GetNormal(Add(krugi[i + 1], 0), Add(krugi[i], z), Add(krugi[i], 0));
				glNormal3d(normal.X(), normal.Y(), normal.Z());
				glVertex3dv(Add(krugi[i], 0));
				glVertex3dv(Add(krugi[i + 1], 0));
				glVertex3dv(Add(krugi[i + 1], z));
				glVertex3dv(Add(krugi[i], 0));
				glVertex3dv(Add(krugi[i], z));
				glVertex3dv(Add(krugi[i + 1], z));
				
			}
		}
		glEnd();
	}
	void normalpoverh(double z) {
		if (z != 0.0)
			glNormal3d(0, 0, 1);
		else
			glNormal3d(0, 0, -1);
	}
	void staticpoverh(double z) {
		glBindTexture(GL_TEXTURE_2D, texId);
		glBegin(GL_TRIANGLES);
		{
			normalpoverh(z);
			for (double* i : { A,H,B,B,D,C,H,D,B,F,D,H,F,E,D }) {
				glVertex3dv(Add(i, z));
			}
		}
		glEnd();
	}
	void imgVipucl(double z) {
		if (ExtraTask[0] == 1) {
			double Midl[2]{ 12.,3.5 };
			glBindTexture(GL_TEXTURE_2D, texId);
			glBegin(GL_TRIANGLE_FAN);
			{
				normalpoverh(z);
				glVertex3dv(Add(Midl, z));
				double Last[2]{ 11.,1. };
				for (double y = 3.5 - std::sqrt(7.25); y <= 1; y += 0.01) {
					double* dot;
					dot = new double[2] { kvadrouravX(y, 0), y };
					if (z == 0.0)
						krug.push_back(dot);//
					glVertex3dv(Add(dot, z));
				}
				krug.push_back(Last);
				glVertex3dv(Add(Last, z));
			}
			glEnd();
			std::reverse(krug.begin(), krug.end());

			glBindTexture(GL_TEXTURE_2D, texId);
			glBegin(GL_TRIANGLE_FAN);
			{
				normalpoverh(z);
				glVertex3dv(Add(Midl, z));
				for (double y = 3.5 - std::sqrt(7.25); y <= 6; y += 0.01) {

					double* dot;
					dot = new double[2] { kvadrouravX(y, 1), y };
					if (z == 0.0)
						krug.push_back(dot);
					glVertex3dv(Add(dot, z));

				}
				double Last[3]{ 13,6,z };
				krug.push_back(Last);
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
			glBindTexture(GL_TEXTURE_2D, texId);
			glBegin(GL_TRIANGLE_FAN);
			{
				normalpoverh(z);
				krug2.push_back(H);
				glVertex3dv(Add(H, z));
				for (double x = 1.0; x <= 11; x += 0.1) {
					double* dot;
					dot = new double[2] { x, circle_vpuclost(cx, cy, x) };
					if (z == 0.0)
						krug2.push_back(dot);
					glVertex3dv(Add(dot, z));
				}
			}
			glEnd();

		}
		else {
			glBindTexture(GL_TEXTURE_2D, texId);
			glBegin(GL_TRIANGLES);
			{
				for (double* i : { G,F,H }) {
					normalpoverh(z);
				}
			}
			glEnd();
		}

	}
public:
	void poverhnost(double height) {
		for (double z : {0.0, height})
		{
			glColor3d(1, 0, 0);
			staticpoverh(z);
			glColor3d(1, 1, 0);
			BokovieKrug(nach, z);
			glColor3d(1, 0, 0);
			imgVipucl(z);
			glColor3d(0, 1, 0);
			BokovieKrug(krug, z);
			glColor3d(1, 0, 0);
			imgVognut(z);
			glColor3d(1, 0, 1);
			BokovieKrug(krug2, z,0);

		}
	}
	void vipucl(int razresh = 1) {
		ExtraTask[0] = razresh;
	}
	void vognut(double x, double y, int razresh = 1) {
		ExtraTask[1] = razresh;
		ExtraVognDot[0] = x;
		ExtraVognDot[1] = y;
	}
};

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;


	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}


	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist * cos(fi2) * cos(fi1),
			camDist * cos(fi2) * sin(fi1),
			camDist * sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}


	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);


		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale * 0.08;
		s.Show();

		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale * 1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL* ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01 * dx;
		camera.fi2 += -0.01 * dy;
	}


	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k * r.direction.X() + r.origin.X();
		y = k * r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02 * dy);
	}


}

void mouseWheelEvent(OpenGL* ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01 * delta;

}

void keyDownEvent(OpenGL* ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL* ogl, int key)
{

}



GLuint texId;

//выполняется перед первым рендером
void initRender(OpenGL* ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);


	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE* texarray;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char* texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);



	//генерируем ИД для текстуры
	glGenTextures(1, &texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH);


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}





void Render(OpenGL* ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//настройка материала
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//Прогать тут  


	//Начало рисования квадратика станкина

	Prisma prism;
	prism.vipucl();
	prism.vognut(6, 2);
	//prism.rotate(0.1);
	prism.poverhnost(10);
	//конец рисования квадратика станкина


   //Сообщение вверху экрана


	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
	//(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - вкл/выкл текстур" << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "G - двигать свет по горизонтали" << std::endl;
	ss << "G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Параметры камеры: R=" << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;

	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}
