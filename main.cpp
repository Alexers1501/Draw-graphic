/*
 * main.cpp
 *
 *  Created on: 15 окт. 2020 г.
 *      Author: Alexers1501
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
#include <chrono>

static constexpr auto Pi = acos (-1.);
constexpr  auto step_big = 1.0, step_small = 0.01;
constexpr  auto scale = 20.0; // -30 до 30 по всем осям
constexpr auto koef = 4.0;	// коэффициент масштабирования
constexpr auto fov = Pi / 6.0;


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
	if (x == 0.0)
		return 1.0;
	return sin(x) / x;
}

double my_function(double x, double y){
	return sinc(hypot(x, y));// так можно hypot - гипотенуза треугольника
}


int main(){

	//построить график функции в некотором буфере
	//КОД
	//записать построенное изображение в файл
	TGA_Header hdr{};
	hdr.width = IMG_WIDTH;
	hdr.height = IMG_HEIGHT;
	hdr.depth = 32;
	hdr.img_type = 2;
	hdr.img_desc = 0x38;	// Угол поворота графика
	//построение графика
	std::vector<uint32_t> picture(IMG_WIDTH * IMG_HEIGHT);
	for (auto && p : picture)
		p = COL_BACKGROUND;

	uint32_t col = COL_FOREGROUND;

	std::vector<int> max_point(IMG_WIDTH);
	for(auto && value : max_point)
		value = IMG_HEIGHT;

constexpr	double ResX = 30.0;	// Растяжение по X
constexpr	double ResY = 30.0;	// Растяжение по Y
constexpr	double ResZ = 300.0;	// Растяжение по Z
	int size = IMG_WIDTH * IMG_HEIGHT;


	for (double x = scale; x > -scale; x -= step_big) {
		for (double y = scale; y > -scale; y -= step_small) {
			double z = (my_function(x, y));
			int x1 = int( IMG_WIDTH / 2 - ResX * x * cos( fov ) + ResY * y * cos( fov ) );
			int y1 = int( IMG_HEIGHT / 2 + ResX * x * sin( fov ) + ResY * y * sin( fov ) - ResZ * z);
			if(0 < x1 && x1 < IMG_WIDTH 			// проверка, чтобы не воходил за ширину
					&& y1 * IMG_WIDTH + x1 <= size
					&& max_point[x1] > y1){			// определение горизонта
				max_point[x1] = y1;
				if( 0 < y1 && y1 <= IMG_HEIGHT )	// проверка выхода за высоту
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
			if(0 < x1 && x1 < IMG_WIDTH 			// проверка, чтобы не воходил за ширину
					&& y1 * IMG_WIDTH + x1 <= size
					&& max_point[x1] > y1){			// определение горизонта
				max_point[x1] = y1;
				if( 0 < y1 && y1 <= IMG_HEIGHT )	// проверка выхода за высоту
					picture[y1 * IMG_WIDTH + x1] = col;
			}
		}
	}




	//создаем поток+открываем файл
	std::ofstream tga_file {"output.tga", std::ios::binary};
	//записываем заголовок и данне картинки
	tga_file.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));
	tga_file.write(reinterpret_cast<char*>(&picture[0]),
			picture.size() * koef);
	//закрываем файл
	tga_file.close();



	return 0;
}
