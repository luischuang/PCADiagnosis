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
	//读取标准数据，分为两部分，100训练，40测试
	vector<vector<double>> datas = vector<vector<double>>();
	readDatas((char*)"normal.csv", 140, datas);
	MatrixXd data(100, 4);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 4; j++) {
			data(i,j)=datas[i][j];
		}
	}
	MatrixXd normalTestData(40, 4);
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 4; j++) {
			normalTestData(i, j) = datas[i+100][j];
		}
	}

	//开始训练
	PCADiagnosis diagnosis = PCADiagnosis();

	//求列向量均值
	RowVectorXd mean(data.cols());
	diagnosis.getMean(data,mean);
	//cout << mean<<endl;

	//求列向量标准差
	//cout << "*************标准差*****" << endl;
	RowVectorXd sigma(data.cols());
	diagnosis.getSigma(data,sigma);
	//cout << sigma <<endl;

	//对训练数据进行标准化
	//cout << "**************************标准化*********************" << endl;
	MatrixXd standardData(100, 4);
	diagnosis.standardizing(data,mean,sigma, standardData);
	//cout << standardData << endl;

	//求协方差矩阵
	//cout << "协方差" << endl;
	MatrixXd cov1(data.cols(),data.cols());
	diagnosis.getCov(standardData,cov1);
	//cout << cov1<<endl;

	//求特征值和特征向量
	//cout << "****************特征值   特征向量" << endl;
	MatrixXd vec;
	MatrixXd val;
	diagnosis.getCharacterMatrix(cov1,vec,val);
	//cout << "特征向量" << endl;
	//cout << vec << endl;
	//cout << "特征值：" << endl;
	//cout << val<<endl;

	//求荷载矩阵
	MatrixXd load;
	diagnosis.calculateLoad(vec,val,load);
	//cout << endl << "负载矩阵：" <<endl;
	//cout << load << endl;

	//计算训练数据的T2值
	RowVectorXd T2(standardData.rows());
	diagnosis.calculateT2(standardData, cov1, T2);

	//计算T2的上下控制限
	double max = 0, min = 0;
	diagnosis.calculateLimit(T2, max, min);
	cout << "上限：" << max << " 下限：" << min << endl;
	//根据正常的测试数据进行测试
	//对正常测试进行标准化
	MatrixXd normalTestStandardData(40, 4);
	diagnosis.standardizing(normalTestData,mean, sigma, normalTestStandardData);
	//计算测试数据的T2值
	RowVectorXd normalTestT2(normalTestData.rows());
	diagnosis.calculateT2(normalTestStandardData,cov1, normalTestT2);
	cout << normalTestT2 << endl;
	//统计正常测试数据的正确率
	int normalCurrect = 0;
	for (int i = 0; i < normalTestT2.size(); i++) {
		if (normalTestT2(i) <= max && normalTestT2(i) >= min) {
			normalCurrect++;
		}
	}
	cout << "正常测试正确率为：" << normalCurrect << "//" << normalTestT2.size()
		<< "(" << (double)normalCurrect / normalTestT2.size() << ")" << endl;


	//读取故障测试数据
	vector<vector<double>> faultDatas = vector<vector<double>>();
	readDatas((char*)"IRFault.csv", 217, faultDatas);
	MatrixXd faultTestData(217, 4);
	for (int i = 0; i < 217; i++) {
		for (int j = 0; j < 4; j++) {
			faultTestData(i, j) = faultDatas[i][j];
		}
	}
	//根据故障的测试数据进行测试
	//对故障测试进行标准化
	MatrixXd faultTestStandardData(faultTestData.rows(), 4);
	diagnosis.standardizing(faultTestData, mean, sigma, faultTestStandardData);
	//计算测试数据的T2值
	RowVectorXd faultTestT2(faultTestData.rows());
	diagnosis.calculateT2(faultTestStandardData, cov1, faultTestT2);
	cout << faultTestT2 << endl;
	//统计正常测试数据的正确率
	int faultCurrect = 0;
	for (int i = 0; i < faultTestT2.size(); i++) {
		if (faultTestT2(i) > max || faultTestT2(i) < min) {
			faultCurrect++;
		}
	}
	cout << "故障数据测试正确率为：" << faultCurrect << "//" << faultTestT2.size()
		<< "(" << (double)faultCurrect / faultTestT2.size() << ")" << endl;

	//计算各个故障频率对故障的贡献度
	MatrixXd C(faultTestData.rows(), faultTestData.cols());
	diagnosis.calculateC(faultTestData,load,cov1,C);
	//cout << "贡献度C" << endl;
	//cout << C << endl;
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
