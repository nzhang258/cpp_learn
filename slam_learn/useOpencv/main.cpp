#include<iostream>
#include <chrono>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char** argv) {
  
  cv::Mat image;
  image = cv::imread(argv[1]);
  if (image.data == nullptr) {
    cerr<<argv[1]<<"不存在"<<endl;
	return 0;
  }

  cout<<"h, w, c = "<<image.rows<<" "<<image.cols<<" "<<image.channels()<<endl;
  cv::imshow("image",image);
  cv::waitKey(0);
  
  
  cout<<"img type"<<image.type()<<endl;
  cout<<CV_32FC3<<" "<<CV_8UC3<<endl;
  cout<<image.type()<<endl;
  
  //chrono用来计时
  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  for (size_t y=0; y<image.rows; ++y) {
    //cv::Mat::ptr图像行指针
    unsigned char* row_ptr = image.ptr<unsigned char>(y);//第y行的头指针
	for (size_t x=0; x<image.cols;++x) {
	  unsigned char* data_ptr = &row_ptr[x*image.channels()];//访问x,y处的像素
	  for (int c=0; c!=image.channels();++c)
	     unsigned char data = data_ptr[c];
	}
  }
  chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
  chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2-t1);
  cout<<"遍历图像用时："<<time_used.count()<<"秒"<<endl;

  cv::Mat img_another = image;
  //直接赋值，会修改原始数据
  img_another(cv::Rect(30,70,100,100)).setTo(0);
  cv::imshow("img",image);
  cv::waitKey(0);

  //使用clone函数来拷贝数据
  cv::Mat img_clone = image.clone();
  img_clone(cv::Rect(0,0,100,100)).setTo(255);
  cv::imshow("image_clone",img_clone);
  cv::waitKey(0);

  cv::destroyAllWindows();
  return 0;

}
