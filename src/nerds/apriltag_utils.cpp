    #include "apriltag_utils.h"

std::vector<nerds::AprilTagDetectionData> nerds::apriltag_vision_from_file(const std::string& file_path) {
    frc::AprilTagDetector detector;
    //// look for tag36h11, correct 7 -> 3 error bits
    detector.AddFamily("tag36h11", 3);

    frc::AprilTagPoseEstimator::Config poseEstConfig = {
        .tagSize = units::length::inch_t(6.5),
        .fx = 699.3778103158814,
        .fy = 677.7161226393544,
        .cx = 345.6059345433618,
        .cy = 207.12741326228522 };
    frc::AprilTagPoseEstimator estimator(poseEstConfig);

    //std::string file_path = cv::samples::findFile("GeneralField1.jpg", false);
    cv::Mat grayMat = cv::imread(file_path, cv::IMREAD_GRAYSCALE);

    cv::Size g_size = grayMat.size();
    frc::AprilTagDetector::Results detections =
        detector.Detect(g_size.width, g_size.height, grayMat.data);
    
    cv::Scalar outlineColor{ 0, 255, 0 };
    cv::Scalar crossColor{ 0, 0, 255 };
    
    std::vector<AprilTagDetectionData> detections_data;

    for (const frc::AprilTagDetection* detection : detections)
        detections_data.push_back({ detection->GetId(), estimator.Estimate(*detection) });
    return detections_data;
}

void nerds::webcam_apriltag_vision_thread(std::vector<nerds::AprilTagDetectionData> *out, bool *end) {
    cv::VideoCapture cap = cv::VideoCapture(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(cv::CAP_PROP_FPS, 30);

    frc::AprilTagDetector detector;
    // look for tag36h11, correct 7->3 error bits
    detector.AddFamily("tag36h11", 3);

    frc::AprilTagPoseEstimator::Config poseEstConfig = {
        .tagSize = units::length::inch_t(6.5),
        .fx = 699.3778103158814,
        .fy = 677.7161226393544,
        .cx = 345.6059345433618,
        .cy = 207.12741326228522 };
    frc::AprilTagPoseEstimator estimator(poseEstConfig);

    assert(cap.isOpened() && "No Capture Device Found");

    cv::Mat frame, greyFrame;
    cv::Scalar outlineColor{ 0, 255, 0 };
    cv::Scalar crossColor{ 0, 0, 255 };
    for (;;) {
        if (*end) return;
        cap.read(frame);

        cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);

        cv::Size g_size = greyFrame.size();
        frc::AprilTagDetector::Results detections =
            detector.Detect(g_size.width, g_size.height, greyFrame.data);

        // have not seen any tags yet

        for (const frc::AprilTagDetection* detection : detections) {
            // remember we saw this tag
            // draw lines around the tag
            for (int i = 0; i <= 3; i++) {
                int j = (i + 1) % 4;
                const frc::AprilTagDetection::Point pti = detection->GetCorner(i);
                const frc::AprilTagDetection::Point ptj = detection->GetCorner(j);
                line(frame, cv::Point(pti.x, pti.y), cv::Point(ptj.x, ptj.y),
                    outlineColor, 2);
            }

            // mark the center of the tag
            const frc::AprilTagDetection::Point c = detection->GetCenter();
            int ll = 10;
            line(frame, cv::Point(c.x - ll, c.y), cv::Point(c.x + ll, c.y),
                crossColor, 2);
            line(frame, cv::Point(c.x, c.y - ll), cv::Point(c.x, c.y + ll),
                crossColor, 2);

            // identify the tag
            putText(frame, std::to_string(detection->GetId()),
                cv::Point(c.x + ll, c.y), cv::FONT_HERSHEY_SIMPLEX, 1,
                crossColor, 3);

            // determine pose
            frc::Transform3d pose = estimator.Estimate(*detection);

            // put pose into NT
            frc::Rotation3d rotation = pose.Rotation();
            //putText(frame, std::to_string(pose.X().value() * 39.3701),
            //    cv::Point(10, 50), cv::FONT_HERSHEY_SIMPLEX, 1,
            //    crossColor, 3);
            //putText(frame, std::to_string(pose.Y().value() * 39.3701),
            //    cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 1,
            //    crossColor, 3);
            //putText(frame, std::to_string(pose.Z().value() * 39.3701),
            //    cv::Point(10, 150), cv::FONT_HERSHEY_SIMPLEX, 1,
            //    crossColor, 3);
            //putText(frame, std::to_string(rotation.X().value() * 180 / IM_PI),
            //    cv::Point(10, 200), cv::FONT_HERSHEY_SIMPLEX, 1,
            //    crossColor, 3);
            //putText(frame, std::to_string(rotation.Y().value() * 180 / IM_PI),
            //    cv::Point(10, 250), cv::FONT_HERSHEY_SIMPLEX, 1,
            //    crossColor, 3);
            //putText(frame, std::to_string(rotation.Z().value() * 180 / IM_PI),
            //    cv::Point(10, 300), cv::FONT_HERSHEY_SIMPLEX, 1,
            //    crossColor, 3);
        }
        cv::waitKey(1);
        cv::imshow("AprilTags :3", frame);
    }
    cap.release();
}

void nerds::webcam_apriltag_vision_test() {
    cv::VideoCapture cap = cv::VideoCapture(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(cv::CAP_PROP_FPS, 30);

    frc::AprilTagDetector detector;
    // look for tag36h11, correct 7->3 error bits
    detector.AddFamily("tag36h11", 3);

    frc::AprilTagPoseEstimator::Config poseEstConfig = {
        .tagSize = units::length::inch_t(6.5),
        .fx = 699.3778103158814,
        .fy = 677.7161226393544,
        .cx = 345.6059345433618,
        .cy = 207.12741326228522 };
    frc::AprilTagPoseEstimator estimator(poseEstConfig);

    assert(cap.isOpened() && "No Capture Device Found");

    cv::Mat frame, greyFrame;
    cv::Scalar outlineColor{ 0, 255, 0 };
    cv::Scalar crossColor{ 0, 0, 255 };

    frc::AprilTagFieldLayout field_layout("2024-cresendo.json");

    for (;;) {
        cap.read(frame);

        cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);

        cv::Size g_size = greyFrame.size();
        frc::AprilTagDetector::Results detections =
            detector.Detect(g_size.width, g_size.height, greyFrame.data);

        // have not seen any tags yet

        for (const frc::AprilTagDetection* detection : detections) {
            // remember we saw this tag
            // draw lines around the tag
            for (int i = 0; i <= 3; i++) {
                int j = (i + 1) % 4;
                const frc::AprilTagDetection::Point pti = detection->GetCorner(i);
                const frc::AprilTagDetection::Point ptj = detection->GetCorner(j);
                line(frame, cv::Point(pti.x, pti.y), cv::Point(ptj.x, ptj.y),
                    outlineColor, 2);
            }

            // mark the center of the tag
            const frc::AprilTagDetection::Point c = detection->GetCenter();
            int ll = 10;
            line(frame, cv::Point(c.x - ll, c.y), cv::Point(c.x + ll, c.y),
                crossColor, 2);
            line(frame, cv::Point(c.x, c.y - ll), cv::Point(c.x, c.y + ll),
                crossColor, 2);

            // identify the tag
            putText(frame, std::to_string(detection->GetId()),
                cv::Point(c.x + ll, c.y), cv::FONT_HERSHEY_SIMPLEX, 1,
                crossColor, 3);

            // determine pose
            frc::Transform3d pose = estimator.Estimate(*detection);

            // put pose into NT
            frc::Rotation3d rotation = pose.Rotation();
            {

                putText(frame, std::to_string(pose.X().value() * 39.3701),
                    cv::Point(10, 50), cv::FONT_HERSHEY_SIMPLEX, 1,
                    crossColor, 3);
                putText(frame, std::to_string(pose.Y().value() * 39.3701),
                    cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 1,
                    crossColor, 3);
                putText(frame, std::to_string(pose.Z().value() * 39.3701),
                    cv::Point(10, 150), cv::FONT_HERSHEY_SIMPLEX, 1,
                    crossColor, 3);
                putText(frame, std::to_string(rotation.X().value() * 180 / IM_PI),
                    cv::Point(10, 200), cv::FONT_HERSHEY_SIMPLEX, 1,
                    crossColor, 3);
                putText(frame, std::to_string(rotation.Y().value() * 180 / IM_PI),
                    cv::Point(10, 250), cv::FONT_HERSHEY_SIMPLEX, 1,
                    crossColor, 3);
                putText(frame, std::to_string(rotation.Z().value() * 180 / IM_PI),
                    cv::Point(10, 300), cv::FONT_HERSHEY_SIMPLEX, 1,
                    crossColor, 3);

            }
            
            auto tracked_target = photon::PhotonTrackedTarget();
            std::vector<photon::PhotonTrackedTarget> tracked_targets = { tracked_target };
            auto pipeline_result = photon::PhotonPipelineResult(20_ms, tracked_targets);
            photon::PhotonPoseEstimator pose_estimator = photon::PhotonPoseEstimator(field_layout, photon::PoseStrategy::AVERAGE_BEST_TARGETS, frc::Transform3d());
            pose_estimator.Update(pipeline_result);
            
            //putText(frame, std::to_string(rotation.Z().value() * 180 / IM_PI),
                //cv::Point(300, 50), cv::FONT_HERSHEY_SIMPLEX, 1,
                //crossColor, 3);
        }
        cv::waitKey(1);
        cv::imshow("AprilTags :3", frame);
    }
    cap.release();
}