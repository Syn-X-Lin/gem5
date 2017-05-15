//
// Created by lf-z on 5/15/17.
//

#ifndef GEM5_DYNAMIC_FREQ_HH
#define GEM5_DYNAMIC_FREQ_HH

#include "engy/state_machine.hh"
#include "params/DynamicFreqSM.hh"

class DynamicFreqSM : public BaseEnergySM
{
public:
    typedef DynamicFreqSMParams Params;
    const Params *params() const
    {
        return reinterpret_cast<const Params *>(_params);
    }
    DynamicFreqSM(const Params *p);
    ~DynamicFreqSM() {}

    enum State {
        STATE_INIT = 0,
        STATE_OFF = 1,
        STATE_LOW = 2,
        STATE_HIGH = 3
    };

    enum MsgType {
        CONSUME_ENERGY = 0,
        POWERON_LOW = 1,
        POWERON_HIGH = 2,
        CONVERT_LOW2HIGH = 3,
        CONVERT_HIGH2LOW = 4,
        POWEROFF = 5
    };

    virtual void init();
    virtual void update(double _energy);

protected:
    State state;
    double thres_convert;
    double thres_poweroff;
    double thres_poweron;
};

#endif //GEM5_DYNAMIC_FREQ_HH
