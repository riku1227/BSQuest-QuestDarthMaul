#include <string>
#include <sstream>

#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../extern/beatsaber-hook/shared/utils/typedefs.h"
                                                                                                                                   
MAKE_HOOK_OFFSETLESS(PlayerController_Update, void, Il2CppObject* self) {
    PlayerController_Update(self);

    Il2CppObject* leftSaber = il2cpp_utils::GetFieldValue(self, "_leftSaber");
    Il2CppObject* rightSaber = il2cpp_utils::GetFieldValue(self, "_rightSaber");

    if(leftSaber != nullptr && rightSaber != nullptr) {
        log(INFO, "Saber's online");
        Il2CppObject* leftSaberTransform = nullptr;
        Il2CppObject* rightSaberTransform = nullptr;

        Il2CppClass* componentsClass = il2cpp_utils::GetClassFromName("", "Saber");
        il2cpp_utils::RunMethod(&leftSaberTransform, leftSaber, il2cpp_functions::class_get_method_from_name(componentsClass, "get_transform", 0));
        il2cpp_utils::RunMethod(&rightSaberTransform, rightSaber, il2cpp_functions::class_get_method_from_name(componentsClass, "get_transform", 0));

        if(leftSaberTransform != nullptr && rightSaberTransform != nullptr) {

            Il2CppClass* transformClass = il2cpp_utils::GetClassFromName("UnityEngine", "Transform");
            const MethodInfo* getMethod = il2cpp_functions::class_get_method_from_name(transformClass, "get_localPosition", 0);
            const MethodInfo* setMethod = il2cpp_functions::class_get_method_from_name(transformClass, "set_localPosition", 1);
            const MethodInfo* setRotate = il2cpp_functions::class_get_method_from_name(transformClass, "Rotate", 1);
            const MethodInfo* setTranslate = il2cpp_functions::class_get_method_from_name(transformClass, "Translate", 1);
            const MethodInfo* getLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "get_localRotation", 0);
            const MethodInfo* setLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "set_localRotation", 1);

                Vector3 rightSaberLocalPosition;
                il2cpp_utils::RunMethod(&rightSaberLocalPosition, rightSaberTransform, getMethod);
                Quaternion rightSaberLocalRotation;
            il2cpp_utils::RunMethod(&rightSaberLocalRotation, rightSaberTransform, getLocalRotation);

            il2cpp_utils::RunMethod(leftSaberTransform, setMethod, rightSaberLocalPosition);
            il2cpp_utils::RunMethod(leftSaberTransform, setLocalRotation, rightSaberLocalRotation);
            il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, 180, 0});
            il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{0, 0, 0.335});
        }
    }
}

// Haptic Remapping (gives errors, Going too need a UnityEngine.XR Replica)
 MAKE_HOOK_OFFSETLESS(HapticFeedbackController_HitNote, void, Il2CppObject* self, int node) 
{
     HapticFeedbackController_HitNote(self , node);
   node = 5;    
    il2cpp_utils::RunMethod(self, "Rumble" ,5, 0.13f, 1.0f, 0.0f);
}



__attribute__((constructor)) void lib_main()
{
    log(INFO, "Hello from the first time this mod is loaded!");
}

extern "C" void load() {
    log(INFO, "Hello from il2cpp_init!");
    log(INFO, "Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(PlayerController_Update, il2cpp_utils::FindMethod("", "PlayerController", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(HapticFeedbackController_HitNote, il2cpp_utils::FindMethod("", "HapticFeedbackController", "HitNote", 1));
    log(INFO, "Installed all hooks!");
}