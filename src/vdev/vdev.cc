//
// Created by lf-z on 4/24/17.
//

#include "vdev/vdev.hh"
#include "engy/state_machine.hh"
#include "debug/EnergyMgmt.hh"
#include "debug/VirtualDevice.hh"

VirtualDevice::DevicePort::DevicePort(const std::string &_name,
                                      VirtualDevice *_vdev)
    : SlavePort(_name, _vdev), vdev(_vdev)
{

}

Tick
VirtualDevice::DevicePort::recvAtomic(PacketPtr pkt)
{
    assert(vdev);
    return vdev->recvAtomic(pkt);
}

void
VirtualDevice::DevicePort::recvFunctional(PacketPtr pkt)
{
    assert(vdev);
    vdev->recvFunctional(pkt);
}

bool
VirtualDevice::DevicePort::recvTimingReq(PacketPtr pkt)
{
    assert(vdev);
    return vdev->recvTimingReq(pkt);
}

void
VirtualDevice::DevicePort::recvRespRetry()
{
    assert(vdev);
    vdev->recvRespRetry();
}

AddrRangeList
VirtualDevice::DevicePort::getAddrRanges() const
{
    assert(vdev);

    AddrRangeList rangeList;
    rangeList.push_back(vdev->getAddrRange());

    return rangeList;
}

VirtualDevice::VirtualDevice(const Params *p)
    : MemObject(p),
      port(name() + ".port", this),
      cpu(p->cpu),
      range(p->range),
      delay_set(p->delay_set),
      delay_self(p->delay_self),
      delay_cpu_interrupt(p->delay_cpu_interrupt),
      is_interruptable(p->is_interruptable),
      delay_remained(0),
      is_intask(0),
      event_interrupt(this, false, Event::Virtual_Interrupt)
{
    trace.resize(0);
}

void
VirtualDevice::init()
{
    MemObject::init();

    if (port.isConnected()) {
        port.sendRangeChange();
    }

    cpu->registerVDev(delay_set);
    DPRINTF(VirtualDevice, "Virtual Device started with range: %#lx - %#lx\n",
            range.start(), range.end());
}

void
VirtualDevice::triggerInterrupt()
{
    /* Todo: add static of finish success. */
    finishSuccess();
    assert(is_intask);
    is_intask = 0;
    cpu->virtualInterrupt(delay_cpu_interrupt);
}

Tick
VirtualDevice::access(PacketPtr pkt)
{
    /* Todo: what if the cpu ask to work on a task when the vdev is busy? **/
    assert(!is_intask);
    is_intask = 1;
    schedule(event_interrupt, curTick() + delay_self);
    return delay_set;
}

int
VirtualDevice::handleMsg(const EnergyMsg &msg)
{
    DPRINTF(EnergyMgmt, "Device handleMsg called at %lu, msg.type=%d\n", curTick(), msg.type);
    switch(msg.type) {
        case (int) SimpleEnergySM::POWEROFF:
            if (is_intask) {
                /* This should be handled if the device is on a task **/
                assert(event_interrupt.scheduled());
                DPRINTF(EnergyMgmt, "device power off occurs in the middle of a task at %lu\n", curTick());
                delay_remained = event_interrupt.when() - curTick();
                deschedule(event_interrupt);
            }
            break;
        case (int) SimpleEnergySM::POWERON:
            if (is_intask) {
                assert(!event_interrupt.scheduled());
                DPRINTF(EnergyMgmt, "device power on to finish a task at %lu\n", curTick());
                if (is_interruptable) {
                    schedule(event_interrupt, curTick() + delay_remained);
                } else {
                    schedule(event_interrupt, curTick() + delay_self);
                }
            }
            break;
        default:
            return 0;
    }
    return 1;
}

BaseSlavePort&
VirtualDevice::getSlavePort(const std::string &if_name, PortID idx)
{
    if (if_name == "port") {
        return port;
    } else {
        return MemObject::getSlavePort(if_name, idx);
    }
}

AddrRange
VirtualDevice::getAddrRange() const
{
    return range;
}

Tick
VirtualDevice::recvAtomic(PacketPtr pkt)
{
    return access(pkt);
}

void
VirtualDevice::recvFunctional(PacketPtr pkt)
{
    fatal("Functional access is not supported now.");
}

bool
VirtualDevice::recvTimingReq(PacketPtr pkt)
{
    fatal("Timing access is not supported now.");
}

void
VirtualDevice::recvRespRetry()
{
    fatal("Timing access is not supported now.");
}

bool
VirtualDevice::finishSuccess()
{
    /* Todo: Need further implementation. */
    return 1;
}

VirtualDevice *
VirtualDeviceParams::create()
{
    return new VirtualDevice(this);
}
