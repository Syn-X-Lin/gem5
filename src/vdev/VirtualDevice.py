from m5.params import *
from MemObject import MemObject
from AtomicSimpleCPU import AtomicSimpleCPU

class VirtualDevice(MemObject):
    type = 'VirtualDevice'
    cxx_header = 'vdev/vdev.hh'
    delay_self = Param.Clock('1ms', "delay of device to finish its mission")
    delay_set = Param.Clock('0t', "delay to set up the device")
    delay_cpu_interrupt = Param.Clock('1ms', "time that the cpu need to take when the device finishes its work")
    cpu = Param.BaseCPU(AtomicSimpleCPU(), "The cpu of the system")