/*
 * main.cpp
 *
 *  Created on: 15 окт. 2020 г.
 *      Author: User
 *      https://github.com/Alexers1501/Draw-graphic.git
 */

/*
 * построить график функции sinc(sqrt(x*x + y*y))
 *
 * 1. придумать формулу для перевода координат с масштабными коэффициентами по всем трем осям
 * задать шаги осям х и у
 * 2. построить график функии перебирая одну кординату с большим шагом а втторую с маленьким
 * 3. построить график еще раз поменяв местами шаги по осям
 * 4. для сокрытия невидимых линий графика реализовать алгоритм плавающего горизонта
 * 		график всегда рисуется от ближних объектов к дальним
 * 		берем максимальный у и перебираем х от максимального к минимальному
 * 		если очередная тока находтися ниже линии горизонта, то ее не рисуем
 * 		если очередная точка находится выше то:
 * 				точку рисуем
 * 				поднимаем в этой колонке изображения горихонт до уровня новой точки
 * 		перед второым проходом для рисоваетки горизонт опускаем
 *
 * 	Область построения: -30 х 30 и -30 у 30
 * 	при рисовании следить за диапозоном
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint>
#include <vector>

//структура заголовка tga-файла
#pragma pack(push, 1)
struct TGA_Header{
	uint8_t id_len;			//длина индетификатора
	uint8_t pal_type;		//тип палитры
	uint8_t img_type;		//тип изображения
	uint8_t pal_desc[5];	//описание палитры
	uint16_t x_pos;			//положение по оси х
	uint16_t y_pos;			//положение по оси у
	uint16_t width;			//ширина
	uint16_t height;		//высота
	uint8_t depth;			//глубина цвета
	uint8_t img_desc;		//описатель изображения
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
	return sinc(hypot(x, y));// так можно hypot - гипотенуза треугольника
}


int main(){

	//построить график функции в некотором буфере

	std::vector<uint32_t> picture(IMG_WIDTH * IMG_HEIGHT);
	for (auto && p : picture)
		p = COL_BACKGROUND;

	//КОД

	//записать построенное изображение в файл
	TGA_Header hdr{};
	hdr.width = IMG_WIDTH;
	hdr.height = IMG_HEIGHT;
	hdr.depth = 32;
	hdr.img_type = 2;
	hdr.img_desc = 0x20;


	//создаем поток+открываем файл
	std::ofstream tga_file {"output.tga", std::ios::binary};
	//записываем заголовок и данне картинки
	tga_file.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));
	tga_file.write(reinterpret_cast<char*>(&picture[0]),
			picture.size() * sizeof(uint32_t));
	//закрываем файл
	tga_file.close();



	return 0;
}

