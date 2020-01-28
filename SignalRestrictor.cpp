//
// Created by abhishek on 30/10/17.
//

#include "SignalRestrictor.hpp"

template<class Value>
double SignalRestrictor<Value>::differentiation(int i) {
    // std::cout << sampleSpace[i + 1].value << "," << sampleSpace[i].value << ":" << sampleSpace[i + 1].timestamp << ","
              // << sampleSpace[i].timestamp << "::";
    if (sampleSpace[i + 1].timestamp == sampleSpace[i].timestamp) {
        return 0;
    }
    return (sampleSpace[i + 1].value - sampleSpace[i].value) /
           (sampleSpace[i + 1].timestamp - sampleSpace[i].timestamp);
}

template<class Value>
double SignalRestrictor<Value>::doubleDifferentiation(long i) {
    if ((sampleSpace[i + 2].timestamp == sampleSpace[i].timestamp))
        return 0;
    return (differentiation(i + 1) - differentiation(i)) / (sampleSpace[i + 2].timestamp - sampleSpace[i].timestamp);
}

template<class Value>
Sample<Value> SignalRestrictor<Value>::getRestrictedValue() {
    long last = sampleSpace.size() - 1;
    if (isFluctuating() && last >= 2) {
        double diff = fluctuationLimit - fluctuation;
        double upperLim = sampleSpace[last - 1].value + (
                (sampleSpace[last].timestamp - sampleSpace[last - 2].timestamp) * diff
                + (sampleSpace[last].timestamp - sampleSpace[last - 1].timestamp) * differentiation(last - 2)
        );
        diff *= -1;
        double lowerLim =
                ((sampleSpace[last].timestamp - sampleSpace[last - 2].timestamp) * diff + differentiation(last - 2)) *
                (sampleSpace[last].timestamp - sampleSpace[last - 1].timestamp) + sampleSpace[last - 1].value;

        sampleSpace[last].value = max(min(upperLim, (double) sampleSpace[last].value),
                                      lowerLim); // Clamped to stay within fluctuation limits
        std::cout << "\tis  Fluctuating:" << sampleSpace[last].value << std::endl;
    }else{
        std::cout<<"\n";
    }
    // cout << "Fluctuation after stabilization: " << isFluctuating() << endl;

    if (last >= 1) {
        if (differentiation(last - 1) > maxGain) {
            //std::cout<<"differentiation in Gain"<<differentiation(last-1)<<std::endl;
            // std::cout << "\tlast:" << sampleSpace[last].value << "Second last:" << sampleSpace[last - 1].value
                      // << std::endl;
            //std::cout<<"difference in value:"<<sampleSpace[last].value-sampleSpace[last-1].value<<std::endl;
            //std::cout<<"difference in time:"<<sampleSpace[last].timestamp-sampleSpace[last-1].timestamp<<std::endl;
            sampleSpace[last].value = maxGain * (sampleSpace[last].timestamp - sampleSpace[last - 1].timestamp) +
                                      sampleSpace[last - 1].value;
            std::cout << "\tmaxGain:" << sampleSpace[last].value << std::endl;
            // return sampleSpace[last];
        } else if (differentiation(last - 1) < -maxLoss) {
            //std::cout<<"differentiation in loss"<<differentiation(last-1)<<std::endl;
            // std::cout << "\tlast:" << sampleSpace[last].value << "Second last:" << sampleSpace[last - 1].value
                      // << std::endl;
            //std::cout<<"difference in value:"<<sampleSpace[last].value-sampleSpace[last-1].value<<std::endl;
            //std::cout<<"difference in time:"<<sampleSpace[last].timestamp-sampleSpace[last-1].timestamp<<std::endl;
            sampleSpace[last].value = sampleSpace[last - 1].value -
                                      maxLoss * (sampleSpace[last].timestamp - sampleSpace[last - 1].timestamp);
            std::cout << "\tmaxloss:" << sampleSpace[last].value << std::endl;
            // return sampleSpace[last];
        }else{
            std::cout<<"\n";
        }

    }else{
        std::cout<<"\n";
    }

    if (sampleSpace[last].value > maxValue) 
    {
        sampleSpace[last].value = maxValue;
        std::cout << "\tmaxValue:" << sampleSpace[last].value << "\n\n";
        // return sampleSpace[last];
    } 
    else if (sampleSpace[last].value < minValue) 
    {
        sampleSpace[last].value = minValue;
        std::cout << "\tminValue:" << sampleSpace[last].value << "\n\n";
        // return sampleSpace[last];
    }
    else
    {
        std::cout<<"\n\n";
    }
    // std::cout << "Normal:" << sampleSpace[last].value << std::endl;
    return sampleSpace[last];
}

template<class Value>
double SignalRestrictor<Value>::getFluctuation(long last) {
    double fluctuation = 0;
    for (long i = 0; i < last - 3; i++) {
        fluctuation += (abs(doubleDifferentiation(i + 1)) + abs(doubleDifferentiation(i))) * 0.5 *
                       (sampleSpace[i + 1].timestamp - sampleSpace[i].timestamp);
//        cout << "=>" << doubleDifferentiation(i);
    }
    return fluctuation / (sampleSpace[last - 1].timestamp - sampleSpace[0].timestamp);
}

template<class Value>
bool SignalRestrictor<Value>::isFluctuating() {
    fluctuation = getFluctuation(sampleSpace.size() - 1);
    if (fluctuation > fluctuationLimit) { // Due to floating point imprecision
        // cout << "FLOATING POINT IMPRECISION detected in isFluctuating";
        fluctuation = fluctuationLimit;
    }
    return getFluctuation(sampleSpace.size()) > fluctuationLimit;
}

template<class Value>
void SignalRestrictor<Value>::addSample(Sample<Value> obj) {
    sampleSpace.push_back(obj);
    std::cout << "New sample value:" << obj.value << std::endl;
    double begin = obj.timestamp - durationOfSampleSpace;
    //std::cout<<"timestamp:"<<obj.timestamp<<std::endl;
    long i = 0;
    for (i = sampleSpace.size() - 1; i >= 0; i--) {
        if (sampleSpace[i].timestamp < begin)
            break;

        //std::cout<<"=>"<<sampleSpace[i].timestamp<<":"<<sampleSpace[i].value;
    }
    std::cout << std::endl;

    sampleSpace.erase(sampleSpace.begin(), sampleSpace.begin() + i + 1);

    /*while(sampleSpace[i].timestamp<begin) {
        std::cout<<"=>"<<sampleSpace[i].timestamp<<":"<<sampleSpace[i].value;
        sampleSpace.erase(sampleSpace.begin() +i+1);
        i++;
    }*/
    // std::cout << std::endl;
    //std::cout<<"sample size:"<<sampleSpace.size()<<std::endl;
}

//template class Sample<float>;
template
class Sample<double>;

//template class Sample<long>;
//template class SignalRestrictor<float>;
template
class SignalRestrictor<double>;
//template class SignalRestrictor<long>;
//template class SignalRestrictor<string>;