#include "ConversionTime.h"

namespace Conversion
{
	int ChangeHour(int frame)
	{
		return (frame / (60 * 60 * 60));
	}

	int ChangeMin(int frame)
	{
		return (frame / (60 * 60)) % 60;
	}

	int ChangeSec(int frame)
	{
		return (frame / 60) % 60;
	}

	int ChangeMilliSec(int frame)
	{
		return (frame * 1000 / 60) % 1000;
	}
}