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

#include "./WaveformReaderForCompetition.h"

void ReadAllSignals(WaveformReaderForCompetition* reader)
{
    assert(reader);
    reader->Read(); //  read all signals
}

void ReadSpecifiedSignals(WaveformReaderForCompetition* reader)
{
    assert(reader);
    std::set<std::string> selectedSignalNames;
    selectedSignalNames.insert("v(0)"); // just for example, you should insert a specified signal name which can be found in the input file
    selectedSignalNames.insert("v(1)");
    selectedSignalNames.insert("v(vdd)");
    reader->Read(selectedSignalNames); // read specified signals
}

void ReadDataAndDump(WaveformReaderForCompetition* reader)
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

void ReadData(WaveformReaderForCompetition* reader)
{
    assert(reader);
    while(!reader->IsDataFinished()) {
        reader->ReadNextPointData();
        reader->GetNextXValue();
        for(int i = 0; i < reader->GetSignalCount(); i++) {
            reader->GetNextSignalValue(i);
        }
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
        printf("Please input a SSDB waveform file\n");
        exit(-1);
    }

    std::string inputFile;
    GetInputFile(argc, argv, inputFile);
    WaveformReaderForCompetition reader(inputFile);

    /**
     * read all signals and get data
     */
    ReadAllSignals(&reader);
    ReadData(&reader);
//    ReadDataAndDump(&reader);

    /**
     * read specified signals and get data
     */
    ReadSpecifiedSignals(&reader);
    ReadData(&reader);
//    ReadDataAndDump(&reader);

    return 0;
}
