#ifndef APFEL_H
#define APFEL_H

#include <string>
#include "Game/SDK/SDK.h"
#include "Game/SDK/Classes/Basic.h"
#include "Game/SDK/Classes/CoreUObject_classes.h"

namespace Apfel {

    enum class EClassCastFlags {
        Function = 1
    };

    class UField {
    public:
        UField* Next = nullptr;
        virtual bool HasTypeFlag(EClassCastFlags Flag) const;
        virtual std::string GetName() const;
    };

    class UFunction : public UField {
    public:
        unsigned long FunctionFlags;
        bool HasTypeFlag(EClassCastFlags Flag) const override;
    };

    class UStruct : public UField {
    public:
        UStruct* Super = nullptr;
        UField* Children = nullptr;
        virtual std::string GetName() const override;
    };

    class UClass : public UStruct {
    public:
        UFunction* GetFunction(const std::string& ClassName, const std::string& FuncName) const;
    };

    namespace Params {
        // Declare the struct here
        struct PlayerController_ServerChangeName {
            SDK::FString S;
        };
    }

    class APlayerController : public SDK::UObject {
    public:
        UClass* Class;

        APlayerController();

        void ServerChangeName(const SDK::FString& S);
    };
}

#endif