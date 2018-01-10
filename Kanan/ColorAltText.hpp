#pragma once

#include <Patch.hpp>

#include "PatchMod.hpp"

namespace kanan {
	class ColorAltText : public PatchMod {
	public:
		ColorAltText();

		void onPatchUI() override;

		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

	private:
		bool m_isEnabledOthers;
		bool m_isEnabledSelf;
		Patch m_patchSelf;
		Patch m_patchOthers;
		float rgbaOthers[4];
		unsigned int argbValOthers;
		float rgbaSelf[4];
		unsigned int argbValSelf;

		void apply();
	};
}