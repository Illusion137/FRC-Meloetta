#pragma once

namespace nerds {
    struct FieldObstacle {
    
    };
    struct NavX {
        bool connected, calibrating;
        frc::Translation3d displacement;
        frc::Rotation3d rotation;
        frc::Translation3d velocity;
        static NavX from_nt(std::shared_ptr<nt::NetworkTable> table) {
            NavX navx;
            navx.connected                       = table->GetEntry("Connected?").GetBoolean(false);
            navx.calibrating                     = table->GetEntry("Calibrating?").GetBoolean(false);
            auto displacement_arr = table->GetEntry("Displacement Meters").GetDoubleArray({});
            auto rotation_arr     = table->GetEntry("Rotation Rad").GetDoubleArray({});
            auto velocity_arr     = table->GetEntry("Velocity Meters|Sec").GetDoubleArray({});


            navx.displacement = displacement_arr.size() == 0 ? frc::Translation3d() : 
                frc::Translation3d(units::meter_t(displacement_arr[0]), units::meter_t(displacement_arr[1]), units::meter_t(displacement_arr[2]));
            navx.rotation = rotation_arr.size() == 0 ? frc::Rotation3d() :
                frc::Rotation3d(units::radian_t(rotation_arr[0]), units::radian_t(rotation_arr[1]), units::radian_t(rotation_arr[2]));
            navx.velocity = velocity_arr.size() == 0 ? frc::Translation3d() :
                frc::Translation3d(units::meter_t(velocity_arr[0]), units::meter_t(velocity_arr[1]), units::meter_t(velocity_arr[2]));
            return navx;
        }
    };
    struct SwerveModule {
        int channel;
        units::radian_t absolute_encoder_value;
        struct State {
            units::meters_per_second_t speed;
            units::radian_t rotation;
        } state;
        static SwerveModule from_nt(std::shared_ptr<nt::NetworkTable> table) {
            SwerveModule module;
            module.channel                = table->GetEntry("Channel").GetInteger(-1);
            module.absolute_encoder_value = units::radian_t(table->GetEntry("Absolute Encoder Value Rad").GetDouble(-1.0));
            module.state.rotation         = units::radian_t(table->GetEntry("State/Rotation Rad").GetDouble(-1.0));
            module.state.speed            = units::meters_per_second_t(table->GetEntry("State/Speed Meters|Sec").GetDouble(-1.0));
            return module;
        }
    };
    struct SwerveDrive {
        units::degree_t robot_heading;
        NavX navx_micro;
        SwerveModule front_right;
        SwerveModule front_left;
        SwerveModule back_right;
        SwerveModule back_left;
        static SwerveDrive from_nt() {
            SwerveDrive swerve_drive;
            swerve_drive.robot_heading = units::degree_t(nt_inst_table->GetEntry("Rotation Heading").GetDouble(0.0));
            swerve_drive.navx_micro = NavX::from_nt(nt_inst_table->GetSubTable("NavX Micro"));
            
            swerve_drive.front_right = SwerveModule::from_nt(nt_inst_table->GetSubTable("Front Right"));
            swerve_drive.front_left = SwerveModule::from_nt(nt_inst_table->GetSubTable("Front Left"));
            swerve_drive.back_right = SwerveModule::from_nt(nt_inst_table->GetSubTable("Back Right"));
            swerve_drive.back_left = SwerveModule::from_nt(nt_inst_table->GetSubTable("Back Left"));
            return swerve_drive;
        }
    };
}