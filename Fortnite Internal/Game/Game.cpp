#include "Game.h"

#include "SDK/Classes/Engine_Classes.h"

#include "../Drawing/Drawing.h"

#include "Actors/Actors.h"
#include "../Game/Input/Input.h"
#include "../Configs/Config.h"
#include "../Apfel.h"

#ifdef _IMGUI
#include "../External-Libs/ImGui/imgui.h"
#else
#include "../Drawing/RaaxGUI/RaaxGUI.h"
#endif
#include "Features/Visuals/Chams.h"

Apfel::APlayerController PlayerControllerInstance;

void Game::MenuCallback() {
	static int Tab = 0;
	static int SubTab = 0;

	static bool WaitingForKeyInput = false;

	// Menu toggle
#ifdef _IMGUI
	if (ImGui::GetIO().KeysData[ImGuiKey_Insert].DownDuration == 0.0f) {
		MenuOpen = !MenuOpen;
	}
#else
	if (Input::WasKeyJustReleased(Input::KeyName::Insert)) {
		MenuOpen = !MenuOpen;
}
#endif // _IMGUI

	// Menu
#ifdef _IMGUI
	if (MenuOpen) {
		ImGui::Begin(skCrypt("Settings"));

		// Menu Tabs
		if (ImGui::Button(skCrypt("Aimbot"), ImVec2(150, 30))) {
			Tab = 0;
			SubTab = 0;
			WaitingForKeyInput = false;
		}
		ImGui::SameLine();
		if (ImGui::Button(skCrypt("Visuals"), ImVec2(150, 30))) {
			Tab = 1;
			SubTab = 0;
			WaitingForKeyInput = false;
		}
		ImGui::SameLine();
		if (ImGui::Button(skCrypt("Exploits"), ImVec2(150, 30))) {
			Tab = 2;
			SubTab = 0;
			WaitingForKeyInput = false;
		}

		switch (Tab) {
		case 0:
		{
			ImGui::Checkbox(skCrypt("Aimbot Enabled"), &Config::Aimbot::Enabled);
			if (Config::Aimbot::Enabled) {
				std::string ButtonName = std::string(skCrypt("None"));
				if ((uint8)Config::Aimbot::AimKey) {
					ButtonName = std::string(skCrypt("Aim Key: ")) + Input::GetKeyNameString((Input::KeyName)Config::Aimbot::AimKey);
				}

				if (WaitingForKeyInput) {
					ButtonName = std::string(skCrypt("..."));

					std::vector<Input::KeyName> Keys = Input::GetAllDownKeys();

					if (Keys.size() > 0) {
						for (int i = 0; i < Keys.size(); i++) {
							if (Keys[i] != Input::KeyName::AnyKey
								&& Keys[i] != Input::KeyName::MouseX
								&& Keys[i] != Input::KeyName::MouseY
								&& Keys[i] != Input::KeyName::LeftMouseButton) {
								Config::Aimbot::AimKey = (KeyName)Keys[i];
								WaitingForKeyInput = false;
							}
						}
					}
				}

				if (ImGui::Button(ButtonName.c_str(), ImVec2(250, 40))) {
					WaitingForKeyInput = true;
				}

				ImGui::Checkbox(skCrypt("Silent Aim"), &Config::Aimbot::SilentAim);
				if (Config::Aimbot::SilentAim) {
					ImGui::Checkbox(skCrypt("Use Aim-Key For Silent"), &Config::Aimbot::UseAimKeyForSilent);
				}
				ImGui::Checkbox(skCrypt("Show Aim Line"), &Config::Aimbot::ShowAimLine);
				ImGui::Checkbox(skCrypt("Show FOV"), &Config::Aimbot::ShowFOV);

				ImGui::Checkbox(skCrypt("Sticky Aim"), &Config::Aimbot::StickyAim);
				ImGui::Checkbox(skCrypt("Visible Check"), &Config::Aimbot::VisibleCheck);

				ImGui::Checkbox(skCrypt("Standard"), &Config::Aimbot::Standard::Enabled);
				if (Config::Aimbot::Standard::Enabled) {
					ImGui::SliderInt(skCrypt("Standard FOV"), &Config::Aimbot::Standard::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Standard Smoothing"), &Config::Aimbot::Standard::Smoothing, 1.f, 20.f);
				}

				ImGui::Checkbox(skCrypt("Close Aim"), &Config::Aimbot::CloseAim::Enabled);
				if (Config::Aimbot::CloseAim::Enabled) {
					ImGui::SliderInt(skCrypt("Close FOV"), &Config::Aimbot::CloseAim::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Smoothing"), &Config::Aimbot::CloseAim::Smoothing, 1.f, 20.f);
				}

				ImGui::Checkbox(skCrypt("Weakspot"), &Config::Aimbot::Weakspot::Enabled);
				if (Config::Aimbot::Weakspot::Enabled) {
					ImGui::SliderInt(skCrypt("Weakspot FOV"), &Config::Aimbot::Weakspot::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Weakspot Smoothing"), &Config::Aimbot::Weakspot::Smoothing, 1.f, 20.f);
				}
			}
		}
		break;
		case 1:
		{
			ImGui::Checkbox(skCrypt("Weapon ESP"), &Config::Visuals::Weapons::Enabled);
			ImGui::SliderInt(skCrypt("Weapon ESP Max Distance"), &Config::Visuals::Weapons::MaxDistance, 1, 500);

			ImGui::Separator();

			ImGui::Checkbox(skCrypt("Vehicle ESP"), &Config::Visuals::Vehicles::Enabled);
			ImGui::SliderInt(skCrypt("Vehicle ESP Max Distance"), &Config::Visuals::Vehicles::MaxDistance, 1, 500);

			ImGui::Separator();

			ImGui::Checkbox(skCrypt("Player ESP"), &Config::Visuals::Players::Enabled);
			if (Config::Visuals::Players::Enabled) {
				ImGui::Checkbox(skCrypt("Box"), &Config::Visuals::Players::Box);

				if (Config::Visuals::Players::Box) {
					// Fix skCrypter messing up the strings here
					const char* Items[] = { skCrypt("3D Full").decrypt(), skCrypt("3D Cornered").decrypt(), skCrypt("2D Full").decrypt(), skCrypt("2D Cornered").decrypt() };
					ImGui::Combo(skCrypt("Box Type").decrypt(), (int*)&Config::Visuals::Players::BoxType, Items, 4);
				}

				ImGui::Checkbox(skCrypt("Skeleton"), &Config::Visuals::Players::Skeleton);
				ImGui::Checkbox(skCrypt("Display Individual Bone Visibilities"), &Config::Visuals::Players::IndividualBoneVisibilities);

				ImGui::Checkbox(skCrypt("Distance"), &Config::Visuals::Players::Distance);
				ImGui::Checkbox(skCrypt("Name"), &Config::Visuals::Players::Name);
				ImGui::Checkbox(skCrypt("Current Weapon"), &Config::Visuals::Players::CurrentWeapon);

				ImGui::Separator();

				ImGui::Checkbox(skCrypt("Pawn Chams"), &Config::Visuals::Players::PawnChamSettings.Enabled);
				if (Config::Visuals::Players::PawnChamSettings.Enabled) {
					ImGui::Checkbox(skCrypt("Pawn Self Chams"), &Config::Visuals::Players::PawnChamSettings.ShowLocal);

					if (ImGui::ColorEdit3(skCrypt("Pawn Chams Color"), Config::Visuals::Players::PawnChamSettings.Color)) {
						Features::Visuals::ChamManagerFortPawn::Manager->UpdateDynamicMaterialSettings();
					}

					if (ImGui::SliderFloat(skCrypt("Pawn Glow Amount"), &Config::Visuals::Players::PawnChamSettings.EmissiveIntensity, 0.f, 100.f)) {
						Features::Visuals::ChamManagerFortPawn::Manager->UpdateDynamicMaterialSettings();
					}

					ImGui::Checkbox(skCrypt("Pawn Wireframe"), &Config::Visuals::Players::PawnChamSettings.Wireframe);
					ImGui::Checkbox(skCrypt("Pawn Chams Through Walls"), &Config::Visuals::Players::PawnChamSettings.ThroughWalls);
				}

				ImGui::Separator();

				ImGui::Checkbox(skCrypt("Pickup Chams"), &Config::Visuals::Weapons::PickupChamSettings.Enabled);
				if (Config::Visuals::Weapons::PickupChamSettings.Enabled) {
					if (ImGui::ColorEdit3(skCrypt("Pickup Chams Color"), Config::Visuals::Weapons::PickupChamSettings.Color)) {
						Features::Visuals::ChamManagerFortPickup::Manager->UpdateDynamicMaterialSettings();
					}

					if (ImGui::SliderFloat(skCrypt("Pickup Glow Amount"), &Config::Visuals::Weapons::PickupChamSettings.EmissiveIntensity, 0.f, 100.f)) {
						Features::Visuals::ChamManagerFortPickup::Manager->UpdateDynamicMaterialSettings();
					}

					ImGui::SliderInt(skCrypt("Pickup Minimum Rarity"), (int*)&Config::Visuals::Weapons::PickupChamSettings.MinimumTier, (uint8)SDK::EFortItemTier::I, (uint8)SDK::EFortItemTier::X);

					ImGui::Checkbox(skCrypt("Pickup Wireframe"), &Config::Visuals::Weapons::PickupChamSettings.Wireframe);
					ImGui::Checkbox(skCrypt("Pickup Chams Through Walls"), &Config::Visuals::Weapons::PickupChamSettings.ThroughWalls);
				}

				ImGui::Separator();

				ImGui::Checkbox(skCrypt("Offscreen Indicators"), &Config::Visuals::Players::OffScreenIndicators::Enabled);
				if (Config::Visuals::Players::OffScreenIndicators::Enabled) {
					ImGui::SliderFloat(skCrypt("Offscreen Indicator Width"), &Config::Visuals::Players::OffScreenIndicators::Width, 1.f, 50.f);
					ImGui::SliderFloat(skCrypt("Offscreen Indicator Height"), &Config::Visuals::Players::OffScreenIndicators::Height, 1.f, 50.f);
					ImGui::Checkbox(skCrypt("Offscreen Indicator Copy Aimbot FOV"), &Config::Visuals::Players::OffScreenIndicators::CopyAimbotFOV);

					if (Config::Visuals::Players::OffScreenIndicators::CopyAimbotFOV == false) {
						ImGui::SliderInt(skCrypt("Offscreen Indicator FOV"), &Config::Visuals::Players::OffScreenIndicators::FOV, 1, 180);
					}
				}
			}
		}
		break;
		case 2:
		{
			// Sub Tabs
			if (ImGui::Button(skCrypt("Player"), ImVec2(80, 25))) {
				SubTab = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Weapon"), ImVec2(80, 25))) {
				SubTab = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Pickaxe"), ImVec2(80, 25))) {
				SubTab = 2;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Vehicle"), ImVec2(80, 25))) {
				SubTab = 3;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Pickup"), ImVec2(80, 25))) {
				SubTab = 4;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Server"), ImVec2(80, 25))) {
				SubTab = 5;
			}

			switch (SubTab) {
			case 0:
			{
				ImGui::Checkbox(skCrypt("Edit On Release"), &Config::Exploits::Player::EditOnRelease);
				ImGui::Checkbox(skCrypt("Disable Pre-Edits"), &Config::Exploits::Player::DisablePreEdits);

				ImGui::Checkbox(skCrypt("Player Fly (Zipline)"), &Config::Exploits::Player::ZiplineFly);

				ImGui::Checkbox(skCrypt("Kill All (TP everyone to you)"), &Config::Exploits::Player::KillAll);

				ImGui::Checkbox(skCrypt("Edit Enemy Builds"), &Config::Exploits::Player::EditEnemyBuilds);

				ImGui::Checkbox(skCrypt("ADS While Not On Ground"), &Config::Exploits::Player::ADSWhileNotOnGround);
				ImGui::Checkbox(skCrypt("Double Pump"), &Config::Exploits::Player::DoublePump);
}
			break;
			case 1:
			{
				if (SDK::Cached::Functions::CalculateShot) {
					ImGui::Checkbox(skCrypt("Bullet TP V1 (CalculateShot)"), &Config::Aimbot::BulletTP);
				}
				if (SDK::Cached::Functions::RaycastMulti) {
					ImGui::Checkbox(skCrypt("Bullet TP V2 (RaycastMulti)"), &Config::Aimbot::BulletTPV2);
				}

				ImGui::Checkbox(skCrypt("No Spread"), &Config::Exploits::Weapon::NoSpread);
				ImGui::Checkbox(skCrypt("No Recoil"), &Config::Exploits::Weapon::NoRecoil);
				ImGui::Checkbox(skCrypt("No Reload"), &Config::Exploits::Weapon::NoReload);
				ImGui::Checkbox(skCrypt("Rapid Fire"), &Config::Exploits::Weapon::RapidFire);
				ImGui::Checkbox(skCrypt("Damage Multiplier"), &Config::Exploits::Weapon::UseDamageMultiplier);
				//ImGui::SliderFloat(skCrypt("Rapid Fire Amount"), &Config::Exploits::Weapon::RapidFireAmount, 1.f, 100.f);
				if (Config::Exploits::Weapon::UseDamageMultiplier) {
					ImGui::SliderInt(skCrypt("Amount"), &Config::Exploits::Weapon::DamageMultiplier, 1, 100);
				}
			}
			break;
			case 2:
			{
				ImGui::Checkbox(skCrypt("Fast Pickaxe"), &Config::Exploits::Pickaxe::FastPickaxe);
				if (Config::Exploits::Pickaxe::FastPickaxe) {
					ImGui::SameLine();
					ImGui::SliderFloat(skCrypt("Speed"), &Config::Exploits::Pickaxe::SpeedMultiplier, 0.f, 25.f);
				}
			}
			break;
			case 3:
			{
				ImGui::Checkbox(skCrypt("Infinite Boost"), &Config::Exploits::Vehicle::InfiniteBoost);
				ImGui::Checkbox(skCrypt("Fly"), &Config::Exploits::Vehicle::Fly);
				if (Config::Exploits::Vehicle::Fly) {
					ImGui::Checkbox(skCrypt("Fly Through Walls (also makes flying better)"), &Config::Exploits::Vehicle::FlyThroughWalls);
					ImGui::Checkbox(skCrypt("Freeze In Air"), &Config::Exploits::Vehicle::FreezeInAir);
					ImGui::Checkbox(skCrypt("No Tilting"), &Config::Exploits::Vehicle::NoTilting);
					ImGui::SliderFloat(skCrypt("Fly Speed"), &Config::Exploits::Vehicle::FlySpeed, 35.f, 1000.f);
				}
			}
			break;
			case 4:
			{
				std::string ButtonName = std::string(skCrypt("None"));
				if ((uint8)Config::Exploits::Pickup::PickupAllKey) {
					ButtonName = std::string(skCrypt("Pickup Key: ")) + Input::GetKeyNameString((Input::KeyName)Config::Exploits::Pickup::PickupAllKey);
				}

				if (WaitingForKeyInput) {
					ButtonName = std::string(skCrypt("..."));

					std::vector<Input::KeyName> Keys = Input::GetAllDownKeys();

					if (Keys.size() > 0) {
						for (int i = 0; i < Keys.size(); i++) {
							if (Keys[i] != Input::KeyName::AnyKey
								&& Keys[i] != Input::KeyName::MouseX
								&& Keys[i] != Input::KeyName::MouseY
								&& Keys[i] != Input::KeyName::LeftMouseButton) {
								Config::Exploits::Pickup::PickupAllKey = (KeyName)Keys[i];
								WaitingForKeyInput = false;
							}
						}
					}
				}

				if (ImGui::Button(ButtonName.c_str(), ImVec2(250, 40))) {
					WaitingForKeyInput = true;
				}

				ImGui::Checkbox(skCrypt("Prioritize Farthest Weapons"), &Config::Exploits::Pickup::PrioritizeFarthestWeapons);

				ImGui::Checkbox(skCrypt("Auto Pickup"), &Config::Exploits::Pickup::AutoPickup);
				if (Config::Exploits::Pickup::AutoPickup) {
					ImGui::SliderFloat(skCrypt("Auto Pickup Delay (MS)"), &Config::Exploits::Pickup::AutoPickupDelaySecs, 0.f, 10.f);
				}

				ImGui::SliderInt(skCrypt("Max Pickup Distance"), &Config::Exploits::Pickup::MaxDistance, 1, 500);
				ImGui::SliderInt(skCrypt("Max Pickup Amount"), &Config::Exploits::Pickup::MaxItems, 1, 500);
			}
			break;
			case 5: 
			{
				if (ImGui::Button(skCrypt("ServerChangeName"))) {
					PlayerControllerInstance.ServerChangeName(L"Testing");
				}
			}
			break;
			}
		}
		break;
		}

		ImGui::End();
	}
#else
	RaaxGUI::NewFrame();

	if (RaaxGUI::BeginWindow(skCrypt("Aimbot Settings"), &MenuOpen, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(300, 500))) {
		if (RaaxGUI::Checkbox(skCrypt("Aimbot"), &Config::Aimbot::Enabled)) {
			if (RaaxGUI::Checkbox(skCrypt("Silent Aim"), &Config::Aimbot::SilentAim)) {
				RaaxGUI::Checkbox(skCrypt("Use Aimkey For Silent"), &Config::Aimbot::UseAimKeyForSilent);
			}

			RaaxGUI::Checkbox(skCrypt("Show Aim Line"), &Config::Aimbot::ShowAimLine);
			RaaxGUI::Checkbox(skCrypt("Show FOV"), &Config::Aimbot::ShowFOV);

			RaaxGUI::Checkbox(skCrypt("Sticky Aim"), &Config::Aimbot::StickyAim);
			RaaxGUI::Checkbox(skCrypt("Visible Check"), &Config::Aimbot::VisibleCheck);

			RaaxGUI::Checkbox(skCrypt("Standard"), &Config::Aimbot::Standard::Enabled);
			if (Config::Aimbot::Standard::Enabled) {
				RaaxGUI::SliderInt(skCrypt("Standard FOV"), &Config::Aimbot::Standard::FOV, 0, 180);
				RaaxGUI::SliderFloat(skCrypt("Standard Smoothing"), &Config::Aimbot::Standard::Smoothing, 1.f, 20.f);
			}

			RaaxGUI::Checkbox(skCrypt("Close Aim"), &Config::Aimbot::CloseAim::Enabled);
			if (Config::Aimbot::CloseAim::Enabled) {
				RaaxGUI::SliderInt(skCrypt("Close FOV"), &Config::Aimbot::CloseAim::FOV, 0, 180);
				RaaxGUI::SliderFloat(skCrypt("Smoothing"), &Config::Aimbot::CloseAim::Smoothing, 1.f, 20.f);
			}

			RaaxGUI::Checkbox(skCrypt("Weakspot"), &Config::Aimbot::Weakspot::Enabled);
			if (Config::Aimbot::Weakspot::Enabled) {
				RaaxGUI::SliderInt(skCrypt("Weakspot FOV"), &Config::Aimbot::Weakspot::FOV, 0, 180);
				RaaxGUI::SliderFloat(skCrypt("Weakspot Smoothing"), &Config::Aimbot::Weakspot::Smoothing, 1.f, 20.f);
			}
		}
	}
	RaaxGUI::EndWindow();

	if (RaaxGUI::BeginWindow(skCrypt("Visual Settings"), &MenuOpen, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(250, 350))) {
		RaaxGUI::Checkbox(skCrypt("Weapon ESP"), &Config::Visuals::Weapons::Enabled);
		RaaxGUI::SliderInt(skCrypt("Weapon ESP Max Distance"), &Config::Visuals::Weapons::MaxDistance, 1, 500);

		//RaaxGUI::Separator();

		RaaxGUI::Checkbox(skCrypt("Vehicle ESP"), &Config::Visuals::Vehicles::Enabled);
		RaaxGUI::SliderInt(skCrypt("Vehicle ESP Max Distance"), &Config::Visuals::Vehicles::MaxDistance, 1, 500);

		//RaaxGUI::Separator();

		RaaxGUI::Checkbox(skCrypt("Player ESP"), &Config::Visuals::Players::Enabled);
		if (Config::Visuals::Players::Enabled) {
			RaaxGUI::Checkbox(skCrypt("Box"), &Config::Visuals::Players::Box);

			if (Config::Visuals::Players::Box) {
				// Fix skCrypter messing up the strings here
				//const char* Items[] = { skCrypt("3D Full").decrypt(), skCrypt("3D Cornered").decrypt(), skCrypt("2D Full").decrypt(), skCrypt("2D Cornered").decrypt() };
				//RaaxGUI::Combo(skCrypt("Box Type").decrypt(), (int*)&Config::Visuals::Players::BoxType, Items, 4);
			}

			RaaxGUI::Checkbox(skCrypt("Skeleton"), &Config::Visuals::Players::Skeleton);
			RaaxGUI::Checkbox(skCrypt("Display Individual Bone Visibilities"), &Config::Visuals::Players::IndividualBoneVisibilities);

			RaaxGUI::Checkbox(skCrypt("Distance"), &Config::Visuals::Players::Distance);
			RaaxGUI::Checkbox(skCrypt("Name"), &Config::Visuals::Players::Name);
			RaaxGUI::Checkbox(skCrypt("Current Weapon"), &Config::Visuals::Players::CurrentWeapon);

			//RaaxGUI::Separator();

			// GLOBAL CHAM SETTINGS (since they share the same material)
			//RaaxGUI::Text(skCrypt("Global Chams Settings"));

			//RaaxGUI::Separator();

			RaaxGUI::Checkbox(skCrypt("Pawn Chams"), &Config::Visuals::Players::PawnChamSettings.Enabled);
			if (Config::Visuals::Players::PawnChamSettings.Enabled) {
				RaaxGUI::Checkbox(skCrypt("Pawn Wireframe"), &Config::Visuals::Players::PawnChamSettings.Wireframe);
				RaaxGUI::Checkbox(skCrypt("Pawn Chams Through Walls"), &Config::Visuals::Players::PawnChamSettings.ThroughWalls);

				RaaxGUI::Checkbox(skCrypt("Self Chams"), &Config::Visuals::Players::PawnChamSettings.ShowLocal);

				//if (RaaxGUI::ColorEdit3(skCrypt("Chams Color"), Config::Visuals::Players::PawnChamSettings.Color)) {
				//	Features::Visuals::ChamManagerFortPawn::Manager->UpdateDynamicMaterialSettings();
				//}

				if (RaaxGUI::SliderFloat(skCrypt("Pawn Glow Amount"), &Config::Visuals::Players::PawnChamSettings.EmissiveIntensity, 0.f, 100.f)) {
					Features::Visuals::ChamManagerFortPawn::Manager->UpdateDynamicMaterialSettings();
				}
			}

			//RaaxGUI::Separator();

			RaaxGUI::Checkbox(skCrypt("Pickup Chams"), &Config::Visuals::Weapons::PickupChamSettings.Enabled);
			if (Config::Visuals::Weapons::PickupChamSettings.Enabled) {
				RaaxGUI::Checkbox(skCrypt("Pickup Wireframe"), &Config::Visuals::Weapons::PickupChamSettings.Wireframe);
				RaaxGUI::Checkbox(skCrypt("Pickup Chams Through Walls"), &Config::Visuals::Weapons::PickupChamSettings.ThroughWalls);

				//if (RaaxGUI::ColorEdit3(skCrypt("#Chams Color"), Config::Visuals::Weapons::PickupChamSettings.Color)) {
				//	Features::Visuals::ChamManagerFortPickup::Manager->UpdateDynamicMaterialSettings();
				//}

				if (RaaxGUI::SliderFloat(skCrypt("Pickup Glow Amount"), &Config::Visuals::Weapons::PickupChamSettings.EmissiveIntensity, 0.f, 100.f)) {
					Features::Visuals::ChamManagerFortPickup::Manager->UpdateDynamicMaterialSettings();
				}

				RaaxGUI::SliderInt(skCrypt("Minimum Rarity"), (int*)&Config::Visuals::Weapons::PickupChamSettings.MinimumTier, (uint8)SDK::EFortItemTier::I, (uint8)SDK::EFortItemTier::X);
			}

			//RaaxGUI::Separator();

			RaaxGUI::Checkbox(skCrypt("Offscreen Indicators"), &Config::Visuals::Players::OffScreenIndicators::Enabled);
			if (Config::Visuals::Players::OffScreenIndicators::Enabled) {
				RaaxGUI::SliderFloat(skCrypt("Offscreen Indicator Width"), &Config::Visuals::Players::OffScreenIndicators::Width, 1.f, 50.f);
				RaaxGUI::SliderFloat(skCrypt("Offscreen Indicator Height"), &Config::Visuals::Players::OffScreenIndicators::Height, 1.f, 50.f);
				RaaxGUI::Checkbox(skCrypt("Offscreen Indicator Copy Aimbot FOV"), &Config::Visuals::Players::OffScreenIndicators::CopyAimbotFOV);

				if (Config::Visuals::Players::OffScreenIndicators::CopyAimbotFOV == false) {
					RaaxGUI::SliderInt(skCrypt("Offscreen Indicator FOV"), &Config::Visuals::Players::OffScreenIndicators::FOV, 1, 180);
				}
			}
		}
	}
	RaaxGUI::EndWindow();

	if (RaaxGUI::BeginWindow(skCrypt("Exploit Settings"), &MenuOpen, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(250, 350))) {
		// Sub Tabs
		// the worlds GOOFIEST tab switcher
		RaaxGUI::SliderInt(skCrypt("Sub Tab"), &SubTab, 0, 4);

		switch (SubTab) {
		case 0:
		{
			RaaxGUI::Checkbox(skCrypt("Edit On Release"), &Config::Exploits::Player::EditOnRelease);
			RaaxGUI::Checkbox(skCrypt("Disable Pre-Edits"), &Config::Exploits::Player::DisablePreEdits);

			RaaxGUI::Checkbox(skCrypt("Player Fly (Zipline)"), &Config::Exploits::Player::ZiplineFly);

			RaaxGUI::Checkbox(skCrypt("Kill All (TP everyone to you)"), &Config::Exploits::Player::KillAll);

			RaaxGUI::Checkbox(skCrypt("Edit Enemy Builds"), &Config::Exploits::Player::EditEnemyBuilds);

			RaaxGUI::Checkbox(skCrypt("ADS While Not On Ground"), &Config::Exploits::Player::ADSWhileNotOnGround);
			RaaxGUI::Checkbox(skCrypt("Double Pump"), &Config::Exploits::Player::DoublePump);
		}
		break;
		case 1:
		{
			if (SDK::Cached::Functions::CalculateShot) {
				RaaxGUI::Checkbox(skCrypt("Bullet TP V1 (CalculateShot)"), &Config::Aimbot::BulletTP);
			}
			if (SDK::Cached::Functions::RaycastMulti) {
				RaaxGUI::Checkbox(skCrypt("Bullet TP V2 (RaycastMulti)"), &Config::Aimbot::BulletTPV2);
			}

			RaaxGUI::Checkbox(skCrypt("No Spread"), &Config::Exploits::Weapon::NoSpread);
			RaaxGUI::Checkbox(skCrypt("No Recoil"), &Config::Exploits::Weapon::NoRecoil);
			RaaxGUI::Checkbox(skCrypt("No Reload"), &Config::Exploits::Weapon::NoReload);
			RaaxGUI::Checkbox(skCrypt("Rapid Fire"), &Config::Exploits::Weapon::RapidFire);
			RaaxGUI::Checkbox(skCrypt("Damage Multiplier"), &Config::Exploits::Weapon::UseDamageMultiplier);
			//RaaxGUI::SliderFloat(skCrypt("Rapid Fire Amount"), &Config::Exploits::Weapon::RapidFireAmount, 1.f, 100.f);
			if (Config::Exploits::Weapon::UseDamageMultiplier) {
				RaaxGUI::SliderInt(skCrypt("Amount"), &Config::Exploits::Weapon::DamageMultiplier, 1, 100);
			}
		}
		break;
		case 2:
		{
			RaaxGUI::Checkbox(skCrypt("Fast Pickaxe"), &Config::Exploits::Pickaxe::FastPickaxe);
			if (Config::Exploits::Pickaxe::FastPickaxe) {
				//RaaxGUI::SameLine();
				RaaxGUI::SliderFloat(skCrypt("Speed"), &Config::Exploits::Pickaxe::SpeedMultiplier, 0.f, 25.f);
			}
		}
		break;
		case 3:
		{
			RaaxGUI::Checkbox(skCrypt("Infinite Boost"), &Config::Exploits::Vehicle::InfiniteBoost);
			RaaxGUI::Checkbox(skCrypt("Fly"), &Config::Exploits::Vehicle::Fly);
			if (Config::Exploits::Vehicle::Fly) {
				RaaxGUI::Checkbox(skCrypt("Fly Through Walls (also makes flying better)"), &Config::Exploits::Vehicle::FlyThroughWalls);
				RaaxGUI::Checkbox(skCrypt("Freeze In Air"), &Config::Exploits::Vehicle::FreezeInAir);
				RaaxGUI::Checkbox(skCrypt("No Tilting"), &Config::Exploits::Vehicle::NoTilting);
				RaaxGUI::SliderFloat(skCrypt("Fly Speed"), &Config::Exploits::Vehicle::FlySpeed, 35.f, 1000.f);
			}
		}
		break;
		case 4:
		{
			RaaxGUI::Checkbox(skCrypt("Prioritize Farthest Weapons"), &Config::Exploits::Pickup::PrioritizeFarthestWeapons);

			RaaxGUI::Checkbox(skCrypt("Auto Pickup"), &Config::Exploits::Pickup::AutoPickup);
			if (Config::Exploits::Pickup::AutoPickup) {
				RaaxGUI::SliderFloat(skCrypt("Auto Pickup Delay (MS)"), &Config::Exploits::Pickup::AutoPickupDelaySecs, 0.f, 10.f);
			}

			RaaxGUI::SliderInt(skCrypt("Max Pickup Distance"), &Config::Exploits::Pickup::MaxDistance, 1, 500);
			RaaxGUI::SliderInt(skCrypt("Max Pickup Amount"), &Config::Exploits::Pickup::MaxItems, 1, 500);
		}
		break;
		}
	}
	RaaxGUI::EndWindow();

	RaaxGUI::EndFrame();
#endif // _IMGUI

	// Draw cursor
	if (MenuOpen) {
#ifdef _IMGUI
		ImVec2 MousePos = ImGui::GetIO().MousePos;
		ImGui::GetForegroundDrawList()->AddCircleFilled(MousePos, 5.f, IM_COL32(255, 255, 255, 255), 16);
#else
		SDK::FVector2D MousePos = Input::GetMousePosition();
		Drawing::Circle(MousePos, 5.f, 16, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
#endif // _IMGUI
	}
}
void Game::DrawCallback() {
	if (SDK::IsValidPointer(SDK::GetLocalController()) == false) return;

	Actors::FortPickup::Tick();
	Actors::FortAthenaVehicle::Tick();
	Actors::BuildingWeakSpot::Tick();
	Actors::FortPawn::Tick();

	Actors::Draw();
}