/*
 * OutputterDemo.cc
 *
 *  Created on: Sep 17, 2019
 *      Author: guogh
 */


#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "./WaveformOutputterForCompetition.h"

void GenerateSignalNames(std::vector<std::string>& selectedSignalNames)
{
    selectedSignalNames.push_back("v(0)");
    selectedSignalNames.push_back("v(1)");
    selectedSignalNames.push_back("v(vdd)");
}

void OutputHead(
        WaveformOutputterForCompetition* outputter,
        double startTime,
        double stopTime,
        const std::vector<std::string>& selectedSignalNames)
{
    assert(outputter);
    outputter->OutputHead(startTime, stopTime, selectedSignalNames);
}

double* GenerateXValues(
        double startTime,
        double stopTime,
        int pointsNumber)
{
    assert(pointsNumber > 0);
    assert(stopTime > startTime);
    const double avg = (stopTime - startTime) / pointsNumber;
    double* xValues = new double [pointsNumber];
    for(int i = 0; i < pointsNumber; ++i) {
        xValues[i] = startTime + i * avg;
    }

    return xValues;
}

double** GenerateSignalValues(int signalsNumber, int pointsNumber)
{
    assert(signalsNumber > 0);
    assert(pointsNumber > 0);
    double** sigValues = new double*[signalsNumber];
    for(int i = 0; i < signalsNumber; ++i) {
        sigValues[i] = new double[pointsNumber];
        for(int j = 0; j < pointsNumber; ++j) {
            sigValues[i][j] = 1e-3 * i + (j+0.1) * 1e-4;
        }
    }

    return sigValues;
}

void DestoryXValues(double* xValues)
{
    delete [] xValues;
}

void DestorySignalValues(double** sigValues, int signalsNumber)
{
    assert(signalsNumber > 0);
    for(int i = 0; i < signalsNumber; ++i) {
        delete [] sigValues[i];
    }
    delete []sigValues;
}

void OutputData(
        WaveformOutputterForCompetition* outputter,
        double* xValues,
        double** sigValues,
        int signalsNumber,
        int pointsNumber)
{
    assert(outputter);
    for(int i = 0; i < pointsNumber; ++i) {
        outputter->OutputXValue(xValues[i]);
        for(int j = 0; j < signalsNumber; ++j) {
            outputter->OutputSignalValue(sigValues[j][i]);
        }
        outputter->FinishOnePointData();
    }
}

void GetInputFile(int argc, const char** argv, std::string& inputFile)
{
    for(int i = 1; i < argc; ++i) {
        if(argv[i][0] != '-') {
            inputFile = argv[i];
            break;
        }
    }

    assert(!inputFile.empty());
}

int main(int argc, const char** argv)
{
    if(argc < 2) {
        printf("Please input a file name to output\n");
        exit(-1);
    }

    std::string inputFile;
    GetInputFile(argc, argv, inputFile);
    WaveformOutputterForCompetition outputter(inputFile);

    std::vector<std::string> selectedSignalNames;
    const double startTime = 0.0; // just for example, you should get the value in the Decompress program
    const double stopTime = 1e-7; // just for example, you should get the value in the Decompress program
    GenerateSignalNames(selectedSignalNames); // just for example, you should add signal names that need to be output
    OutputHead(&outputter, startTime, stopTime, selectedSignalNames); // output signal names to the waveform file

    const int pointsNumber = 100; // just for example, you should get the value in the Decompress program
    const int signalsNumber = selectedSignalNames.size();
    double* xValues = GenerateXValues(startTime, stopTime, pointsNumber); // just for example, you should get the values in the Decompress program
    double** sigValues = GenerateSignalValues(signalsNumber, pointsNumber); // just for example, you should get the values in the Decompress program
    OutputData(&outputter, xValues, sigValues, signalsNumber, pointsNumber);

    outputter.FinishOutput(); // do not forget to call it

    DestoryXValues(xValues);
    DestorySignalValues(sigValues, signalsNumber);

    return 0;
}


