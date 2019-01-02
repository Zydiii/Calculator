#include "head.h"
#include "D:\1\Graph.h"
#include <cmath>
#include <string>
#include "D:\1\GUI.h"
#include "D:\1\std_lib_facilities.h"
using namespace Graph_lib;
//抛出异常
class Bad_area { };
//放回函数，类似缓冲区
void Token_stream::putback(Token t)
{
	if (full)
		throw Bad_area{};
	buffer = t;
	full = true;
}
//获取一个Token
Token Token_stream::get(stringstream& ss)
{
	if (full)
	{
		full = false;
		return buffer;
	}
	char ch;
	ss.get(ch);
	switch (ch)//运算符、数字、不合法符号的不同处理方法
	{
		//处理运算符
	case '(':
	case ')':
	case ';':
	case 'q':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '!':
	case'A':
	case'N':
	case'S':
		return Token{ ch };
		//处理数字
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
		ss.putback(ch);
		double val;
		ss >> val;
		return Token{ number,val };
	}
	//非法输入
	default:
		throw Bad_area{};
	}
}
//忽略字符直到找到合适的字符
void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind)
	{
		full = false;
		return;
	}
	full = false;
	char ch = 0;
	while (cin >> ch)
		if (ch == c)
			return;
}
//窗口类
Calculator_window::Calculator_window(Point xy, int w, int h, const string& title) :
	Window(xy, w, h, title),
	left_button(Point(170, 90), 45, 24, "(", cb_left),
	right_button(Point(220, 90), 45, 24, ")", cb_right),
	factorial_button(Point(220, 170), 45, 45, "!", cb_factorial),
	ANS_button(Point(120, 90), 45, 24, "ANS", cb_ANS),
	seven_button(Point(10, 120), 45, 45, "7", cb_seven),
	eight_button(Point(65, 120), 45, 45, "8", cb_eight),
	nine_botton(Point(120, 120), 45, 45, "9", cb_nine),
	four_botton(Point(10, 170), 45, 45, "4", cb_four),
	five_botton(Point(65, 170), 45, 45, "5", cb_five),
	six_button(Point(120, 170), 45, 45, "6", cb_six),
	one_button(Point(10, 220), 45, 45, "1", cb_one),
	two_button(Point(65, 220), 45, 45, "2", cb_two),
	three_button(Point(120, 220), 45, 45, "3", cb_three),
	zero_button(Point(10, 270), 100, 45, "0", cb_zero),
	point_button(Point(120, 270), 45, 45, ".", cb_point),
	division_button(Point(170, 120), 45, 45, "/", cb_division),
	mod_button(Point(220, 120), 45, 45, "%", cb_mod),
	plus_button(Point(170, 170), 45, 45, "+", cb_plus),
	substraction_button(Point(170, 220), 45, 45, "-", cb_substraction),
	multi_button(Point(170, 270), 45, 45, "*", cb_multi),
	calculate_button(Point(220, 220), 45, 95, "=", cb_calculate),
	clear_button(Point(10, 90), 45, 24, "C", cb_clear),
	back_button(Point(65, 90), 45, 24, "<-", cb_back),
	value_in(Point(10, 20), 255, 25, ""),
	value_out(Point(10, 55), 255, 25, "")
{
	attach(left_button);
	attach(right_button);
	attach(factorial_button);
	attach(ANS_button);
	attach(seven_button);
	attach(eight_button);
	attach(nine_botton);
	attach(division_button);
	attach(mod_button);
	attach(four_botton);
	attach(five_botton);
	attach(six_button);
	attach(one_button);
	attach(two_button);
	attach(three_button);
	attach(zero_button);
	attach(point_button);
	attach(plus_button);
	attach(substraction_button);
	attach(multi_button);
	attach(clear_button);
	attach(back_button);
	attach(calculate_button);
	attach(value_in);
	attach(value_out);
}