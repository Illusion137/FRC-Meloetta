#pragma once

#include <iostream>
#include <vector>

#include <imgui.h>
#include <imgui_internal.h>

#include <frc/apriltag/AprilTagDetection.h>
#include <frc/apriltag/AprilTagDetector.h>
#include <frc/apriltag/AprilTagPoseEstimator.h>
#include <frc/apriltag/AprilTagFieldLayout.h>
#include <frc/geometry/Transform3d.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <units/angle.h>
#include <units/length.h>
#include <units/math.h>

#include <photon/PhotonPoseEstimator.h>

namespace nerds {
    struct AprilTagDetectionData {
        int id;
        frc::Transform3d pose;
    };

    extern std::vector<AprilTagDetectionData> apriltag_vision_from_file(const std::string& file_path);
    extern void webcam_apriltag_vision_thread(std::vector<AprilTagDetectionData>* out, bool* end);
    extern void webcam_apriltag_vision_test();
}