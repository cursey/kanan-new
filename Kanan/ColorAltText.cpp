#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "ColorAltText.hpp"

using namespace std;

namespace kanan {
	ColorAltText::ColorAltText()
		: PatchMod{ "Color Alt Text", "" },
        m_isEnabledOthers{ false },
		argbValOthers{ 0xFFFFFFFF },
		rgbaOthers{ 1.0f, 1.0f, 1.0f },
		m_isEnabledSelf{ false },
		argbValSelf{ 0xFFFFFFFF },
		rgbaSelf{ 1.0f, 1.0f, 1.0f },
		m_patchSelf{},
		m_patchOthers {}
	{
		auto address = scan("client.exe", "B8 ? ? ? ? B9 ? ? ? ? 0F 45 ? 41 89");

		if (address) {
			log("Got ColorAltText %p", *address);

			m_patchOthers.address = *address + 1;
			m_patchOthers.bytes = { 0xFF, 0xFF, 0xFF, 0xFF }; //in the order b g r a

			m_patchSelf.address = *address + 6;
			m_patchSelf.bytes = { 0xFF, 0xFF, 0xFF, 0xFF }; //in the order b g r a
		}
		else {
			log("Failed to find ColorAltText address");
		}
	}

	void ColorAltText::onPatchUI() {
		if (m_patchOthers.address == 0 || m_patchSelf.address == 0) {
			return;
		}

		bool reapply = false;
		ImGui::Checkbox("Color Alt Text Others", &m_isEnabledOthers);
		if (m_isEnabledOthers) {
			if (ImGui::ColorEdit4("Others", rgbaOthers, ImGuiColorEditFlags_DisplayHex)) {
				u_int r = (u_int)(255 * rgbaOthers[0]);
				u_int g = (u_int)(255 * rgbaOthers[1]);
				u_int b = (u_int)(255 * rgbaOthers[2]);
				u_int a = (u_int)(255 * rgbaOthers[3]);
				argbValOthers = (a << 24) + (r << 16) + (g << 8) + b;
				reapply = true;
			}
		}
		ImGui::Checkbox("Color Alt Text Self", &m_isEnabledSelf);
		if (m_isEnabledSelf) {
			if (ImGui::ColorEdit4("Self", rgbaSelf, ImGuiColorEditFlags_DisplayHex)) {
				u_int r = (u_int)(255 * rgbaSelf[0]);
				u_int g = (u_int)(255 * rgbaSelf[1]);
				u_int b = (u_int)(255 * rgbaSelf[2]);
				u_int a = (u_int)(255 * rgbaSelf[3]);
				argbValSelf = (a << 24) + (r << 16) + (g << 8) + b;
				reapply = true;
			}
		}
		if (reapply) {
			apply();
		}
	}

	void ColorAltText::onConfigLoad(const Config& cfg) {
		m_isEnabledSelf = cfg.get<bool>("ColorAltTextSelf.Enabled").value_or(false);
		argbValSelf = cfg.get<unsigned int>("ColorAltTextSelf.Color").value_or(0xFFFFFFFF);
		rgbaSelf[0] = ((argbValSelf >> 16) & 0xFF) / 255.0f; //r
		rgbaSelf[1] = ((argbValSelf >> 8) & 0xFF) / 255.0f; //g
		rgbaSelf[2] = (argbValSelf & 0xFF) / 255.0f; //b
		rgbaSelf[3] = ((argbValSelf >> 24) & 0xFF) / 255.0f; //a

		m_isEnabledOthers = cfg.get<bool>("ColorAltTextOthers.Enabled").value_or(false);
		argbValOthers = cfg.get<unsigned int>("ColorAltTextOthers.Color").value_or(0xFFFFFFFF);
		rgbaOthers[0] = ((argbValOthers >> 16) & 0xFF) / 255.0f; //r
		rgbaOthers[1] = ((argbValOthers >> 8) & 0xFF) / 255.0f; //g
		rgbaOthers[2] = (argbValOthers & 0xFF) / 255.0f; //b
		rgbaOthers[3] = ((argbValOthers >> 24) & 0xFF) / 255.0f; //a

		if (m_isEnabledSelf || m_isEnabledOthers) {
			apply();
		}
	}

	void ColorAltText::onConfigSave(Config& cfg) {
		cfg.set<bool>("ColorAltTextOthers.Enabled", m_isEnabledOthers);
		cfg.set<unsigned int>("ColorAltTextOthers.Color", argbValOthers);

		cfg.set<bool>("ColorAltTextSelf.Enabled", m_isEnabledSelf);
		cfg.set<unsigned int>("ColorAltTextSelf.Color", argbValSelf);
	}

	void ColorAltText::apply() {
		if (m_patchSelf.address == 0 || m_patchOthers.address == 0) {
			return;
		}

		log("Toggling ColorAltText...");

		if (m_isEnabledSelf) {
			int16_t a = (argbValSelf >> 24) & 0xFF;
			int16_t r = (argbValSelf >> 16) & 0xFF;
			int16_t g = (argbValSelf >> 8) & 0xFF;
			int16_t b = (argbValSelf) & 0xFF;

			m_patchSelf.bytes = { b, g, r, a };
			patch(m_patchSelf);
		}
		else {
			undoPatch(m_patchSelf);
		}

		if (m_isEnabledOthers) {
			int16_t a = (argbValOthers >> 24) & 0xFF;
			int16_t r = (argbValOthers >> 16) & 0xFF;
			int16_t g = (argbValOthers >> 8) & 0xFF;
			int16_t b = (argbValOthers) & 0xFF;

			m_patchOthers.bytes = { b, g, r, a };
			patch(m_patchOthers);
		}
		else {
			undoPatch(m_patchOthers);
		}
	}
}