#include <iostream>

using namespace std;
#include <ctime>

#include <Eigen/Core>
#include <Eigen/Dense>

#define MATRIX_SIZE 50

int main()
{
  //模板类 Eigen::Matrix<类型，行，列>
  Eigen::Matrix<float, 2, 3> mat_23f;

  //下面两行完全相同，都表示列向量
  Eigen::Vector3d v_3d;
  //Eigen::Matrix<double, 3, 1> v_3d;

  Eigen::Matrix<float, 3, 1> v_3f;

  //Matrix3d 相当于 Matrix<double,3,3>
  Eigen::Matrix3d mat_33d = Eigen::Matrix3d::Zero();//初始化为零

  mat_23f << 1,2,3,4,5,6;
  cout<<mat_23f<<endl;

  for(int i=0;i<2;++i)
  {
    for (int j=0;j<3;++j)
	  cout<<mat_23f(i,j)<<"\t";
	cout<<endl;
  }

  v_3d<<3,2,1;
  v_3f<<4,5,6;

  Eigen::Matrix<double,2,1> res = mat_23f.cast<double>() * v_3d;
  
  Eigen::Matrix<float,2,1> res2 = mat_23f * v_3f;
  

  return 0;


}
