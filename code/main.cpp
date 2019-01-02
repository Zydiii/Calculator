#include "D:\1\Graph.h"
#include <cmath>
#include <string>
#include "D:\1\GUI.h"
#include "D:\1\std_lib_facilities.h"
#include "head.h"
using namespace Graph_lib;
double ANS = 0;//�洢��ANS
int flag1 = 0;//��¼'('���ִ���
int flag2 = 0;//��¼')'���ִ���
string _in = "";//����
string s = "";//���
class Bad_area { };//�׳��쳣
Token_stream ts;//Token��
//������
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
//primary ��������,���Ÿ���,�����ŵ�,�׳�,ANS
double primary(stringstream& ss, Token_stream &ts)
{
	Token t = ts.get(ss);
	switch (t.kind)
	{
	case'A'://����ANS
	{
		Token t1 = ts.get(ss);
		Token t2 = ts.get(ss);
		Token t3 = ts.get(ss);
		if (t1.kind != 'N' || t2.kind != 'S' || t3.kind == '(' || t3.kind == 'A')
		{
			throw Bad_area{};
		}
		if (t1.kind == 'N'&& t2.kind == 'S' && t3.kind == '!')//����ANS�����ǽ׳˵����
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
		flag1++;//'('���ִ�����һ
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
	case number://��������
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
	case '-'://������
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
	case '+'://��������
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
//expression  ����+,-
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
			flag2++;//')'���ִ�����һ
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}
//term ����*,/,%
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
			if (d == 0) //��������Ϊ0
				throw Bad_area{};
			left /= d;
			break;
		}
		case '%':
		{
			double d = primary(ss, ts);
			int i1 = int(left);
			int i2 = int(d);
			if (i1 != left || i2 != d || i2 == 0)//������С��ȡģ��Ҳ����ȡ0
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
		while (t.kind == print)//���
			t = ts.get(ss);
		if (t.kind == quit)//�˳�
			return;
		ts.putback(t);
		ANS = expression(ss, ts);//��¼ANS
		if (flag1 != flag2)//���'('��')'���ִ�����һ��Ϊ�Ƿ�ʽ��
			throw Bad_area{};
		else
			os << ANS;//������
		flag1 = 0;//�������ų��ִ���
		flag2 = 0;
		break;
	}
	catch (Bad_area)
	{
		flag1 = 0;//�������ų��ִ���
		flag2 = 0;
		_in = "";
		os << "error";
		break;
	}
}
//factorial ����׳�
double factorial(double d)
{
	double num = 1;
	for (int i = 1; i <= d; i++)
		num *= i;
	return num;
}
//����
void Calculator_window::cal()
{
	ostringstream os;
	stringstream ss;
	ss.str(_in + ';');
	calculate(ss, os, ts);
	string s(os.str());
	value_out.put(s);
}
//������ʾ
void Calculator_window::input(char x)
{
	_in.push_back(x);
	value_in.put(_in);
	value_out.put(s);
}
//����ANS
void Calculator_window::input_()
{
	_in.push_back('A');
	_in.push_back('N');
	_in.push_back('S');
	value_in.put(_in);
	value_out.put(s);
}
//��պ���
void Calculator_window::clear_()
{
	_in = "";
	s = "";
	value_in.put(_in);
	value_out.put(s);
}
//�������
void Calculator_window::back_()
{
	if (_in != "")
		_in.erase(_in.end() - 1);
	value_in.put(_in);
	value_out.put(s);
}
