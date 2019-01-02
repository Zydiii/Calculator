#include "D:\1\Graph.h"
#include <cmath>
#include <string>
#include "D:\1\GUI.h"
#include "D:\1\std_lib_facilities.h"
#include "head.h"
using namespace Graph_lib;
double ANS = 0;//存储量ANS
int flag1 = 0;//记录'('出现次数
int flag2 = 0;//记录')'出现次数
string _in = "";//输入
string s = "";//输出
class Bad_area { };//抛出异常
Token_stream ts;//Token流
//主函数
int main()
{
	try
	{
		Calculator_window win(Point(0, 0), 275, 330, "Calculator");
		return gui_main();
	}
	catch (...) {
		cerr << "error\n";
		keep_window_open();
		return 2;
	}
}
//primary 处理数字,正号负号,带括号的,阶乘,ANS
double primary(stringstream& ss, Token_stream &ts)
{
	Token t = ts.get(ss);
	switch (t.kind)
	{
	case'A'://处理ANS
	{
		Token t1 = ts.get(ss);
		Token t2 = ts.get(ss);
		Token t3 = ts.get(ss);
		if (t1.kind != 'N' || t2.kind != 'S' || t3.kind == '(' || t3.kind == 'A')
		{
			throw Bad_area{};
		}
		if (t1.kind == 'N'&& t2.kind == 'S' && t3.kind == '!')//处理ANS后面是阶乘的情况
		{
			Token t4 = ts.get(ss);
			if (t4.kind == '!' || t4.kind == number)
				throw Bad_area{};
			else
			{
				ts.putback(t4);
				if (ANS == 0)
					return 1;
				else if (ANS < 0)
					throw Bad_area{};
				else
					return factorial(ANS);
			}
		}
		else
		{
			ts.putback(t3);
			return ANS;
		}
	}
	case '(':
	{
		flag1++;//'('出现次数加一
		double d = expression(ss,ts);
		t = ts.get(ss);
		if (t.kind != ')')
		{
			ts.putback(t);
			throw Bad_area{};
		}
		t = ts.get(ss);
		if (t.kind != '!')
		{
			ts.putback(t);
			return d;
		}
		else
		{
			Token t1 = ts.get(ss);
			if (t1.kind == '!' || t1.kind == number)
				throw Bad_area{};
			else
			{
				ts.putback(t1);
				if (d<0 || floor(d) != d)
					throw Bad_area{};
				else if (d == 0)
					return 1;
				else
					return factorial(d);
			}
		}
	}
	case number://处理数字
	{
		double d = t.value;
		Token t1 = ts.get(ss);
		if (t1.kind == '(' || t1.kind == 'A')
			throw Bad_area{};
		if (t1.kind != '!')
		{
			ts.putback(t1);
			return d;
		}
		else
		{
			Token t2 = ts.get(ss);
			if (t2.kind == '!' || t2.kind == number)
				throw Bad_area{};
			else
			{
				ts.putback(t2);
				if (d == 0)
					return 1;
				else if (floor(d) != d)
					throw Bad_area{};
				else if (d<0)
					return -factorial(-d);
				else
					return factorial(d);
			}
		}
	}
	case '-'://处理负号
	{
		double d = primary(ss,ts);
		t = ts.get(ss);
		if (t.kind != '!')
		{
			ts.putback(t);
			return -d;
		}
		else
		{
			Token t1 = ts.get(ss);
			if (t1.kind == '!' || t1.kind == number)
				throw Bad_area{};
			else
			{
				ts.putback(t1);
				if (d == 0)
					return 1;
				else if (floor(d) != d)
					throw Bad_area{};
				else if (d<0)
					return -factorial(-d);
				else
					return factorial(d);
			}
		}
	}
	case '+'://处理正号
	{
		double d = primary(ss,ts);
		t = ts.get(ss);
		if (t.kind != '!')
		{
			ts.putback(t);
			return d;
		}
		else
		{
			Token t1 = ts.get(ss);
			if (t1.kind == '!' || t1.kind == number)
				throw Bad_area{};
			else
			{
				ts.putback(t1);
				if (d == 0)
					return 1;
				else if (floor(d) != d)
					throw Bad_area{};
				else if (d<0)
					return -factorial(-d);
				else
					return factorial(d);
			}
		}
	}
	default:
		ts.putback(t);
		throw Bad_area{};
	}
}
//expression  处理+,-
double expression(stringstream& ss, Token_stream &ts)
{
	double left = term(ss, ts);
	while (true)
	{
		Token t = ts.get(ss);
		switch (t.kind)
		{
		case '+':
		{
			left += term(ss, ts);
			break;
		}
		case '-':
		{
			left -= term(ss, ts);
			break;
		}
		case ')':
		{
			flag2++;//')'出现次数加一
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}
//term 处理*,/,%
double term(stringstream& ss, Token_stream &ts)
{
	double left = primary(ss, ts);
	while (true)
	{
		Token t = ts.get(ss);
		switch (t.kind)
		{
		case '*':
			left *= primary(ss, ts);
			break;
		case '/':
		{
			double d = primary(ss, ts);
			if (d == 0) //除数不能为0
				throw Bad_area{};
			left /= d;
			break;
		}
		case '%':
		{
			double d = primary(ss, ts);
			int i1 = int(left);
			int i2 = int(d);
			if (i1 != left || i2 != d || i2 == 0)//不能是小数取模，也不能取0
				throw Bad_area{};
			left = i1 % i2;
			break;
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}
//calculate
void calculate(stringstream& ss, ostringstream& os, Token_stream& ts)
{
	while (ss)
		try
	{
		Token t = ts.get(ss);
		while (t.kind == print)//输出
			t = ts.get(ss);
		if (t.kind == quit)//退出
			return;
		ts.putback(t);
		ANS = expression(ss, ts);//记录ANS
		if (flag1 != flag2)//如果'('、')'出现次数不一样为非法式子
			throw Bad_area{};
		else
			os << ANS;//输出结果
		flag1 = 0;//重置括号出现次数
		flag2 = 0;
		break;
	}
	catch (Bad_area)
	{
		flag1 = 0;//重置括号出现次数
		flag2 = 0;
		_in = "";
		os << "error";
		break;
	}
}
//factorial 处理阶乘
double factorial(double d)
{
	double num = 1;
	for (int i = 1; i <= d; i++)
		num *= i;
	return num;
}
//计算
void Calculator_window::cal()
{
	ostringstream os;
	stringstream ss;
	ss.str(_in + ';');
	calculate(ss, os, ts);
	string s(os.str());
	value_out.put(s);
}
//输入显示
void Calculator_window::input(char x)
{
	_in.push_back(x);
	value_in.put(_in);
	value_out.put(s);
}
//输入ANS
void Calculator_window::input_()
{
	_in.push_back('A');
	_in.push_back('N');
	_in.push_back('S');
	value_in.put(_in);
	value_out.put(s);
}
//清空函数
void Calculator_window::clear_()
{
	_in = "";
	s = "";
	value_in.put(_in);
	value_out.put(s);
}
//清除函数
void Calculator_window::back_()
{
	if (_in != "")
		_in.erase(_in.end() - 1);
	value_in.put(_in);
	value_out.put(s);
}
