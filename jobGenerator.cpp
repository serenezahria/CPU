#include "jobGenerator.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

void generateData::generateJobData(){
  ofstream dataFile("jobData.txt");
  srand(time(nullptr));
  //generate data
  auto generateJobData=[&](char jobType, int maxJobs, int arrivalTime, int arrivalRange, int processingTime, int processingRange){
    int lastjobTime=0;//time since last job arrived
    for(int i=0;i<maxJobs;i++){
      int arrTime=(arrivalTime+rand()% arrivalRange)+lastjobTime;
      lastjobTime=arrTime;
      
      int procTime=(processingTime+rand()% processingRange)+lastjobTime;

      //prints data to file
      dataFile<<jobType<<" "<<arrTime<<" "<<procTime<<endl;
    }

    
  };
 
  //generate data with specific processing times for each job type
   generateJobData('A',2400,5,3,3,2);//A jobs
   generateJobData('B',1650,7,3,5,3);//B jobs
   generateJobData('C',1100,11,7,9,3);//C jobs
   generateJobData('D',1200,9,5,12,10);//D jobs

  dataFile.close();
}
