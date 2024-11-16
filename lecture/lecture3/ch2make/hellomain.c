#include "hellofunc.h"

int main(int argc, char** argv) {
  // call a function in another file
  if(argc ==2) myPrintHelloMake(argv[1]);
  else myPrintHelloMake("nothing");


  return(0);
}