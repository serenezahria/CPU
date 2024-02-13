#include "jobProcessor.h"
#include "jobGenerator.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// intitilizes constructor values to 0
jobProcessor::jobProcessor(int numProcessors)
    : processors(numProcessors, 0), totalTimeInQueue(0), maxJobsInQueue(0),
      totalJobsCompleted(0), totalTimeCpuProcessing(0), totalTimeCpuIdle(0),
      totalJobsInQueue(0), totalJobsArrived(4, 0) {
  // set processors to idle
  for (int i = 0; i < numProcessors; i++) {
    processors[i] = 0;
  }
}

void jobProcessor::simulation() {
  readFileData();
  int currentTime = 0;
  while (currentTime <= 10000) {
    // updates processor status at each time increment
    updateProcessorCout(currentTime);
    
    if (currentTime <=
        500) { // run for 500 time units to generate initial report metrics
      for (const auto &job : data) { // loop through each job in dataFile
        if (job.arrivalTime == currentTime) { // if job arrives at current time
          bool processorAvailable = false;
          for (size_t i = 0; i < processors.size();
               i++) { // iterates through available processors
            if (processors[i] ==
                0) { // if the processor is idle(it is avaliable)
              processorAvailable = true;
              processors[i] = job.processingTime; // assigns job processing time
                                                  // to processor
              totalJobsCompleted++;
              totalJobsArrived[job.jobType - 'A']++;
              break;
            }
          }
          if (!processorAvailable) { // no processor available
            jobQueue.push(jobData(job));
            totalJobsInQueue++;
            if (totalJobsInQueue > maxJobsInQueue) {
              maxJobsInQueue = totalJobsInQueue;
            }
          }
        }
      }
    } else if (currentTime > 500) { // after initial report metrics are
                                    // generated
      while (!jobQueue.empty()) { // assign jobs in the queue to processors
        bool processorAvailable = false;
        for (size_t i = 0; i < processors.size();
             i++) { // iterates through available processors
          if (!jobQueue.empty() &&
              processors[i] ==
                  0) { // checks if job is in queue and processor is available
            jobData next = jobQueue.front(); // gets next job from front of
                                             // queue
            jobQueue.pop(); // removes job from queue
            processors[i] =
                next.remainingTime; // assigns job processing time to processor
            totalJobsCompleted++;
            totalJobsInQueue--;
            processorAvailable = true; // job was assigned to available
                                       // processor
          }
        }
        if (!processorAvailable) { // stop precessing jobs if no processors are
                                   // available
          break;
        }
      }
    }
    currentTime++;
  }
  updateMetrics(); // updates metric values after simulation is finished
}

// updates processor status
void jobProcessor::logOutput(int time, ofstream &logFile) {
  logFile << "Time " << time << ": "; // prints current time

  for (const auto &job : data) {
    if (job.arrivalTime == time) {
      bool processorAvailable = false;
      for (size_t i = 0; i < processors.size(); i++) {
        if (processors[i] == 0) {
          processorAvailable = true;
          processors[i] = job.processingTime;
          logFile << "Arrival: Overall Job:" << (totalJobsCompleted + 1)
                  << ", Job " << job.jobType << ":"
                  << totalJobsArrived[job.jobType - 'A'] + 1
                  << ", Processing Time " << job.processingTime << ";" << endl;
          totalJobsCompleted++;
          totalJobsArrived[job.jobType - 'A']++;
          logFile << "Time " << time
                  << ":- Begin Processing Job:" << (totalJobsCompleted)
                  << ", Job " << job.jobType << ":"
                  << totalJobsArrived[job.jobType - 'A'] << " in CPU "
                  << (i + 1) << endl;
          break;
        }
      }
      if (!processorAvailable) {
        jobQueue.push(jobData(job));
        totalJobsInQueue++;
        if (totalJobsInQueue > maxJobsInQueue) {
          maxJobsInQueue = totalJobsInQueue;
        }
        logFile << "Time " << time
                << ": Arrival: Overall Job:" << (totalJobsCompleted + 1)
                << ", Job " << job.jobType << ":"
                << totalJobsArrived[job.jobType - 'A'] + 1 << ", Queued"
                << endl;
      }
    }
  }

  for (size_t i = 0; i < processors.size(); i++) {
    if (processors[i] > 0) {
      processors[i]--;
      logFile << "CPU " << i + 1 << " Run Time:" << (time - processors[i])
              << "; ";
      if (processors[i] == 0) {
        logFile << "Complete Processing Job:"
                << (totalJobsCompleted - totalJobsInQueue + 1) << ", Job "
                << data[totalJobsCompleted - totalJobsInQueue].jobType << ":"
                << totalJobsArrived[data[totalJobsCompleted - totalJobsInQueue]
                                        .jobType -
                                    'A']
                << endl;
        totalJobsInQueue--;
      }
    } else {
      logFile << "CPU " << i + 1 << " Idle Time:" << time << "; ";
    }
  }
  logFile << "Queue: " << jobQueue.size() << " Job(s)" << endl;
}

 void jobProcessor::updateProcessorCout(int time) {
  cout << "Time " << time << ": "; // prints current time

  for (const auto &job : data) {
    if (job.arrivalTime == time) {
      bool processorAvailable = false;
      for (size_t i = 0; i < processors.size(); i++) {
        if (processors[i] == 0) {
          processorAvailable = true;
          processors[i] = job.processingTime;
          cout << "Arrival: Overall Job:" << (totalJobsCompleted + 1)
               << ", Job " << job.jobType << ":"
               << totalJobsArrived[job.jobType - 'A'] + 1
               << ", Processing Time " << job.processingTime << ";" << endl;
          totalJobsCompleted++;
          totalJobsArrived[job.jobType - 'A']++;
          cout << "Time " << time
               << ":- Begin Processing Job:" << (totalJobsCompleted) << ", Job "
               << job.jobType << ":" << totalJobsArrived[job.jobType - 'A']
               << " in CPU " << (i + 1) << endl;
          break;
        }
      }
      if (!processorAvailable) {
        jobQueue.push(jobData(job));
        totalJobsInQueue++;
        if (totalJobsInQueue > maxJobsInQueue) {
          maxJobsInQueue = totalJobsInQueue;
        }
        cout << "Time " << time
             << ": Arrival: Overall Job:" << (totalJobsCompleted + 1)
             << ", Job " << job.jobType << ":"
             << totalJobsArrived[job.jobType - 'A'] + 1 << ", Queued" << endl;
      }
    }
  }

  for (size_t i = 0; i < processors.size(); i++) {
    if (processors[i] > 0) {
      processors[i]--;
      cout << "CPU " << i + 1 << " Run Time:" << (time - processors[i]) << "; ";
      if (processors[i] == 0) {
        cout << "Complete Processing Job:"
             << (totalJobsCompleted - totalJobsInQueue + 1) << ", Job "
             << data[totalJobsCompleted - totalJobsInQueue].jobType << ":"
             << totalJobsArrived
                    [data[totalJobsCompleted - totalJobsInQueue].jobType - 'A']
             << endl;
        totalJobsInQueue--;
      }
    } else {
      cout << "CPU " << i + 1 << " Idle Time:" << time << "; ";
    }
  }
  cout << "Queue: " << jobQueue.size() << " Job(s)" << endl;
}

void jobProcessor::updateMetrics() {
  cout << "Number of processor(s) being used: " << processors.size() << endl;
  cout << "Current queue size: " << jobQueue.size() << endl;
  cout << "Average queue size: "
       << (totalJobsInQueue / (totalJobsCompleted + 1)) << endl;
  cout << "Maximum jobs in queue: " << maxJobsInQueue << endl;
  cout << "Total time jobs are in queue: " << totalJobsInQueue << " time units"
       << endl;
  cout << "Average time jobs are in queue: "
       << (totalJobsInQueue / (totalJobsCompleted + 1)) << " time units"
       << endl;
  cout << "Total number of A jobs arrived: " << totalJobsArrived[0] << endl;
  cout << "Total number of A jobs completed: " << (2400 - totalJobsArrived[0])
       << endl;
  cout << "Total number of B jobs arrived: " << totalJobsArrived[1] << endl;
  cout << "Total number of B jobs completed: " << (1650 - totalJobsArrived[1])
       << endl;
  cout << "Total number of C jobs arrived: " << totalJobsArrived[2] << endl;
  cout << "Total number of C jobs completed: " << (1100 - totalJobsArrived[2])
       << endl;
  cout << "Total number of D jobs arrived: " << totalJobsArrived[3] << endl;
  cout << "Total number of D jobs completed: " << (1200 - totalJobsArrived[3])
       << endl;
  cout << "Total jobs completed: " << totalJobsCompleted << endl;
  cout << "Total time CPU(s) were processing: " << totalTimeCpuProcessing
       << " time units" << endl;
  cout << "Total time CPU(s) were idle: " << totalTimeCpuIdle << " time units"
       << endl;
}

void jobProcessor::readFileData() {
  ifstream dataFile("jobData.txt"); // read data from file
  char type;
  int arrivalTime;
  int processingTime;
  while (dataFile >> type >> arrivalTime >> processingTime) {
    Job newJob(type, arrivalTime, processingTime);
    data.push_back(newJob); // add new job instance to vector
  }
  dataFile.close();
}
void jobProcessor::somemethod() {
  // Example: using totalTimeInQueue in a method
  cout << "Total time in queue: " << totalTimeInQueue << endl;
}

void jobProcessor::logActions() {
  ofstream logFile("log.txt", ios::out | ios::app);
  if (!logFile.is_open()) {
    cout << "Error opening log file!" << endl;
    return;
  }

  logFile << "Number of processor(s) being used: " << processors.size() << endl;
  logFile << "Current queue size: " << jobQueue.size() << endl;
  logFile << "Average queue size: "
          << (totalJobsInQueue / (totalJobsCompleted + 1)) << endl;
  logFile << "Maximum jobs in queue: " << maxJobsInQueue << endl;
  logFile << "Total time jobs are in queue: " << totalJobsInQueue
          << " time units" << endl;
  logFile << "Average time jobs are in queue: "
          << (totalJobsInQueue / (totalJobsCompleted + 1)) << " time units"
          << endl;
  logFile << "Total number of A jobs arrived: " << totalJobsArrived[0] << endl;
  logFile << "Total number of A jobs completed: "
          << (2400 - totalJobsArrived[0]) << endl;
  logFile << "Total number of B jobs arrived: " << totalJobsArrived[1] << endl;
  logFile << "Total number of B jobs completed: "
          << (1650 - totalJobsArrived[1]) << endl;
  logFile << "Total number of C jobs arrived: " << totalJobsArrived[2] << endl;
  logFile << "Total number of C jobs completed: "
          << (1100 - totalJobsArrived[2]) << endl;
  logFile << "Total number of D jobs arrived: " << totalJobsArrived[3] << endl;
  logFile << "Total number of D jobs completed: "
          << (1200 - totalJobsArrived[3]) << endl;
  logFile << "Total jobs completed: " << totalJobsCompleted << endl;
  logFile << "Total time CPU(s) were processing: " << totalTimeCpuProcessing
          << " time units" << endl;
  logFile << "Total time CPU(s) were idle: " << totalTimeCpuIdle
          << " time units" << endl;
  logFile.close();
}
