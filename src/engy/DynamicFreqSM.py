from m5.SimObject import SimObject
from BaseEnergySM import BaseEnergySM
from m5.params import *
from m5.proxy import *

class DynamicFreqSM(BaseEnergySM):
    type = 'DynamicFreqSM'
    cxx_header = "engy/dynamic_freq.hh"
    thres_poweron = Param.Float(Parent.thres_poweron, "power-on threshold of energy state machine")
    thres_poweroff = Param.Float(Parent.thres_poweroff, "power-off threshold of energy state machine")
    thres_convert = Param.Float(Parent.thres_convert, "convert threshold of energy state machine")