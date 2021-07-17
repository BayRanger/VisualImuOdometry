/*************************************************************************
        > File Name: subsystem.cpp
        > Author:
        > Mail:
        > Created Time: Fr 16 Jul 2021 15:19:50 CEST
 ************************************************************************/
#include "backend/subproblem.h"
#include <iostream>
using namespace std;

namespace myslam {
namespace backend {
subproblem::subproblem(ProblemType problemtype) : Problem(problemtype){};
void subproblem::show() {
  std::cout << "Implementation of LM first method" << std::endl;
}

void subproblem::ComputeLambdaInitLM() {
  ni_ = 2.;
  currentLambda_ = -1.;
  currentChi_ = 0.0;
  // TODO:: robust cost chi2
  for (auto edge : edges_) {    
    currentChi_ += edge.second->RobustChi2();
  }
  if (err_prior_.rows() > 0)
    currentChi_ += err_prior_.norm();
currentChi_ *= 0.5;

  stopThresholdLM_ = 1e-10 * currentChi_; // 迭代条件为 误差下降 1e-6 倍//1e-10

  double maxDiagonal = 0;
  ulong size = Hessian_.cols();
  assert(Hessian_.rows() == Hessian_.cols() && "Hessian is not square");
  for (ulong i = 0; i < size; ++i) {
    maxDiagonal = std::max(fabs(Hessian_(i, i)), maxDiagonal);
  }
  currentLambda_ = 1e-3; // user defined
}

bool subproblem::IsGoodStepInLM() {

  double tempChi = 0.0;
  for (auto edge : edges_) {
    edge.second->ComputeResidual();
    tempChi += edge.second->RobustChi2();
  }
    if (err_prior_.size() > 0)
        tempChi += err_prior_.norm();
    tempChi *= 0.5;          // 1/2 * err^2
  cout<<"IsGoodStepInLM tempchi "<<tempChi<<endl;

  double rho =
      (currentChi_ - tempChi) /
      (delta_x_.transpose() *
       (currentLambda_ * Hessian_.diagonal().asDiagonal() * delta_x_ + b_));
  double L_up = 11;
  double L_down = 9;
  if (rho > 0 && isfinite(tempChi)) // last step was good, 误差在下降
  {
    currentLambda_ = std::max(currentLambda_ / L_down, 1e-7);
     currentChi_ = tempChi;
    return true;
  } else {
    if (!isfinite(tempChi)) {
      std::cout << "residual is too large" << std::endl;
    }
    currentLambda_ = std::min(currentLambda_ * L_up, 1e7);
    return false;
  }
}
}
}
