#pragma once
#include "Game/GhidraDefs.h"
#include "Core/Interfaces.h"
#define SNAPSHOT_PREALLOC_SIZE  10

class Snapshot {
public:
	char padding[0xa10000];
	/*char padding[0x623e0C];
	CharData p1_chardata_training_;
	CharData p2_chardata_training_;
	char padding_after[0x3a2f00];*/
	//btw chardata resides on buf + 0x623E10 for P1 and for p2 buf + 0x623E10 + 0x24978 for training mode THIS IS NOT TRUE, SEEMS TO CHANGE
	
};

static Snapshot snapshot_replay_pre_allocated[SNAPSHOT_PREALLOC_SIZE]; //keeping this with only one element for now while its not used for any implementation to save space



class SnapshotApparatus {


public:
	unsigned int snapshot_count;
	CharData* p1_ptr;//p1 CharData*
	CharData* p2_ptr; //p2 CharData*
	//p1 and p2 ptrs are used for now to determine when I need to remake the snapshot
	GGPOSessionCallbacks*  callbacks_ptr;
	SnapshotApparatus();

	
bool save_snapshot(Snapshot** pbuf);
bool save_snapshot_prealloc();
bool load_snapshot(Snapshot* buf);
bool load_snapshot_prealloc(int index);
bool check_if_valid(CharData* p1, CharData* p2);
void clear_count();
int get_nearest_prealloc_frame(int current_frame, std::map<int, Snapshot*> frame_snap_map);
};