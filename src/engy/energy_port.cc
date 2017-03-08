//
// Created by lf-z on 1/6/17.
//

#include "engy/energy_port.hh"
#include "sim/sim_object.hh"
#include "debug/EnergyMgmt.hh"

int EnergyPort::handleMsg(EnergyMsg msg)
{
    int rlt = 1;

    if (!owner)
        return 0;

    switch(msg.type)
    {
        case CONSUME_ENERGY:
            if (port_type != MASTERPORT)
                return 1;
            rlt = owner->consumeEnergy(msg.val);
            break;
        case POWEROFF:
            if (port_type != SLAVEPORT)
                return 1;
            rlt = owner->powerOff();
            break;
        case POWERON:
            if (port_type != SLAVEPORT)
                return 1;
            rlt = owner->powerOn();
            break;
    }

    return rlt;
}

int MasterEnergyPort::bindSlave(SlaveEnergyPort &_slave)
{
    slave_list.push_back(&_slave);
    _slave.setMaster(*this);
    return 1;
}

int MasterEnergyPort::broadcastMsg(EnergyMsg msg)
{
    int rlt = 1;
    unsigned long len = slave_list.size();

    if (len == 0)
        rlt = 0;

    for (int i = 0; i < len; i++) {
        if (!slave_list[i]->handleMsg(msg)) {
            rlt = 0;
        }
    }
    return rlt;
}

int SlaveEnergyPort::setMaster(MasterEnergyPort &_master)
{
    master = &_master;
    DPRINTF(EnergyMgmt, "energy port connected. master: %s, slave: %s\n",
            master->owner->name().c_str(), owner->name().c_str());
    return 1;
}

int SlaveEnergyPort::signalMsg(EnergyMsg msg)
{
    if (!master)
        return 0;

    return master->handleMsg(msg);
}