
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <iomanip>

#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <eigen3/Eigen/Dense>
#include "System.h"

using namespace std;
using namespace cv;
using namespace Eigen;

const int nDelayTimes = 2;
string sData_path = "/home/dataset/EuRoC/MH-05/mav0/";
string sConfig_path = "../config/";

std::shared_ptr<System> pSystem;



void ParseImuData()
{
	string sImu_data_file = sConfig_path + "imu_pose.txt";
	cout << "I parse imu data from file : " << sImu_data_file << endl;
	ifstream fsImu;
	fsImu.open(sImu_data_file.c_str());
	if (!fsImu.is_open())
	{
		cerr << "Failed to open imu file! " << sImu_data_file << endl;
		return;
	}

	std::string sImu_line;
	double StampNSec = 0.0;
	Vector3d vAcc;
	Vector3d vGyr;
	double _;//read useless data
	while (std::getline(fsImu, sImu_line) && !sImu_line.empty()) // read imu data
	{
		std::istringstream ssImuData(sImu_line);
		ssImuData >> StampNSec >>_>>_>>_>>_>>_>>_>>_ >>vGyr.x() >> vGyr.y() >> vGyr.z() >>vAcc.x() >> vAcc.y() >> vAcc.z();
		cout << "Imu t: " <<StampNSec << " gyr: " << vGyr.transpose() << " acc: " << vAcc.transpose() << endl;
		//pSystem->PubImuData(StampNSec, vGyr, vAcc);
		usleep(5000*nDelayTimes);
	}
	fsImu.close();
}
vector<double> ParseImageTimestamp()
{
	vector<double> ts_vec;

	string sImgTImestampFile = sConfig_path + "cam_pose.txt";

	cout << "Parse timestamp file: " << sImgTImestampFile << endl;

	ifstream fsCam;
	fsCam.open(sImgTImestampFile.c_str());
	if (!fsCam.is_open())
	{
		cerr << "Failed to open camera file! " << sImgTImestampFile << endl;
		return ts_vec;
	}
	std::string ts_line;
	double StampSec = 0.0;
 	while (std::getline(fsCam, ts_line) && !ts_line.empty()) // read imu data
	{
		std::istringstream ssTimestamp(ts_line);
		ssTimestamp >> StampSec;
		cout << "timestamp: " <<StampSec << endl;
		ts_vec.push_back(StampSec);
		//pSystem->PubImuData(StampNSec, vGyr, vAcc);
		usleep(5000*nDelayTimes);
	}
	fsCam.close();

	return ts_vec;
}
/**
 * @brief pass the image timestamp and feature points info to the pSystem
 * 
 */
void ParseImageData()
{
	vector<double> ts_data = ParseImageTimestamp();
	//define a image pair
	vector<pair<double,double>> feature_pts;

	int ts_size= ts_data.size();
	cout << "In total timestamp number of imgs is  " <<ts_size << endl;


	std::string sImage_line;
	double dStampNSec;
	string sImgFileName;
	
	// cv::namedWindow("SOURCE IMAGE", CV_WINDOW_AUTOSIZE);
	//while (std::getline(fsImage, sImage_line) && !sImage_line.empty())
	for (int i=0;i<ts_size;i++)
	{

		std::stringstream file;
        file<<"/keyframe/all_points_"<<i<<".txt";
		string filename = file.str();
		string imagePath = sConfig_path +filename;
		std::cout<<imagePath<<std::endl;
		ifstream fsImg;
		fsImg.open(imagePath.c_str());
		if (!fsImg.is_open())
		{
			cerr << "Failed to open image file! " << imagePath << endl;
			return;
		}
		std::string sImg_line;
		double _;//read useless data
		int image_w = 640;
		Mat image_frame = Mat::zeros( image_w, image_w, CV_8UC3 );

		while (std::getline(fsImg, sImg_line) && !sImg_line.empty()) // read imu data
		{
			pair<double,double> img_pair;
			std::istringstream ssImgData(sImg_line);
			double f = 460;
			double cx= 255;
			ssImgData >>_>>_>>_>>_>>img_pair.first >> img_pair.second;
			cout << img_pair.first*f+cx<<", "<<img_pair.second*f+cx<< endl;
			auto center = cv::Point(img_pair.first*f+cx,img_pair.second*f+cx);
        	cv::circle(image_frame, center,1,CV_RGB(255,0,0),3);
			//pSystem->PubImuData(StampNSec, vGyr, vAcc);
			//usleep(5000*nDelayTimes);
		}
		fsImg.close();


		// Mat img = imread(imagePath.c_str(), 0);
		// if (img.empty())
		// {
		// 	cerr << "image is empty! path: " << imagePath << endl;
		// 	return;
		// }

		// pSystem->PubImageData(dStampNSec / 1e9, img);
		cv::imshow("SOURCE IMAGE", image_frame);
		cv::waitKey(1);
		usleep(5000*nDelayTimes);
	}
	
}

void PubImageData()
{
	string sImage_file = sConfig_path + "MH_05_cam0.txt";

	cout << "1 PubImageData start sImage_file: " << sImage_file << endl;

	ifstream fsImage;
	fsImage.open(sImage_file.c_str());
	if (!fsImage.is_open())
	{
		cerr << "Failed to open image file! " << sImage_file << endl;
		return;
	}

	std::string sImage_line;
	double dStampNSec;
	string sImgFileName;
	
	// cv::namedWindow("SOURCE IMAGE", CV_WINDOW_AUTOSIZE);
	while (std::getline(fsImage, sImage_line) && !sImage_line.empty())
	{
		std::istringstream ssImuData(sImage_line);
		ssImuData >> dStampNSec >> sImgFileName;
		// cout << "Image t : " << fixed << dStampNSec << " Name: " << sImgFileName << endl;
		string imagePath = sData_path + "cam0/data/" + sImgFileName;

		Mat img = imread(imagePath.c_str(), 0);
		if (img.empty())
		{
			cerr << "image is empty! path: " << imagePath << endl;
			return;
		}
		pSystem->PubImageData(dStampNSec / 1e9, img);
		// cv::imshow("SOURCE IMAGE", img);
		// cv::waitKey(0);
		usleep(50000*nDelayTimes);
	}
	fsImage.close();
}

 

int main(int argc, char **argv)
{
	// if(argc < 2)
	// {
	// 	cerr << "./run_euroc PATH_TO_FOLDER/MH-05/mav0 PATH_TO_CONFIG/config \n" 
	// 		<< "For example: ./run_euroc /home/stevencui/dataset/EuRoC/MH-05/mav0/ ../config/"<< endl;
	// 	return -1;
	// }
	sData_path ="useless junk";
	//sConfig_path = argv[2];
	sConfig_path = "../sensor_data/";//TODO change it 
	//ParseImageTimestamp();


	//pSystem.reset(new System(sConfig_path));
	
	//std::thread thd_BackEnd(&System::ProcessBackEnd, pSystem);
		
	// sleep(5);
	//std::thread thd_ParseImuData(ParseImuData);
	//thd_ParseImuData.join();
	 std::thread thd_PubImageData(ParseImageData);

	// std::thread thd_Draw(&System::Draw, pSystem);
	// thd_PubImuData.join();
	thd_PubImageData.join();

	// thd_BackEnd.join();
	// thd_Draw.join();

	cout << "main end... see you ..." << endl;
	return 0;
}
