#pragma once

#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTableListener.h>
#include <networktables/NetworkTable.h>
#include <networktables/StructArrayTopic.h>
#include <networktables/StructTopic.h>

namespace nerds {
    inline nt::NetworkTableInstance nt_inst = nt::NetworkTableInstance::GetDefault();
    inline std::shared_ptr<nt::NetworkTable> nt_fgear_table = nt_inst.GetTable("FGear");
}