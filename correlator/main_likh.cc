#include <fstream>
#include <iostream>
#include "correlator_likh.h"


int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage:" << std::endl
              << "Program INPUT_FILE OUTPUT_FILE" << std::endl;
    return 1;
  }

  Correlator c;
  c.setsize(32, 16, 2);
  c.initialize();

  std::ifstream fin;
  double val;
  fin.open(argv[1]);
  if (!fin.is_open() || fin.bad()) {
    std::cout << "Error opening file!" << std::endl;
    return 1;
  }
  while (!fin.eof()) {
    fin >> val;
    c.add(val);
  }
  fin.close();

  std::ofstream fout;
  fout.open(argv[2]);
  c.evaluate();
  for (unsigned int i = 0; i < c.npcorr; ++i)
    fout << c.t[i] << " " << c.f[i] << std::endl;
  fout.close();

  return 0;
}
