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
  cout<<res<<endl;
  cout<<res2<<endl;

  //随机数矩阵
  mat_33d = Eigen::Matrix3d::Random();
  cout<<"随机数矩阵"<<endl;
  cout<<mat_33d<<endl<<endl;
  cout<<"转置"<<endl;
  cout<<mat_33d.transpose()<<endl;
  cout<<"求和"<<endl;
  cout<<mat_33d.sum()<<endl;
  cout<<"迹"<<endl;
  cout<<mat_33d.trace()<<endl;
  cout<<"数乘10"<<endl;
  cout<<10*mat_33d<<endl;
  cout<<"逆"<<endl;
  cout<<mat_33d.inverse()<<endl;
  cout<<"行列式"<<endl;
  cout<<mat_33d.determinant()<<endl;

  //特征值和特征向量
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigen_solver(mat_33d.transpose()*mat_33d);
  cout<<"Eigen values = \n" <<eigen_solver.eigenvalues()<<endl;
  cout<<"Eigen vectors = \n"<<eigen_solver.eigenvectors()<<endl;

  
  //求解方程
  Eigen::Matrix<double, MATRIX_SIZE, MATRIX_SIZE> mat_NNd;
  mat_NNd = Eigen::MatrixXd::Random(MATRIX_SIZE,MATRIX_SIZE);
  Eigen::Matrix<double,MATRIX_SIZE,1> v_Nd;
  v_Nd = Eigen::MatrixXd::Random(MATRIX_SIZE,1);
  
  clock_t time_stt = clock();
  //直接求逆
  Eigen::Matrix<double,MATRIX_SIZE,1> x = mat_NNd.inverse()*v_Nd;
  cout<<"直接求逆时间为"<<1000*(clock()-time_stt)/(double)CLOCKS_PER_SEC<<"ms"<<endl;
  //矩阵分解，如QR分解
  time_stt = clock();
  x = mat_NNd.colPivHouseholderQr().solve(v_Nd);
  cout<<"矩阵分解时间为"<<1000*(clock()-time_stt)/(double)CLOCKS_PER_SEC<<"ms"<<endl;

  return 0;


}
