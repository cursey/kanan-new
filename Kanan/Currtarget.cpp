#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

#include "Mabinogi.hpp"
#include "KCharacter.hpp"
#include "Kanan.hpp"
#include "Currtarget.hpp"

namespace kanan {

	//most of this is based off status ui and entity viewer



	Currtarget::Currtarget()
		: m_hp{}
	{
		m_hp.reserve(100);
	}

	void Currtarget::onFrame()
	{

		auto game = g_kanan->getGame();
		auto localCharacter = game->getLocalCharacter();
		KCharacter* target = nullptr;

		//check if there is a local char or target for local char. if not dont bother rendering window
		if (!localCharacter) { hastarget = false; }
		else { hastarget = true; }
		if (localCharacter) {
			target = g_kanan->getGame()->getCharacterByID(localCharacter->targetID);
			if (target == nullptr) { hastarget = false; }
			else { hastarget = true; }
		}


		if (g_kanan->isUIOpen()) { hastarget = true; }

		//if enabled create new window for details
		if (m_is_enabled && hastarget) {
			createtargetwindow();
		}
	}

	void Currtarget::onUI()
	{
		if (ImGui::CollapsingHeader("Show details of targeted mob")) {
			if (ImGui::Checkbox("Show details", &m_is_enabled)) {}
			if (m_is_enabled) {
				if (ImGui::Checkbox("Show Target Pos", &showpos)) {}
				if (ImGui::Checkbox("Show Targets, Target", &showothertarget)) {}
				if (ImGui::Checkbox("Show Target HP", &showtargethp)) {}
			}
		}




	}



	//create imgui window for details of target to exist in
	void Currtarget::createtargetwindow() {
		// Clear the current list.
		m_characters.clear();
		auto game = g_kanan->getGame();
		//get local char
		auto localCharacter = game->getLocalCharacter();


		//if no local char do nothing

		//get target of local char
		KCharacter* target = nullptr;
		KCharacter* targetchar = nullptr;

		//if we have a localCharacter do rest of logic, if not do nothing
		if (localCharacter) {
			// Iterate over every character and add it to our list.
			target = g_kanan->getGame()->getCharacterByID(localCharacter->targetID);
			auto entityList = game->getEntityList();
			//get all other characters
			auto& characters = entityList->characters;
			auto highestIndex = characters.count;
			auto node = characters.root;



			//create a list of all local chars

			for (uint32_t i = 0; i <= highestIndex && node != nullptr; ++i, node = node->next) {
				auto character = (KCharacter*)node->entry->character;

				if (character == nullptr || !character->getID()) {
					continue;
				}

				m_characters.emplace_front(character);
			}



			//check over all the enitys and find the one being targeted
			for (auto& character : m_characters) {
				if (target) {
					std::string targetstring = target->getName()->c_str();
					auto name = character->getName();
					if (name == targetstring) {
						targetchar = character;
					}
				}
			}
		}

		//create imgui window

		ImGui::SetNextWindowSize(ImVec2{ 282, 154 }, ImGuiCond_FirstUseEver);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(77, 77, 77, 120));
		if (g_kanan->isUIOpen()) {
			if (!ImGui::Begin("Target Details", &m_is_enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
				ImGui::End();
				return;
			}
		}
		else {
			if (!ImGui::Begin("Target Details", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
				ImGui::End();
				return;
			}
		}


		//if local char does not exist then say no details to show
		if (localCharacter == nullptr) {
			ImGui::TextWrapped("no local char");

		}
		else {

			//fill out imgui window
			auto localname = localCharacter->getName();
			if (target) { ImGui::TextWrapped("%s's Target: %s", localname->c_str(), (!target) ? "No Target" : target->getName()->c_str()); }
			else { ImGui::TextWrapped("%s has no target", localname->c_str()); }

			if (targetchar != nullptr) {
				auto id = targetchar->getID();
				auto name = targetchar->getName();
				auto pos = targetchar->getPosition();
				auto parameter = targetchar->parameter;
				auto targetoftarget = g_kanan->getGame()->getCharacterByID(targetchar->targetID);;


				auto hp = parameter->life.value;
				auto maxHP = parameter->lifeMaxBase.value + parameter->lifeMaxMod.value;
				auto hpRatio = ffdiv(hp, maxHP);

				ImGui::Indent();
				if (showpos) {
					ImGui::BulletText("Pos: %f, %f", pos->x, pos->y);

				}
				if (showothertarget) {
					ImGui::BulletText("Target: %s", (!targetoftarget) ? "No Target" : targetoftarget->getName()->c_str());

				}
				if (showtargethp) {
					ImGui::BulletText("Health: %f/%f", parameter->life.value, parameter->lifeMaxBase.value + parameter->lifeMaxMod.value);

					
					

				}
				ImGui::Unindent();
				if (showtargethp) {
					progressBar(hpRatio, ImVec2{ -1.0f, 0.0f }, 0xFF9140CF, "Target hp");
				}

			}
		}

		ImGui::End();
		ImGui::PopStyleColor();
	}


	//stolen from status ui
	float Currtarget::ffdiv(float x, float y) {
		if (y == 0.0f) {
			return 0.0f;
		}

		return x / y;
	}

	//edited imgui progress bar to have custom colour. stolen from statusUI and reused here
	void Currtarget::progressBar(float fraction, const ImVec2& sizeArg, ImU32 color, const char* overlay) {
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		ImVec2 pos = window->DC.CursorPos;
		ImRect bb(pos, pos + CalcItemSize(sizeArg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, 0))
			return;

		// Render
		fraction = ImSaturate(fraction);
		RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
		bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
		const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
		RenderRectFilledRangeH(window->DrawList, bb, GetColorU32(color), 0.0f, fraction, style.FrameRounding);

		// Default displaying the fraction as percentage string, but user can override it
		char overlay_buf[32];
		if (!overlay)
		{
			ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%", fraction * 100 + 0.01f);
			overlay = overlay_buf;
		}

		ImVec2 overlay_size = CalcTextSize(overlay, NULL);
		if (overlay_size.x > 0.0f)
			RenderTextClipped(ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x, bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x), bb.Min.y), bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f, 0.5f), &bb);
	}



	void Currtarget::onConfigLoad(const Config& cfg)
	{
		m_is_enabled = cfg.get<bool>("targetdetail.Enabled").value_or(false);
	}

	void Currtarget::onConfigSave(Config& cfg)
	{
		cfg.set<bool>("targetdetail.Enabled", m_is_enabled);
	}

	void Currtarget::onKeyUp(DWORD key)
	{
	}

	void Currtarget::onKeyDown(DWORD key)
	{
	}

}
