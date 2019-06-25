#include "pch.h"
#include <iostream>
#include "PCADiagnosis.h"
#include "myUtil.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace splab;
//读取记录数据
int readDatas(char* fileName, int length, vector<vector<double>> &datas) {
	int count = 0;
	ifstream file;
	file.open(fileName, ios::in);

	if (!file.is_open())

		return 0;

	std::string strLine;
	while (getline(file, strLine) && count < length)
	{
		if (strLine.empty())
			continue;
		replace_all(strLine, " ", "");
		vector<std::string> strings = split(strLine, ",");
		vector<double> temp = vector<double>();
		for (int i = 0; i < strings.size(); i++) {
			temp.push_back(atof(strings[i].c_str()));
		}
		datas.push_back(temp);
		count++;
		//cout << strLine << endl;
		strings.~vector();
		temp.~vector();
	}
	file.close();
}
int main()
{
	vector<vector<double>> datas = vector<vector<double>>();
	readDatas((char*)"normal.csv", 140, datas);
	std::cout << "Hello World!\n";
	cout << endl;
	MatrixXd data(100, 4);
	for (int i = 0; i < 100; i++) {
		//data << datas[i][0], datas[i][1], datas[i][2], datas[i][3];
		for (int j = 0; j < 4; j++) {
			data(i,j)=datas[i][j];
		}
	}
	//data << 1, 2,
	//	3, 6,
	//	4, 2,
	//	5, 2;
	PCADiagnosis diagnosis = PCADiagnosis();
	RowVectorXd mean;
	diagnosis.getMean(data,mean);
	cout << mean<<endl;
	cout << "*************标准差*****" << endl;
	RowVectorXd sigma;
	diagnosis.getSigma(data,sigma);
	cout << sigma;
	cout << endl;
	cout << "**************************标准化*********************" << endl;
	diagnosis.standardizing(mean,sigma,data);
	cout << data << endl;
	cout << "协方差" << endl;
	MatrixXd cov1;
	diagnosis.getCov(data,mean,cov1);
	cout << cov1<<endl;
	cout << "****************特征值   特征向量" << endl;
	MatrixXd vec;
	MatrixXd val;

	
	diagnosis.getCharacterMatrix(cov1,vec,val);
	cout << "特征向量" << endl;
	cout << vec << endl;
	cout << "特征值：" << endl;
	cout << val<<endl;


	MatrixXd load;
	diagnosis.calculateLoad(vec,val,load);
	cout << endl << "负载矩阵：" <<endl;
	cout << load << endl;

	cout << "T平方" << endl;
	MatrixXd T;
	diagnosis.calculateT(data,cov1,T);
	cout << T << endl;


}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
