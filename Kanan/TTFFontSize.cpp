#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "TTFFontSize.hpp"

using namespace std;

namespace kanan {
	TTFFontSize::TTFFontSize()
		: PatchMod{ "TTF Font Size", "" },
		m_choice{ 0 },
		m_patch{},
		m_originalByte{},
		foundNPCFix{ false },
		code_address{ NULL }

	{
		auto address = scan("client.exe", "C7 86 C4 00 00 00 0B 00 00 00 C7");				// Pattern for locating location of the byte that controls text font size
		auto npctextaddress1 = scan("client.exe", "E8 ? ? ? ? 56 53 8D 8D ? ? ? ? E8 ? ? ? ? 8B F0");	// Pattern that locates the function that calls the text sizing function when speaking with an NPC - Used for redirection.
		auto npctextmidcall = scan("client.exe", "8B 01 8B 40 08 48 83 F8 07 77 19");		//Start of a function that is called in the middle of our modified NPC text function
		if (address) {
			log("[TTF Font Size] Found address TTF Font Size %p", *address);

			m_patch.address = *address + 6;
			m_originalByte = { *reinterpret_cast<unsigned char*>(m_patch.address) };

			if (npctextaddress1 && npctextmidcall) {										//Create patches for redirecting function calls made when speaking to an NPC. NPC text boxes crash when using a trext size larger than 13. We create a copy to allow rest of the client to use larger values and redirect the NPC calls to our copy.
				log("[TTF Font Size] Found address TTFFontSize NPC Caller %p", *npctextaddress1);
				log("[TTF Font Size] Found address TTFFontSize NPC Call %p", *npctextmidcall);
				foundNPCFix = true;

				m_patchNPC1.address = *npctextaddress1;
				m_patchNPCCall.address = *npctextmidcall;
				// Array of bytes that make up the function called when drawing text. We define a copy, allocate our own memory for it and redirect NPC calls to this function instead of the client's. 
				// With a text size > 13 When an NPC is spoken to - the NPC dialogue box uses this function instead - using a size value of 13 as sizes higher will crash the game. 
				code = vector<uint8_t>{
					0x55,																	//-  1 push ebp
					0x8B, 0xEC,																//-  2 mov ebp, esp
					0x80, 0xB9, 0x09, 0x09, 0x00, 0x00, 0x00,								//-  3 cmp byte ptr[ecx + 00000909], 00
					0x74, 0x60,																//-  4 je Client.exe + C9E3CC
					0x56,																	//-  5 push esi
					0x8B, 0x75, 0x08,														//-  6 mov esi,[ebp + 08]
					0x80, 0xBE, 0xA8, 0x00, 0x00, 0x00, 0x00,								//-  7 cmp byte ptr[esi + 000000A8], 00
					0x74, 0x52,																//-  8 je Client.exe + C9E3CB
					0xB9, 0x44, 0x03, 0x6A, 0x03,											//-  9 mov ecx, Client.exe + 329B254 This changes from client to client patch. We patch this before redirecting
					0xE8, 0xED, 0x50, 0x4F, 0xFF,											//- 10 call Client.exe + 193470 -- This is a call to another function. We  update this call location as we enable this patch
					0x83, 0xF8, 0x01,														//- 11 cmp eax, 01
					0x75, 0x43,																//- 12 jne Client.exe + C9E3CB
					0x83, 0xBE, 0xB0, 0x00, 0x00, 0x00, 0x02,								//- 13 cmp dword ptr[esi + 000000B0], 02
					0x75, 0x0B,																//- 14 jne Client.exe + C9E39C
					0x8B, 0x45, 0x0C,														//- 15 mov eax,[ebp + 0C]
					0x5E,																	//- 16 pop esi
					0x83, 0x00, 0x03,														//- 17 add dword ptr[eax], 03
					0x5D,																	//- 18 pop ebp
					0xC2, 0x08, 0x00,														//- 19 ret 0008
					0x80, 0xBE, 0x83, 0x00, 0x00, 0x00, 0x00,								//- 20 cmp byte ptr[esi + kanan.dtor_list_head + 63], 00
					0x75, 0x26,																//- 21 jne Client.exe + C9E3CB
					0xC7, 0x86, 0xC4, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00,				//- 22 mov[esi + 000000C4], kanan._Init_thread_epoch + 2 -- This influences the text size. Currently set to max stable text size for NPC text, 0x0D (13)
					0xC7, 0x86, 0xC8, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,				//- 23 mov[esi + 000000C8], kanan.kanan::g_pages + B
					0x83, 0x46, 0x10, 0xFC,													//- 24 add dword ptr[esi + 10], -04
					0xC6, 0x86, 0x85, 0x00, 0x00, 0x00, 0x01,								//- 25 mov byte ptr[esi + kanan.dtor_list_head + 65], 01
					0xC6, 0x86, 0xA8, 0x00, 0x00, 0x00, 0x00,								//- 26 mov byte ptr[esi + 000000A8], 00
					0x5E,																	//- 27 pop esi
					0x5D,																	//- 28 pop ebp
					0xC2, 0x08, 0x00														//- 29 ret 0008
				};


			}
			else {
				log("[TTF Font Size] Failed to find TTFFontSize NPC Fix addresses.");
			}

		}
		else {
			log("[TTF Font Size] Failed to find TTFFontSize address.");
		}
	}


	//Apply NPC function redirection. Passing in true will enable the patch and allocate memory to store the patch if it hasn't been done alresady. Passing in False will disable and de-allocate the patch. Returns false if we are unable to allocate memory for any reason. 

	bool TTFFontSize::applyNPCFix(bool npcFixEnable) {
		//Enabling NPC Fix. Make copy of the function with max stable text size set for when an NPC text box is drawn. 
		if (npcFixEnable == true) {
			//First check to see if we already allocated and constructed our redircted NPC function.
			if (code_address == NULL) {
				// Allocate memory to store our copied function
				code_address = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				if (code_address == NULL) {
					log("[TTF Font Size] Failed to allocate memory for TTFFontSize NPC Fix.");
					return false;
				}
				else {
					log("[TTF Font Size] Successfully allocated memory for TTFFontSize NPC Fix %p", (uintptr_t)code_address);

					// Create patch to replace calls to text size function made by NPC text draws. We calculate relative call address and create our patch.
					//Find relative offset amount between our allocated code and the call we need to make
					uintptr_t callToBytesNPC1 = (uintptr_t)code_address - (m_patchNPC1.address + 5);
					//create array of bytes containing our relative address offset
					unsigned char callToArrayNPC1[sizeof(void*)]; // 
					memcpy(callToArrayNPC1, &callToBytesNPC1, sizeof(void*));
					//patch call code_address
					m_patchNPC1.bytes = { 0xE8, callToArrayNPC1[0], callToArrayNPC1[1], callToArrayNPC1[2], callToArrayNPC1[3] };

					// Calculate relative location of the call to another function in the middle of our function copy (Asm code Line 10) so we can update the call to point to the correct address in memory should it be needed.
					uintptr_t midCallToBytes = (m_patchNPCCall.address - 0x23) - (uintptr_t)code_address;
					unsigned char midCallTextArray[sizeof(void*)];
					memcpy(midCallTextArray, &midCallToBytes, sizeof(void*));
					//Patch relative call to the function in the middle of our copied function.
					code[31] = midCallTextArray[0];
					code[32] = midCallTextArray[1];
					code[33] = midCallTextArray[2];
					code[34] = midCallTextArray[3];


					//Patch mov reference in original function  (Asm code Line 11) - this changes from client build to build.
					uintptr_t movRefToBytes = m_patch.address - 0x31;
					unsigned char movRefArray[5];
					memcpy(movRefArray, &movRefToBytes, 5);
					code[26] = movRefArray[0];
					code[27] = movRefArray[1];
					code[28] = movRefArray[2];
					code[29] = movRefArray[3];

					// Copy our modified function that NPC text calls will use to memory and apply the patch to have NPC text call this copied function.
					memcpy(code_address, code.data(), code.size());

					patch(m_patchNPC1);
					return true;
				}
			}
			//Memory is already allocated, just apply the patch
			else {
				patch(m_patchNPC1);
				return true;
			}

		}
		//Disable NPC Fix. Check to see if memory for NPC is allocated. If it is, deallocate the memory. 
		else {
			if (code_address != NULL) {
				log("[TTF Font Size] Disabling TTFont Size NPCFix...");
				undoPatch(m_patchNPC1);
				VirtualFree(code_address, 0, MEM_RELEASE);
				code_address = NULL;
				return true;
			}
			//NPC Text fix wasn't in use. Nothing to do.
			else {
				return true;
			}
		}
	}


	void TTFFontSize::onPatchUI() {
		if (m_patch.address == 0) {
			return;
		}

		ImGui::Text("TTF Original Font Size: %d", m_originalByte);
		if (ImGui::InputInt("TTF Font Size", &m_choice)) {
			if (m_choice < 0) {
				m_choice = 0;
			}
			else if (m_choice > 255) {
				m_choice = 255;
			}
			apply();
		}
	}

	void TTFFontSize::onConfigLoad(const Config& cfg) {
		m_choice = cfg.get<int>("TTFFontSize.Choice").value_or(0);

		if (m_choice != 0) {
			apply();
		}
		else {
			m_choice = m_originalByte;
		}
	}

	void TTFFontSize::onConfigSave(Config& cfg) {
		cfg.set<int>("TTFFontSize.Choice", m_choice);
	}


	void TTFFontSize::apply() {
		if (m_patch.address == 0) {
			return;
		}
		// Set back to default size if 0 is selected
		if (m_choice == 0) {
			m_choice = m_originalByte;
			short choice = m_choice;
			m_patch.bytes = { choice };
			patch(m_patch);
			applyNPCFix(false);
		}
		// If our size is less than 14 we don't need to patch NPC functions
		else if (m_choice < 14) {
			log("[TTF Font Size] Applying TTFFontSize...");
			short choice = m_choice;
			m_patch.bytes = { choice };
			patch(m_patch);
			applyNPCFix(false);
		}
		// Selected font size is greater than 13 - In order to speak with NPCs we must redirect them to a copy of the function with a font size lower than 14 to prevent a crash
		else if (m_choice >= 14) {
			//First check if we were able to find the addresses needed for the NPC fix
			if (foundNPCFix == true) {
				log("[TTF Font Size] Applying TTFFontSize with NPC Fix...");
				short choice = m_choice;
				m_patch.bytes = { choice };
				patch(m_patch);
				//attempt NPC fix, if the memory to hold the patch fails to allocate fall back to setting the text size to max stable (13)
				if (applyNPCFix(true) == false) {
					log("[TTF Font Size] Applying TTFFontSize - NPC fix failed, setting size to 13...");
					short choice = 13;
					m_patch.bytes = { choice };
					patch(m_patch);
				}

				;
			}
			//Addresses necesarry to make the NPC fix patch were not found. Set text size to max stable size (13) instead
			else {
				log("[TTF Font Size] Applying TTFFontSize - NPC fix addresses were not found, setting size to 13...");
				short choice = 13;
				m_patch.bytes = { choice };
				patch(m_patch);

			}

		}
	}
}