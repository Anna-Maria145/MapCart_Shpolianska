#include "Common.h"
#include "MapCart.h"


//
// создать из карты список связных областей
//
void ColorMap::CreatePlist()
{
	Itype row, col;
	Itype color, num;

	num = 0;
	PlCount = 0;			// количество пикселей в списке Plist
	

	K_Reg = 0;
	P_Reg[K_Reg] = PlCount;
	for (row = 0; row < Ysize; row++)		
	{
		for (col = 0; col < Xsize; col++)
		{
			if (Map[row][col] != 0)				// поиск закрашенного пикселя на карте
			{
				color = Map[row][col];			// запомнили цвет
				num = PlCount;
			
				PixelAdd(color, num, row, col);

				PlCount++;						// количество пикселей в списке Plist
				K_Reg ++;						// кол-во областей
				P_Reg[K_Reg] = PlCount;			// начало следующей области в Plist
			}
		}
	}

	for (num = 0; num < P_Reg[K_Reg]; num++)		// восстанавливаем карту
	{
		row = Plist[num]->y;
		col = Plist[num]->x;
		Map[row][col] = Plist[num]->color;
	}

	return;
}

//
// добавление пикселя в Plist
//
void ColorMap::PixelAdd(Itype color, Itype num, Itype row, Itype col)
{
		
		Map[row][col] = 0;
		// заносим пиксель в список
		Plist[PlCount]->x = col;
		Plist[PlCount]->y = row;
		Plist[PlCount]->color = color;

		PixelCheck(color, num, row, col);			// проверка соседних пикселей

		return;
}

//
// проверка принадлежности соседних пикселей к связанной области
//
void ColorMap::PixelCheck (Itype color, Itype num, Itype row, Itype col)
{
	Itype rb, cb, re, ce;
	Itype i, j;

	cb = Plist[num]->x;	// начало координат области проверки
	rb = Plist[num]->y;

	ce = Plist[num]->x;	// конец координат области проверки
	re = Plist[num]->y;

	if (cb > 0) cb--;		// если координаты текущей точки не [i][0]
	if (rb > 0) rb--;		// если координаты текущей точки не [0][i]

	if (ce < Xsize-1) ce++;	// если координаты текущей точки не [i][Xsize-1]
	if (re < Ysize-1) re++; // если координаты текущей точки не [Ysize-1][i]
	
	
	for (i = rb; i <= re; i++)
	{
		for (j = cb; j <= ce; j++)
		{
			if (Map[i][j] == color)			// если пиксель на карте нашего цвета
			{
				PlCount++;						// 
				num= PlCount;
				PixelAdd(color, num, i, j);
			}
		}
	}

	return;
}

//
// количество пикселей одного цвета
//

void ColorMap::RegionSize()
{
	Itype i, j, k, rmax, cmax;
	Itype *RColor;

	RColor = new Itype[ncol+1];
	memset(RColor, 0, ncol * sizeof(Itype));

	rmax = 0;
	cmax = 0;

	for (i = 1; i <= K_Reg; i++)
	{
		k = P_Reg[i] - P_Reg[i - 1];	// размер области
		j = P_Reg[i - 1];				// первый пиксель области

		RColor[Plist[j]->color] += k;	// количество пикселей определенного цвета

		if (k > rmax)					// область максимального размера
		{
			rmax = k;
			cmax = Plist[j]->color;		// цвет первого пиксеся области
		}
	}

	for (i = 1; i <= ncol; i++)
	{
		if (RColor[i] > 0)
			cout << "*  " << RColor[i] << " Pixels of Color: " << i << endl;
	}
	
	cout << "Max Region size  " << rmax << " Pixels of Color: " << cmax << endl;

	delete RColor;

	return;
}


//
// расстояние между двумя пикселями
//
Dtype ColorMap::Distance2(Itype x1, Itype y1, Itype x2, Itype y2)
{
	Itype dx, dy;

	Dtype d;

	dx = x1 - x2;
	dy = y1 - y2;
	d = sqrt((Dtype)(dx*dx + dy*dy));
	return d;
}

//
// является ли связанная область rnum отрезком
// да -  возвращает  1, Len - длина линии
// нет - возвращает  <0, Len = 0
//
Itype ColorMap::IsLine(Itype rnum, Dtype *Len)
{
	char si;
	Itype i, n, k;
	Itype x0,x1, y0,y1, d1, d2;
	Itype kmin, kmax, xmin, xmax, ymin, ymax, in, ik;

	Dtype a, b;
	

	n = P_Reg[rnum];		// начало области rnum в Plist
	k = P_Reg[rnum+1];		// начало области rnum+1 в Plist

	if ( k - n  < 2)		// в области меньше двух пикселей - не отрезок
	{
		*Len = 0.0;
		return -1;
	}

	xmin = Xsize;
	xmax = 0;
	for (i = n; i < k; i++)						// находим пиксель с x_min x_max
	{
		x1 = Plist[i]->x;
		if (x1 > xmax) { xmax = x1; ik = i;	}
		if (x1 < xmin) { xmin = x1; in = i;	}
	}

	ymin = Ysize;
	ymax = 0;
	
	if (Plist[ik]->y >= Plist[in]->y)			// проверяем наклон линии
	{
		kmax = xmax;
		kmin = xmin;
	}
	else
	{
		kmin = xmax;
		kmax = xmin;
	}
	for (i = n; i < k; i++)						// находим пиксель с y_min y_max у которых x1 == kmax или x1 == kmin
	{
		x1 = Plist[i]->x;						// 
		y1 = Plist[i]->y;
		if (x1 == kmax && y1 > ymax) { ymax = y1; ik = i; }
		if (x1 == kmin && y1 < ymin) { ymin = y1; in = i; }
	}

	// координаты наиболее удаленных друг от друга пикселей предполагаемого отрезка
	x0 = Plist[in]->x;				
	y0 = Plist[in]->y;
	x1 = Plist[ik]->x;
	y1 = Plist[ik]->y;

	d1 = y1 - y0;
	d2 = x1 - x0;
	
	*Len = Distance2(x0, y0, x1, y1);			// расстояние между пикселями

	if (d2 == 0)								// вертикальная прямая
	{
		x0 = Plist[n]->x;						// координата начального пикселя
		for (i = n+1; i < k; i++)				// для каждого пикселя проверяем
		{
			x1 = Plist[i]->x;
				
			if (x0 != x1 )						// пикселm не лежит на вертикальной прямой
			{
				*Len = 0.0;
				return -2;
			}
		}

		cout << "x= " << x0 << endl;			// уравнение прямой
		return 1;
	}

	if (d1 == 0)								// горизонтальная прямая
	{
		y0 = Plist[n]->y;						// координата начального пикселя
		for (i = n+1; i < k; i++)				// для каждого пикселя проверяем
		{
			y1 = Plist[i]->y;
		
			if ( y0 != y1 )						// пиксель не лежит на горизонтальной прямой
			{
				*Len = 0.0;
				return -3;
			}
				
		}

		cout << "y= " << y0 << endl;			// уравнение прямой
		return 1;
	}

	a = (Dtype)d1 / (Dtype)d2;					// наклонная прямая
	b = (Dtype)y1 - a * (Dtype)x1;				// коэф. уравнения прямой

	for (i = n; i < k; i++)
	{
		y1 = Plist[i]->y;
		x1 = Plist[i]->x;

		d1 = (Itype) abs(y1 - round((Dtype)x1*a + b));		// разница между "пиксельной" и "аналитической" координатой

		if (d1 > 1)
		{
			*Len = 0.0;
			return -4;
		}
	}

	if (b <= 0.0) si = ' ';
	else          si = '+';

	cout << "y= " << a << "x ";					// уравнение прямой
	if ( b!= 0) cout << si << b;
	cout << endl;

	return 1;

}

//
// является ли связанная область rnum прямоугольником // сторонам Map
// да -  возвращает  1, P - периметр, Sq - площадь
// нет - возвращает  <0,  P=0, Sq=0 
//
Itype ColorMap::IsRectangle(Itype rnum, Dtype *P, Dtype *Sq, Itype *XY)
{
	Itype i, m, n, k;
	Itype x0, x1, y0, y1, d1, d2;
	Itype kmin, kmax, xmin, xmax, ymin, ymax, in, ik;


	n = P_Reg[rnum];
	k = P_Reg[rnum + 1];

	if (k - n < 4)		// в области меньше четырех пикселей - не прямоугольник
	{
		*P = 0.0;
		*Sq = 0.0;
		return -1;
	}

	xmin = Xsize;
	xmax = 0;
	ymin = Ysize;
	ymax = 0;

	for (i = n; i < k; i++)						// находим пиксели с x_min x_max
	{
		x1 = Plist[i]->x;
		if (x1 > xmax) { xmax = x1; ik = i; }
		if (x1 < xmin) { xmin = x1; in = i; }
	}

	for (i = n; i < k; i++)						// находим пиксель с y_min y_max у которых x1 == xmax или x1 == xmin
	{
		x1 = Plist[i]->x;						// 
		y1 = Plist[i]->y;
		if (x1 == xmax && y1 > ymax) { ymax = y1; ik = i; }
		if (x1 == xmin && y1 < ymin) { ymin = y1; in = i; }
	}

	kmax = 0;
	for (i = n; i < k; i++)						// проверка углов прямоугольника
	{
		x1 = Plist[i]->x;						// 
		y1 = Plist[i]->y;
		if (x1 == xmin && y1 == ymin) kmax++;		//  левый верхний
		if (x1 == xmax && y1 == ymax) kmax++;		//  правый нижний
		if (x1 == xmax && y1 == ymin) kmax++;		//  правый верхний
		if (x1 == xmin && y1 == ymax) kmax++;		//  левый нижний	
	}

	if ( kmax != 4 )		// не 4 угла
	{
		*P = 0.0;
		*Sq = 0.0;
		return -2;
	}

	d1 = ymax - ymin+1;		// высота
	d2 = xmax - xmin+1;		// ширина

	*P = (d1+d2)*2;			// периметр
	*Sq = d1*d2;			// площадь
	
	y0 = Plist[in]->y;

	if ((Itype)*Sq != k-n )				// площадь != кол-ву пикселей в области
	{
		*P = 0.0;
		*Sq = 0.0;
		return -3;
	}

	XY[0] = xmin;
	XY[1] = ymin;
	XY[2] = xmax;
	XY[3] = ymax;

	return 1;

}

//
// является ли связанная область rnum эллипсом 
// да -  возвращает  1, координаты центра и радиусы в El
// нет - возвращает  < 0
//
Itype ColorMap::IsEllips(Itype rnum, Itype *El)
{
	Itype i, m, n, k;
	Itype x0, x1, y0, y1;
	Itype xmin, xmax, ymin, ymax;

	Itype p[6];

	Dtype k1, k2;
	Dtype A, B, Alfa;

	n = P_Reg[rnum];
	k = P_Reg[rnum + 1];

	if (k - n < 4)	return -1;	// в области меньше 4 пикселей - не эллипс
	
	xmin = Xsize;
	xmax = 0;
	ymin = Ysize;
	ymax = 0;

	for (i = n; i < k; i++)						// находим пиксели с x_min x_max
	{
		x1 = Plist[i]->x;
		y1 = Plist[i]->y;
		if (x1 > xmax) { xmax = x1; p[0] = i; }
		if (x1 < xmin) { xmin = x1; p[1] = i; }
		if (y1 > ymax) { ymax = y1; p[2] = i; }
		if (y1 < ymin) { ymin = y1; p[3] = i; }
	}

	x0 = (xmax + xmin) / 2;				// центр элипса
	y0 = (ymax + ymin) / 2;

	for (i = n; i < k; i++)
	{
		if (Plist[i]->x == x0 && Plist[i]->y == y0) break;	// центр находится в связанной области
	}

	if (i == k) 	return -2;			// центр не находится в связанной области
	

	A = xmax - x0;			// радиусы элипса
	B = ymax - y0;
	
	El[0] = (Itype) x0;
	El[1] = (Itype) y0;
	El[2] = (Itype) A;
	El[3] = (Itype) B;

	// Проверяем все точки

	m = 0;
	for (i = n; i < k; i++)
	{
		x1 = Plist[i]->x;
		y1 = Plist[i]->y;
		if (x1 == x0 )
		{
			if ( abs(y1 - y0) <= B) { m++; continue; }		// пиксель принадлежит эллипсу
			else break;
		}

		if (y1 == y0)
		{
			if (abs(x1 - x0) <= A) { m++; continue; }		// пиксель принадлежит эллипсу
			else break;
		}

		Alfa = atan((Dtype)(y1 - y0) / (Dtype)(x1 - x0));	// угол наклон к оси Х

		xmax = x0+round(A * cos(Alfa));						// координаты на дуге пикселя эллипса
		ymax = y0+round(B * sin(Alfa) );

		k1 = Distance2(x0, y0, x1, y1);						// расстояние от центра до пикселя
		k2 = Distance2(x0, y0, xmax, ymax);					// расстояние от центра до дуги

		if (  k2 + 1.0 > k1 ) m++;							// пиксель принадлежит эллипсу
		else  break;
				
	}

	if (m == k - n) return 1;
	else			return -3;				// не элипс

}

// Конструкторы

ColorMap::ColorMap(char * file_name)		// create map 
{
	Itype ir[3];
	
	file.open(file_name, ios::in | ios::binary);		// открыть файл с картой
	if (!file.is_open())
	{
		cout << "Unable to open file" << file_name << endl;
		exit (- 1);
	}

	file.read((char *)ir, 3*sizeof(Itype));

	if (!file)
	{
		cout << "Error reading: " << file_name << endl;
		exit(-1);
	}

	Xsize = ir[0];				// размер карты по горизонтали
	Ysize = ir[1];				// размер карты по вертикали
	ncol = ir[0];				// максимально возможное количество цветов на карте

	Map = new Itype*[Ysize];					//  строки в массиве

	if (Map == nullptr)
	{
		cout << "Map can't be allocated." << endl;
		exit(-1);
	}

	for (Itype row = 0; row < Ysize; row++)
	{
		Map[row] = new Itype[Xsize];			//  столбцы в массиве

		if (Map[row] == nullptr)
		{
			cout << "Map[ " << row << "] can't be allocated." << endl;
			exit(-1);
		}
		memset(Map[row], 0, Xsize * sizeof(Itype));
	}

	for (Itype row = 0; row < Ysize; row++)		// читаем карту из файла
	{
		file.read((char *)Map[row], Xsize * sizeof(Itype));
		if (!file)
		{
			cout << "Error reading: " << file_name <<" row="<< row<< endl;
			exit(-1);
		}
	}

}


ColorMap::ColorMap()
{
}

ColorMap::~ColorMap()
{
	// освобождаем память на карту
	for (Itype row = 0; row < Ysize; row++)
	{
		delete Map[row];
	}

	delete Map;

	file.close();
}


