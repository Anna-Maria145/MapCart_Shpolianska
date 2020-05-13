#pragma once

struct PixelList		// ���������, ���. ��������� ������� ������ ��������� �������
{
	Itype x, y;					// ���������� �������
	Itype color;				// ���� �������
};


class ColorMap
{
public:
	ifstream file;						// ���� �����
	PixelList ** Plist;					// ������ ��������� ��������� ������� (��� ������� � ����� ������)

	Itype PlCount, PlCurs;				// PlCount - ���-�� ��������� � Plist, PlCurs - ������� ��������� � Plist
	Itype RCount;						// ���������� ��������� ��������
	Itype Xsize, Ysize, ncol;			// Map size Xsize x Ysize, ncol ���������� ������ �� �����
	Itype **Map;						// �����
	Itype *P_Reg;						// P_Reg[i] ��������� �� ������ c�������� ������� � Plist
	Itype K_Reg;						// ���������� c�������� �������� � P_Reg


private:
	void PixelAdd(Itype, Itype, Itype, Itype);			// ���������� ������� � ������ Plist
	void PixelCheck(Itype, Itype, Itype, Itype);		// �������� �������� ��������
	Dtype Distance2(Itype, Itype, Itype, Itype);		// ���������� ���������� ����� ����� ���������

public:
	void CreatePlist(void);									// ������� �� ����� ������ ������� ��������
	void RegionSize(void);									// ���������� �������� ������ �����
	Itype  IsLine(Itype, Dtype *);							// �������� �� ��������� ������� ��������
	Itype  IsRectangle(Itype, Dtype *, Dtype *, Itype *);	// �������� �� ��������� ������� ���������������
	Itype  IsEllips(Itype, Itype *);						// �������� �� ��������� ������� ��������

public:
	ColorMap(char *);					// char * - ��� ����� � ������
	ColorMap();
	~ColorMap();
};
