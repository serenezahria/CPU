#ifndef JOBPROCESSOR_H
#define JOBPROCESSOR_H
#include "jobGenerator.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
struct jobData {
  Job job;
  int remainingTime; // remaining time when interrupted job is removed from the
                     // processor
  // constructor
  jobData(const Job &job) : job(job), remainingTime(job.processingTime) {}
};

class jobProcessor {
private:
  vector<Job> data;
  queue<jobData> jobQueue; // priority queue for job data if processors are full
  std::vector<int> processors; // stores remaing time for each processor
  // metrics
  int totalTimeInQueue;
  int maxJobsInQueue;
  int totalJobsCompleted;
  int totalTimeCpuProcessing;
  int totalTimeCpuIdle;
  int totalJobsInQueue;
  vector<int> totalJobsArrived;

public:
  // constructor
  jobProcessor(int numProcessors);
  void simulation();
  void updateProcessorCout(int time);
  void logOutput(int time, ofstream &logFile);

  void updateMetrics();
  void logActions();
  void readFileData();
  void somemethod();
};

#endif // JOBPROCESSOR_H