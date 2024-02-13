#ifndef JOBGENERATOR_H
#define JOBGENERATOR_H
#include <string>
#include <vector>
#include <queue>
#include <iostream>
//Structure for job information 
struct Job{
char jobType;
int arrivalTime;
int processingTime;

//Constructor
Job(char type, int arrival, int processing){
jobType=type;
arrivalTime=arrival;
processingTime=processing;
  }
};

class generateData{
public:
void generateJobData();
};

#endif //JOBGENERATOR_H 