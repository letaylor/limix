/*
 * Linear.cpp
 *
 *  Created on: Nov 11, 2011
 *      Author: stegle
 */

#include "linear.h"
#include <math.h>
#include <cmath>
#include "limix/utils/matrix_helper.h"

namespace limix {


/***** CCovLinearISO *******/

CCovLinearISO::~CCovLinearISO() {
}

void CCovLinearISO::aKcross(MatrixXd* out,const CovarInput& Xstar) const throw(CGPMixException)
{
	//create result matrix:
	out->resize(this->X.rows(),Xstar.rows());
	//We dfine Xstar [N X D] where N are samples...
	if (Xstar.cols()!=this->X.cols())
	{
		std::ostringstream os;
		os << this->getName() <<": Xstar has wrong number of dimensions. Xstar.cols() = "<< Xstar.cols() <<". X.cols() = "<< this->X.cols() << ".";
		throw CGPMixException(os.str());
	}
	//kernel matrix is constant hyperparmeter and dot product
	(*out).noalias() = std::pow(params(0),2)* Xstar*this->X.transpose();
}


void CCovLinearISO::aKcross_diag(VectorXd* out, const CovarInput& Xstar) const throw(CGPMixException)
{
	out->resize(Xstar.rows());
	(*out) = std::pow(params(0),2)* (Xstar*Xstar.transpose()).diagonal();
}


void CCovLinearISO::aKgrad_param(MatrixXd* out, const muint_t i ) const throw(CGPMixException)
{
	if (i==0)
	{
		out->resize(this->X.rows(),this->X.rows());
		(*out).noalias() = 2*params(0)*this->X*this->X.transpose();
	}
	else
	{
		std::ostringstream os;
		os << this->getName() <<": wrong index of hyperparameter. i = "<< i <<". this->params.cols() = "<< this->getNumberParams() << ".";
		throw CGPMixException(os.str());
	}
}
    
void CCovLinearISO::aKhess_param(MatrixXd* out, const muint_t i, const muint_t j) const throw(CGPMixException)
{
    if (i>=(muint_t)this->numberParams || j>=(muint_t)this->numberParams)   {
		throw CGPMixException("Parameter index out of range.");
    }
    (*out).noalias()=2*this->X*this->X.transpose();
}

void CCovLinearISO::aKcross_grad_X(MatrixXd* out,const CovarInput& Xstar, const muint_t d) const throw(CGPMixException)
{
	if (d>this->numberDimensions)
	{
		std::ostringstream os;
		os << this->getName() <<": wrong dimension index";
		throw CGPMixException(os.str());
	}

	//create empty matrix
	(*out) = MatrixXd::Zero(Xstar.rows(),this->X.rows());
	//otherwise update computation:
	(*out).rowwise() = std::pow(params(0),2)*Xstar.col(d).transpose();

}

void CCovLinearISO::aKdiag_grad_X(VectorXd* out, const muint_t d ) const throw (CGPMixException)
{
	if (d>=this->numberDimensions)
	{
		std::ostringstream os;
		os << this->getName() <<": wrong dimension index";
		throw CGPMixException(os.str());
	}
	(*out) = VectorXd::Zero(this->X.rows());
	(*out) = 2.0*std::pow(params(0),2)*this->X.col(d);
}



/***** CCovLinearISODelta *******/

CCovLinearISODelta::~CCovLinearISODelta() {
}

void CCovLinearISODelta::aKcross(MatrixXd* out,const CovarInput& Xstar) const throw(CGPMixException)
{
	//create result matrix:
	out->resize(this->X.rows(),Xstar.rows());
	//We dfine Xstar [N X D] where N are samples...
	if (Xstar.cols()!=this->X.cols())
	{
		std::ostringstream os;
		os << this->getName() <<": Xstar has wrong number of dimensions. Xstar.cols() = "<< Xstar.cols() <<". X.cols() = "<< this->X.cols() << ".";
		throw CGPMixException(os.str());
	}
	//kernel matrix is constant hyperparmeter and dot product
	for (muint_t ir=0;ir< (muint_t)out->rows();++ir)
	{
		for (muint_t ic=0;ic<(muint_t)out->cols();++ic)
		{
			//kernel value is number of coinciding elements in that row
			muint_t count = (X.row(ir).array()==Xstar.row(ic).array()).sum();
			(*out)(ir,ic) = std::pow(params(0),2)*count;
		}
	}
}


void CCovLinearISODelta::aKcross_diag(VectorXd* out, const CovarInput& Xstar) const throw(CGPMixException)
{
	out->resize(Xstar.rows());
	out->setConstant(std::pow(params(0),2)* Xstar.cols());
}


void CCovLinearISODelta::aKgrad_param(MatrixXd* out, const muint_t i ) const throw(CGPMixException)
{
    out->resize(this->X.rows(),X.rows());

	if (i>0)
	{
		std::ostringstream os;
		os << this->getName() <<": wrong index of hyperparameter. i = "<< i <<". this->params.cols() = "<< this->getNumberParams() << ".";
		throw CGPMixException(os.str());
	}
    
	for (muint_t ir=0;ir< (muint_t)out->rows();++ir)
	{
		for (muint_t ic=0;ic<(muint_t)out->cols();++ic)
		{
			//kernel value is number of coinciding elements in that row
			muint_t count = (X.row(ir).array()==X.row(ic).array()).sum();
			(*out)(ir,ic) = count;
		}
	}
    
}
    
void CCovLinearISODelta::aKhess_param(MatrixXd* out, const muint_t i, const muint_t j) const throw(CGPMixException)
{
    if (i>=(muint_t)this->numberParams || j>=(muint_t)this->numberParams)   {
        throw CGPMixException("Parameter index out of range.");
    }
    (*out)=MatrixXd::Zero(this->X.rows(),this->X.rows());
}




/***** CCovLinearARD *******/

CCovLinearARD::~CCovLinearARD()
{
}

void CCovLinearARD::setNumberDimensions(muint_t numberDimensions)
{
	this->numberDimensions = numberDimensions;
	this->numberParams = numberDimensions;
}


//overloaded pure virtual functions:
void CCovLinearARD::aKcross(MatrixXd* out, const CovarInput& Xstar ) const throw (CGPMixException)
{
	//get all amplitude parameters, one per dimension
	VectorXd L = params.unaryExpr(std::bind2nd(std::ptr_fun<double,double,double>(pow),2));
	(*out).noalias() = Xstar*L.asDiagonal()*this->X.transpose();
}

void CCovLinearARD::aKcross_diag(VectorXd* out, const CovarInput& Xstar) const throw(CGPMixException)
{
    VectorXd L = params.unaryExpr(std::bind2nd(std::ptr_fun<double,double,double>(pow),2));
    (*out).noalias() = (Xstar*L.asDiagonal()*Xstar.transpose()).diagonal();
}

void CCovLinearARD::aKgrad_param(MatrixXd* out,const muint_t i) const throw (CGPMixException)
{
	//is the requested gradient within range?
	if (i >= (muint_t)this->X.cols()) //WARNING: muint_t conversion
		throw CGPMixException("unknown hyperparameter derivative requested in CLinearCFISO");
	//ok: calculcate:
	//1. get row i from x1:
	MatrixXd x1i = X.col(i);
	//outer product of the corresponding dimension.
	(*out).noalias() =  2*params(i)*x1i*x1i.transpose();
}
    
void CCovLinearARD::aKhess_param(MatrixXd* out, const muint_t i, const muint_t j) const throw(CGPMixException)
{
    if (i>=(muint_t)this->numberParams || j>=(muint_t)this->numberParams)   {
        throw CGPMixException("Parameter index out of range.");
    }
    if (i==j) {
    	MatrixXd x1i = X.col(i);
    	(*out).noalias()=2*x1i*x1i.transpose();
    }
    else		(*out)=MatrixXd::Zero(this->X.rows(),this->X.rows());
}

void CCovLinearARD::aKcross_grad_X(MatrixXd* out,const CovarInput& Xstar, const muint_t d) const throw (CGPMixException)
{
	if (d>=numberDimensions)
	{
		throw CGPMixException("derivative for gradient outside specification requested.");
	}
	(*out) = MatrixXd::Zero(Xstar.rows(),this->X.rows());
	(*out).rowwise() = std::pow(params(d),2)*Xstar.col(d).transpose();
}

void CCovLinearARD::aKdiag_grad_X(VectorXd* out,const muint_t d) const throw (CGPMixException)
{
	(*out) = VectorXd::Zero(X.rows());
	(*out).noalias() = 2.0*std::pow(params(d),2)*X.col(d);
}




} /* namespace limix */
