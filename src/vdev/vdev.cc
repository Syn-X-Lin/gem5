//
// Created by lf-z on 4/24/17.
//

#include "vdev/vdev.hh"

VirtualDevice::VirtualDevice(const Params *p)
    : MemObject(p), cpu(p->cpu),
      delay_set(p->delay_set),
      delay_self(p->delay_self),
      delay_cpu_interrupt(p->delay_cpu_interrupt),
      event_interrupt(this, false, Event::Virtual_Interrupt)
{
    trace.resize(0);
}

void VirtualDevice::init()
{
    cpu->registerVDev(delay_set);
}

void VirtualDevice::triggerInterrupt()
{
    /* Todo: add static of finish success. */
    finish_success();
    cpu->virtualInterrupt(delay_cpu_interrupt);
}

Tick VirtualDevice::access(PacketPtr pkt)
{
    schedule(event_interrupt, curTick() + delay_self);
    return delay_set;
}

int VirtualDevice::handleMsg(const EnergyMsg &msg)
{
    /* Todo: Need further implementation. */
    return 1;
}

bool VirtualDevice::finish_success()
{
    /* Todo: Need further implementation. */
    return 1;
}

VirtualDevice *
VirtualDeviceParams::create()
{
    return new VirtualDevice(this);
}
