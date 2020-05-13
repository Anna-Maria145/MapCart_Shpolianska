#pragma once

struct PixelList		// структура, кот. описывает элемент списка св€занной области
{
	Itype x, y;					// координаты пиксел€
	Itype color;				// цвет пиксел€
};


class ColorMap
{
public:
	ifstream file;						// файл карты
	PixelList ** Plist;					// список элементов св€занной области (все области в одном списке)

	Itype PlCount, PlCurs;				// PlCount - кол-во элементов в Plist, PlCurs - текущий элементов в Plist
	Itype RCount;						// количество св€занных областей
	Itype Xsize, Ysize, ncol;			// Map size Xsize x Ysize, ncol количество цветов на карте
	Itype **Map;						//  арта
	Itype *P_Reg;						// P_Reg[i] указывает на начало cв€занные области в Plist
	Itype K_Reg;						// количество cв€занных областей в P_Reg


private:
	void PixelAdd(Itype, Itype, Itype, Itype);			// добавление пиксел€ в список Plist
	void PixelCheck(Itype, Itype, Itype, Itype);		// проверка соседних пикселей
	Dtype Distance2(Itype, Itype, Itype, Itype);		// определ€ет рассто€ние между двум€ пиксел€ми

public:
	void CreatePlist(void);									// создать из карты списки св€зных областей
	void RegionSize(void);									// количество пикселей одного цвета
	Itype  IsLine(Itype, Dtype *);							// €вл€етс€ ли св€занна€ область отрезком
	Itype  IsRectangle(Itype, Dtype *, Dtype *, Itype *);	// €вл€етс€ ли св€занна€ область пр€моугольником
	Itype  IsEllips(Itype, Itype *);						// €вл€етс€ ли св€занна€ область эллипсом

public:
	ColorMap(char *);					// char * - им€ файла с картой
	ColorMap();
	~ColorMap();
};
