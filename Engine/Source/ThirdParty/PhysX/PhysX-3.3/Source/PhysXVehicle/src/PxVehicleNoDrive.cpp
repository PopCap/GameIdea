/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#include "PxVehicleNoDrive.h"
#include "PxVehicleWheels.h"
#include "PxVehicleSDK.h"
#include "PxVehicleSuspWheelTire4.h"
#include "PxVehicleSuspLimitConstraintShader.h"
#include "PxVehicleDefaults.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "PsFoundation.h"
#include "PsUtilities.h"
#include "CmPhysXCommon.h"
#include "PxScene.h"
#include "PxMemory.h"
#include "CmUtils.h"

namespace physx
{

bool PxVehicleNoDrive::isValid() const
{
	PX_CHECK_AND_RETURN_VAL(PxVehicleWheels::isValid(), "invalid PxVehicleDrive", false);
	return true;
}

PxVehicleNoDrive* PxVehicleNoDrive::allocate(const PxU32 numWheels)
{
	PX_CHECK_AND_RETURN_NULL(numWheels>0, "Cars with zero wheels are illegal");

	//Compute the bytes needed.
	const PxU32 numWheels4 = (((numWheels + 3) & ~3) >> 2);
	const PxU32 inputByteSize16 = sizeof(PxReal)*numWheels4*4;
	const PxU32 byteSize = sizeof(PxVehicleNoDrive) + 3*inputByteSize16 + PxVehicleWheels::computeByteSize(numWheels4);

	//Allocate the memory.
	PxVehicleNoDrive* veh = (PxVehicleNoDrive*)PX_ALLOC(byteSize, PX_DEBUG_EXP("PxVehicleNoDrive"));
	Cm::markSerializedMem(veh, byteSize);
	new(veh) PxVehicleNoDrive();

	//Patch up the pointers.
	PxU8* ptr = (PxU8*)veh + sizeof(PxVehicleNoDrive);
	ptr=PxVehicleWheels::patchupPointers(veh,ptr,numWheels4,numWheels);
	veh->mSteerAngles = (PxReal*)ptr;
	ptr+=inputByteSize16;
	veh->mDriveTorques = (PxReal*)ptr;
	ptr+=inputByteSize16;
	veh->mBrakeTorques = (PxReal*)ptr;
	ptr+=inputByteSize16;

	PxMemZero(veh->mSteerAngles, inputByteSize16);
	PxMemZero(veh->mDriveTorques, inputByteSize16);
	PxMemZero(veh->mBrakeTorques, inputByteSize16);

	//Set the vehicle type.
	veh->mType = PxVehicleTypes::eNODRIVE;

	return veh;
}

void PxVehicleNoDrive::free()
{
	PxVehicleWheels::free();
}

void PxVehicleNoDrive::setup
(PxPhysics* physics, PxRigidDynamic* vehActor, const PxVehicleWheelsSimData& wheelsData)
{
	//Set up the wheels.
	PxVehicleWheels::setup(physics,vehActor,wheelsData,0,wheelsData.getNbWheels());
}

PxVehicleNoDrive* PxVehicleNoDrive::create
(PxPhysics* physics, PxRigidDynamic* vehActor, 
 const PxVehicleWheelsSimData& wheelsData)
{
	PxVehicleNoDrive* veh=PxVehicleNoDrive::allocate(wheelsData.getNbWheels());
	veh->setup(physics,vehActor,wheelsData);
	return veh;
}

void PxVehicleNoDrive::setToRestState()
{
	const PxU32 numWheels4 = (((mWheelsSimData.getNbWheels() + 3) & ~3) >> 2);
	const PxU32 inputByteSize = sizeof(PxReal)*numWheels4*4;
	const PxU32 inputByteSize16 = (inputByteSize + 15) & ~15;
	PxMemZero(mSteerAngles, 3*inputByteSize16);

	//Set core to rest state.
	PxVehicleWheels::setToRestState();
}

void PxVehicleNoDrive::setBrakeTorque(const PxU32 id, const PxReal brakeTorque)
{
	PX_CHECK_AND_RETURN(id < mWheelsSimData.getNbWheels(), "PxVehicleNoDrive::setBrakeTorque - Illegal wheel");
	PX_CHECK_AND_RETURN(brakeTorque>=0, "PxVehicleNoDrive::setBrakeTorque - negative brake torques are illegal");
	mBrakeTorques[id] = brakeTorque;
}

void PxVehicleNoDrive::setDriveTorque(const PxU32 id, const PxReal driveTorque)
{
	PX_CHECK_AND_RETURN(id < mWheelsSimData.getNbWheels(), "PxVehicleNoDrive::setDriveTorque - Illegal wheel");
	mDriveTorques[id] = driveTorque;
}

void PxVehicleNoDrive::setSteerAngle(const PxU32 id, const PxReal steerAngle)
{
	PX_CHECK_AND_RETURN(id < mWheelsSimData.getNbWheels(), "PxVehicleNoDrive::setSteerAngle - Illegal wheel");
	mSteerAngles[id] = steerAngle;
}

PxReal PxVehicleNoDrive::getBrakeTorque(const PxU32 id) const
{
	PX_CHECK_AND_RETURN_VAL(id < mWheelsSimData.getNbWheels(), "PxVehicleNoDrive::getBrakeTorque - Illegal wheel", 0);
	return mBrakeTorques[id];
}

PxReal PxVehicleNoDrive::getDriveTorque(const PxU32 id) const
{
	PX_CHECK_AND_RETURN_VAL(id < mWheelsSimData.getNbWheels(), "PxVehicleNoDrive::getDriveTorque - Illegal wheel",0);
	return mDriveTorques[id];
}

PxReal PxVehicleNoDrive::getSteerAngle(const PxU32 id) const
{
	PX_CHECK_AND_RETURN_VAL(id < mWheelsSimData.getNbWheels(), "PxVehicleNoDrive::getSteerAngle - Illegal wheel",0);
	return mSteerAngles[id];
}

} //namespace physx

