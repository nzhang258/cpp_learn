#include <iostream>
#include <vector>

#include <ctime>
// Eigen库
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
//Opencv库
#include <opencv2/core/core.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char** argv) {
  cv::Mat image0,image;
  int h,w,c;
  int size;

  image0 = cv::imread(argv[1]);
  cv::resize(image0, image, cv::Size(0, 0), 0.5, 0.5);
 
  h = image.rows;
  w = image.cols;
  c = image.channels();
  size = h*w;
  cout<<" h, w, c is "<<h<<" "<<w<<" "<<c<<endl;
  
  //有时可能会希望生成mask
  cv::Mat mask = (image<=100);

  ///  **************************************************************   ///
  //把cv的Mat值存到eigen矩阵里
  Eigen::VectorXf v_1(size);
  Eigen::VectorXf v_2(size);
  //方法1：直接读Mat数据，使用.at
  for (int i=0; i<h; ++i) {
    for (int j=0; j<w; ++j) {
	  float tmp;
	  int idx = i*w+j;
	  tmp = image.at<cv::Vec3b>(i,j)[0];
	  v_1(idx) = tmp;
	}
  }

  //方法2：使用指针进行，比较简单，推荐！！但这种情况需要先进行图片格式的转换
  cv::Mat image_f;
  image.convertTo(image_f, CV_32FC3);
  float *d_img_f = (float*)image_f.data;
  for (int i=0; i<h; ++i) {
    for (int j=0; j<w; ++j) {
	  int idx = (i*w+j);
	  int id3 = idx*c;
	  v_2(idx) = d_img_f[id3];
	}
  }
  for (int i=0;i<10;++i)
    cout<<v_1(i)<<"  "<<v_2(i)<<endl;
  ///  **************************************************************  ///

  //将eigen的矩阵转成opencv的mat进行可视化  
  cv::Mat res1(h, w, CV_8UC3);
  cv::Mat res2(h, w, CV_8UC3);
  res1=0;
  res2=0;
  for (int ch=0; ch <c; ++ch) {
    for (int a=0; a<size; ++a) {
      uchar *bgr1 = res1.data + a*3;
	  float tmp1 = v_1(a);
	  bgr1[ch] = tmp1;
      uchar *bgr2 = res2.data + a*3;
	  float tmp2 = v_2(a);
	  bgr2[ch] = tmp2;
	}
  }





  cv::imshow("test",image);
  cv::waitKey();
  cv::imshow("mask",mask);
  cv::waitKey();
  cv::imshow("res1",res1);
  cv::waitKey();
  cv::imshow("res2",res2);
  cv::waitKey();
  return 0;
}
