/*
 * main.cpp
 *
 *  Created on: 15 ���. 2020 �.
 *      Author: User
 *      https://github.com/Alexers1501/Draw-graphic.git
 */

/*
 * ��������� ������ ������� sinc(sqrt(x*x + y*y))
 *
 * 1. ��������� ������� ��� �������� ��������� � ����������� �������������� �� ���� ���� ����
 * ������ ���� ���� � � �
 * 2. ��������� ������ ������ ��������� ���� ��������� � ������� ����� � ������� � ���������
 * 3. ��������� ������ ��� ��� ������� ������� ���� �� ����
 * 4. ��� �������� ��������� ����� ������� ����������� �������� ���������� ���������
 * 		������ ������ �������� �� ������� �������� � �������
 * 		����� ������������ � � ���������� � �� ������������� � ������������
 * 		���� ��������� ���� ��������� ���� ����� ���������, �� �� �� ������
 * 		���� ��������� ����� ��������� ���� ��:
 * 				����� ������
 * 				��������� � ���� ������� ����������� �������� �� ������ ����� �����
 * 		����� ������� �������� ��� ���������� �������� ��������
 *
 * 	������� ����������: -30 � 30 � -30 � 30
 * 	��� ��������� ������� �� ����������
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint>
#include <vector>

//��������� ��������� tga-�����
#pragma pack(push, 1)
struct TGA_Header{
	uint8_t id_len;			//����� ��������������
	uint8_t pal_type;		//��� �������
	uint8_t img_type;		//��� �����������
	uint8_t pal_desc[5];	//�������� �������
	uint16_t x_pos;			//��������� �� ��� �
	uint16_t y_pos;			//��������� �� ��� �
	uint16_t width;			//������
	uint16_t height;		//������
	uint8_t depth;			//������� �����
	uint8_t img_desc;		//��������� �����������
};
#pragma pack(pop)

constexpr uint16_t IMG_WIDTH = 1280;
constexpr uint16_t IMG_HEIGHT = 720;
constexpr uint32_t COL_BACKGROUND = 0xff003f3f;
constexpr uint32_t COL_FOREGROUND = 0xffcfcfcf;

double sinc(double x){
	if (x == 0)
		return 1;
	return sin(x) / x;
}

double my_function(double x, double y){
	return sinc(hypot(x, y));// ��� ����� hypot - ���������� ������������
}


int main(){

	//��������� ������ ������� � ��������� ������

	std::vector<uint32_t> picture(IMG_WIDTH * IMG_HEIGHT);
	for (auto && p : picture)
		p = COL_BACKGROUND;

	//���

	//�������� ����������� ����������� � ����
	TGA_Header hdr{};
	hdr.width = IMG_WIDTH;
	hdr.height = IMG_HEIGHT;
	hdr.depth = 32;
	hdr.img_type = 2;
	hdr.img_desc = 0x20;


	//������� �����+��������� ����
	std::ofstream tga_file {"output.tga", std::ios::binary};
	//���������� ��������� � ����� ��������
	tga_file.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));
	tga_file.write(reinterpret_cast<char*>(&picture[0]),
			picture.size() * sizeof(uint32_t));
	//��������� ����
	tga_file.close();



	return 0;
}

