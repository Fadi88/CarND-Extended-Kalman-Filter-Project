#include "kalman_filter.h"
#include <iostream>

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  x_ = F_ * x_ ;
  P_ = F_ * P_ * F_.transpose() + Q_ ;
}

void KalmanFilter::Update(const VectorXd &z) {
	VectorXd z_pred = H_ * x_;
	VectorXd y = z - z_pred;
	MatrixXd Ht = H_.transpose();
	MatrixXd S = H_ * P_ * Ht + R_;
	MatrixXd Si = S.inverse();
	MatrixXd PHt = P_ * Ht;
	MatrixXd K = PHt * Si;

	//new estimate
	x_ = x_ + (K * y);
	P_ = (MatrixXd::Identity(x_.size(), x_.size()) - K * H_) * P_;

 
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
    float px  = x_(0) ;
    float py  = x_(1) ;
    float vx = x_(2) ;
    float vy = x_(3) ;

    float ro    = sqrt(px*px + py*py) ;
    float theta = atan2(py,px);
    float rodot = (px*vx + py*vy)/ro ;

    VectorXd z_pred = VectorXd(3);


    z_pred << ro , theta , rodot ; 

    VectorXd y = z - z_pred ;
    
    std::cout << "y = " << y << std::endl ;
    while(y(1) > M_PI){
	//std::cout << "y = " << y << std::endl ;
	//std::cout << " ----------------------- theta --" << std::endl;
        y(1) -= 1 * M_PI;
    }

    while(y(1) < -M_PI){
	//std::cout << "y = " << y << std::endl ;
	//std::cout << " ----------------------- theta ++" << std::endl;
        y(1) += 1 * M_PI;
    }
 
    MatrixXd Ht = H_.transpose();
    MatrixXd S = H_ * P_ * Ht + R_;
    MatrixXd Si = S.inverse();
    MatrixXd PHt = P_ * Ht;
    MatrixXd K = PHt * Si;
	std::cout << "K = " << K << std::endl ;
    //new estimate
    x_ = x_ + (K * y);
    P_ = (MatrixXd::Identity(x_.size(), x_.size()) - K * H_) * P_;
}


