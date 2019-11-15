#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

void find_feature_matches(
  const Mat& img1, const Mat& img2,
  vector<KeyPoint>& kp1,
  vector<KeyPoint>& kp2,
  vector<DMatch>& matches);

void pe2d2d( vector<KeyPoint> kp1,
             vector<KeyPoint> kp2,
			 vector<DMatch> matches,
			 Mat& R, Mat& t);

Point2d pixel2cam( const Point2d& p, const Mat& K);


int main(int argc, char** argv) {
  
  if(argc!=3) {
    cout<<"usage: pe2d2d img1 img2"<<endl;
	return 1;
  }
  Mat img1 = imread(argv[1],1);
  Mat img2 = imread(argv[2],1);

  vector<KeyPoint> kp1,kp2;
  vector<DMatch> matches;
  find_feature_matches(img1,img2,kp1,kp2,matches);
  cout<<"一共找到了"<<matches.size()<<"组匹配点"<<endl;

  Mat R,t;
  pe2d2d(kp1, kp2, matches, R, t);

  Mat tx = ( Mat_<double> (3,3) <<
             0, -t.at<double>(2,0), t.at<double>(1,0),
             t.at<double>(2,0), 0, -t.at<double>(0,0),
             -t.at<double>(1,0), t.at<double>(0,0), 0 );
  cout<<"t^R="<<endl<<tx*R<<endl;

  double *d_t = (double*) t.data;
  Mat txx = ( Mat_<double> (3,3) <<
              0, -d_t[2], d_t[1], d_t[2], 0, -d_t[0], -d_t[1], d_t[0], 0);
  cout<<"txxR"<<endl<<txx*R<<endl;

  //验证対极约束
  Mat K = (Mat_<double> (3,3) <<520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);
  for (DMatch m: matches) {
    Point2d pt1 = pixel2cam(kp1[m.queryIdx].pt, K);
	Mat y1 = ( Mat_<double>(3,1) << pt1.x, pt1.y, 1);
    Point2d pt2 = pixel2cam(kp2[m.trainIdx].pt, K);
	Mat y2 = ( Mat_<double>(3,1) << pt2.x, pt2.y, 1);
	Mat d = y2.t() * tx * R * y1;
	cout<<"对极约束 "<<d<<endl;
  }


  return 0;
}

void find_feature_matches ( const Mat& img1, const Mat& img2,
    						vector<KeyPoint>& kp1,
	  						vector<KeyPoint>& kp2,
	    					vector<DMatch>& matches)
{
  Mat dp1,dp2;
  Ptr<FeatureDetector> detector = ORB::create();
  Ptr<DescriptorExtractor> descriptor = ORB::create();
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

  detector->detect (img1, kp1);
  detector->detect (img2, kp2);

  descriptor->compute( img1, kp1, dp1);
  descriptor->compute( img2, kp2, dp2);

  vector<DMatch> match;
  matcher->match( dp1, dp2, match);

  double min_d = 10000,max_d = 0;

  for (int i = 0; i < dp1.rows; ++i) {
    double dist = match[i].distance;
	if(dist < min_d) min_d = dist;
	if(dist > max_d) max_d = dist;
  }

  printf("max dist : %f \n",max_d);
  printf("min dist : %f \n",min_d);

  for (int i =0; i < dp1.rows;++i) {
    if (match[i].distance <= max( 2*min_d, 30.0 ))
	  matches.push_back(match[i]);
  }

}

void pe2d2d( vector<KeyPoint> kp1,
             vector<KeyPoint> kp2,
			 vector<DMatch> matches,
			 Mat& R, Mat& t) {
  //相机内参
  Mat K = ( Mat_<double> (3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);
  
  vector<Point2f> pts1;
  vector<Point2f> pts2;

  for ( int i=0; i<(int) matches.size();++i) {
    pts1.push_back(kp1[matches[i].queryIdx].pt);
    pts2.push_back(kp2[matches[i].trainIdx].pt);
  }

  //计算基础矩阵
  Mat fund_mat;
  fund_mat = findFundamentalMat(pts1, pts2, CV_FM_8POINT);
  cout<<"基础矩阵F为 "<<endl<<fund_mat<<endl;

  //计算本质矩阵
  Point2d prin_pt(325.1, 249.7);
  double f_len = 521;
  Mat ess_mat;
  ess_mat = findEssentialMat (pts1, pts2, f_len, prin_pt);
  cout<<"本质矩阵E为 "<<endl<<ess_mat<<endl;

  //计算单应矩阵
  Mat hom_mat;
  hom_mat = findHomography(pts1, pts2, RANSAC, 3);
  cout<<"单应矩阵H为 "<<endl<<hom_mat<<endl;

  //从本质矩阵E恢复旋转和平移信息
  recoverPose ( ess_mat, pts1, pts2, R, t, f_len, prin_pt);
  cout<<"R "<<endl<<R<<endl;
  cout<<"t "<<endl<<t<<endl;

}

Point2d pixel2cam(const Point2d& p, const Mat& K) {
  return Point2d (
      (p.x - K.at<double> (0,2))/K.at<double>(0,0),
      (p.y - K.at<double> (1,2))/K.at<double>(1,1)
	);
}
