#include "D:\1\Graph.h"
#include <cmath>
#include <string>
#include "D:\1\GUI.h"
#include "D:\1\std_lib_facilities.h"
#include <iostream>
using namespace Graph_lib;
const char number = '8';   //数字
const char quit = 'q';   //退出
const char print = ';';   //输出结果
#ifndef _HEAD_H_
#define _HEAD_H_
//定义Token获得输入元素信息
struct Token
{
	char kind;//类型
	double value;//值
};
//Token流
class Token_stream
{
public:
	Token get(stringstream& ss);
	void putback(Token t);
	void ignore(char c);
private:
	bool full{ false };
	Token buffer;
};
double expression(stringstream& ss, Token_stream &ts);
double primary(stringstream& ss, Token_stream &ts);
double factorial(double);
double term(stringstream& ss, Token_stream &ts);
//窗口结构
struct Calculator_window : Window
{
	Calculator_window(Point xy, int w, int h, const string& title);

private:
	Button clear_button, back_button, calculate_button, left_button, right_button, factorial_button, ANS_button, division_button, mod_button, point_button, plus_button, substraction_button, multi_button;
	Button seven_button, eight_button, nine_botton, four_botton, five_botton, six_button, one_button, two_button, three_button, zero_button;
	Out_box value_in;
	Out_box value_out;

	void cal();
	void input(char x);
	void input_();
	void clear_();
	void back_();

	static void cb_calculate(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).cal();
	}
	static void cb_zero(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('0');
	}
	static void cb_one(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('1');
	}
	static void cb_left(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('(');
	}
	static void cb_right(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input(')');
	}
	static void cb_factorial(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('!');
	}
	static void cb_ANS(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input_();
	}
	static void cb_seven(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('7');
	}
	static void cb_eight(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('8');
	}
	static void cb_nine(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('9');
	}
	static void cb_four(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('4');
	}
	static void cb_five(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('5');
	}
	static void cb_six(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('6');
	}
	static void cb_two(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('2');
	}
	static void cb_three(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('3');
	}
	static void cb_point(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('.');
	}
	static void cb_division(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('/');
	}
	static void cb_mod(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('%');
	}
	static void cb_plus(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('+');
	}
	static void cb_substraction(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('-');
	}
	static void cb_multi(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).input('*');
	}
	static void cb_clear(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).clear_();
	}
	static void cb_back(Address, Address pw)
	{
		reference_to<Calculator_window>(pw).back_();
	}
};
#endif
