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
	void standardizing(MatrixXd data, RowVectorXd  mean, RowVectorXd sigma, MatrixXd& standardData);
	/*
	计算协方差
	*/
	void getCov(MatrixXd data , MatrixXd& cov);
	/*
	计算特征矩阵和特征值
	*/
	void getCharacterMatrix(MatrixXd  data, MatrixXd& matrix, MatrixXd& value);

	/*
	计算荷载
	*/
	void calculateLoad(MatrixXd  characterMatrix, MatrixXd characterValue, MatrixXd& load);
	/*
	计算指标T
	*/
	void calculateT2(MatrixXd data , MatrixXd cov, RowVectorXd& T2);
	/*
	计算T2指标的上下限
	*/
	void calculateLimit(RowVectorXd T2, double& max, double&min);
	/*
	计算故障贡献度
	*/
	void calculateC(MatrixXd data , MatrixXd load, MatrixXd cov, MatrixXd& C);
};

