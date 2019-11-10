#include <iostream>
#include <cmath>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>

int main()
{
  Eigen::Matrix3d rot_mat = Eigen::Matrix3d::Identity();
  cout<<rot_mat<<endl;
  //旋转向量
  Eigen::AngleAxisd rot_vec (M_PI/4,Eigen::Vector3d(0,0,1));
  cout .precision(3);
  cout<<rot_vec.matrix()<<endl;
  //也可以直接赋值  
  rot_mat=rot_vec.toRotationMatrix();
  cout<<rot_mat<<endl;

  Eigen::Vector3d v (1,0,0);
  //用旋转向量计算
  Eigen::Vector3d v_roted = rot_vec *v;
  cout<<v_roted.transpose()<<endl;
  //旋转矩阵计算效果相同
  v_roted = rot_mat*v;
  cout<<v_roted.transpose()<<endl;
  
  //欧拉角：可以将旋转矩阵直接转为欧拉角
  Eigen::Vector3d euler_angles = rot_mat.eulerAngles(2,1,0);// ZYX顺序，即roll pitch yaw
  cout<<"yaw pitch roll = "<<euler_angles.transpose()<<endl;
  
  //欧式变换矩阵使用 Isometry
  Eigen::Isometry3d T = Eigen::Isometry3d::Identity();//实际为4*4的矩阵
  T.rotate(rot_vec);
  T.pretranslate(Eigen::Vector3d(1, 3, 4));
  cout<<"Tranform matrix = \n"<<T.matrix()<<endl;

  //用变换矩阵进行坐标变换
  Eigen::Vector3d v_trans = T*v;
  cout<<"v transformed = "<<v_trans.transpose()<<endl;

  //仿射和射影变换使用 Affine3d 和 Projective3d

  //四元数 可以使用旋转向量也可以使用旋转矩阵
  Eigen::Quaterniond q = Eigen::Quaterniond (rot_vec);
  cout<< "quaternion = \n"<<q.coeffs()<<endl;
  //coeffs顺序为(x,y,z,w),w为实部，前三者为虚部
  q = Eigen::Quaterniond(rot_mat);
  cout<<"quaternion = \n"<<q.coeffs()<<endl;
  v_roted = q*v;
  cout<< "(1,0,0) after rotation = "<<v_roted.transpose()<<endl;

  return 0;

}
