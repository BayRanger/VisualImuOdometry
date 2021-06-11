//
// Created by hyj on 18-11-11.
//
#include <iostream>
#include <vector>
#include <random>  
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>

struct Pose
{
    Pose(Eigen::Matrix3d R, Eigen::Vector3d t):Rwc(R),qwc(R),twc(t) {};
    Eigen::Matrix3d Rwc;
    Eigen::Quaterniond qwc;
    Eigen::Vector3d twc;
    
    Eigen::Matrix<double,3, 4> getPwc() const
    {
        Eigen::Matrix<double,3, 4> Pwc;
        Pwc.block(0,0,3,3) = Rwc;
        Pwc.block(0,3,3,1) = twc;
        return Pwc;
    }
    Eigen::Matrix<double,3, 4> getPcw() const
    {
        //from world to camera
        Eigen::Matrix<double,3, 4> Pwc;
        Pwc.block(0,0,3,3) = Rwc.transpose();
        Pwc.block(0,3,3,1) = -Rwc.transpose()*twc;
        return Pwc;
    }

    Eigen::Vector2d uv;    // 这帧图像观测到的特征坐标
};
int main()
{

    int poseNums = 10;
    double radius = 8;
    double fx = 1.;
    double fy = 1.;
    std::vector<Pose> camera_pose;
    for(int n = 0; n < poseNums; ++n ) {
        double theta = n * 2 * M_PI / ( poseNums * 4); // 1/4 圆弧
        // 绕 z轴 旋转
        Eigen::Matrix3d R;
        R = Eigen::AngleAxisd(theta, Eigen::Vector3d::UnitZ());
        Eigen::Vector3d t = Eigen::Vector3d(radius * cos(theta) - radius, radius * sin(theta), 1 * sin(2 * theta));
        camera_pose.push_back(Pose(R,t));//TODO: use emplace back instead of push back
        //std::cout<<t[0]<<","<<t[1]<<", "<<t[2]<<std::endl;  //for visualize
    }

    // 随机数生成 1 个 三维特征点
    std::default_random_engine generator;
    std::uniform_real_distribution<double> xy_rand(-4, 4.0);
    std::uniform_real_distribution<double> z_rand(8., 10.);
    double tx = xy_rand(generator);
    double ty = xy_rand(generator);
    double tz = z_rand(generator);

    Eigen::Vector3d Pw(tx, ty, tz);
    // 这个特征从第三帧相机开始被观测，i=3
    int start_frame_id = 3;
    int end_frame_id = poseNums;
    for (int i = start_frame_id; i < end_frame_id; ++i) {
        Eigen::Matrix3d Rcw = camera_pose[i].Rwc.transpose();
        Eigen::Vector3d Pc = Rcw * (Pw - camera_pose[i].twc);//transform the point in world coordinate to camera

        double x = Pc.x();
        double y = Pc.y();
        double z = Pc.z();

        camera_pose[i].uv = Eigen::Vector2d(x/z,y/z);
    }
    
    /// TODO::homework; 请完成三角化估计深度的代码
    // 遍历所有的观测数据，并三角化
    
    Eigen::Vector3d P_est;           // 结果保存到这个变量
    P_est.setZero();
    /* your code begin */
    int n = (end_frame_id - start_frame_id); 
    Eigen::Matrix<double,Eigen::Dynamic, 4> D_mat;
    D_mat.resize(2*n,4);
    for (int i=0;i<n;i++)
    {
        int cam_idx = start_frame_id+i;
        std::cout<<"uv "<<camera_pose[cam_idx].uv<<std::endl;
        //std::cout<<"camera pose "<<i<<": "<<std::endl<<camera_pose[cam_idx].getPwc()<<std::endl;
        D_mat.block(i*2,0,1,4) = camera_pose[cam_idx].uv[0]* camera_pose[cam_idx].getPcw().block(2,0,1,4)- camera_pose[cam_idx].getPcw().block(0,0,1,4);
        D_mat.block(i*2+1,0,1,4) = camera_pose[cam_idx].uv[1]* camera_pose[cam_idx].getPcw().block(2,0,1,4)- camera_pose[cam_idx].getPcw().block(1,0,1,4);
    }
    Eigen::Matrix<double,4, 4> DTD= D_mat.transpose()*D_mat;
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(DTD, Eigen::ComputeThinU | Eigen::ComputeThinV);
    std::cout<<"V is "<<svd.matrixV()<<std::endl;
    std::cout<<"U is "<<svd.matrixU()<<std::endl;
    std::cout << "Its singular values are:" <<std::endl<< svd.singularValues() << std::endl;
    P_est = svd.matrixV().block(0,3,3,1)*1.0/svd.matrixV()(3,3);//u4




 
    /* your code end */
    
    std::cout <<"ground truth: \n"<< Pw.transpose() <<std::endl;
    std::cout <<"your result: \n"<< P_est.transpose() <<std::endl;
    // TODO:: 请如课程讲解中提到的判断三角化结果好坏的方式，绘制奇异值比值变化曲线
    return 0;
}
