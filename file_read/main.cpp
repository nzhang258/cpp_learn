#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

  string test_word1,test_word2;
  ifstream ifs(argv[1]);

  getline(ifs, test_word1);
  getline(ifs, test_word2);

  cout<<test_word1<<endl<<test_word2<<endl;

  ifs.close();

  return 0;
}

