/*
 * ReaderDemo.cc
 *
 *  Created on: Sep 10, 2019
 *      Author: guogh
 */


#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "./BasicException.h"
#include "./WaveformReaderForCompetition.h"

void GetData(WaveformReaderForCompetition* reader)
{
    assert(reader);
    while(!reader->IsDataFinished()) {
        reader->ReadNextPointData();
        reader->GetNextXValue(); // you can store or print this value
        for(int i = 0; i < reader->GetSignalCount(); i++) {
            reader->GetNextSignalValue(i); // you can store or print this value
        }
    }
}

void GetDataAndDump(WaveformReaderForCompetition* reader)
{
    assert(reader);
    int pointsNumber = 0;
    while(!reader->IsDataFinished()) {
        reader->ReadNextPointData();
        printf("x value: %g\n", reader->GetNextXValue());
        for(int i = 0; i < reader->GetSignalCount(); i++) {
            printf("signal index: %d, name: %s, value: %g\n", i, reader->GetSignalName(i).c_str(), reader->GetNextSignalValue(i));
        }
        ++pointsNumber;
    }
    printf("Finish reading data, total points number: %d\n", pointsNumber);
}

void ReadDataOfAllSignals(WaveformReaderForCompetition* reader)
{
    assert(reader);
    reader->Read(); //  read all signals
    GetData(reader); // call GetDataAndDump(reader) instead if you want to dump data values
}

void GenerateSignalNames(std::set<std::string>& selectedSignalNames)
{
    /**
     * just for example, you should insert specified signal names which can be found in the input file
     */
    selectedSignalNames.insert("v(0)");
    selectedSignalNames.insert("v(1)");
    selectedSignalNames.insert("v(vdd)");
}

void ReadDataOfSpecifiedSignals(WaveformReaderForCompetition* reader)
{
    assert(reader);
    std::set<std::string> selectedSignalNames;
    GenerateSignalNames(selectedSignalNames);
    reader->Read(selectedSignalNames); // read specified signals
    GetData(reader); // call GetDataAndDump(reader) instead if you want to dump data values
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
        printf("Please input a SSDB waveform file\n");
        exit(-1);
    }

    std::string inputFile;
    GetInputFile(argc, argv, inputFile);

    try {
        WaveformReaderForCompetition reader(inputFile);
        ReadDataOfAllSignals(&reader); // read all signals and get data
        //ReadDataOfSpecifiedSignals(&reader); // if only need to read data of some specified signals, call this function
    }
    catch (const BasicException& exception)
    {
        printf(exception.GetMessage());
        printf("\n");
        exit(-1);
    }

    return 0;
}
