/*
 * WaveformReaderForCompetition.h
 *
 *  Created on: Sep 17, 2019
 *      Author: guogh
 */

#ifndef EDA_COMPETITION_LIBREADER_WAVEFORMREADERFORCOMPETITION_H_
#define EDA_COMPETITION_LIBREADER_WAVEFORMREADERFORCOMPETITION_H_

#include <string>
#include <set>

typedef std::set<std::string> SignalNameSet;

class SsdbWaveformReader;

class WaveformReaderForCompetition
{
public:
    WaveformReaderForCompetition(const std::string& fileName);

    ~WaveformReaderForCompetition();

    void Read();

    void Read(const SignalNameSet& selectedSignalNames);

    bool IsDataFinished() const;

    int GetSignalCount() const;

    const std::string& GetSignalName(int sigIndex) const;

    void ReadNextPointData();

    double GetNextXValue();

    double GetNextSignalValue(int sigIndex);

private:
    SsdbWaveformReader* reader_;
};



#endif /* EDA_COMPETITION_LIBREADER_WAVEFORMREADERFORCOMPETITION_H_ */
