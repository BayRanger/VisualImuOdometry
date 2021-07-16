/*************************************************************************
	> File Name: subproblem.h
	> Author: 
	> Mail: 
	> Created Time: Fr 16 Jul 2021 14:25:47 CEST
 ************************************************************************/

#ifndef _SUBPROBLEM_H
#define _SUBPROBLEM_H


#include "problem.h"

namespace myslam{
    namespace backend{
        

class subproblem : public Problem
        {
            public:
                void show();
                subproblem(ProblemType problemType);

            
        };
    }

}



#endif
