# include "Common.h"
# include "MapCart.h"

void CreateMap(char *, Itype, Itype, Itype);

int main()
{
	ColorMap *CM;

	ofstream fileres;
	bool fo;
	char * FPath;
	char filename[256], file_res[256];

	Itype row, col, checked;
	Itype i, k, KL, rP, rSq, kr;
	Itype l_col, kl, m_col, i_col, kp, ke;
	Itype El[4];
	Dtype Len, P, Sq, maxP, maxSq, maxLen, S_ellips;

	//ifstream file;

	Itype Xsize = 100, Ysize = 100;					// размеры карты
	Itype ncol = 10;								// количество цветов на карте

	
	FPath = _getcwd(NULL, 0);						// путь на директорию
	strcpy_s(filename, FPath);
	strcpy_s(file_res, FPath);
	free(FPath);

	strcat_s(filename, "\\MapCart_test_map.dat");		// имя файла карты
	strcat_s(file_res, "\\MapCart_test_res.txt");		// имя файла результата
			
	CreateMap(filename, Xsize, Ysize, ncol);

	CM = new ColorMap(filename);						// создаем объект класса ColorMap

	KL = 0;

	for (row = 0; row < Ysize; row++)
	{
		for (col = 0; col < Xsize; col++)
			if (CM->Map[row][col] != 0) KL++;		// количество закрашенных пикселей на карте
	}

	CM->Plist = new struct PixelList *[KL + 1];		// заказ памяти на массив структур пикселей для построения связных областей

	if (CM->Plist == nullptr)
	{
		cout << "Plist can't be allocated." << endl;
		exit(-1);
	}

	for (i = 0; i < KL; i++)
	{
		CM->Plist[i] = new struct PixelList;
		if (CM->Plist[i] == nullptr)
		{
			cout << "Plist["<< i<<"] can't be allocated." << endl;
			exit(-1);
		}
	}


	CM->P_Reg = new Itype[KL + 1];						// начала связных регионов в Plist

	if (CM->P_Reg == nullptr)
	{
		cout << "P_Reg can't be allocated." << endl;
		exit(-1);
	}

	CM->CreatePlist();				//создаем списки связанных областей

	cout << endl;
	CM->RegionSize();				//определяем размеры связанных областей


	fileres.open(file_res, ios::out | ios::in | ios_base::trunc);			// создать файл для записи результатов
	fo = fileres.is_open();
	if (!fo)
	{
		cout << "Test_CreateMap - Unable to open file" << file_res << endl;
		exit(-1);
	}

	for (row = 0; row < Ysize; row++)
	{
		for (col = 0; col < Xsize; col++)			// запись карты в файл
			fileres << CM->Map[row][col];
		fileres << endl;
	}

	// установка начальных значений для определения характеристик

	// отрезков
	maxLen = 30.0;		
	l_col = 2;				// цвет отрезка
	kl = 0;					// кол-во отрезков L>maxLen & color=l_col

	// прямоугольников
	maxP = 0.0;				// периметр
	maxSq = 0.0;			// площадь
	kr = 0;					// кол-во
	rP = -1;
	rSq = -1;

	// эллипсов
	ke = 0;					// кол-во
	S_ellips = 25.0;		// площадь


	m_col = 1;				// цвет макс. области
	kp = 0;					// кол-во ее пикселей

	for (i = 0; i < CM->K_Reg; i++)					// цикл по областям
	{
		cout << endl;
		cout << "Region " << i << endl;

		fileres << endl;
		fileres << "Region " << i << endl;

		checked = 0;								// область не проверена

		i_col = CM->Plist[CM->P_Reg[i]]->color;		// цвет области

		if (i_col == m_col) kp += (CM->P_Reg[i + 1] - CM->P_Reg[i]);		// суммарное количество пикселей цвета m_col

		k = CM->IsLine(i, &Len);					// является ли область отрезком
		if (k > 0)
		{
			checked = 1;
			cout << " Is a Line. Len=" << Len << endl;
			fileres << " Is a Line. Len=" << Len << endl;
			if (Len > maxLen && i_col == l_col) kl++;

			continue;
		}

		k = CM->IsRectangle(i, &P, &Sq, El);		// является ли область прямоугольником
		if (k > 0)
		{
			checked = 2;
			kr++;
			if (P > maxP) { maxP = P; rP = i; }
			if (Sq > maxSq) { maxSq = Sq; rSq = i; }

			cout << " Is a Rectangle. P=" << P << "  Sq= " << Sq << endl;
			cout << "x0= " << El[0] << "  y0= " << El[1] << endl;
			cout << "x1= " << El[2] << "  y1= " << El[3] << endl;

			fileres << " Is a Rectangle. P=" << P << "  Sq= " << Sq << endl;
			fileres << "x0= " << El[0] << "  y0= " << El[1] << endl;
			fileres << "x1= " << El[2] << "  y1= " << El[3] << endl;

			continue;
		}

		k = CM->IsEllips(i, El);					// является ли область эллипсом
		if (k > 0)
		{
			checked = 3;
			if (M_PI* El[2] * El[3] > S_ellips) { ke++; }

			cout << " Is an Ellips" << endl;
			cout << "x0= " << El[0] << "  y0= " << El[1] << endl;
			cout << "A= " << El[2] << "  B= " << El[3] << endl;

			fileres << " Is an Ellips" << endl;
			fileres << "x0= " << El[0] << "  y0= " << El[1] << endl;
			fileres << "A= " << El[2] << "  B= " << El[3] << endl;

			continue;
		}

		if (checked == 0)
		{
			cout << " Is not Recognized" << endl;
			fileres << " Is not Recognized" << endl;
		}
	}

	cout << endl;
	cout << "Number of Regions is " << CM->K_Reg << endl;
	cout << endl;
	cout << "Number of Rectangle is " << kr << endl;
	cout << "Region " << rP << " is Rectangle whith max P=" << maxP << endl;
	cout << "Region " << rSq << " is Rectangle whith max Sq=" << maxSq << endl;
	cout << endl;
	cout << "Number of Lines whith Len>" << maxLen << " of color " << l_col << " is " << kl << endl;
	cout << endl;
	cout << "Number of Elips whith Sq > " << S_ellips << " is " << ke << endl;
	cout << endl;
	cout << "Number of Pixels of color " << m_col << " is " << kp << endl;


	fileres << endl;
	fileres << "Number of Regions is " << CM->K_Reg << endl;
	fileres << endl;
	fileres << "Number of Rectangle is " << kr << endl;
	fileres << "Region " << rP << " is Rectangle whith max P=" << maxP << endl;
	fileres << "Region " << rSq << " is Rectangle whith max Sq=" << maxSq << endl;
	fileres << endl;
	fileres << "Number of Lines whith Len>" << maxLen << " of color " << l_col << " is " << kl << endl;
	fileres << endl;
	fileres << "Number of Elips whith Sq > " << S_ellips << " is " << ke << endl;
	fileres << endl;
	fileres << "Number of Pixels of color " << m_col << " is " << kp << endl;

	for (i = 0; i < KL; i++)	delete CM->Plist[i];
		
	delete CM->Plist;
	delete CM->P_Reg;

	fileres.close();

}

