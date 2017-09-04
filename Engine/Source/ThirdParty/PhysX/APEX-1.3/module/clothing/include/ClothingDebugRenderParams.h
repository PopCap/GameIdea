/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */


// This file was generated by NxParameterized/scripts/GenParameterized.pl
// Created: 2015.06.02 04:11:48

#ifndef HEADER_ClothingDebugRenderParams_h
#define HEADER_ClothingDebugRenderParams_h

#include "NxParametersTypes.h"

#ifndef NX_PARAMETERIZED_ONLY_LAYOUTS
#include "NxParameterized.h"
#include "NxParameters.h"
#include "NxParameterizedTraits.h"
#include "NxTraitsInternal.h"
#endif

namespace physx
{
namespace apex
{
namespace clothing
{

#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to __declspec(align())

namespace ClothingDebugRenderParamsNS
{



struct ParametersStruct
{

	bool Actors;
	physx::PxF32 SkinnedPositions;
	bool Backstop;
	physx::PxF32 BackstopPrecise;
	bool MaxDistance;
	bool MaxDistanceInwards;
	bool SkinMapAll;
	bool SkinMapBad;
	bool SkinMapActual;
	bool SkinMapInvalidBary;
	physx::PxF32 PhysicsMeshWire;
	physx::PxF32 PhysicsMeshSolid;
	physx::PxF32 PhysicsMeshNormals;
	bool Skeleton;
	physx::PxF32 BoneFrames;
	physx::PxF32 BoneNames;
	physx::PxF32 Velocities;
	physx::PxF32 Wind;
	bool GraphicalVertexBones;
	bool PhysicalVertexBones;
	bool CollisionShapes;
	bool CollisionShapesWire;
	bool LengthFibers;
	bool CrossSectionFibers;
	bool BendingFibers;
	bool ShearingFibers;
	bool ZerostretchFibers;
	bool TethersActive;
	bool TethersInactive;
	bool VirtualCollision;
	bool FiberRange;
	bool ShowInLocalSpace;
	bool GlobalPose;
	bool RecomputeSubmeshes;
	bool RecomputeVertices;
	bool PhysicsMeshIndices;
	bool MassScale;
	bool SelfCollision;
	bool SelfCollisionWire;
	physx::PxF32 SelfCollisionAttenuation;
	bool SolverMode;

};

static const physx::PxU32 checksum[] = { 0x0814367c, 0x9b811470, 0x122fd821, 0xd5717b8e, };

} // namespace ClothingDebugRenderParamsNS

#ifndef NX_PARAMETERIZED_ONLY_LAYOUTS
class ClothingDebugRenderParams : public NxParameterized::NxParameters, public ClothingDebugRenderParamsNS::ParametersStruct
{
public:
	ClothingDebugRenderParams(NxParameterized::Traits* traits, void* buf = 0, PxI32* refCount = 0);

	virtual ~ClothingDebugRenderParams();

	virtual void destroy();

	static const char* staticClassName(void)
	{
		return("ClothingDebugRenderParams");
	}

	const char* className(void) const
	{
		return(staticClassName());
	}

	static const physx::PxU32 ClassVersion = ((physx::PxU32)0 << 16) + (physx::PxU32)0;

	static physx::PxU32 staticVersion(void)
	{
		return ClassVersion;
	}

	physx::PxU32 version(void) const
	{
		return(staticVersion());
	}

	static const physx::PxU32 ClassAlignment = 8;

	static const physx::PxU32* staticChecksum(physx::PxU32& bits)
	{
		bits = 8 * sizeof(ClothingDebugRenderParamsNS::checksum);
		return ClothingDebugRenderParamsNS::checksum;
	}

	static void freeParameterDefinitionTable(NxParameterized::Traits* traits);

	const physx::PxU32* checksum(physx::PxU32& bits) const
	{
		return staticChecksum(bits);
	}

	const ClothingDebugRenderParamsNS::ParametersStruct& parameters(void) const
	{
		ClothingDebugRenderParams* tmpThis = const_cast<ClothingDebugRenderParams*>(this);
		return *(static_cast<ClothingDebugRenderParamsNS::ParametersStruct*>(tmpThis));
	}

	ClothingDebugRenderParamsNS::ParametersStruct& parameters(void)
	{
		return *(static_cast<ClothingDebugRenderParamsNS::ParametersStruct*>(this));
	}

	virtual NxParameterized::ErrorType getParameterHandle(const char* long_name, NxParameterized::Handle& handle) const;
	virtual NxParameterized::ErrorType getParameterHandle(const char* long_name, NxParameterized::Handle& handle);

	void initDefaults(void);

protected:

	virtual const NxParameterized::DefinitionImpl* getParameterDefinitionTree(void);
	virtual const NxParameterized::DefinitionImpl* getParameterDefinitionTree(void) const;


	virtual void getVarPtr(const NxParameterized::Handle& handle, void*& ptr, size_t& offset) const;

private:

	void buildTree(void);
	void initDynamicArrays(void);
	void initStrings(void);
	void initReferences(void);
	void freeDynamicArrays(void);
	void freeStrings(void);
	void freeReferences(void);

	static bool mBuiltFlag;
	static NxParameterized::MutexType mBuiltFlagMutex;
};

class ClothingDebugRenderParamsFactory : public NxParameterized::Factory
{
	static const char* const vptr;

public:
	virtual NxParameterized::Interface* create(NxParameterized::Traits* paramTraits)
	{
		// placement new on this class using mParameterizedTraits

		void* newPtr = paramTraits->alloc(sizeof(ClothingDebugRenderParams), ClothingDebugRenderParams::ClassAlignment);
		if (!NxParameterized::IsAligned(newPtr, ClothingDebugRenderParams::ClassAlignment))
		{
			NX_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ClothingDebugRenderParams");
			paramTraits->free(newPtr);
			return 0;
		}

		memset(newPtr, 0, sizeof(ClothingDebugRenderParams)); // always initialize memory allocated to zero for default values
		return NX_PARAM_PLACEMENT_NEW(newPtr, ClothingDebugRenderParams)(paramTraits);
	}

	virtual NxParameterized::Interface* finish(NxParameterized::Traits* paramTraits, void* bufObj, void* bufStart, physx::PxI32* refCount)
	{
		if (!NxParameterized::IsAligned(bufObj, ClothingDebugRenderParams::ClassAlignment)
		        || !NxParameterized::IsAligned(bufStart, ClothingDebugRenderParams::ClassAlignment))
		{
			NX_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ClothingDebugRenderParams");
			return 0;
		}

		// Init NxParameters-part
		// We used to call empty constructor of ClothingDebugRenderParams here
		// but it may call default constructors of members and spoil the data
		NX_PARAM_PLACEMENT_NEW(bufObj, NxParameterized::NxParameters)(paramTraits, bufStart, refCount);

		// Init vtable (everything else is already initialized)
		*(const char**)bufObj = vptr;

		return (ClothingDebugRenderParams*)bufObj;
	}

	virtual const char* getClassName()
	{
		return (ClothingDebugRenderParams::staticClassName());
	}

	virtual physx::PxU32 getVersion()
	{
		return (ClothingDebugRenderParams::staticVersion());
	}

	virtual physx::PxU32 getAlignment()
	{
		return (ClothingDebugRenderParams::ClassAlignment);
	}

	virtual const physx::PxU32* getChecksum(physx::PxU32& bits)
	{
		return (ClothingDebugRenderParams::staticChecksum(bits));
	}
};
#endif // NX_PARAMETERIZED_ONLY_LAYOUTS

} // namespace clothing
} // namespace apex
} // namespace physx

#pragma warning(pop)

#endif
