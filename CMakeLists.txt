# This file is automatically generated from cmake.toml - DO NOT EDIT
# See https://github.com/build-cpp/cmkr for more information

cmake_minimum_required(VERSION 3.15)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-tree builds are not supported. Run CMake from a separate directory: cmake -B build")
endif()

# Regenerate CMakeLists.txt automatically in the root project
set(CMKR_ROOT_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(CMKR_ROOT_PROJECT ON)

	# Bootstrap cmkr
	include(cmkr.cmake OPTIONAL RESULT_VARIABLE CMKR_INCLUDE_RESULT)
	if(CMKR_INCLUDE_RESULT)
		cmkr()
	endif()

	# Enable folder support
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Create a configure-time dependency on cmake.toml to improve IDE support
if(CMKR_ROOT_PROJECT)
	configure_file(cmake.toml cmake.toml COPYONLY)
endif()

project(Kanan)

set(VCPKG_TARGET_TRIPLET x64-windows-static)

if(CMKR_ROOT_PROJECT AND NOT CMKR_DISABLE_VCPKG)
	include(FetchContent)
	message(STATUS "Fetching vcpkg (2023.10.19)...")
	FetchContent_Declare(vcpkg URL "https://github.com/microsoft/vcpkg/archive/refs/tags/2023.10.19.tar.gz")
	FetchContent_MakeAvailable(vcpkg)
	include("${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake")
endif()

include(FetchContent)

message(STATUS "Fetching safetyhook (ff5eb17ece60397463acae3da02509e305496407)...")
FetchContent_Declare(safetyhook
	GIT_REPOSITORY
		"https://github.com/cursey/safetyhook.git"
	GIT_TAG
		ff5eb17ece60397463acae3da02509e305496407
)
FetchContent_MakeAvailable(safetyhook)

set_target_properties(safetyhook PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Packages
find_package(bddisasm REQUIRED CONFIG)

find_package(imgui REQUIRED CONFIG)

find_package(nlohmann_json REQUIRED CONFIG)

# Target Core
set(CMKR_TARGET Core)
set(Core_SOURCES "")

list(APPEND Core_SOURCES
	"Core/Config.cpp"
	"Core/FunctionHook.cpp"
	"Core/Memory.cpp"
	"Core/Module.cpp"
	"Core/Patch.cpp"
	"Core/Pattern.cpp"
	"Core/Process.cpp"
	"Core/RemoteMemory.cpp"
	"Core/RemoteModule.cpp"
	"Core/RemoteProcess.cpp"
	"Core/RemoteThread.cpp"
	"Core/Scan.cpp"
	"Core/String.cpp"
	"Core/Utility.cpp"
	"Core/Config.hpp"
	"Core/FunctionHook.hpp"
	"Core/Memory.hpp"
	"Core/Module.hpp"
	"Core/Patch.hpp"
	"Core/Pattern.hpp"
	"Core/Process.hpp"
	"Core/RemoteMemory.hpp"
	"Core/RemoteModule.hpp"
	"Core/RemoteProcess.hpp"
	"Core/RemoteThread.hpp"
	"Core/Scan.hpp"
	"Core/String.hpp"
	"Core/Utility.hpp"
)

list(APPEND Core_SOURCES
	cmake.toml
)

set(CMKR_SOURCES ${Core_SOURCES})
add_library(Core STATIC)

if(Core_SOURCES)
	target_sources(Core PRIVATE ${Core_SOURCES})
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Core_SOURCES})

target_compile_definitions(Core PUBLIC
	UNICODE
)

target_compile_features(Core PUBLIC
	cxx_std_20
)

target_include_directories(Core PUBLIC
	"Core/"
)

target_link_libraries(Core PUBLIC
	safetyhook::safetyhook
)

set_target_properties(Core PROPERTIES
	MSVC_RUNTIME_LIBRARY
		MultiThreaded$<$<CONFIG:Debug>:Debug>
)

unset(CMKR_TARGET)
unset(CMKR_SOURCES)

# Target Kanan
set(CMKR_TARGET Kanan)
set(Kanan_SOURCES "")

list(APPEND Kanan_SOURCES
	"Kanan/AutoChangeChannels.cpp"
	"Kanan/AutoSetMTU.cpp"
	"Kanan/BorderlessWindow.cpp"
	"Kanan/ChangeChannelHotkey.cpp"
	"Kanan/CharacterWindowTitle.cpp"
	"Kanan/ColorAltText.cpp"
	"Kanan/CookingMod.cpp"
	"Kanan/Currtarget.cpp"
	"Kanan/D3D9Hook.cpp"
	"Kanan/DInputHook.cpp"
	"Kanan/DisableNagle.cpp"
	"Kanan/DontMoveToSquadChat.cpp"
	"Kanan/DrawDistance.cpp"
	"Kanan/EnableMultiClient.cpp"
	"Kanan/EntityViewer.cpp"
	"Kanan/EquipmentOverride.cpp"
	"Kanan/FieldOfView.cpp"
	"Kanan/FreezeTimeOfDay.cpp"
	"Kanan/Game.cpp"
	"Kanan/KCamera.cpp"
	"Kanan/KCharacter.cpp"
	"Kanan/KItem.cpp"
	"Kanan/Kanan.cpp"
	"Kanan/KananInit.cpp"
	"Kanan/Log.cpp"
	"Kanan/LoginScreen.cpp"
	"Kanan/Mods.cpp"
	"Kanan/PatchMod.cpp"
	"Kanan/PetDesummonFix.cpp"
	"Kanan/PetSummonGroups.cpp"
	"Kanan/RangedAttackSwap.cpp"
	"Kanan/StatusUI.cpp"
	"Kanan/TTFFontSize.cpp"
	"Kanan/UseDataFolder.cpp"
	"Kanan/WindowsMessageHook.cpp"
	"Kanan/metrics_gui/ProggyTiny.cpp"
	"Kanan/metrics_gui/metrics_gui.cpp"
	"Kanan/AutoChangeChannels.hpp"
	"Kanan/AutoSetMTU.hpp"
	"Kanan/BorderlessWindow.hpp"
	"Kanan/ChangeChannelHotkey.hpp"
	"Kanan/CharacterWindowTitle.hpp"
	"Kanan/ColorAltText.hpp"
	"Kanan/CookingMod.hpp"
	"Kanan/Currtarget.hpp"
	"Kanan/D3D9Hook.hpp"
	"Kanan/DInputHook.hpp"
	"Kanan/DisableNagle.hpp"
	"Kanan/DontMoveToSquadChat.hpp"
	"Kanan/DrawDistance.hpp"
	"Kanan/EnableMultiClient.hpp"
	"Kanan/EntityViewer.hpp"
	"Kanan/EquipmentOverride.hpp"
	"Kanan/FieldOfView.hpp"
	"Kanan/FontData.hpp"
	"Kanan/FreezeTimeOfDay.hpp"
	"Kanan/Game.hpp"
	"Kanan/KCamera.hpp"
	"Kanan/KCharacter.hpp"
	"Kanan/KItem.hpp"
	"Kanan/Kanan.hpp"
	"Kanan/Log.hpp"
	"Kanan/LoginScreen.hpp"
	"Kanan/Mabinogi.hpp"
	"Kanan/Mabinogi/CAccount.hpp"
	"Kanan/Mabinogi/CCharacter.hpp"
	"Kanan/Mabinogi/CEntityList.hpp"
	"Kanan/Mabinogi/CItem.hpp"
	"Kanan/Mabinogi/CRenderer.hpp"
	"Kanan/Mabinogi/CString.hpp"
	"Kanan/Mabinogi/CWorld.hpp"
	"Kanan/Mabinogi/Matrix4x4.hpp"
	"Kanan/Mabinogi/Vector3.hpp"
	"Kanan/Mabinogi/Vector4.hpp"
	"Kanan/Mod.hpp"
	"Kanan/Mods.hpp"
	"Kanan/PatchMod.hpp"
	"Kanan/PetDesummonFix.hpp"
	"Kanan/PetSummonGroups.hpp"
	"Kanan/RangedAttackSwap.hpp"
	"Kanan/StatusUI.hpp"
	"Kanan/TTFFontSize.hpp"
	"Kanan/UseDataFolder.hpp"
	"Kanan/WindowsMessageHook.hpp"
	"Kanan/metrics_gui/countof.h"
	"Kanan/metrics_gui/metrics_gui.h"
	"Kanan/metrics_gui/perf_timer.h"
	"Kanan/metrics_gui/snprintf.h"
)

list(APPEND Kanan_SOURCES
	cmake.toml
)

set(CMKR_SOURCES ${Kanan_SOURCES})
add_library(Kanan SHARED)

if(Kanan_SOURCES)
	target_sources(Kanan PRIVATE ${Kanan_SOURCES})
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Kanan_SOURCES})

target_compile_definitions(Kanan PUBLIC
	IMGUI_DISABLE_INCLUDE_IMCONFIG_H
)

target_include_directories(Kanan PUBLIC
	"Kanan/"
)

target_link_libraries(Kanan PUBLIC
	Core
	imgui::imgui
	nlohmann_json::nlohmann_json
	ws2_32
)

set_target_properties(Kanan PROPERTIES
	MSVC_RUNTIME_LIBRARY
		MultiThreaded$<$<CONFIG:Debug>:Debug>
)

# Copy Kanan/Patches.json to the output directory.
add_custom_command(TARGET Kanan POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Kanan/Patches.json
        $<TARGET_FILE_DIR:Kanan>
)

unset(CMKR_TARGET)
unset(CMKR_SOURCES)

# Target Loader
set(CMKR_TARGET Loader)
set(Loader_SOURCES "")

list(APPEND Loader_SOURCES
	"Loader/Main.cpp"
)

list(APPEND Loader_SOURCES
	cmake.toml
)

set(CMKR_SOURCES ${Loader_SOURCES})
add_executable(Loader)

if(Loader_SOURCES)
	target_sources(Loader PRIVATE ${Loader_SOURCES})
endif()

get_directory_property(CMKR_VS_STARTUP_PROJECT DIRECTORY ${PROJECT_SOURCE_DIR} DEFINITION VS_STARTUP_PROJECT)
if(NOT CMKR_VS_STARTUP_PROJECT)
	set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Loader)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Loader_SOURCES})

target_link_libraries(Loader PRIVATE
	Core
)

target_link_options(Loader PRIVATE
	"/MANIFESTUAC:level='requireAdministrator'"
)

set_target_properties(Loader PROPERTIES
	MSVC_RUNTIME_LIBRARY
		MultiThreaded$<$<CONFIG:Debug>:Debug>
)

# Copy Loader/Loader.txt to the output directory
add_custom_command(TARGET Loader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Loader/Loader.txt
        $<TARGET_FILE_DIR:Loader>
)

unset(CMKR_TARGET)
unset(CMKR_SOURCES)

# Target Launcher
set(CMKR_TARGET Launcher)
set(Launcher_SOURCES "")

list(APPEND Launcher_SOURCES
	"Launcher/App.cpp"
	"Launcher/Crypto.cpp"
	"Launcher/HttpClient.cpp"
	"Launcher/Image.cpp"
	"Launcher/LauncherApp.cpp"
	"Launcher/RegQuery.cpp"
	"Launcher/Renderer.cpp"
	"Launcher/WinMain.cpp"
	"Launcher/WmiQuery.cpp"
	"Launcher/App.hpp"
	"Launcher/BackgroundPng.hpp"
	"Launcher/Crypto.hpp"
	"Launcher/HttpClient.hpp"
	"Launcher/Image.hpp"
	"Launcher/LauncherApp.hpp"
	"Launcher/RegQuery.hpp"
	"Launcher/Renderer.hpp"
	"Launcher/ScopeExit.hpp"
	"Launcher/WmiQuery.hpp"
)

list(APPEND Launcher_SOURCES
	cmake.toml
)

set(CMKR_SOURCES ${Launcher_SOURCES})
add_executable(Launcher)

if(Launcher_SOURCES)
	target_sources(Launcher PRIVATE ${Launcher_SOURCES})
endif()

get_directory_property(CMKR_VS_STARTUP_PROJECT DIRECTORY ${PROJECT_SOURCE_DIR} DEFINITION VS_STARTUP_PROJECT)
if(NOT CMKR_VS_STARTUP_PROJECT)
	set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Launcher)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Launcher_SOURCES})

target_compile_definitions(Launcher PRIVATE
	IMGUI_DISABLE_INCLUDE_IMCONFIG_H
)

target_include_directories(Launcher PRIVATE
	"Kanan/"
)

target_link_libraries(Launcher PRIVATE
	Core
	imgui::imgui
	nlohmann_json::nlohmann_json
	Bcrypt
	Crypt32
	Winhttp
	Rpcrt4
	D3d9
	Wbemuuid
)

target_link_options(Launcher PRIVATE
	"/MANIFESTUAC:level='requireAdministrator'"
)

set_target_properties(Launcher PROPERTIES
	MSVC_RUNTIME_LIBRARY
		MultiThreaded$<$<CONFIG:Debug>:Debug>
	WIN32_EXECUTABLE
		ON
)

unset(CMKR_TARGET)
unset(CMKR_SOURCES)

install(
	TARGETS
		Kanan
		Loader
		Launcher
	DESTINATION
		bin
	COMPONENT
		Kanan
)
install(
	FILES
		"Loader/Loader.txt"
		"Kanan/Patches.json"
	DESTINATION
		bin
)
