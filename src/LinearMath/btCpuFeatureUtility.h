
#ifndef BT_CPU_UTILITY_H
#define BT_CPU_UTILITY_H

#include "LinearMath/btScalar.h"

/// Rudimentary btCpuFeatureUtility for CPU features: only report the features that Bullet actually uses (SSE4/FMA3, NEON_HPFP)
/// We assume SSE2 in case BT_USE_SSE2 is defined in LinearMath/btScalar.h
class btCpuFeatureUtility
{
public:
	enum btCpuFeature
	{
		CPU_FEATURE_FMA3 = 1,
		CPU_FEATURE_SSE4_1 = 2,
		CPU_FEATURE_NEON_HPFP = 4
	};

	static int getCpuFeatures()
	{
		int capabilities = 0;

#ifdef BT_USE_NEON
		{
			capabilities |= CPU_FEATURE_NEON_HPFP;
		}
#endif // BT_USE_NEON

#ifdef BT_ALLOW_SSE4
		{
			capabilities |= btCpuFeatureUtility::CPU_FEATURE_FMA3;
			capabilities |= btCpuFeatureUtility::CPU_FEATURE_SSE4_1;
		}
#endif // BT_ALLOW_SSE4

		return capabilities;
	}
};

#endif // BT_CPU_UTILITY_H
