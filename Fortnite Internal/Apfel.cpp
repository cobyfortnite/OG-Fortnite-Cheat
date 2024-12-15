#include <string>
#include <unordered_map>
#include <iostream>
#include "Apfel.h"  // Include the header file

namespace Apfel {

    // Definitions for UField
    bool UField::HasTypeFlag(EClassCastFlags Flag) const {
        return false;
    }

    std::string UField::GetName() const {
        return "";
    }

    // Definitions for UFunction
    bool UFunction::HasTypeFlag(EClassCastFlags Flag) const {
        return Flag == EClassCastFlags::Function;
    }

    // Definitions for UStruct
    std::string UStruct::GetName() const {
        return "";
    }

    // Definition for UClass::GetFunction
    UFunction* UClass::GetFunction(const std::string& ClassName, const std::string& FuncName) const {
        for (const UStruct* Clss = this; Clss; Clss = Clss->Super) {
            if (Clss->GetName() != ClassName)
                continue;

            for (UField* Field = Clss->Children; Field; Field = Field->Next) {
                if (Field->HasTypeFlag(EClassCastFlags::Function) && Field->GetName() == FuncName) {
                    return static_cast<UFunction*>(Field);
                }
            }
        }
        return nullptr;
    }

    // APlayerController constructor
    APlayerController::APlayerController() : Class(new UClass()) {}

    // ServerChangeName function implementation
    void APlayerController::ServerChangeName(const SDK::FString& S) {
        static UFunction* Func = nullptr;

        if (Func == nullptr) {
            Func = Class->GetFunction("PlayerController", "ServerChangeName");
        }

        if (Func == nullptr) {
            std::cerr << "Function not found: ServerChangeName" << std::endl;
            return;
        }

        // Fully define the struct so that it can be used here
        Params::PlayerController_ServerChangeName Parms{};
        Parms.S = std::move(S);

        auto Flgs = Func->FunctionFlags;
        Func->FunctionFlags |= 0x400;

        // Call ProcessEvent directly on the current UObject instance
        this->ProcessEvent(Func, &Parms);

        Func->FunctionFlags = Flgs;
    }
}