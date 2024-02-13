#include <iostream>
#include "jobGenerator.h"
#include "jobProcessor.h"
using namespace std;

int main(){
 // generateData data;
 // data.generateJobData(); (ran once already, data file was created)
  

  int numProcessors=8;
  jobProcessor processor(numProcessors);
  processor.simulation();
 // processor.logActions();
  return 0;

}
