#pragma once
#include "Core/interfaces.h"
#include "Core/Settings.h"
#include "Core/utils.h"
#include "Game/gamestates.h"
#include "Game/EntityData.h"
#include <ctime>
#include <cstdlib>
#include <array>
#include <map>
#include <memory>
#include "SnapshotApparatus.h"
//#include "Core/Interfaces.h"

SnapshotApparatus::SnapshotApparatus() {
	this->p1_ptr = g_interfaces.player1.GetData();
	this->p2_ptr = g_interfaces.player2.GetData();
	this->snapshot_count = 0;
		char* base_addr = GetBbcfBaseAdress();
		void* addr = base_addr + 0x65bd08;
		SteamPeer2PeerBackend* bckend = *(SteamPeer2PeerBackend**)addr;
		static_DAT_of_PTR_on_load_4* DAT_on_load_4_addr = (static_DAT_of_PTR_on_load_4*)(base_addr + 0x612718);
		SnapshotManager* snap_manager = 0;
		if (DAT_on_load_4_addr) {
			snap_manager = DAT_on_load_4_addr->ptr_snapshot_manager_mine;
		}
		void* initialize_ggpo_callbacks_struct_ptr = base_addr + 0x383dd0;
		typedef void (*initialize_ggpo_callbacks_struct_decl)(GGPOSessionCallbacks*);
		initialize_ggpo_callbacks_struct_decl initialize_ggpo_callbacks_struct = reinterpret_cast<initialize_ggpo_callbacks_struct_decl>(initialize_ggpo_callbacks_struct_ptr);
		this->callbacks_ptr = new GGPOSessionCallbacks;
		initialize_ggpo_callbacks_struct(this->callbacks_ptr);

		void* DAT_01292888 = base_addr + 0x612888;
		if (*(int*)DAT_01292888 == 0) {

			///PRELUDE
			auto mem_offset_1 = 0xe5701;//2 bytes
			auto mem_offset_2 = 0xe577d;//2 bytes
			auto mem_offset_3 = 0xe5a9c;//6 bytes
			char nops[] = "\x90\x90\x90\x90\x90\x90\x90\x90";
			char oldmem_1[2];
			char oldmem_2[2];
			char oldmem_3[6];
			void* ptr_oldmem_1 = base_addr + mem_offset_1;
			void* ptr_oldmem_2 = base_addr + mem_offset_2;
			void* ptr_oldmem_3 = base_addr + mem_offset_3;
			memcpy(oldmem_1, ptr_oldmem_1, 2);
			WriteToProtectedMemory((uintptr_t)ptr_oldmem_1, nops, 2);
			memcpy(oldmem_2, ptr_oldmem_2, 2);
			WriteToProtectedMemory((uintptr_t)ptr_oldmem_2, nops, 2);
			memcpy(oldmem_3, ptr_oldmem_3, 6);
			WriteToProtectedMemory((uintptr_t)ptr_oldmem_3, nops, 6);
			//memcpy(ptr_oldmem_1, nops, 2);
			///PRELUDE_END

			//this is copied from "maybe_network_stuff_init
			void** SCENE_CBattle_static_ptr = (void**)(base_addr + 0x8929B4);
			void* maybe_network_stuff_init_ptr = base_addr + 0xe56f0;
			typedef void (*maybe_network_stuff_init_decl)(void*);
			maybe_network_stuff_init_decl maybe_network_stuff_init = reinterpret_cast<maybe_network_stuff_init_decl>(maybe_network_stuff_init_ptr);
			maybe_network_stuff_init(*SCENE_CBattle_static_ptr);
			WriteToProtectedMemory((uintptr_t)ptr_oldmem_1, oldmem_1, 2);
			WriteToProtectedMemory((uintptr_t)ptr_oldmem_2, oldmem_2, 2);
			WriteToProtectedMemory((uintptr_t)ptr_oldmem_3, oldmem_3, 6);
			//memcpy(ptr_oldmem_1, oldmem_1, 2);
		}
	}

bool SnapshotApparatus::save_snapshot(Snapshot** pbuf_mine)
{/* leave pbuf_mine as zero to not involve out own buffers and just the "built in" snapshot buffer of 10*/
	char* base_addr = GetBbcfBaseAdress();
	//memcpy(&temp_savestate_loc, savedstate_mine[savegame_index], 4);
	//callbacks_ptr->load_game_state((unsigned char*)temp_savestate_loc);
	/// COPIES_FROM_OUR_BUFFER_TO_FIRST_ROLLBACK_SLOT
	static_DAT_of_PTR_on_load_4* DAT_on_load_4_addr = (static_DAT_of_PTR_on_load_4*)(base_addr + 0x612718);
	SnapshotManager* snap_manager = 0;
	if (DAT_on_load_4_addr) {
		snap_manager = DAT_on_load_4_addr->ptr_snapshot_manager_mine;
	}
	else {
		return false;
	}

	unsigned char** pbuf = &snap_manager->_saved_states_related_struct[this->snapshot_count%10]._ptr_buf_saved_frame;
	
	//unsigned char** pbuf = (unsigned char**)&snap_manager->_saved_states_related_struct[0]._ptr_buf_saved_frame;
	int checksum = 0;
	int counter_of_some_sort = 1;
	int sizeofstate = 0xa10000;
	
	this->callbacks_ptr->free_buffer((unsigned char*)*pbuf);
	this->callbacks_ptr->save_game_state((unsigned char**)pbuf,
		&sizeofstate, //&counter_of_some_sort, I still dont know for sure if this is supposed to be the counter or the sie 
		&checksum); //I assume this is supposed to be checksum but idk
	this->snapshot_count += 1;
	if (pbuf_mine != 0) {
		memcpy(*pbuf_mine, *pbuf, 0xa10000);
	}

	return true;
}

bool SnapshotApparatus::load_snapshot(Snapshot* buf)
{
	/* leave buf as zero to not involve our own buffers and just the "built in" snapshot buffer of 10*/
	char* base_addr = GetBbcfBaseAdress();
	
	//memcpy(&temp_savestate_loc, savedstate_mine[savegame_index], 4);
	//callbacks_ptr->load_game_state((unsigned char*)temp_savestate_loc);
	/// COPIES_FROM_OUR_BUFFER_TO_FIRST_ROLLBACK_SLOT
	static_DAT_of_PTR_on_load_4* DAT_on_load_4_addr = (static_DAT_of_PTR_on_load_4*)(base_addr + 0x612718);
	SnapshotManager* snap_manager = 0;
	if (DAT_on_load_4_addr) {
		snap_manager = DAT_on_load_4_addr->ptr_snapshot_manager_mine;
	}
	else {
		return false;
	}
	unsigned char* dest_buf = (unsigned char*)snap_manager->_saved_states_related_struct[(snapshot_count-1)%10]._ptr_buf_saved_frame;
	if (buf != 0) {
		memcpy(dest_buf, buf, 0xA10000);
	}
	
	/// COPIES_FROM_OUR_BUFFER_TO_FIRST_ROLLBACK_SLOT_END
	///PRELUDE
	auto mem_offset_1 = 0x383f63;//3 bytes
	void* ptr_oldmem_load = base_addr + mem_offset_1;
	char nops[] = "\x90\x90\x90\x90\x90\x90\x90\x90";
	char jmp_short_23[] = "\xEB\x23";
	char oldmem_load[3];
	memcpy(oldmem_load, ptr_oldmem_load, 3);
	WriteToProtectedMemory((uintptr_t)ptr_oldmem_load, jmp_short_23, 2);
	WriteToProtectedMemory((uintptr_t)ptr_oldmem_load + 2, nops, 1);
	/// PRELUDE_END



	

	//unsigned char* buf = (unsigned char*)snap_manager->_saved_states_related_struct[(savegame_count % 10) - 1]._ptr_buf_saved_frame;
	this->callbacks_ptr->load_game_state(dest_buf);

	///CLEANUP
	WriteToProtectedMemory((uintptr_t)ptr_oldmem_load, oldmem_load, 3);
	///CLEANUP_END
	return true;
}

bool SnapshotApparatus::check_if_valid(CharData* p1, CharData* p2)
{
	if (this->p1_ptr == p1 && this->p2_ptr == p2) {
		return true;
	}
	return false;
}
