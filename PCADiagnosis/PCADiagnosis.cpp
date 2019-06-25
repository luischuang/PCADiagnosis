#include "pch.h"
#include "PCADiagnosis.h"


PCADiagnosis::PCADiagnosis()
{
}


PCADiagnosis::~PCADiagnosis()
{
}

//列向量均值
void PCADiagnosis::getMean(MatrixXd data, RowVectorXd& mean)
{
	MatrixXd meanval = data.colwise().mean();
	//cout << "均值" << meanval << endl;
	 mean = meanval;
}

//列向量标准差
void PCADiagnosis::getSigma(MatrixXd data, RowVectorXd& sigma)
{
	MatrixXd meanval = data.colwise().mean();
	RowVectorXd meanvecRow = meanval;	//均值
	data.rowwise() -= meanvecRow;
	RowVectorXd tem=data.colwise().squaredNorm()/data.rows();
	sigma = tem.cwiseSqrt();
}

	/*
	标准化
	*/
void PCADiagnosis::standardizing(RowVectorXd mean, RowVectorXd sigma, MatrixXd& data)
{
	data.rowwise() -= mean;
	for (int i = 0; i < data.rows();i++) {
		for (int j = 0; j < data.cols(); j++) {
			data(i, j) = data(i, j) / sigma(j);
		}
	}
}

/*
	计算协方差
*/
void PCADiagnosis::getCov(MatrixXd data, RowVectorXd mean, MatrixXd& cov)
{
	data.rowwise() -= mean;
	cov=data.adjoint()* data;
	cov = cov.array() / (data.rows()-1);
}

/*
	计算特征矩阵和特征值
*/
void PCADiagnosis::getCharacterMatrix(MatrixXd cov, MatrixXd& vec, MatrixXd& val)
{
	SelfAdjointEigenSolver<MatrixXd> eig(cov);

	vec = eig.eigenvectors();//特征矩阵
	val = eig.eigenvalues();//特征值

}



/*
	计算荷载
*/
void PCADiagnosis::calculateLoad(MatrixXd characterMatrix, MatrixXd characterValue, MatrixXd& load)
{
	MatrixXd tem(characterValue.rows(),characterValue.rows());
	for (int i = 0; i < characterValue.rows(); i++) {
		for (int j = 0; j < characterValue.rows(); j++) {
			tem(j, i) = sqrt(characterValue(i, 0)) * characterMatrix(j, i);
		}
	}
	load = tem;
}
/*
	计算指标T
*/
void PCADiagnosis::calculateT(MatrixXd data, MatrixXd cov, MatrixXd& T)
{
	MatrixXd invcov(cov.rows(),cov.cols());
	invcov = cov.inverse();
	T=data* invcov* data.transpose();
	

}
/*
计算故障贡献度
*/
void PCADiagnosis::calculateC(MatrixXd data, MatrixXd load, MatrixXd cov, MatrixXd C)
{

}


