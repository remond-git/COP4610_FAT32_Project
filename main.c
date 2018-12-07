#include "program.h"
#include "utils.h"
#include "dirparse.h"
#include "parse.h"

int main(int argc, const char * argv[]) {
  OpenImageFile(argv[1]);
  ParseBootSector();
  SetCurrentDirectoryClusterNum(GetRootClusterNum());
  RunProgram();
  return 0; 
}
