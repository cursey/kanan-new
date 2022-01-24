#pragma once

#include <Patch.hpp>

#include "PatchMod.hpp"

namespace kanan {
	class TTFFontSize : public PatchMod {
	public:
		TTFFontSize();

		bool applyNPCFix(bool npcFixEnable);
		void onPatchUI() override;
		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

	private:
		Patch m_patch;
		Patch m_patchNPC1;
		Patch m_patchNPCCall;
		int m_choice;
		unsigned char m_originalByte;
		std::vector<uint8_t> code;
		LPVOID code_address;
		bool foundNPCFix;
		void apply();
	};
}