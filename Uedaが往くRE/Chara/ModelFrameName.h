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
	const std::string kLeftFoot = kPlayerRig + "LeftFoot";			// 左足首
	const std::string kLeftEnd = kPlayerRig + "LeftToe_End_end";	// 左足終点

	const std::string kRightUpLeg = kPlayerRig + "RightUpLeg";		// 右もも
	const std::string kRightLeg = kPlayerRig + "RightLeg";			// 右膝
	const std::string kRightFoot = kPlayerRig + "RightFoot";		// 右足首
	const std::string kRightEnd = kPlayerRig + "RightToe_End_end";	// 右足終点
}

// 敵のフレーム名
namespace EnemyFrameName
{
	const std::string kEnemyRig = "mixamorig12:";

	const std::string kLeftShoulder = kEnemyRig + "LeftShoulder";	// 左肩
	const std::string kLeftForeArm = kEnemyRig + "LeftForeArm";		// 左肘
	const std::string kLeftHandPos = kEnemyRig + "LeftHand";		// 左手首

	const std::string kRightShoulder = kEnemyRig + "RightShoulder";	// 右肩
	const std::string kRightForeArm = kEnemyRig + "RightForeArm";	// 右肘
	const std::string kRightHand = kEnemyRig + "RightHand";			// 右手首

	const std::string kLeftUpLeg = kEnemyRig + "LeftUpLeg";			// 左もも
	const std::string kLeftLeg = kEnemyRig + "LeftLeg";				// 左膝
	const std::string kLeftFoot = kEnemyRig + "LeftFoot";			// 左足首
	const std::string kLeftEnd = kEnemyRig + "LeftToe_End_end";		// 左足終点

	const std::string kRightUpLeg = kEnemyRig + "RightUpLeg";		// 右もも
	const std::string kRightLeg = kEnemyRig + "RightLeg";			// 右膝
	const std::string kRightFoot = kEnemyRig + "RightFoot";			// 右足首
	const std::string kRightEnd = kEnemyRig + "RightToe_End_end";	// 右足終点
}