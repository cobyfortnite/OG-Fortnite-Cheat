#pragma once
#include "SDK.h"

#include "Classes/CoreUObject_Classes.h"

#include "../../External-Libs/skCrypter.h"

#include "../../Utilities/Error.h"
#include "../../Utilities/Memory.h"

/* This class is used to initialize the SDK by updating the offsets and VFT indicies */
class SDKInitializer {
private:
	/* Cached address for "EditModeInputComponent0" string (to avoid searching multiple times) */
	static uintptr_t EditModeInputComponent0;

private:
	/*
	 * @brief Finds the first wildcard (question mark) in a given pattern.
	 *
	 * @param Pattern - The pattern to search for wildcard (narrow character)
	 * @return Returns - the index the wildcard was found divided by 3 (2 characters for byte, 1 for space)
	 */
	static const int FindFirstWildCard(const char* Pattern) {
		int Position = 0;
		const char* CurrentChar = Pattern;

		while (*CurrentChar != '\0') {
			if (*CurrentChar == '?') {
				// divide by 3 to account for the 2 characters for one byte + 1 character for one space
				return Position / 3;
			}

			++CurrentChar;
			++Position;
		}

		THROW_ERROR(std::string(skCrypt("Failed to extract first wildcard!")), true);
		return 0;
	}

	/*
	 * @brief Initializes a VFT (Virtual Function Table) index using a search string and ranged pattern scanning
	 *
	 * @param VFTName - The name of the VFT
	 * @param PossibleSigs - A vector of possible signatures for pattern scanning
	 * @param SearchString - The search string (wide character) used to find the address
	 * @param VFTIndex - Reference to the variable that will store the found VFT index
	 * @param SearchRange - The range of bytes to search for the pattern
	 * @param SearchBytesBehind - The amount of bytes behind the search string to start scanning
	 */
	static void InitVFTIndex(const char* VFTName, std::vector<const char*> PossibleSigs, const wchar_t* SearchString, uintptr_t& VFTIndex, int SearchRange, int SearchBytesBehind = 0x0);

	/*
	 * @brief Initializes a Function offset using a search string and ranged pattern scanning (Overload 1)
	 *
	 * @param FunctionName - The name of the function
	 * @param PossibleSigs - A vector of possible signatures for pattern scanning
	 * @param SearchString - The search string (wide character) used to find the address
	 * @param FunctionOffset - Reference to the variable that will store the found function offset
	 * @param SearchRange - The range of bytes to search for the pattern
	 * @param SearchBytesBehind - The amount of bytes behind the search string to start scanning
	 */
	static void InitFunctionOffset(const char* FunctionName, std::vector<const char*> PossibleSigs, const wchar_t* SearchString, uintptr_t& FunctionOffset, int SearchRange = 0x600, int SearchBytesBehind = 0x0);

	/*
	 * @brief Initializes a Function offset using a search string and ranged pattern scanning (Overload 2)
	 *
	 * @param FunctionName - The name of the function
	 * @param PossibleSigs - A vector of possible signatures for pattern scanning
	 * @param SearchString - The search string (narrow character) used to find the address
	 * @param FunctionOffset - Reference to the variable that will store the found function offset
	 * @param SearchRange - The range of bytes to search for the pattern
	 * @param SearchBytesBehind - The amount of bytes behind the search string to start scanning
	 */
	static void InitFunctionOffset(const char* FunctionName, std::vector<const char*> PossibleSigs, const char* SearchString, uintptr_t& FunctionOffset, int SearchRange = 0x600, int SearchBytesBehind = 0x0);

	/*
	* @brief Walks a VFT (Virtual Function Table) searching for a specific function
	*
	* @param TargetFunctionName - The name of the function to search for
	* @param VFT - The VFT to walk
	* @param SearchFunction - The search function used to find the address
	* @param VFTIndex - Reference to the variable that will store the found VFT index
	* @param SearchRange - The range of bytes to search for the pattern
	*/
	static void WalkVFT(const char* TargetFunctionName, void** VFT, void* TargetFunction, uintptr_t& VFTIndex, int SearchRange);

public:
	/* Update the GObject offset (for finding UObjects) */
	static void InitGObjects();

	/* Update the AppendString function offset (for converting FNames to strings) */
	static void InitAppendString();

	/* Update the FNameConstructor function offset (for creating FNames) */
	static void InitFNameConstructor();

	/* Update the LineTraceSingle function offset (for visible check) */
	static void InitLineTraceSingle();

	/* Update the CalculateShot function offset (for bullet tp) */
	static void InitCalculateShot();

	/* Update the RaycastMulti functino offset (for bullet tp v2) */
	static void InitRaycastMulti();

	/* Update the Fire function offset (for trigger bot) */
	static void InitFire();

	/* Update the EditSelectRelease function offset (for edit on release) */
	static void InitEditSelectRelease();

	/* Update the CompleteBuildingEditInteraction function offset (for edit on release) */
	static void InitCompleteBuildingEditInteraction();

	/* Update the PerformBuildingEditInteraction function offset (for disable pre-edits) */
	static void InitPerformBuildingEditInteraction();



	/* Update the DrawTransition VFT index (for engine rendering, and on ImGui builds for caching draw data) */
	static void InitDTIndex();

	/* Update the PostRender VFT index (for calling UFunctions) */
	static void InitPEIndex();

	/* Update the GetPlayerViewpoint VFT index (for SilentAim) */
	static void InitGPVIndex();

	/* Update the GetViewpoint VFT index (for SilentAim) */
	static void InitGVIndex();

	/*
	* @brief Update the GetWeaponStats VFT index (for some weapon exploits)
	*
	* @param WeaponObject - The weapon actor to get the VFT index from
	*/
	static void InitGetWeaponStatsIndex(const SDK::UObject* WeaponActor);



	// CREDITS TO: Dumper-7
	static uint32 FindCastFlagsOffset() {
		std::vector<std::pair<void*, SDK::EClassCastFlags>> infos = {
			{ SDK::UObject::FindObjectFast(std::string(skCrypt("Actor"))), SDK::EClassCastFlags::Actor},
			{ SDK::UObject::FindObjectFast(std::string(skCrypt("Class"))), SDK::EClassCastFlags::Field | SDK::EClassCastFlags::Struct | SDK::EClassCastFlags::Class }
		};
		return Memory::FindOffset(infos);
	}
	// CREDITS TO: Dumper-7
	static uint32 FindDefaultObjectOffset() {
		std::vector<std::pair<void*, void*>> infos = {
			{ SDK::UObject::FindObjectFast(std::string(skCrypt("Object"))), SDK::UObject::FindObjectFast(std::string(skCrypt("Default__Object"))) },
			{ SDK::UObject::FindObjectFast(std::string(skCrypt("Field"))), SDK::UObject::FindObjectFast(std::string(skCrypt("Default__Field"))) }
		};
		return Memory::FindOffset(infos);
	}
	// CREDITS TO: Dumper-7
	static uint32 FindSuperOffset() {
		std::vector<std::pair<void*, void*>> infos = {
			{ SDK::UObject::FindObjectFast(std::string(skCrypt("Struct"))), SDK::UObject::FindObjectFast(std::string(skCrypt("Field"))) },
			{ SDK::UObject::FindObjectFast(std::string(skCrypt("Class"))), SDK::UObject::FindObjectFast(std::string(skCrypt("Struct"))) }
		};

		// Thanks to the ue4 dev who decided UStruct should be spelled Ustruct
		if (infos[0].first == nullptr) {
			infos[0].first = infos[1].second = SDK::UObject::FindObjectFast(std::string(skCrypt("struct")));
		}

		return Memory::FindOffset(infos);
	}
	// CREDITS TO: Dumper-7
	static uint32 FindChildPropertiesOffset() {
		uint8* ObjA = (uint8*)SDK::UObject::FindObjectFast(std::string(skCrypt("Color")));
		uint8* ObjB = (uint8*)SDK::UObject::FindObjectFast(std::string(skCrypt("Guid")));

		return Memory::GetValidPointerOffset(ObjA, ObjB, SDK::UStruct::SuperOffset + (sizeof(void*) * 2), 0x80);
	}
	// CREDITS TO: Dumper-7
	static uint32 FindFunctionFlagsOffset() {
		std::vector<std::pair<void*, SDK::EFunctionFlags>> Infos;

		Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("WasInputKeyJustPressed"))), SDK::EFunctionFlags::Final | SDK::EFunctionFlags::Native | SDK::EFunctionFlags::Public | SDK::EFunctionFlags::BlueprintCallable | SDK::EFunctionFlags::BlueprintPure | SDK::EFunctionFlags::Const });
		Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("ToggleSpeaking"))), SDK::EFunctionFlags::Exec | SDK::EFunctionFlags::Native | SDK::EFunctionFlags::Public });
		Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("SwitchLevel"))), SDK::EFunctionFlags::Exec | SDK::EFunctionFlags::Native | SDK::EFunctionFlags::Public });

		int32 Ret = Memory::FindOffset(Infos);

		if (Ret == 0x28)
		{
			for (auto& [_, Flags] : Infos)
				Flags = Flags | SDK::EFunctionFlags::RequiredAPI;
		}

		return Memory::FindOffset(Infos);
	}
	// CREDITS TO: Dumper-7
	static uint32 FindChildrenOffset() {
		std::vector<std::pair<void*, void*>> Infos;

		Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("PlayerController"))), SDK::UObject::FindObjectFastInOuter(std::string(skCrypt("WasInputKeyJustReleased")), std::string(skCrypt("PlayerController"))) });
		Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("Controller"))), SDK::UObject::FindObjectFastInOuter(std::string(skCrypt("UnPossess")), std::string(skCrypt("Controller"))) });

		uint32 Ret = Memory::FindOffset(Infos);

		if (Ret == 0x28)
		{
			Infos.clear();

			Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("Vector"))), SDK::UObject::FindObjectFastInOuter(std::string(skCrypt("X")), std::string(skCrypt("Vector"))) });
			Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("Vector4"))), SDK::UObject::FindObjectFastInOuter(std::string(skCrypt("X")), std::string(skCrypt("Vector4"))) });
			Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("Vector2D"))), SDK::UObject::FindObjectFastInOuter(std::string(skCrypt("X")), std::string(skCrypt("Vector2D"))) });
			Infos.push_back({ SDK::UObject::FindObjectFast(std::string(skCrypt("Guid"))), SDK::UObject::FindObjectFastInOuter(std::string(skCrypt("A")), std::string(skCrypt("Guid"))) });

			return Memory::FindOffset(Infos);
		}

		return Ret;
	}
	// CREDITS TO: Dumper-7
	static uint32 FindUFieldNextOffset() {
		uint8_t* KismetSystemLibraryChild = reinterpret_cast<uint8_t*>(SDK::UObject::FindObjectFast<SDK::UStruct>(std::string(skCrypt("KismetSystemLibrary")))->Children());
		uint8_t* KismetStringLibraryChild = reinterpret_cast<uint8_t*>(SDK::UObject::FindObjectFast<SDK::UStruct>(std::string(skCrypt("KismetStringLibrary")))->Children());

		return Memory::GetValidPointerOffset(KismetSystemLibraryChild, KismetStringLibraryChild, 0x20 + 0x08, 0x48);
	}
};