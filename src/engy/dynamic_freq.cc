//
// Created by lf-z on 5/15/17.
//

#include "engy/dynamic_freq.hh"

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
                state = STATE_HIGH;
                msg.type = MsgType::POWERON_HIGH;
            }
            else {
                state = STATE_LOW;
                msg.type = MsgType::POWERON_LOW;
            }
            broadcastMsg(msg);
        }
    } else if (state == STATE_HIGH) {
        if (_energy < thres_poweroff) {
            state = STATE_OFF;
            msg.type = MsgType::POWEROFF;
            broadcastMsg(msg);
        } else if (_energy < thres_convert) {
            state = STATE_LOW;
            msg.type = MsgType::CONVERT_HIGH2LOW;
            broadcastMsg(msg);
        }
    } else if (state == STATE_LOW) {
        if (_energy < thres_poweroff) {
            state = STATE_OFF;
            msg.type = MsgType::POWEROFF;
            broadcastMsg(msg);
        } else if (_energy >= thres_convert) {
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
