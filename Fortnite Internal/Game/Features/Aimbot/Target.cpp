#include "Target.h"

#include "../../../Utilities/Math.h"
#include "../../../Configs/Config.h"

#include "../FortPawnHelper/Bone.h"

void Features::Aimbot::Target::UpdateLocalInfoAndType(Target& TargetToUpdate) {
	switch (TargetToUpdate.GlobalInfo.Type) {
	case TargetType::ClosePlayer:
		TargetToUpdate.LocalInfo.CurrentFOVSizePixels = (float)Config::Aimbot::CloseAim::FOV * Game::PixelsPerDegree;
		TargetToUpdate.LocalInfo.CurrentFOVSizeDegrees = Config::Aimbot::CloseAim::FOV;
		TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::CloseAim::Smoothing;

		if (TargetToUpdate.LocalInfo.DistanceFromPlayer > Config::Aimbot::CloseAim::Range || !Config::Aimbot::CloseAim::Enabled) {
			TargetToUpdate.GlobalInfo.Type = TargetType::FarPlayer;
		}
		break;
	case TargetType::FarPlayer:
		TargetToUpdate.LocalInfo.CurrentFOVSizePixels = (float)Config::Aimbot::Standard::FOV * Game::PixelsPerDegree;
		TargetToUpdate.LocalInfo.CurrentFOVSizeDegrees = Config::Aimbot::Standard::FOV;
		TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Standard::Smoothing;

		if (TargetToUpdate.LocalInfo.DistanceFromPlayer <= Config::Aimbot::CloseAim::Range && Config::Aimbot::CloseAim::Enabled) {
			TargetToUpdate.GlobalInfo.Type = TargetType::ClosePlayer;
		}
		break;
	case TargetType::Weakspot:
		TargetToUpdate.LocalInfo.CurrentFOVSizePixels = (float)Config::Aimbot::Weakspot::FOV * Game::PixelsPerDegree;
		TargetToUpdate.LocalInfo.CurrentFOVSizeDegrees = Config::Aimbot::Weakspot::FOV;
		TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Weakspot::Smoothing;
		break;
	case TargetType::Other:
		TargetToUpdate.LocalInfo.CurrentFOVSizePixels = (float)Config::Aimbot::Standard::FOV * Game::PixelsPerDegree;
		TargetToUpdate.LocalInfo.CurrentFOVSizeDegrees = Config::Aimbot::Standard::FOV;
		TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Standard::Smoothing;
		break;
	}
}
void Features::Aimbot::Target::ResetTarget() {
	// Reset the global info
	{
		GlobalInfo.Type = TargetType::NONE;

		GlobalInfo.TargetActor = nullptr;

		GlobalInfo.TargetActorPosition = SDK::FVector(0, 0, 0);
		GlobalInfo.TargetActorPosition2D = SDK::FVector2D(0, 0);

		GlobalInfo.TargetBonePosition = SDK::FVector(0, 0, 0);
		GlobalInfo.TargetBonePosition2D = SDK::FVector2D(0, 0);
		GlobalInfo.TargetBoneId = Features::FortPawnHelper::Bone::None;
	}

	// Reset the local info
	{
		// Set to float max so that any target will be less than this
		LocalInfo.DistanceFromCrosshairDegrees = FLT_MAX;
		LocalInfo.DistanceFromCrosshairPixels = FLT_MAX;
		LocalInfo.DistanceFromPlayer = FLT_MAX;
		LocalInfo.SmartTargetingDistance = FLT_MAX;

		LocalInfo.IsTargeting = false;
		LocalInfo.IsOnScreen = false;

		LocalInfo.TargetRotation = SDK::FRotator(0, 0, 0);
		LocalInfo.TargetRotationChange = SDK::FRotator(0, 0, 0);

		LocalInfo.CurrentFOVSizePixels = 0;
		LocalInfo.CurrentFOVSizeDegrees = 0;
		LocalInfo.CurrentSmoothing = 0.f;
	}
}
void Features::Aimbot::Target::TargetTick(bool SeenTargetThisFrame) {
	// Resets if:
	// - The target wasn't seen this frame
	// - The target is outside of the FOV circle in pixels and is on the screen
	// - The target is outside of the FOV circle in degrees and is NOT on the screen
	// - The target pointer is null

	UpdateLocalInfoAndType(*this);

	if (LocalInfo.IsOnScreen) {
		if (LocalInfo.DistanceFromCrosshairPixels > LocalInfo.CurrentFOVSizePixels) {
			// If the player is on the screen and outside of the FOV circle in pixels, then don't update the target
			ResetTarget();
		}
	}
	else {
		if (LocalInfo.DistanceFromCrosshairDegrees > LocalInfo.CurrentFOVSizeDegrees) {
			// If the player is on the screen and outside of the FOV circle in degrees, then don't update the target
			ResetTarget();
		}
	}

	if (!SeenTargetThisFrame || GlobalInfo.TargetActor == nullptr) {
		ResetTarget();
		return;
	}
}
bool Features::Aimbot::Target::ShouldSetTarget(Target PotentialTarget) {
	// Long reason to why we have to do this goofy method, but it's pretty much because the player camera isn't first person
	// and our calculate rotation angles function doesn't take that into account.
	// So we have to do this goofy method to simulate the distance from the crosshair in degrees. It's not perfect, but it's good enough and not noticeable.

	// Update the info so that we use the correct FOV size for verifying if we should update the target
	UpdateLocalInfoAndType(PotentialTarget);

	if (PotentialTarget.LocalInfo.IsOnScreen) {
		if (PotentialTarget.LocalInfo.DistanceFromCrosshairPixels > PotentialTarget.LocalInfo.CurrentFOVSizePixels) {
			// If the player is on the screen and outside of the FOV circle in pixels, then don't update the target
			return false;
		}
	}
	else {
		if (PotentialTarget.LocalInfo.DistanceFromCrosshairDegrees > PotentialTarget.LocalInfo.CurrentFOVSizeDegrees) {
			// If the player is on the screen and outside of the FOV circle in degrees, then don't update the target
			return false;
		}
	}

	if (PotentialTarget.GlobalInfo.Type > GlobalInfo.Type) {
		// If the target priority is lower than the current target, then don't update the target
		return false;
	}
	else if (PotentialTarget.GlobalInfo.Type < GlobalInfo.Type) {
		// If the target priority is higher than the current target, then update the target
		return true;
	}
	else if (PotentialTarget.GlobalInfo.Type == GlobalInfo.Type) {
		// If the target priority is the same as the current target, then check the parameters
		if (LocalInfo.IsTargeting) {
			// If we are targeting, then don't update
			return false;
		}

		float CurrentDistance;
		float PotentialTargetDistance;

		// Get the distance based off the target distance types
		switch (Config::Aimbot::TargettingType) {
		case ConfigTypes::AimbotType::Smart:
			CurrentDistance = LocalInfo.SmartTargetingDistance;
			PotentialTargetDistance = PotentialTarget.LocalInfo.SmartTargetingDistance;
			break;
		case ConfigTypes::AimbotType::Crosshair:
			CurrentDistance = LocalInfo.DistanceFromCrosshairDegrees;
			PotentialTargetDistance = PotentialTarget.LocalInfo.DistanceFromCrosshairDegrees;
			break;
		case ConfigTypes::AimbotType::Distance:
			CurrentDistance = LocalInfo.DistanceFromPlayer;
			PotentialTargetDistance = PotentialTarget.LocalInfo.DistanceFromPlayer;
			break;
		default:
			return false;
		}

		if (PotentialTargetDistance < CurrentDistance) {
			// If the potential new target distance is less than the current target distance, then update the target
			return true;
		}
	}

	return false;
}
void Features::Aimbot::Target::SetTarget(Target NewTarget, bool ForceSetTarget) {
	UpdateLocalInfoAndType(NewTarget);

	if (ForceSetTarget) {
		*this = NewTarget;
		return;
	}
	else if (ShouldSetTarget(NewTarget)) {
		*this = NewTarget;
		return;
	}
}

void Features::Aimbot::PlayerTarget::UpdateTargetInfo(Target& Target, Actors::Caches::FortPawnCache& TargetCache, const Actors::CameraCache& MainCamera, const Actors::CameraCache& AimbotCamera, const float FPSScale) {
	// Update global information
	Target.GlobalInfo.TargetActor = TargetCache.FortPawn;
	Target.GlobalInfo.TargetBoneId = Features::FortPawnHelper::Bone::FindBestBone(Features::FortPawnHelper::Bone::Head, TargetCache);

	// Determine target type
	Target.GlobalInfo.Type = (Target.LocalInfo.DistanceFromPlayer <= Config::Aimbot::CloseAim::Range && Config::Aimbot::CloseAim::Enabled) ? Target::TargetType::ClosePlayer : Target::TargetType::FarPlayer;

	// Update positions
	Target.GlobalInfo.TargetActorPosition = TargetCache.BoneRegister[Features::FortPawnHelper::Bone::Root];
	Target.GlobalInfo.TargetActorPosition2D = TargetCache.BoneRegister2D[Features::FortPawnHelper::Bone::Root];
	Target.GlobalInfo.TargetBonePosition = TargetCache.BoneRegister[Target.GlobalInfo.TargetBoneId];
	Target.GlobalInfo.TargetBonePosition2D = TargetCache.BoneRegister2D[Target.GlobalInfo.TargetBoneId];

	SDK::FRotator TargetCameraRotation = SDK::UKismetMathLibrary::StaticClass()->FindLookAtRotation(AimbotCamera.Position, Target.GlobalInfo.TargetBonePosition);

	// Update local information
	Target.LocalInfo.DistanceFromCrosshairDegrees = Math::GetDegreeDistance(MainCamera.Rotation, TargetCameraRotation);
	Target.LocalInfo.DistanceFromCrosshairPixels = Math::GetDistance2D(Target.GlobalInfo.TargetBonePosition2D.X, Target.GlobalInfo.TargetBonePosition2D.Y, Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f);
	Target.LocalInfo.DistanceFromPlayer = TargetCache.DistanceFromLocal;
	Target.LocalInfo.SmartTargetingDistance = Target.LocalInfo.DistanceFromCrosshairDegrees + Target.LocalInfo.DistanceFromPlayer;
	Target.LocalInfo.IsOnScreen = TargetCache.IsOnScreen;

	// Apply FPS scaling for smoothing
	if (FPSScale) {
		float AimbotSpeed;
		if (Target.LocalInfo.CurrentSmoothing <= 1) {
			AimbotSpeed = Target.LocalInfo.CurrentSmoothing;
		}
		else {
			AimbotSpeed = Target.LocalInfo.CurrentSmoothing * FPSScale;
		}
		Target.LocalInfo.TargetRotation = TargetCameraRotation;

		// Calculate smoothed rotation
		Target.LocalInfo.TargetRotationChange = SDK::FRotator(Target.LocalInfo.TargetRotation.Pitch - AimbotCamera.Rotation.Pitch, Target.LocalInfo.TargetRotation.Yaw - AimbotCamera.Rotation.Yaw, 0.f);
		Target.LocalInfo.TargetRotationChange = Math::NormalizeAxis(Target.LocalInfo.TargetRotationChange);

		SDK::FRotator RotationAfterSmooth = Target.LocalInfo.TargetRotationChange / AimbotSpeed;
		RotationAfterSmooth = Math::NormalizeAxis(RotationAfterSmooth);

		Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(AimbotCamera.Rotation.Pitch + RotationAfterSmooth.Pitch, AimbotCamera.Rotation.Yaw + RotationAfterSmooth.Yaw, 0.f);
		Target.LocalInfo.TargetRotationWithSmooth = Math::NormalizeAxis(Target.LocalInfo.TargetRotationWithSmooth);
		Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(Target.LocalInfo.TargetRotationWithSmooth.Pitch, Target.LocalInfo.TargetRotationWithSmooth.Yaw, 0.f); // 0 on the roll so the camera doesn't get stuck tilted
	}
}

void Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(Target& Target, SDK::ABuildingWeakSpot* WeakSpot, const Actors::CameraCache& MainCamera, const Actors::CameraCache& AimbotCamera, const float FPSScale) {
	// Update global information
	Target.GlobalInfo.TargetActor = WeakSpot;

	// Set target type
	Target.GlobalInfo.Type = Target::TargetType::Weakspot;

	// Update positions
	SDK::FVector RootComponentPosition = WeakSpot->GetRootComponent()->GetPosition();
	SDK::FVector2D RootComponentPosition2D = SDK::Project(RootComponentPosition);

	Target.GlobalInfo.TargetActorPosition = RootComponentPosition;
	Target.GlobalInfo.TargetActorPosition2D = RootComponentPosition2D;
	Target.GlobalInfo.TargetBonePosition = RootComponentPosition;
	Target.GlobalInfo.TargetBonePosition2D = RootComponentPosition2D;

	SDK::FRotator TargetCameraRotation = SDK::UKismetMathLibrary::StaticClass()->FindLookAtRotation(AimbotCamera.Position, Target.GlobalInfo.TargetBonePosition);

	// Update local information
	Target.LocalInfo.DistanceFromCrosshairDegrees = Math::GetDegreeDistance(MainCamera.Rotation, TargetCameraRotation);
	Target.LocalInfo.DistanceFromCrosshairPixels = Math::GetDistance2D(Target.GlobalInfo.TargetActorPosition2D.X, Target.GlobalInfo.TargetActorPosition2D.Y, Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f);
	Target.LocalInfo.DistanceFromPlayer = Actors::LocalPawnCache.Position.Distance(Target.GlobalInfo.TargetActorPosition) / 100.f;
	Target.LocalInfo.SmartTargetingDistance = Target.LocalInfo.DistanceFromCrosshairDegrees + Target.LocalInfo.DistanceFromPlayer;
	Target.LocalInfo.IsOnScreen = Math::IsOnScreen(Target.GlobalInfo.TargetActorPosition2D);

	// Apply FPS scaling for smoothing
	if (FPSScale) {
		float AimbotSpeed;
		if (Target.LocalInfo.CurrentSmoothing <= 1) {
			AimbotSpeed = Target.LocalInfo.CurrentSmoothing;
		}
		else {
			AimbotSpeed = Target.LocalInfo.CurrentSmoothing * FPSScale;
		}
		Target.LocalInfo.TargetRotation = TargetCameraRotation;

		// Calculate smoothed rotation
		Target.LocalInfo.TargetRotationChange = SDK::FRotator(Target.LocalInfo.TargetRotation.Pitch - AimbotCamera.Rotation.Pitch, Target.LocalInfo.TargetRotation.Yaw - AimbotCamera.Rotation.Yaw, 0.f);
		Target.LocalInfo.TargetRotationChange = Math::NormalizeAxis(Target.LocalInfo.TargetRotationChange);

		SDK::FRotator RotationAfterSmooth = Target.LocalInfo.TargetRotationChange / AimbotSpeed;
		RotationAfterSmooth = Math::NormalizeAxis(RotationAfterSmooth);

		Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(AimbotCamera.Rotation.Pitch + RotationAfterSmooth.Pitch, AimbotCamera.Rotation.Yaw + RotationAfterSmooth.Yaw, 0.f);
		Target.LocalInfo.TargetRotationWithSmooth = Math::NormalizeAxis(Target.LocalInfo.TargetRotationWithSmooth);
		Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(Target.LocalInfo.TargetRotationWithSmooth.Pitch, Target.LocalInfo.TargetRotationWithSmooth.Yaw, 0.f); // 0 on the roll so the camera doesn't get stuck tilted
	}
}