#include <iostream>
#include <opencv2/opencv.hpp>
#include <glog/logging.h>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <../ugv-share/defines.h>

using namespace std;
using namespace victl;

bool b2v(string bfile, string vfile);
bool v2b(string vfile, string bfile);

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        cout << "Error: too few arguments'" << endl
             << "./Usage: v3bconverter [-v/-b]'in_file' 'output_file'" << endl;
        return -1;
    }
    if(!strcmp(argv[1], "-b"))
    {
        return v2b(argv[2], argv[3]);
    }else if(!strcmp(argv[1], "-v"))
    {
        return b2v(argv[2], argv[3]);
    }else{
        return b2v(argv[1], argv[2]);
    }
    return 0;
}

bool b2v(string bfile, string vfile)
{
    struct stat buffer;
    if(stat(bfile.c_str(), &buffer))
    {
        DLOG(ERROR) << "3b file not exists: " << bfile;
        return false;
    }
    cv::Mat b = cv::imread(bfile);
    if(!b.data)
    {
        DLOG(ERROR) << "Couldn't open 3b file: " << bfile;
        return false;
    }
    cv::Mat v(b.rows, b.cols, CV_8UC3, cv::Scalar(127, 127, 127));
    for(int row = 0; row < b.rows; ++row)
    {
        for(int col = 0; col < b.cols; ++col)
        {
            unsigned char base = b.at<cv::Vec3b>(row, col)[0];
            if(isPresent(base, ROADEDGE_OCCUPIED))
            {
                v.at<cv::Vec3b>(row, col) = cv::Vec3b(0,0,0);
            }
            else if(isPresent(base, ROADEDGE_CLEAR)) {
                v.at<cv::Vec3b>(row, col) = cv::Vec3b(255,255,255);
            }
//            if (isPresent(base, LANELINE_CAMERA))
//            {
//                v.at<cv::Vec3b>(row, col) = cv::Vec3b(0,255,0);
//            }
//            if (isPresent(base, STOPLINE_YES))
//            {
//                v.at<cv::Vec3b>(row, col) = cv::Vec3b(0,0,255);
//            }
        }
    }
    return !cv::imwrite(vfile, v);
}

bool v2b(string vfile, string bfile)
{
    struct stat buffer;
    if(stat(vfile.c_str(), &buffer) || stat(bfile.c_str(), &buffer))
    {
        DLOG(ERROR) << "visualize file or 3b file not exists: " << vfile << " and " << bfile;
        return false;
    }
    cv::Mat b = cv::imread(bfile);
    cv::Mat v = cv::imread(vfile);
    if(!b.data || !v.data)
    {
        DLOG(ERROR) << "Couldn't open 3b or visualize file: " << bfile << " and " << vfile;
        return false;
    }
    for(int row = 0; row < b.rows; ++row)
    {
        for(int col = 0; col < b.cols; ++col)
        {
            unsigned char &base = b.at<cv::Vec3b>(row, col)[0];
            unsigned char &v_vaule = v.at<cv::Vec3b>(row, col)[0];
            switch(v_vaule)
            {
            case 0:
                base |= ROADEDGE_OCCUPIED;
                base &= (~ROADEDGE_CLEAR);
                break;
            case 255:
                base |= ROADEDGE_CLEAR;
                base &= (~ROADEDGE_OCCUPIED);
                break;
            }
        }
    }
    return !cv::imwrite("out-"+bfile, b);

    return false;
}
