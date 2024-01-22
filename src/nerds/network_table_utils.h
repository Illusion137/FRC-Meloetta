#pragma once

#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTableListener.h>
#include <networktables/NetworkTable.h>
#include <networktables/DoubleTopic.h>

namespace nerds {
    inline nt::NetworkTableInstance nt_inst = nt::NetworkTableInstance::GetDefault();
    inline std::shared_ptr<nt::NetworkTable> nt_inst_table = nt_inst.GetTable("Meloetta");
}