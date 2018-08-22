#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../utility/opencv/helper.cpp"
#include <iostream>
#include <opencv2/opencv.hpp>

#include <vector>

using namespace cv;
using namespace std;


void pipeline(Mat &image){
  Size img_size = image.size();
  Point p_1(0, img_size.height);
  Point p_2(img_size.width/2,img_size.height/2); 
  Point p_3(img_size.width,img_size.height); 
  vector<Point> vertices;
  vertices.push_back(p_1);
  vertices.push_back(p_2);
  vertices.push_back(p_3);
  vector <vector <Point> > roi_pts;
  roi_pts.push_back(vertices); 
  Mat play_image = OpencvHelper::convertToGrayScale(image);
  //Mat blur_image = OpencvHelper::gaussianBlur(bw_image, 7, 7, 5);
  play_image = OpencvHelper::gaussianBlur(play_image, 5, 5, 5);
  //OpencvHelper::display(play_image, "temp_window");
  play_image =  OpencvHelper::canny_edge(play_image, 50, 3);
  play_image = OpencvHelper::getRoI(play_image, roi_pts);
  OpencvHelper::display(play_image, "temp_window");
  //OpencvHelper::display(mask_image, "temp_window");
  //OpencvHelper::ApplyMask(bw_image, color_mask);
  double rho = 2;
  double theta = CV_PI/180;
  int threshold = 15;
  double minLineLength = 40;
  double maxLineGap = 20;
  vector<Vec4i> lines = OpencvHelper::GetHoughLines(play_image,rho, theta, threshold, minLineLength, maxLineGap, true);
  
}

int main(){
  Mat image = imread("test_images/solidWhiteCurve.jpg");
  pipeline(image);
  return 0;
}
