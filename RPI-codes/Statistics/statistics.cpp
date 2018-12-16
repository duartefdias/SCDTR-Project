#include <cmath>
#include <vector>

using namespace std;


float energy_consumption(vector<float> sampleTime, vector<float> pwm) {
    float energy = 0;
    float P = 1;    //Duty cycle between 0 an 1

    for (int i=1; i<sampleTime.size(); i++) {
        energy += pwm[i-1]*(sampleTime[i]-sampleTime[i-1]); //Sample time in seconds
    }

    return energy*P;    //Energy in Joule
}

float confort_error(vector<float> luxMeas, vector<float> luxRef) {
    int N = luxRef.size();
    float cError = 0;

    for (int i=0; i<N; i++) {
         cError += fmax(luxRef[i]-luxMeas[i], 0);
    }

    return cError/N;
}

float confort_flicker(vector<float> luxMeas) {
    int Ts = 100;   //Sampling period
    int N = luxMeas.size();
    float cFlicker = 0;

    for (int i=2; i<N; i++) {
        if ((luxMeas[i]-luxMeas[i-1]) * (luxMeas[i-1]-luxMeas[i-2]) < 0) {
            cFlicker += (abs(luxMeas[i]-luxMeas[i-1]) + abs(luxMeas[i-1]-luxMeas[i-2])) / (2*Ts);
        }
        else {
            cFlicker += 0;
        }
    }

    return cFlicker/N;
}
