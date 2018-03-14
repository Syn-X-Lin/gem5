#ifndef GEM5_DFS_HH
#define GEM5_DFS_HH
#include "state_machine.hh"
#include "params/DFSSM.hh"

class DFSSM : public BaseEnergySM
{

public:
    typedef DFSSMParams Params;
    const Params *params() const
    {
        return reinterpret_cast<const Params *>(_params);
    }
    DFSSM(const Params *p);
    ~DFSSM() {}
    virtual void init();
    virtual void update(double _energy);

    enum State {
        STATE_INIT = 0,
        STATE_POWEROFF = 1,
        STATE_HIGH_FREQ = 2,
        STATE_LOW_FREQ = 3
    };

    enum MsgType {
        CONSUMEENERGY = 0,
        POWEROFF = 1,
        HIGH_FREQ = 2,
        LOW_FREQ = 3
    };

protected:
    State state;
    double thres_high;
    double thres_low;
};
#endif //GEM5_TWO_THRES_HH
