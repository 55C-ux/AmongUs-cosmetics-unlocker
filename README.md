# Among Us Cosmetics Unlocker v1 🎭

A lightweight, 100% autonomous cosmetics unlocker for the PC version of Among Us. It unlocks all skins, hats, and pets dynamically via the Unity IL2CPP API using a proxy DLL (`version.dll`) hijacking method.

## ✨ Features
- **Zero-Configuration:** Just drop the file into the game directory and play.
- **Dynamic Pattern Scanning:** Automatically requests function pointers from the Unity engine at startup, bypassing the need for manual memory offsets or *Il2CppDumper*.
- **Lightweight & Clean:** Pure C++ implementation powered by MinHook.

## 📊 Compatibility & Platforms

| Platform / Store | Supported | Notes |
| :--- | :---: | :--- |
| **Steam** (PC) |  Yes | Fully supported |
| **Epic Games Store** (PC) |  Yes | Fully supported |
| **Microsoft Store / Xbox App** (PC) |  Yes | Fully supported |
| **Android** (Phones/Tablets) | ❌ No | `.dll` files are not supported on mobile OS |
| **iOS / iPadOS** (iPhone/iPad) | ❌ No | `.dll` files are not supported on mobile OS |
| **Consoles** (PlayStation, Xbox, Switch) | ❌ No | Closed platforms, proxy DLL methods cannot be used |

## 🚀 Installation
1. Go to the ([**Releases**](https://github.com/) section and download the latest compiled `version.dll`.
2. Place the `version.dll` file directly into your Among Us root directory (right next to `Among Us.exe`).
3. Launch the game normally via your launcher. 

*To completely uninstall the unlocker, simply delete the `version.dll` file from the game folder.*

### 📂 How to find the Among Us installation folder:

Depending on where you bought the game, follow these instructions to find `Among Us.exe`:

#### 🎮 Steam:
1. Open your Steam Library.
2. Right-click on **Among Us** and select **Properties...**
3. Go to the **Installed Files** (or **Local Files**) tab on the left.
4. Click the **Browse...** button. A Windows Explorer window will open directly in the game's root folder.

#### 🛒 Epic Games Store:
1. Open the Epic Games Launcher and go to your **Library**.
2. Click on the **three dots (...)** underneath the Among Us tile.
3. Select **Manage** and click the **Folder icon** next to the "Installation" label.

#### 📦 Microsoft Store / Xbox App (PC):
1. Open the Xbox App on your PC and navigate to **Among Us** in your installed games list.
2. Click the **three dots (...)** next to the Play button and choose **Manage**.
3. Go to the **Files** tab and click **Browse...**
4. Open the **Among Us** folder, then navigate inside the `Content` folder to find the `.exe` file.

## ⚙️ Build Instructions & Disclaimer
- **Compilation:** Open the project solution in Visual Studio, ensure the MinHook library is linked, compile it in **Release (x64)** mode, and rename the output binary to `version.dll`.
- **Disclaimer:** This project is created for educational and private testing purposes only. Use it at your own risk.
