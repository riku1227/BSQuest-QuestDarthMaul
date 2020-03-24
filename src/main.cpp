#include "../include/main.hpp"
#include "../extern/beatsaber-hook/shared/customui/customui.hpp"

int stop = 0;

CustomUI::TextObject Maul;

float grayScaleColor = 1.0f;

Vector2 posOffset = {-41.3f,-16.9f};

float fontSize = 3.6f;

bool leftHand;

bool noArrows;

MAKE_HOOK_OFFSETLESS(Refresh, void, Il2CppObject* self) {
    if(stop == 1) {
        Il2CppObject* nameText = il2cpp_utils::GetFieldValue(self, "_nameText");
	    Il2CppObject *nameTextTransform;
	    Il2CppObject *nameTextParent;

        il2cpp_utils::RunMethod(&nameTextTransform, nameText, "get_transform");
	    il2cpp_utils::RunMethod(&nameTextParent, nameTextTransform, "GetParent");

        Maul.text = "/Darth Maul Mode";
	    Maul.fontSize = fontSize;
	    Maul.parentTransform = nameTextParent;
        Maul.sizeDelta = posOffset;
        Maul.color = {grayScaleColor,grayScaleColor,grayScaleColor,grayScaleColor};
	    Maul.create();
    }
    stop++;

    Refresh(self);
}

MAKE_HOOK_OFFSETLESS(SongStart, void, Il2CppObject* self, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects) {

    il2cpp_utils::RunMethod(&noArrows, gameplayModifiers, "get_noArrows");

    il2cpp_utils::RunMethod(&leftHand, playerSpecificSettings, "get_leftHanded");
    
    SongStart(self, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
}

MAKE_HOOK_OFFSETLESS(PlayerController_Update, void, Il2CppObject* self) {

    PlayerController_Update(self);

    if(noArrows) {
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
                Vector3 leftSaberLocalPosition;
                il2cpp_utils::RunMethod(&rightSaberLocalPosition, rightSaberTransform, getMethod);
                il2cpp_utils::RunMethod(&leftSaberLocalPosition, leftSaberTransform, getMethod);
                Quaternion rightSaberLocalRotation;
                Quaternion leftSaberLocalRotation;
                il2cpp_utils::RunMethod(&rightSaberLocalRotation, rightSaberTransform, getLocalRotation);
                il2cpp_utils::RunMethod(&leftSaberLocalRotation, leftSaberTransform, getLocalRotation);

                if(!leftHand) {
                    il2cpp_utils::RunMethod(leftSaberTransform, setMethod, rightSaberLocalPosition);
                    il2cpp_utils::RunMethod(leftSaberTransform, setLocalRotation, rightSaberLocalRotation);
                    il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, 180, 0});
                    il2cpp_utils::RunMethod(leftSaberTransform, setTranslate, Vector3{0, 0, 0.335});
                } else {
                    il2cpp_utils::RunMethod(rightSaberTransform, setMethod, leftSaberLocalPosition);
                    il2cpp_utils::RunMethod(rightSaberTransform, setLocalRotation, leftSaberLocalRotation);
                    il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{ 0, 180, 0});
                    il2cpp_utils::RunMethod(rightSaberTransform, setTranslate, Vector3{0, 0, 0.335});
                }
            }
        }
    }
}

MAKE_HOOK_OFFSETLESS(HapticFeedbackController_HitNote, void, Il2CppObject* self, int node) {
    if(noArrows) {
        if(!leftHand) {
            node = 5;
        } else {
            node = 4;
        }
    }
    HapticFeedbackController_HitNote(self, node);
}


extern "C" void load() {
    log(INFO, "Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(Refresh, il2cpp_utils::FindMethodUnsafe("", "GameplayModifierToggle", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(SongStart, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 8));
    INSTALL_HOOK_OFFSETLESS(PlayerController_Update, il2cpp_utils::FindMethodUnsafe("", "PlayerController", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(HapticFeedbackController_HitNote, il2cpp_utils::FindMethodUnsafe("", "HapticFeedbackController", "HitNote", 1));
    log(INFO, "Installed all hooks!");
}
