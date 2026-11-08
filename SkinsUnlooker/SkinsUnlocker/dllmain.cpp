#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>

#pragma comment(linker, "/export:GetFileVersionInfoA=C:\\Windows\\System32\\version.GetFileVersionInfoA")
#pragma comment(linker, "/export:GetFileVersionInfoByHandle=C:\\Windows\\System32\\version.GetFileVersionInfoByHandle")
#pragma comment(linker, "/export:GetFileVersionInfoSizeA=C:\\Windows\\System32\\version.GetFileVersionInfoSizeA")
#pragma comment(linker, "/export:GetFileVersionInfoSizeW=C:\\Windows\\System32\\version.GetFileVersionInfoSizeW")
#pragma comment(linker, "/export:GetFileVersionInfoW=C:\\Windows\\System32\\version.GetFileVersionInfoW")
#pragma comment(linker, "/export:VerFindFileA=C:\\Windows\\System32\\version.VerFindFileA")
#pragma comment(linker, "/export:VerFindFileW=C:\\Windows\\System32\\version.VerFindFileW")
#pragma comment(linker, "/export:VerInstallFileA=C:\\Windows\\System32\\version.VerInstallFileA")
#pragma comment(linker, "/export:VerInstallFileW=C:\\Windows\\System32\\version.VerInstallFileW")
#pragma comment(linker, "/export:VerLanguageNameA=C:\\Windows\\System32\\version.VerLanguageNameA")
#pragma comment(linker, "/export:VerLanguageNameW=C:\\Windows\\System32\\version.VerLanguageNameW")
#pragma comment(linker, "/export:VerQueryValueA=C:\\Windows\\System32\\version.VerQueryValueA")
#pragma comment(linker, "/export:VerQueryValueW=C:\\Windows\\System32\\version.VerQueryValueW")

#include "minhook/MinHook.h"

typedef void* (*il2cpp_domain_get_t)();
typedef void** (*il2cpp_domain_get_assemblies_t)(void* domain, size_t* size);
typedef void* (*il2cpp_assembly_get_image_t)(void* assembly);
typedef void* (*il2cpp_class_from_name_t)(void* image, const char* namespaze, const char* name);
typedef void* (*il2cpp_class_get_methods_t)(void* klass, void** iter);
typedef const char* (*il2cpp_method_get_name_t)(void* method);

struct MethodInfo {
    void* methodPointer;
};

bool __fastcall Detour_CheckOwnership(void* __this, void* arg1, void* arg2, void* arg3) {
    return true;
}

void* oOriginalMethod = nullptr;

void HookMethodsForClass(void* klass, il2cpp_class_get_methods_t il2cpp_class_get_methods, il2cpp_method_get_name_t il2cpp_method_get_name) {
    if (!klass) return;

    void* iter = nullptr;
    while (void* method = il2cpp_class_get_methods(klass, &iter)) {
        const char* methodName = il2cpp_method_get_name(method);

        if (lstrcmpiA(methodName, "GetPurchase") == 0 ||
            lstrcmpiA(methodName, "HasItem") == 0 ||
            lstrcmpiA(methodName, "HasOwned") == 0 ||
            lstrcmpiA(methodName, "HasPurchased") == 0 ||
            lstrcmpiA(methodName, "AmIOwner") == 0)
        {
            MethodInfo* methodInfo = (MethodInfo*)method;
            void* functionPointer = methodInfo->methodPointer;

            if (functionPointer) {
                MH_CreateHook(functionPointer, (LPVOID)&Detour_CheckOwnership, &oOriginalMethod);
                MH_EnableHook(functionPointer);
            }
        }
    }
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    while (GetModuleHandleA("GameAssembly.dll") == nullptr) {
        Sleep(500);
    }

    HMODULE gameAssembly = GetModuleHandleA("GameAssembly.dll");

    auto il2cpp_domain_get = (il2cpp_domain_get_t)GetProcAddress(gameAssembly, "il2cpp_domain_get");
    auto il2cpp_domain_get_assemblies = (il2cpp_domain_get_assemblies_t)GetProcAddress(gameAssembly, "il2cpp_domain_get_assemblies");
    auto il2cpp_assembly_get_image = (il2cpp_assembly_get_image_t)GetProcAddress(gameAssembly, "il2cpp_assembly_get_image");
    auto il2cpp_class_from_name = (il2cpp_class_from_name_t)GetProcAddress(gameAssembly, "il2cpp_class_from_name");
    auto il2cpp_class_get_methods = (il2cpp_class_get_methods_t)GetProcAddress(gameAssembly, "il2cpp_class_get_methods");
    auto il2cpp_method_get_name = (il2cpp_method_get_name_t)GetProcAddress(gameAssembly, "il2cpp_method_get_name");

    if (!il2cpp_domain_get || !il2cpp_class_from_name || !il2cpp_class_get_methods) return 0;

    if (MH_Initialize() != MH_OK) return 0;

    void* domain = il2cpp_domain_get();
    size_t size = 0;
    void** assemblies = il2cpp_domain_get_assemblies(domain, &size);

    const char* targetClasses[] = { "PlayerStoreData", "PlayerPurchasesData", "HatManager", "CosmeticManager" };
    size_t classCount = 4; // Виправлено: жорстко задана кількість елементів масиву

    for (size_t i = 0; i < size; ++i) {
        void* image = il2cpp_assembly_get_image(assemblies[i]);
        if (!image) continue;

        for (size_t j = 0; j < classCount; ++j) {
            void* klass = il2cpp_class_from_name(image, "", targetClasses[j]);
            if (klass) {
                HookMethodsForClass(klass, il2cpp_class_get_methods, il2cpp_method_get_name);
            }
        }
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        break;
    }
    return TRUE;
}
