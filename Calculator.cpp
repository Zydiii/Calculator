/*lab_1*calculator*/
/**expression:
term
expression "+" term  //addition
expression "-" term  //subtraction
***term:
factorial
term "*" factorial   //multiplication
term "/" factorial   //division
term "%" factorial   //remainder(modulo)
***primary:
number
"("expression")"   //grouping
***number:
floating-point-literal
"+"number   //positive
"-"number   //negative
print:   ;
quit:    q
*/
#include <iostream>
#include <math.h>
using namespace std;
//------------------------------------------------------------------------------------------
//函数声明
double expression();
double primary();
double factorial(double);
double term();
void clean_up_mess();
//结果简洁化
const char number = '8';   //数字
const char quit = 'q';   //退�?
const char print = ';';   //输出结果
const string prompt = ">";//输入提示
const string result = "= ";   //等于
extern double ANS = 0;//存储上一次的�?
extern int flag1 = 0;//记录'('出现次数
extern int flag2 = 0;//记录')'出现次数
//------------------------------------------------------------------------------------------
inline void error(const string& s)
{
	throw runtime_error(s);
}
//------------------------------------------------------------------------------------------
//定义Token获得输入元素信息
struct Token
{
	char kind;//类型
	double value;//�?
};
//Token�?
class Token_stream
{
public:
	Token get();
	void putback(Token t);
	void ignore(char c);
private:
	bool full{ false };
	Token buffer;
};
Token_stream ts;
//放回函数，类似缓冲区
void Token_stream::putback(Token t)
{
	if (full) 
		error("error");
	buffer = t;
	full = true;
}
//获取一个Token
Token Token_stream::get()
{
	if (full)
	{
		full = false;
		return buffer;
	}
	char ch;
	cin >> ch;
	switch (ch)//运算符、数字、不合法符号的不同处理方�?
	{
	//处理运算�?
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
		cin.putback(ch);
		double val;
		cin >> val;
		return Token{ number,val };
	}
	//非法输入
	default:
		error("error");
        return Token{};
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
//------------------------------------------------------------------------------------------
//clean up mess
void clean_up_mess()
{
	ts.ignore(print);
}
//------------------------------------------------------------------------------------------
//expression
//处理+,-
double expression()
{
	double left = term();
	while (true)
	{
		Token t = ts.get();
		switch (t.kind)
		{
		case '+':
		{
			left += term();
			break;
		}
		case '-':
		{
			left -= term();
			break;
		}
		case '(':
			error("error");
		case ')':
			flag2++;//')'出现次数加一
		default:
			ts.putback(t);
			return left;
		}
	}
}
//term
//处理*,/,%
double term()
{
	double left = primary();
	while (true)
	{
		Token t = ts.get();
		switch (t.kind)
		{
		case '*':
			left *= primary();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) //除数不能�?0
				error("error");
			left /= d;
			break;
		}
		case '%':
		{
			double d = primary();
			int i1 = int(left);
			int i2 = int(d);
			if (i1 != left || i2 != d || i2 == 0)//不能是小数取模，也不能取0
				error("error");
			left = i1 % i2;
			break;
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}
//factorial
//处理阶乘
double factorial(double d)
{
	double num = 1;
	for (int i = 1; i <= d; i++)
		num *= i;
	return num;
}
//primary
//处理数字,正号负号,带括号的,阶乘,ANS
double primary()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case'A'://处理ANS
	{
		Token t1 = ts.get();
		Token t2 = ts.get();
		Token t3 = ts.get();
		if (t1.kind != 'N' || t2.kind != 'S' || t3.kind == '(' )
		{
			error("error");
			ts.putback(t3);
		}
		if (t1.kind == 'N'&& t2.kind == 'S' && t3.kind == '!')//处理ANS后面是阶乘的情况
		{
			Token t4 = ts.get();
			if (t4.kind == '!' || t4.kind == number || t4.kind == '(' || t4.kind == ')'||t4.kind=='A'||t4.kind=='N'||t4.kind=='S')
				error("error");
			else
			{
				ts.putback(t4);
				if (ANS == 0)
					return 1;
				else if (ANS < 0)
					error("error");
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
		flag1 ++;//'('出现次数加一
		double d = expression();
		t = ts.get();
		if (t.kind != ')')
		{
			ts.putback(t);
			error("error");
		}
		t = ts.get();
		if (t.kind != '!')
		{
			ts.putback(t);
			return d;
		}
		else
		{
			Token t1 = ts.get();
			if (t1.kind == '!'||t1.kind == number||t1.kind=='('||t1.kind==')'||t1.kind=='A'||t1.kind=='N'||t1.kind=='S')
				error("error");
			else
			{
				ts.putback(t1);
				if (d<0 || floor(d) != d)
					error("error");
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
		Token t1 = ts.get();
		if (t1.kind == '(')
			error("error");
		if (t1.kind != '!')
		{
			ts.putback(t1);
			return d;
		}
		else
		{
			Token t2 = ts.get();
			if (t2.kind == '!'|| t2.kind == number||t2.kind=='('||t2.kind==')'||t2.kind=='A'||t2.kind=='N'||t2.kind=='S')
				error("error");
			else
			{
				ts.putback(t2);
				if (d == 0)
					return 1;
				else if (floor(d) != d)
					error("error");
				else if (d<0)
					return -factorial(-d);
				else
					return factorial(d);
			}
		}
	}
	case '-'://处理负号
	{
		double d = primary();
		t = ts.get();
		if (t.kind != '!')
		{
			ts.putback(t);
			return -d;
		}
		else
		{
			Token t1 = ts.get();
			if (t1.kind == '!'||t1.kind==number||t1.kind=='('||t1.kind==')'||t1.kind=='A'||t1.kind=='N'||t1.kind=='S')
				error("error");
			else
			{
				ts.putback(t1);
				if (d == 0)
					return 1;
				else if (floor(d) != d)
					error("error");
				else if (d<0)
					return -factorial(-d);
				else
					return factorial(d);
			}
		}
	}
	case '+'://处理正号
	{
		double d = primary();
		t = ts.get();
		if (t.kind != '!')
		{
			ts.putback(t);
			return d;
		}
		else
		{
			Token t1 = ts.get();
			if (t1.kind == '!'||t1.kind == number||t1.kind=='('||t1.kind==')'||t1.kind==')'||t1.kind=='A'||t1.kind=='N'||t1.kind=='S')
				error("error");
			else
			{
				ts.putback(t1);
				if (d == 0)
					return 1;
				else if (floor(d) != d)
					error("error");
				else if (d<0)
					return -factorial(-d);
				else
					return factorial(d);
			}
		}
	}
	default:
		ts.putback(t);
		error("error");
        return 0;
    }
}
//calculate
void calculate()
{
	while (cin)
	try
	{
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print)//输出
			t = ts.get();
		if (t.kind == quit)//退�?
			return;
		ts.putback(t);
		ANS = expression();//记录ANS
		if (flag1 != flag2)//如果'('�?')'出现次数不一样为非法式子
			error("error");
		else
		    cout << result << ANS << endl;//输出结果
		flag1 = 0;//重置括号出现次数
		flag2 = 0;
	}
	catch (exception& e)
	{
		flag1 = 0;//重置括号出现次数
		flag2 = 0;
		cerr << e.what() << endl;
		clean_up_mess();
	}
}
//------------------------------------------------------------------------------------------
//主函�?
int main()
{
	try
	{
		calculate();
		return 0;
	}
	catch (...)
	{
		error("error");
		return 2;
	}
}
//------------------------------------------------------------------------------------------

