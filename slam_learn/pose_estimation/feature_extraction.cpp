#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  
  if (argc!=3) {
    cout<<"usage: demp img1 img2"<<endl;
	return 1;
  }
  Mat img1 = imread(argv[1], 1);
  Mat img2 = imread(argv[2], 1);

  //初始化
  vector<KeyPoint> kp1,kp2;
  Mat dp1,dp2;
  Ptr<FeatureDetector> detector = ORB::create();
  Ptr<DescriptorExtractor> descriptor = ORB::create();
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
 
  //第一步，检测Oriented FAST角点位置
  detector->detect(img1, kp1);
  detector->detect(img2, kp2);

  //第二步，根据角点位置计算BRIEF描述子
  descriptor->compute (img1, kp1, dp1);
  descriptor->compute (img2, kp2, dp2);

  Mat outimg1;
  drawKeypoints(img1, kp1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
  imshow("ORB特征点", outimg1);
  waitKey(0);

  //第三步，对两幅图中的BRIEF描述子进行匹配，使用Hamming距离
  vector<DMatch> matches;
  matcher->match (dp1, dp2, matches);

  //第四步，匹配点对筛选,计算所有匹配之间的最小距离和最大距离
  double min_dist = 10000, max_dist=0;

  for (int i = 0; i<dp1.rows; ++i) {
    double dist = matches[i].distance;
	if (dist<min_dist) min_dist = dist;
	if (dist>max_dist) max_dist = dist;
  }

  printf("Maxdist : %f \n",max_dist);
  printf("Mindist : %f \n",min_dist);

  vector<DMatch> good_matches;
  for(int i=0; i<dp1.rows;++i) {
    if (matches[i].distance <= max(2*min_dist, 30.0))
	  good_matches.push_back(matches[i]);
  }

  //第五步，绘制结果
  Mat img_match;
  Mat img_goodmatch;
  drawMatches (img1, kp1, img2, kp2, matches, img_match);
  drawMatches (img1, kp1, img2, kp2, good_matches, img_goodmatch);
  imshow("所有匹配点对", img_match);
  imshow("优化匹配点对", img_goodmatch);
  waitKey(0);

  return 0;
}
