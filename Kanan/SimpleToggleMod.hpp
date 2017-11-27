#pragma once

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
	class SimpleToggleMod : public Mod {
	public:
		SimpleToggleMod(const char* configName, const char* patchName, const char* tooltip);

		void onPatchUI() override;

		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

		bool addPatch(const char* scanBytes, int byteOffset, std::vector<int16_t> newBytes);

	private:
		bool m_isEnabled;
		bool m_isApplied;
		std::vector<Patch*> m_patches;
		const char* m_config_name;
		const char* m_patch_name;
		const char* m_tooltip;

		void applyPatches();
		void undoPatches();
		void apply();
	};
}
