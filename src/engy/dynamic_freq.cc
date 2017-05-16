//
// Created by lf-z on 5/15/17.
//

#include "engy/dynamic_freq.hh"
#include "debug/EnergyMgmt.hh"

DynamicFreqSM::DynamicFreqSM(const Params *p) :
    BaseEnergySM(p), state(DynamicFreqSM::State::STATE_INIT),
    thres_convert(p->thres_convert), thres_poweroff(p->thres_poweroff),
    thres_poweron(p->thres_poweron)
{

}

void
DynamicFreqSM::init()
{
    state = DynamicFreqSM::State::STATE_OFF;
    EnergyMsg msg;
    msg.val = 0;
    msg.type = MsgType::POWEROFF;
    broadcastMsg(msg);
}

void
DynamicFreqSM::update(double _energy)
{
    EnergyMsg msg;
    msg.val = 0;

    if (state == STATE_OFF) {
        if (_energy >= thres_poweron) {
            if (_energy >= thres_convert) {
                DPRINTF(EnergyMgmt, "off->high\n");
                state = STATE_HIGH;
                msg.type = MsgType::POWERON_HIGH;
            }
            else {
                DPRINTF(EnergyMgmt, "off->low\n");
                state = STATE_LOW;
                msg.type = MsgType::POWERON_LOW;
            }
            broadcastMsg(msg);
        }
    } else if (state == STATE_HIGH) {
        if (_energy < thres_poweroff) {
            DPRINTF(EnergyMgmt, "high->off\n");
            state = STATE_OFF;
            msg.type = MsgType::POWEROFF;
            broadcastMsg(msg);
        } else if (_energy < thres_convert) {
            DPRINTF(EnergyMgmt, "high->low\n");
            state = STATE_LOW;
            msg.type = MsgType::CONVERT_HIGH2LOW;
            broadcastMsg(msg);
        }
    } else if (state == STATE_LOW) {
        if (_energy < thres_poweroff) {
            DPRINTF(EnergyMgmt, "low->off\n");
            state = STATE_OFF;
            msg.type = MsgType::POWEROFF;
            broadcastMsg(msg);
        } else if (_energy >= thres_convert) {
            DPRINTF(EnergyMgmt, "low->high\n");
            state = STATE_HIGH;
            msg.type = MsgType::CONVERT_LOW2HIGH;
            broadcastMsg(msg);
        }
    }

}

DynamicFreqSM *
DynamicFreqSMParams::create()
{
    return new DynamicFreqSM(this);
}
