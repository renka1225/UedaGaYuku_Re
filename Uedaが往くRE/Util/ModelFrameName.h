#pragma once
#include <string>

// モデルのフレーム名

namespace
{
	const std::string kModelRig = "mixamorig:";
}

namespace PlayerFrameName
{
	std::string kLeftShoulder = kModelRig + "LeftShoulder";		// 左肩
	std::string kLeftForeArm = kModelRig + "LeftForeArm";		// 左肘
	std::string kLeftHandPos =  kModelRig + "LeftHand";			// 左手首

	std::string kRightShoulder = kModelRig + "RightShoulder";	// 右肩
	std::string kRightForeArm = kModelRig + "RightForeArm";		// 右肘
	std::string kRightHand = kModelRig + "RightHand";			// 右手首

	std::string kLeftUpLeg = kModelRig + "LeftUpLeg";			// 左もも
	std::string kLeftLeg = kModelRig + "LeftLeg";				// 左膝
	std::string kLeftFoot = kModelRig + "LeftFoot";				// 左足首
	std::string kLeftEnd = kModelRig + "LeftToe_End_end";		// 左足終点

	std::string kRightUpLeg = kModelRig + "RightUpLeg";			// 右もも
	std::string kRightLeg = kModelRig + "RightLeg";				// 右膝
	std::string kRightFoot = kModelRig + "RightFoot";			// 右足首
	std::string kRightEnd = kModelRig + "RightToe_End_end";		// 右足終点
}