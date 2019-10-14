/*
 * WaveformOutputterForCompetition.h
 *
 *  Created on: Sep 17, 2019
 *      Author: guogh
 */

#ifndef EDA_COMPETITION_LIBOUTPUTTER_WAVEFORMOUTPUTTERFORCOMPETITION_H_
#define EDA_COMPETITION_LIBOUTPUTTER_WAVEFORMOUTPUTTERFORCOMPETITION_H_

#include <string>
#include <vector>

class SsdbWaveformOutputter;

class WaveformOutputterForCompetition
{
public:
    WaveformOutputterForCompetition(const std::string& fileName);

    ~WaveformOutputterForCompetition();

    void OutputHead(double startTime, double stopTime, const std::vector<std::string>& selectedSignalNames);

    void OutputXValue(double xValue);

    void OutputSignalValue(double sigValue);

    void FinishOnePointData();

    void FinishOutput();

private:
    SsdbWaveformOutputter* outputter_;
};



#endif /* EDA_COMPETITION_LIBOUTPUTTER_WAVEFORMOUTPUTTERFORCOMPETITION_H_ */
