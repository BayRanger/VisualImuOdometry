/*************************************************************************
	> File Name: subsystem.cpp
	> Author: 
	> Mail: 
	> Created Time: Fr 16 Jul 2021 15:19:50 CEST
 ************************************************************************/
#include "backend/subproblem.h"
#include<iostream>
using namespace std;

namespace myslam{
    namespace backend{
        subproblem::subproblem(ProblemType problemtype):Problem(problemtype){};
        void subproblem::show()
        {
            std::cout<<"subproblem works showing"<<std::endl;
        }

        }
}
