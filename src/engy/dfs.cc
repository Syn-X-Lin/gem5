#include "dfs.hh"
#include "debug/EnergyMgmt.hh"

DFSSM::DFSSM(const Params *p)
    : BaseEnergySM(p), state(DFSSM::State::STATE_INIT),
      thres_high(p->thres_high), thres_low(p->thres_low)
{

}

void DFSSM::init()
{
    state = DFSSM::State::STATE_POWEROFF;
    EnergyMsg msg;
    msg.val = 0;
    msg.type = MsgType::POWEROFF;
    broadcastMsg(msg);
}

void DFSSM::update(double _energy)
{
    EnergyMsg msg;
    msg.val = 0;

    if (state == STATE_INIT) {
        state = STATE_HIGH_FREQ;
    } else if (state == STATE_HIGH_FREQ && _energy < thres_high) {
        DPRINTF(EnergyMgmt, "State change: high->low state=%d, _energy=%lf, thres_low=%lf\n", state, _energy, thres_low);
        state = STATE_LOW_FREQ;
        msg.type = MsgType::LOW_FREQ;
        broadcastMsg(msg);
    } else if (state == STATE_LOW_FREQ && _energy < thres_low) {
        DPRINTF(EnergyMgmt, "State change: low->off state=%d, _energy=%lf, thres_low=%lf\n", state, _energy, thres_low);
        state = STATE_POWEROFF;
        msg.type = MsgType::POWEROFF;
        broadcastMsg(msg);
    } else if (state == STATE_LOW_FREQ && _energy > thres_high) {
        DPRINTF(EnergyMgmt, "State change: low->high state=%d, _energy=%lf, thres_high=%lf\n", state, _energy, thres_high);
        state = STATE_HIGH_FREQ;
        msg.type = MsgType::HIGH_FREQ;
        broadcastMsg(msg);
    } else if (state == STATE_POWEROFF && _energy > thres_high) {
        DPRINTF(EnergyMgmt, "State change: off->high state=%d, _energy=%lf, thres_high=%lf\n", state, _energy, thres_high);
        state = STATE_HIGH_FREQ;
        msg.type = MsgType::HIGH_FREQ;
        broadcastMsg(msg);
    }

}

DFSSM *
DFSSMParams::create()
{
    return new DFSSM(this);
}
