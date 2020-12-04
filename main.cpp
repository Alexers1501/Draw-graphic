/*
 * main.cpp
 *
 *  Created on: 15 ���. 2020 �.
 *      Author: Alexers1501
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
#include <chrono>

static constexpr auto Pi = acos (-1.);
constexpr  auto step_big = 1.0, step_small = 0.01;
constexpr  auto scale = 20.0; // -30 �� 30 �� ���� ����
constexpr auto koef = 4.0;	// ����������� ���������������
constexpr auto fov = Pi / 6.0;


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
	if (x == 0.0)
		return 1.0;
	return sin(x) / x;
}

double my_function(double x, double y){
	return sinc(hypot(x, y));// ��� ����� hypot - ���������� ������������
}


int main(){

	//��������� ������ ������� � ��������� ������
	//���
	//�������� ����������� ����������� � ����
	TGA_Header hdr{};
	hdr.width = IMG_WIDTH;
	hdr.height = IMG_HEIGHT;
	hdr.depth = 32;
	hdr.img_type = 2;
	hdr.img_desc = 0x38;	// ���� �������� �������
	//���������� �������
	std::vector<uint32_t> picture(IMG_WIDTH * IMG_HEIGHT);
	for (auto && p : picture)
		p = COL_BACKGROUND;

	uint32_t col = COL_FOREGROUND;

	std::vector<int> max_point(IMG_WIDTH);
	for(auto && value : max_point)
		value = IMG_HEIGHT;

constexpr	double ResX = 30.0;	// ���������� �� X
constexpr	double ResY = 30.0;	// ���������� �� Y
constexpr	double ResZ = 300.0;	// ���������� �� Z
	int size = IMG_WIDTH * IMG_HEIGHT;


	for (double x = scale; x > -scale; x -= step_big) {
		for (double y = scale; y > -scale; y -= step_small) {
			double z = (my_function(x, y));
			int x1 = int( IMG_WIDTH / 2 - ResX * x * cos( fov ) + ResY * y * cos( fov ) );
			int y1 = int( IMG_HEIGHT / 2 + ResX * x * sin( fov ) + ResY * y * sin( fov ) - ResZ * z);
			if(0 < x1 && x1 < IMG_WIDTH 			// ��������, ����� �� ������� �� ������
					&& y1 * IMG_WIDTH + x1 <= size
					&& max_point[x1] > y1){			// ����������� ���������
				max_point[x1] = y1;
				if( 0 < y1 && y1 <= IMG_HEIGHT )	// �������� ������ �� ������
					picture[y1 * IMG_WIDTH + x1] = col;
			}
		}
	}
	for(auto && value : max_point)
		value = IMG_HEIGHT;
	for (double y = scale; y > -scale; y -= step_big) {
		for (double x = scale; x > -scale; x -= step_small) {
			double z = my_function(double(x), double(y));
			int x1 = int( IMG_WIDTH / 2 - ResX * x * cos( fov ) + ResY * y * cos( fov ) );
			int y1 = int( IMG_HEIGHT / 2 + ResX * x * sin( fov ) + ResY * y * sin( fov ) - ResZ * z);
			if(0 < x1 && x1 < IMG_WIDTH 			// ��������, ����� �� ������� �� ������
					&& y1 * IMG_WIDTH + x1 <= size
					&& max_point[x1] > y1){			// ����������� ���������
				max_point[x1] = y1;
				if( 0 < y1 && y1 <= IMG_HEIGHT )	// �������� ������ �� ������
					picture[y1 * IMG_WIDTH + x1] = col;
			}
		}
	}




	//������� �����+��������� ����
	std::ofstream tga_file {"output.tga", std::ios::binary};
	//���������� ��������� � ����� ��������
	tga_file.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));
	tga_file.write(reinterpret_cast<char*>(&picture[0]),
			picture.size() * koef);
	//��������� ����
	tga_file.close();



	return 0;
}
