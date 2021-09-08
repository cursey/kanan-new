#include <Scan.hpp>
#include <Utility.hpp>
#include <imgui.h>
#include <iostream>
#include "Kanan.hpp"
#include "Log.hpp"



#include "ChangeChannelHotkey.hpp"


namespace kanan {


	//copyed from autochangechannels

	static ChangeChannelHotkey* g_auto_cc{};
	ChangeChannelHotkey::ChangeChannelHotkey() {

	}
	//runs on each frame
	void ChangeChannelHotkey::onFrame() {
		if (!m_is_enabled) {
			return;
		}

		// Get the local player.
		auto game = g_kanan->getGame();
		auto me = game->getLocalCharacter();

		if (me == nullptr) {
			return;
		}

		auto me_id = me->getID();

		if (!me_id) {
			return;
		}




	}

	//creates our ui
	void ChangeChannelHotkey::onUI() {
		if (ImGui::CollapsingHeader("Change Channel on hotkey")) {
			ImGui::TextWrapped("uses the Left shift + numpad to change channels");
			ImGui::Checkbox("Enable change channel hot key", &m_is_enabled);
		}
	}

	//load configs
	void ChangeChannelHotkey::onConfigLoad(const Config& cfg) {
		m_is_enabled = cfg.get<bool>("ChangeChannelHotkey.Enabled").value_or(false);
	}

	//save config
	void ChangeChannelHotkey::onConfigSave(Config& cfg) {
		cfg.set<bool>("ChangeChannelHotkey.Enabled", m_is_enabled);
	}

	//use Direct Input Keyboard to see if key is being pressed

	void ChangeChannelHotkey::onKeyUp(DWORD key)
	{
		if (m_is_enabled) {
			if (key == DIK_LSHIFT) {
				modiDown = false;
				//log("alt released");
			}
		}

	}


	void ChangeChannelHotkey::onKeyDown(DWORD key)
	{
		if (m_is_enabled) {
			if (key == DIK_LSHIFT) {
				modiDown = true;
				//log("alt pressed");

			}
			if (modiDown) {
				if (key == DIK_NUMPAD1) {
					ChangeCh(1);
				}
				if (key == DIK_NUMPAD2) {
					ChangeCh(2);
				}
				if (key == DIK_NUMPAD3) {
					ChangeCh(3);
				}
				if (key == DIK_NUMPAD4) {
					ChangeCh(4);
				}
				if (key == DIK_NUMPAD5) {
					ChangeCh(5);
				}
				if (key == DIK_NUMPAD6) {
					ChangeCh(6);
				}
				if (key == DIK_NUMPAD7) {
					ChangeCh(7);
				}
				if (key == DIK_NUMPAD8) {
					ChangeCh(8);
				}
				if (key == DIK_NUMPAD9) {
					ChangeCh(9);
				}
				if (key == DIK_NUMPAD0) {
					ChangeCh(10);
				}
			}
		}
	}

	//wrapper for ChangeChannel to makle the keydownfunc tidy
	void ChangeChannelHotkey::ChangeCh(int ch) {
		//for some reason log doesnt like doing strings the way im use to
		//this switch case is my half-arsed work around
		switch(ch){
		case 1:
			log("attempting to CC to ch1");
			break;
		case 2:
			log("attempting to CC to ch2");
			break;
		case 3:
			log("attempting to CC to ch3");
			break;
		case 4:
			log("attempting to CC to ch4");
			break;
		case 5:
			log("attempting to CC to ch5");
			break;
		case 6:
			log("attempting to CC to ch6");
			break;
		case 7:
			log("attempting to CC to ch7");
			break;
		case 8:
			log("attempting to CC to ch8");
			break;
		case 9:
			log("attempting to CC to ch9");
			break;
		case 10:
			log("attempting to CC to ch10");
			break;
		default:
			log("trying to cc to ch outside of range.");
			log("canceling cc to prevent issues");
			return;
		}	
		//get game thread? check if local char exists
		auto game = g_kanan->getGame();
		auto localCharacter = game->getLocalCharacter();
		//if local char does exist then cc
		if (localCharacter != nullptr) {
			game->changeChannel(ch);
		}
		else {
			log("local char does not exist, can not cc. doing nothing");
		}
	}


















}// namespace kanan
