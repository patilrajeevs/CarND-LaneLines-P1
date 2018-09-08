#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cv.h>
#include <vector>
#include <iostream>

#include "../utility/opencv/opencv_helper.h"
#include "../utility/geometry/geometry.h"

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
  //OpencvHelper::display(play_image, "temp_window");
  //OpencvHelper::display(mask_image, "temp_window");
  //OpencvHelper::ApplyMask(bw_image, color_mask);
  double rho = 2;
  double theta = CV_PI/180;
  int threshold = 15;
  double minLineLength = 40;
  double maxLineGap = 20;
  vector<Vec4i> lines = OpencvHelper::GetHoughLines(play_image,rho, theta, threshold, minLineLength, maxLineGap, true);
  vector<Line> identified_lines;
  for (vector<Vec4i>::iterator it = lines.begin(); it != lines.end(); ++it){
    identified_lines.push_back(Line(*it));
  }
  map <int, vector <Line> > line_groups;
  Geometry::GroupLinesBySlopeIntercept(identified_lines, line_groups, 2);
  for (map <int, vector <Line> >::iterator it = line_groups.begin(); it != line_groups.end(); ++it){
      Line averaged_line = Geometry::AverageLines(it->second);
      averaged_line.py1 = img_size.height;
      averaged_line.py2 = img_size.height/2;
      averaged_line.px1 = averaged_line.GetXInterceptAt(averaged_line.py1);
      averaged_line.px2 = averaged_line.GetXInterceptAt(averaged_line.py2);
      line(image, Point(averaged_line.px1,averaged_line.py1), Point(averaged_line.px2,averaged_line.py2),CvScalar(0,255,0));
  /*
      CvScalar color(0,255,0);
      vector <Line> temp_vector = it->second;
      for (vector<Line>::iterator it_line = temp_vector.begin(); it_line != temp_vector.end(); ++it_line){
        line(image, Point((*it_line).px1,(*it_line).py1), Point((*it_line).px2,(*it_line).py2),CvScalar(0,255,0));
        color = (0,0,255);
      }
    */
  }
  OpencvHelper::display(image, "temp_window");
}

int main(){
  Mat image = imread("test_images/solidWhiteCurve.jpg");
  pipeline(image);
  return 0;
}
