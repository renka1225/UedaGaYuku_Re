#pragma once
#include <string>

// プレイヤーのフレーム名
namespace PlayerFrameName
{
	const std::string kPlayerRig = "mixamorig:";

	const std::string kLeftShoulder = kPlayerRig + "LeftShoulder";	// 左肩
	const std::string kLeftForeArm = kPlayerRig + "LeftForeArm";	// 左肘
	const std::string kLeftHandPos = kPlayerRig + "LeftHand";		// 左手首

	const std::string kRightShoulder = kPlayerRig + "RightShoulder";// 右肩
	const std::string kRightForeArm = kPlayerRig + "RightForeArm";	// 右肘
	const std::string kRightHand = kPlayerRig + "RightHand";		// 右手首

	const std::string kLeftUpLeg = kPlayerRig + "LeftUpLeg";		// 左もも
	const std::string kLeftLeg = kPlayerRig + "LeftLeg";			// 左膝
	const std::string kLeftFoot = kPlayerRig + "LeftToeBase";		// 左足首
	const std::string kLeftEnd = kPlayerRig + "LeftToe_End_end";	// 左足終点

	const std::string kRightUpLeg = kPlayerRig + "RightUpLeg";		// 右もも
	const std::string kRightLeg = kPlayerRig + "RightLeg";			// 右膝
	const std::string kRightFoot = kPlayerRig + "RightToeBase";		// 右足首
	const std::string kRightEnd = kPlayerRig + "RightToe_End_end";	// 右足終点
}

// 敵のフレーム名
namespace EnemyFrameName
{
	const std::string kLeftShoulder = "LeftShoulder";	// 左肩
	const std::string kLeftForeArm = "LeftForeArm";		// 左肘
	const std::string kLeftHandPos = "LeftHand";		// 左手首

	const std::string kRightShoulder = "RightShoulder";	// 右肩
	const std::string kRightForeArm = "RightForeArm";	// 右肘
	const std::string kRightHand = "RightHand";			// 右手首

	const std::string kLeftUpLeg = "LeftUpLeg";			// 左もも
	const std::string kLeftLeg = "LeftLeg";				// 左膝
	const std::string kLeftFoot = "LeftFoot";			// 左足首
	const std::string kLeftEnd = "LeftToe_End_end";		// 左足終点

	const std::string kRightUpLeg = "RightUpLeg";		// 右もも
	const std::string kRightLeg = "RightLeg";			// 右膝
	const std::string kRightFoot = "RightFoot";			// 右足首
	const std::string kRightEnd = "RightToe_End_end";	// 右足終点
}