#pragma once
#include <stdio.h>
#include <vector>
#include "vector.h"

#include"Eigen/Dense"
using namespace std;
using namespace splab;
using namespace Eigen;
class PCADiagnosis
{
	
public:
	PCADiagnosis();
	~PCADiagnosis();
	/*
	计算列向量均值
	*/
	void getMean(MatrixXd data, RowVectorXd& mean);
	/*
	计算列向量标准差
	*/
	void getSigma(MatrixXd data, RowVectorXd& sigma);
	/*
	标准化
	*/
	void standardizing(RowVectorXd  mean, RowVectorXd sigma, MatrixXd& data);
	/*
	计算协方差
	*/
	void getCov(MatrixXd data , RowVectorXd mean, MatrixXd& cov);
	/*
	计算特征矩阵和特征值
	*/
	void getCharacterMatrix(MatrixXd  data, MatrixXd& matrix, MatrixXd& value);
	///*
	//计算特征值
	//*/
	//void getCharacterValue(MatrixXd  data, MatrixXd& value);
	/*
	计算荷载
	*/
	void calculateLoad(MatrixXd  characterMatrix, MatrixXd characterValue, MatrixXd& load);
	/*
	计算指标T
	*/
	void calculateT(MatrixXd data , MatrixXd cov, MatrixXd& T);
	/*
	计算故障贡献度
	*/
	void calculateC(MatrixXd data , MatrixXd load, MatrixXd cov, MatrixXd C);
};

