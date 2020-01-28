//
// Created by abhishek on 30/10/17.
//

#ifndef PROJECT_SIGNALRESTRICTOR_HPP
#define PROJECT_SIGNALRESTRICTOR_HPP

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

template<class Value>
class Sample{
public:
    double timestamp;
    Value value;
};

template <class Value> class SignalRestrictor
{
    vector<Sample<Value>> sampleSpace;
    double durationOfSampleSpace;
    double fluctuationLimit;
    Value minValue;
    Value maxValue;
    double maxGain;
    double maxLoss;
    double fluctuation;
public:
    SignalRestrictor<Value>(double durationOfSampleSpace,double fluctuationLimit,Value minValue,
                        Value maxValue,double maxGain,double maxLoss)
    {
        this->sampleSpace = {};
        this->durationOfSampleSpace=durationOfSampleSpace;
        this->fluctuationLimit=fluctuationLimit;
        this->minValue=minValue;
        this->maxValue=maxValue;
        this->maxGain=maxGain;
        this->maxLoss=maxLoss;
        this->fluctuation=0;
    }

    void addSample(Sample<Value>);
    Sample<Value> getRestrictedValue();
    double getFluctuation(long);
    bool isFluctuating();
    double differentiation(int i);
    double doubleDifferentiation(long i);
};


#endif //PROJECT_SIGNALRESTRICTOR_HPP
