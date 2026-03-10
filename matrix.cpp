#include<vector>
#include<iostream>
#include<random>
#include<iomanip>
#include<chrono>
#include <windows.h>
const int Data_Size = 50;//数据规模
const int N = 10000;// 向量位数
using namespace std;

struct Vector//Vector是一个结构体，其下含有一个数组 Vecter x(10) 表示一个10维数组
{
	vector<double> data;
	Vector(size_t n = 0) : data(vector(n, 0.0)) {}
	double operator*(const Vector& other) const//向量内积
	{
		size_t length_data = data.size();//表示当前向量的长度
		size_t length_other = other.data.size();//表示第二个位置的向量长度
		if (length_data != length_other)//首先判断向量长度是否相等，否则无法进行内积运算.
		{
			cout << " Vector size error! " << endl;
			return -1;
		}
		double res = 0;//初始化内积计算变量
		for (size_t i = 0; i < length_data; i++)
		{
			res = res + data[i] * other.data[i];//相应位置向量相加
		}
		return res;
	}
	Vector operator+(const Vector& other) const//向量相加的运算符重载
	{
		size_t length_data = data.size();
		size_t length_other = other.data.size();
		if (length_data != length_other)//如果向量位置不对就返回错误
		{
			cout << " Vector size error! " << endl;
			return 0;
		}
		Vector res(length_data);//新建向量
		for (int i = 0; i < length_data; i++)
		{
			res.data[i] = data[i] + other.data[i];//相应位置的向量相加
		}
		return res;
	}
};
Vector operator*(double k, const Vector& other)//向量数乘运算
{
    size_t n = other.data.size();//确定数据长度
	Vector res(n);//新建长度为n的向量
	for (int i = 0; i < n; i++)
	{
		res.data[i] = k * other.data[i];//进行数据处理
	}
	return res;
}
ostream& operator<<(ostream& os, const Vector& v)//这个是向量输出的运算符重载
{
	os << "[ ";
	for (size_t i = 0; i < v.data.size() - 1; i++)
	{
		os << v.data[i] << ", ";
	}
	os << v.data[v.data.size() - 1] << " ]";
	return os;//支持连续输出
}
struct Matrix//矩阵结构体的建立
{
	int rows, cols;//矩阵的行列
	vector<Vector> data;//矩阵的具体数据，一共有rows个Vector类型的向量，每一个Vector类型的向量的规模为cols
	Matrix(int r, int c) : rows(r), cols(c), data(r, Vector(c)) {}//参数方法初始化
	Matrix operator+(const Matrix& other) const//矩阵加法
	{
		if ((rows != other.rows) || (cols != other.cols))
		{
			cout << " Matrix size error! " << endl;
			exit(1);
		}
		Matrix res(rows, cols);
		for (int i = 0; i < rows; i++)
		{
			res.data[i] = data[i] + other.data[i];//相应行向量相加
		}
		return res;
	}
	Matrix operator*(const Matrix& other) const//矩阵乘法
	{
		if (cols != other.rows)
		{
			cout << " Matrix size error! " << endl;
			exit(1);
		}
		Matrix res(rows, other.cols);
		//预处理other矩阵的列向量
		vector<Vector> other_data(other.cols, Vector(other.rows));//other_data[i]存储第i列的内容
		for (int i = 0; i < other.cols; i++)
		{
			for (int j = 0; j < other.rows; j++)
			{
				other_data[i].data[j] = other.data[j].data[i];
			}
		}//本质是矩阵的转置
		for (int i = 0; i < rows; i++)
		{
			// data[i] 行向量
			for (int j = 0; j < cols; j++)
			{
				res.data[i].data[j] = data[i] * other_data[j];
			}
		}
		return res;
	}
	Matrix Matrix_T()//矩阵的转置
	{

		Matrix A_T(cols, rows);
		for (int i = 0; i < cols; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				A_T.data[j].data[i] = data[i].data[j];
			}
		}
		return A_T;
	};
	void Matrix_print()//矩阵的输出，不过有了运算符重载以后就不需要了
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				cout << setw(4) << data[i].data[j];
			}
			cout << endl;
		}
		cout << endl;
	}
};
Matrix operator*(double k, const Matrix& other)//矩阵的数乘运算
{
	Matrix res(other.rows, other.cols);
	for (size_t i = 0; i < other.rows; i++)
	{
		res.data[i] = k * other.data[i];
	}
	return res;
}
ostream& operator<<(ostream& os, const Matrix& A)//矩阵的运算符重载输出
{
	cout << endl;
	for (size_t i = 0; i < A.rows; i++)
	{
		os << A.data[i] << endl;//直接输出行向量
	}
	return os;//返回值os可以直接用于下一个输出所需
}
void test();//测试函数，用于检验各模块正确性
void preceptron(vector<Vector>& x, vector<int>& y);//感知机函数 给若干个向量，去寻找一个分划的方法
void preceptron_dual(vector<Vector>& x, vector<int>& y,Matrix gram);//感知机对偶模型 利用gram矩阵进行处理
void measure_performance(vector<Vector>& x, vector<int>& y,Matrix gram);
void Random_data(vector<Vector>& x, vector<int>& y, int Data_Size, int N);
void Gram_data(Matrix& gram, vector<Vector>& x);

int main()
{
	SetConsoleOutputCP(65001);//设置字体

	//test();//检查数据是否有问题
	//preceptron();//普通的随机梯度下降方法
	//preceptron_dual();//gram方法
	vector<Vector> x(Data_Size, Vector(N));//感知机算法内容
	vector<int> y(Data_Size, 0);
	Matrix gram(Data_Size, Data_Size);
	Random_data(x, y, Data_Size, N);//随机生成数据
	Gram_data(gram, x);
	measure_performance(x, y, gram);//用于看两个效率的
	return 0;
}

void test() {
	// 1. 初始化 A (2x2) 
	Matrix A(2, 2);
	A.data[0].data = { 1, 2 };
	A.data[1].data = { 3, 4 };

	// 2. 初始化 B (2x2)
	Matrix B(2, 2);
	B.data[0].data = { 5, 6 };
	B.data[1].data = { 7, 8 };

	// 3. 执行乘法
	// 预期结果：
	// [1*5+2*7, 1*6+2*8] -> [19, 22]
	// [3*5+4*7, 3*6+4*8] -> [43, 50]
	Matrix C = A * B;

	cout << A << endl;
	cout << B << endl;
	cout << C << endl;
	cout << A.Matrix_T() << endl;
}
void preceptron(vector<Vector>& x, vector<int>& y)
{
	bool has_flag = true;//用于判断整理好一组数据以后是否进行了一次数据更新
	double eta = 0.1;//eta表示步长
	Vector w(N);//w数组表示直线的法向量，也就是对应超平面的系数
	double b = 0;//b表示对应超平面的截距
	while (has_flag)//如果has_flag,也就是数据更新过了，就继续循环
	{
		has_flag = false;
		double point = 0;//初始化
		for (int i = 0; i < Data_Size; i++)
		{
			point = 0;
			point = w * x[i] + b;//计算当前点的得分
			if (point * y[i] <= 0)//判断分类是否正确
			{
				has_flag = true;//数据更新确认，并且更新w和b的值
				w = w + eta * y[i] * x[i];
				b = b + eta * y[i];
			}
		}
	}
	//cout << " w = " << w << endl;
	//cout << " b= " << b << endl;
}
void preceptron_dual(vector<Vector>& x, vector<int>& y, Matrix gram)
{

	//前面的初始化和precenptron内容一样，下面生成gram矩阵

	vector<double> alpha(10001, 0);//alpha数组用于存储x[i]向量的累加次数
	bool has_flag = true;//用于判断整理好一组数据以后是否进行了一次数据更新
	double eta = 0.1;//eta表示步长
	Vector w(N);//w数组表示直线的法向量，也就是对应超平面的系数
	double b = 0;//b表示对应超平面的截距
	while (has_flag)//如果has_flag,也就是数据更新过了，就继续循环
	{
		has_flag = false;
		double point = 0;//初始化
		for (int i = 0; i < Data_Size; i++)
		{
			point = b;
			for (int j = 0; j < Data_Size; j++)
			{
				point = point + alpha[j] * y[j] * gram.data[j].data[i];
			}
			if (y[i] * point <= 0)
			{
				alpha[i] = alpha[i] + eta;
				b = b + eta * y[i];
				has_flag = true;
			}
		}
	}
	for (int i = 0; i < Data_Size; i++)
	{
		w = w + alpha[i] * y[i] * x[i];
	}
	//cout << " w = " << w << endl;
	//cout << " b = " << b;
}
void measure_performance(vector<Vector>& x, vector<int>& y,Matrix gram) {
	// 1. 准备数据（确保两边用的是同一套随机数据，公平竞争）
	// ... 你的数据生成代码 ...

	// 2. 测试原始感知机
	auto start_primal = chrono::high_resolution_clock::now(); // 开始计时
	preceptron(x, y);
	auto end_primal = chrono::high_resolution_clock::now();   // 结束计时

	// 计算耗时（单位：毫秒）
	chrono::duration<double, std::milli> elapsed_primal = end_primal - start_primal;

	// 3. 测试对偶感知机
	auto start_dual = chrono::high_resolution_clock::now();
	preceptron_dual(x, y, gram);
	auto end_dual = chrono::high_resolution_clock::now();

	chrono::duration<double, std::milli> elapsed_dual = end_dual - start_dual;

	// 4. 输出对比结果
	cout << fixed << setprecision(3); // 设置输出精度
	cout << "\n================================" << endl;
	cout << "原始感知机耗时: " << elapsed_primal.count() << " ms" << endl;
	cout << "对偶感知机耗时: " << elapsed_dual.count() << " ms" << endl;
	cout << "================================" << endl;
}
void Random_data(vector<Vector>& x, vector<int>& y, int Data_Size, int N)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(-10.0, 10.0);

	// 1. 生成数据：我们设定上帝直线为 x2 = x1 + 0.5
	// 只要是在线上的点标为 1，线下的标为 -1，这组数据绝对线性可分
	vector<double> a(N, 0);
	for (int i = 0; i < N; i++)
	{
		a[i] = dis(gen);
	}
	for (int i = 0; i < Data_Size; i++) {
		double sum = 0;
		for (int j = 0; j < N; j++)
		{
			x[i].data[j] = dis(gen);
			sum = sum +  x[i].data[j] * a[j];
		}
		y[i] = (sum > 1) ? 1 : -1;
	}//随机生成1000个数据点
	//上述x2>x1+0.5 理论上是y-x=0.5这条直线

}
void Gram_data(Matrix& gram, vector<Vector>& x)
{
	for (int i = 0; i < Data_Size; i++)
	{
		for (int j = 0; j < Data_Size; j++)
		{
			gram.data[i].data[j] = x[i] * x[j];
		}
	}
}