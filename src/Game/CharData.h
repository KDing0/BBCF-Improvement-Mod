#pragma once
#include <cstdint>
// Generated using ReClass 2015


class CharData
{
public:
	class OBJ_CCharBase* objCharbase; //0x0000
	int32_t frame_count_minus_1; //thanks to kding0
	int32_t hitstop; //thanks to kding0
	char pad_0008[4]; //0x0004
	int32_t unknownStatus1; //0x0010
	char pad_0014[4]; //0x0014
	int32_t stateChangedCount; //0x0018
	char pad_001C[24]; //0x001C
	int32_t charIndex; //0x0034
	char pad_0038[20]; //0x0038
	class JonbEntry* pJonbEntryBegin; //0x004C
	char pad_0050[68]; //0x0050
	uint32_t hurtboxCount; //0x0094
	uint32_t hitboxCount; //0x0098
	char pad_009C[60]; //0x009C
	char* current_sprite_img; //0x00D8
	char pad_00DC[120]; //0x00DC
	char unknown_status2; //0x0154  compared if 2 or not? I think 2 might mean its an entity that is anchored to the character that spawned it 
	char pad_0158[8]; ///0x0158

	int32_t actionTime; //0x0160
	int32_t actionTime2; //0x0164
	char pad_0168[8]; //0x0168
	int32_t actionTimeNoHitstop; //0x0170
	char pad_0174[8]; //0x0174
	int32_t EsBuff; //0x17C
	char pad_0180[32]; //0x0180
	int32_t Drive10; //0x1A0 //azrael fireball /Plat item type//bang seal
	int32_t Drive20; //0x1A4 //bang seal
	int32_t Drive30; //0x1A8 ////bang seal
	int32_t lambda_nu_drive_hitcount; //0x01AC//bang seal
	char pad_01B0[16]; //0x01B0
	int32_t overdriveTimeleft; //0x01C0
	int32_t overdriveTimerStartedAt; //0x01C4
	char pad_01C8[12]; //0x01C8
	int32_t moveSuperflashTime; //0x01D4
	char pad_01D8[4]; //0x01D8
	int32_t superflashTime; //0x01DC
	int32_t isDoingDistortion; //0x01E0



	char pad_01E4[4]; //0x01E4
	class CharData* ownerEntity; //0x01E8

	char pad_01EC[4];
	class CharData* enemyChar; //0x01F0 dont know exactly what it is yet, just that it is checked and I need to save it 
	char pad_01F4[54]; //0x01F4 
	//char pad_01EC[64];
	class CharData* last_child_entity_spawned; // 0x022C 
	class CharData* extra_child_entities[7]; // 0x0230 should hold up to 7(?) idk extra child entities
	class CharData* main_child_entity; // 0x024C holds the main child entity, varies by character but its the puppets, arakune curse circle over enemy, etc. Some characters dont have it
	char pad_0250[20]; // 0x0250



	int32_t facingLeft; //0x0264 is it not facing right?
	int32_t position_x; //0x0268
	int32_t position_y; //0x026C
	char pad_0270[4]; //0x0270
	int32_t offsetX_1; //0x0274
	char pad_0278[4]; //0x0278
	int32_t rotationDegrees; //0x027C
	char pad_0280[4]; //0x0280
	int32_t scaleX; //0x0284
	int32_t scaleY; //0x0288
	char pad_028C[80]; //0x028C
	int32_t position_x_dupe; //0x02DC
	int32_t position_y_dupe; //0x02E0
	int32_t position_x3;//0x02E4
	char pad_02E4[12]; //0x02E8
	int32_t offsetX_2; //0x02F4
	char pad_02F8[4]; //0x02F8
	int32_t offsetY_2; //0x02FC
	char pad_0300[28]; //0x0300
	int32_t BoundingX; //0x0316
	int32_t BoundingY; //0x0320
	int32_t BoundingFixX; //0x0324
	int32_t BoundingFixY; //0x0328
	int32_t BoundingAddY; //0x032C do not use not sure what it is, but it's Unknown23087?
	int32_t BoundingAddX; //0x0330
	int32_t stageEdgeTouchTimer; //0x0334
	char pad_0338[336]; //0x0338
	int32_t typeOfAttack; //0x0488 1=normal, 2=special, 3=DD/EA, 5=AH
	int32_t attackLevel; //0x048C
	int32_t moveDamage; //0x0490; raw damage
	char pad_0494[44]; //0x0494
	int32_t moveSpecialBlockstun; //0x04C0
	char pad_04C4[4]; //0x04C4
	int32_t moveGuardCrushTime; //0x04C8
	char pad_04CC[8]; //0x04CC
	int32_t vectorcheckX_1; //0x04D0
	int32_t vectorcheckY_1; //0x04D4
	int32_t vectorcheckX_2; //0x04D8
	int32_t vectorcheckY_2; //0x04DC
	int32_t ThrowRange; //0x04E4
	char pad_04E8[28]; //0x04E8
	char performedThrowName[32]; //0x0504
	char pad_0524[320]; //0x0524
	int32_t moveAirPushbackX; //0x0664
	int32_t moveAirPushbackY; //0x0668
	char pad_066C[20]; //0x066C
	int32_t moveHitstunOverwrite; //0x0680
	int32_t moveUntechOverwrite; //0x0684
	char pad_0688[12]; //0x0688
	int32_t movePushbackX; //0x0694
	char pad_0698[12]; //0x0698
	int32_t moveP1Overwrite; //0x06A4
	int32_t moveP2Overwrite; //0x06A8
	char pad_06AC[40]; //0x06AC
	int32_t moveCounterHitAirPushbackY; //0x06D4
	char pad_06D8[760]; //0x06D8
	int32_t previousHP; //0x09D0
	int32_t currentHP; //0x09D4
	int32_t maxHP; //0x09D8
	char pad_09DC[5656]; //0x09DC
	char lastAction[32]; //0x1FF4
	//char pad_2008[12]; //0x2008 
	char currentActionNoNeutral[32]; //0x2014
	char pad_2034[60]; //0x2034 
	char currentAction[32]; //0x2070
	char pad_2090[232]; //0x2090
	int32_t TagerMagnetism; //0x2178
	char pad_217C[204];  //0x217C
	char char_abbr[4]; //0x2248
	char pad_224C[40]; //0x224C
	int32_t blockstun; //0x2274
	char pad_2278[172]; //0x2278
	char id1[8]; //0x2324
	char pad_232C[11844]; //0x232C
	int32_t hitstun; //0x5170
	char pad_5174[160]; //0x5174
	int32_t defaultProration[6]; //0x5214-0x5227, for Lv0-Lv5
	char pad_5228[1348]; //0x5228
	//possible identifier: bl06 //0x52AC //4
	int32_t hitCount; //0x5770
	int32_t hitCount2; //0x5774
	int32_t timeAfterTechIsPerformed; //0x5778
	int32_t timeAfterLatestHit; //0x577C
	int32_t comboDamage; //0x5780
	int32_t comboDamage2; //0x5784
	int32_t lastcomboDamage; //0x5788
	int32_t comboProration; //0x578C
	int32_t starterRating; //0x5790
	int32_t comboTime; //0x5794
	int32_t singleHitDamage; //0x5798
	char pad_579C[4]; //0x579C
	int32_t realTimeComboTime; //0x57A0; THIS IS NOT COMBO TIME!
	int32_t heatGeneratedForCombo; //0x57A4; this does not take heatGainCooldown into account
	char pad_57A4[56]; //0x57A4
	char hitByWhichAction[32]; //0x57E0
	char pad_5800[28]; //0x5800
	char sameMoveProrationStack[32]; //0x581C; idk how long the stack is but it's pretty long lmao
	char pad_583C[664]; //0x583C
	int32_t heatMeter; //0x5AD4
	char pad_5AD8[4]; //0x5AD8
	int32_t heatGainCooldown; //0x5ADC
	char pad_5AE0[4]; //0x5AE0
	int32_t overdriveMeter; //0x5AE4
	char pad_5AE8[16]; //0x5AE8
	int32_t overdriveMaxtime; //0x5AF8
	char pad_5AFC[8]; //0x5AFC
	int32_t barrier; //0x5B04
	char pad_5B08[102072]; //0x5B08
	int32_t Drive12; //0x1e9c0 //ReliusDollState /SusanUnlocks //izaFloatingTime //nineMagicSlots //izayoiStance
	int32_t Drive11; //0x1e9c4 //BulletHeatLevel //PlatNextItemType //SusanDrivePosition //nineMagicSlotsBackup //KokoGravState
	int32_t UnknownDriveVal0;//0x1e9c8 //izaRibcage
	int32_t UnknownDriveVal1;//0x1e9cc //IzaBitStance
	int32_t CelicaRegen;//0x1e9d0
	int32_t UnknownDriveVal3;//0x1e9d4
	int32_t UnknownDriveVal4;//0x1e9d8
	int32_t LitchiStaffState; //0x1e9dd
	char pad_1e9e1[5927]; //0x1e9e1
	int32_t Drive0; //0x20108 //Rachel wind meter; Az top weakspot //PlatMaxItem
	int32_t Drive1; //0x2010C //PlatCurItemVal //ReliusDollMeter //ValkWolfMeter //JubeiSuperBuff //nineSpell //KokoGravCount //izayoistocks//bangNails
	char pad_20110[12]; //0x20110
	int32_t Drive1_type; //0x2011C //ReliusDollInCooldown //ValkWolfMode 160 == cooldown
	char pad_20120[12]; //0x20120
	int32_t DriveX; //0x2012C //Az bot weakspot
	int32_t Drive2; //0x20130 //jubei mark timer //nineSpellBackup //izayoiSuperModeTimer
	char pad_20134[32]; //0x20134
	int32_t Drive3; //0x20154 //Iza bit cooldown //kokoTrapAvailable
	char pad_20158[4972]; //0x20158
}; //Size: 0x214C4

/*
class CharData
{
public:
		class OBJ_CCharBase* objCharbase; //0x0000
	int32_t frame_count_minus_1; //thanks to kding0
	int32_t hitstop; //thanks to kding0
	char pad_0008[4]; //0x0004
	int32_t unknownStatus1; //0x0010
	char pad_0014[4]; //0x0014
	int32_t stateChangedCount; //0x0018
	char pad_001C[24]; //0x001C
	int32_t charIndex; //0x0034
	char pad_0038[20]; //0x0038
	class JonbEntry* pJonbEntryBegin; //0x004C
	char pad_0050[68]; //0x0050
	uint32_t hurtboxCount; //0x0094
	uint32_t hitboxCount; //0x0098
	char pad_009C[60]; //0x009C
	char* current_sprite_img; //0x00D8
	char pad_00DC[120]; //0x00DC
	char unknown_status2; //0x0154  compared if 2 or not? I think 2 might mean its an entity that is anchored to the character that spawned it 
	char pad_0158[8]; ///0x0158

	int32_t actionTime; //0x0160
	int32_t actionTime2; //0x0164
	char pad_0168[8]; //0x0168
	int32_t actionTimeNoHitstop; //0x0170
	char pad_0174[8]; //0x0174
	int32_t EsBuff; //0x17C
	char pad_0180[32]; //0x0180
	int32_t Drive10; //0x1A0 //azrael fireball /Plat item type//bang seal
	int32_t Drive20; //0x1A4 //bang seal
	int32_t Drive30; //0x1A8 ////bang seal
	int32_t lambda_nu_drive_hitcount; //0x01AC//bang seal
	char pad_01B0[16]; //0x01B0
	int32_t overdriveTimeleft; //0x01C0
	int32_t overdriveTimerStartedAt; //0x01C4
	char pad_01C8[12]; //0x01C8
	int32_t moveSuperflashTime; //0x01D4
	char pad_01D8[4]; //0x01D8
	int32_t superflashTime; //0x01DC
	int32_t isDoingDistortion; //0x01E0



	char pad_01E4[4]; //0x01E4
	class CharData* ownerEntity; //0x01E8

	char pad_01EC[4];
	class CharData* enemyChar; //0x01F0 dont know exactly what it is yet, just that it is checked and I need to save it 
	char pad_01F4[54]; //0x01F4 
	//char pad_01EC[64];
	class CharData* last_child_entity_spawned; // 0x022C 
	class CharData* extra_child_entities[7]; // 0x0230 should hold up to 7(?) idk extra child entities
	class CharData* main_child_entity; // 0x024C holds the main child entity, varies by character but its the puppets, arakune curse circle over enemy, etc. Some characters dont have it
	char pad_0250[20]; // 0x0250



	int32_t facingLeft; //0x0264 is it not facing right?
	int32_t position_x; //0x0268
	int32_t position_y; //0x026C
	char pad_0270[4]; //0x0270
	int32_t offsetX_1; //0x0274
	char pad_0278[4]; //0x0278
	int32_t rotationDegrees; //0x027C
	char pad_0280[4]; //0x0280
	int32_t scaleX; //0x0284
	int32_t scaleY; //0x0288
	char pad_028C[80]; //0x028C
	int32_t position_x_dupe; //0x02DC
	int32_t position_y_dupe; //0x02E0
	int32_t position_x3;//0x02E4
	char pad_02E4[12]; //0x02E8
	int32_t offsetX_2; //0x02F4
	char pad_02F8[4]; //0x02F8
	int32_t offsetY_2; //0x02FC
	char pad_0300[28]; //0x0300
	int32_t BoundingX; //0x0316
	int32_t BoundingY; //0x0320
	int32_t BoundingFixX; //0x0324
	int32_t BoundingFixY; //0x0328
	int32_t BoundingAddY; //0x032C do not use not sure what it is, but it's Unknown23087?
	int32_t BoundingAddX; //0x0330
	int32_t stageEdgeTouchTimer; //0x0334
	char pad_0338[336]; //0x0338
	int32_t typeOfAttack; //0x0488 1=normal, 2=special, 3=DD/EA, 5=AH
	int32_t attackLevel; //0x048C
	int32_t moveDamage; //0x0490; raw damage
	char pad_0494[44]; //0x0494
	int32_t moveSpecialBlockstun; //0x04C0
	char pad_04C4[4]; //0x04C4
	int32_t moveGuardCrushTime; //0x04C8
	char pad_04CC[8]; //0x04CC
	int32_t vectorcheckX_1; //0x04D0
	int32_t vectorcheckY_1; //0x04D4
	int32_t vectorcheckX_2; //0x04D8
	int32_t vectorcheckY_2; //0x04DC
	int32_t ThrowRange; //0x04E4
	char pad_04E8[28]; //0x04E8
	char performedThrowName[32]; //0x0504
	char pad_0524[320]; //0x0524
	int32_t moveAirPushbackX; //0x0664
	int32_t moveAirPushbackY; //0x0668
	char pad_066C[20]; //0x066C
	int32_t moveHitstunOverwrite; //0x0680
	int32_t moveUntechOverwrite; //0x0684
	char pad_0688[12]; //0x0688
	int32_t movePushbackX; //0x0694
	char pad_0698[12]; //0x0698
	int32_t moveP1Overwrite; //0x06A4
	int32_t moveP2Overwrite; //0x06A8
	char pad_06AC[40]; //0x06AC
	int32_t moveCounterHitAirPushbackY; //0x06D4
	char pad_06D8[760]; //0x06D8
	int32_t previousHP; //0x09D0
	int32_t currentHP; //0x09D4
	int32_t maxHP; //0x09D8
	char pad_09DC[5656]; //0x09DC
	char lastAction[32]; //0x1FF4
	//char pad_2008[12]; //0x2008 
	char currentActionNoNeutral[32]; //0x2014
	char pad_2034[60]; //0x2034 
	char currentAction[32]; //0x2070
	char pad_2090[232]; //0x2090
	int32_t TagerMagnetism; //0x2178
	char pad_217C[204];  //0x217C
	char char_abbr[4]; //0x2248
	char pad_224C[40]; //0x224C
	int32_t blockstun; //0x2274
	char pad_2278[172]; //0x2278
	char id1[8]; //0x2324
	char pad_232C[11844]; //0x232C
	int32_t hitstun; //0x5170
	char pad_5174[160]; //0x5174
	int32_t defaultProration[6]; //0x5214-0x5227, for Lv0-Lv5
	char pad_5228[1348]; //0x5228
	//possible identifier: bl06 //0x52AC //4
	int32_t hitCount; //0x5770
	int32_t hitCount2; //0x5774
	int32_t timeAfterTechIsPerformed; //0x5778
	int32_t timeAfterLatestHit; //0x577C
	int32_t comboDamage; //0x5780
	int32_t comboDamage2; //0x5784
	int32_t lastcomboDamage; //0x5788
	int32_t comboProration; //0x578C
	int32_t starterRating; //0x5790
	int32_t comboTime; //0x5794
	int32_t singleHitDamage; //0x5798
	char pad_579C[4]; //0x579C
	int32_t realTimeComboTime; //0x57A0; THIS IS NOT COMBO TIME!
	int32_t heatGeneratedForCombo; //0x57A4; this does not take heatGainCooldown into account
	char pad_57A4[56]; //0x57A4
	char hitByWhichAction[32]; //0x57E0
	char pad_5800[28]; //0x5800
	char sameMoveProrationStack[32]; //0x581C; idk how long the stack is but it's pretty long lmao
	char pad_583C[664]; //0x583C
	int32_t heatMeter; //0x5AD4
	char pad_5AD8[4]; //0x5AD8
	int32_t heatGainCooldown; //0x5ADC
	char pad_5AE0[4]; //0x5AE0
	int32_t overdriveMeter; //0x5AE4
	char pad_5AE8[16]; //0x5AE8
	int32_t overdriveMaxtime; //0x5AF8
	char pad_5AFC[8]; //0x5AFC
	int32_t barrier; //0x5B04
	char pad_5B08[102072]; //0x5B08
	int32_t Drive12; //0x1e9c0 //ReliusDollState /SusanUnlocks //izaFloatingTime //nineMagicSlots //izayoiStance
	int32_t Drive11; //0x1e9c4 //BulletHeatLevel //PlatNextItemType //SusanDrivePosition //nineMagicSlotsBackup //KokoGravState
	int32_t UnknownDriveVal0;//0x1e9c8 //izaRibcage
	int32_t UnknownDriveVal1;//0x1e9cc //IzaBitStance
	int32_t CelicaRegen;//0x1e9d0
	int32_t UnknownDriveVal3;//0x1e9d4
	int32_t UnknownDriveVal4;//0x1e9d8
	int32_t LitchiStaffState; //0x1e9dd
	char pad_1e9e1[5927]; //0x1e9e1
	int32_t Drive0; //0x20108 //Rachel wind meter; Az top weakspot //PlatMaxItem
	int32_t Drive1; //0x2010C //PlatCurItemVal //ReliusDollMeter //ValkWolfMeter //JubeiSuperBuff //nineSpell //KokoGravCount //izayoistocks//bangNails
	char pad_20110[12]; //0x20110
	int32_t Drive1_type; //0x2011C //ReliusDollInCooldown //ValkWolfMode 160 == cooldown
	char pad_20120[12]; //0x20120
	int32_t DriveX; //0x2012C //Az bot weakspot
	int32_t Drive2; //0x20130 //jubei mark timer //nineSpellBackup //izayoiSuperModeTimer
	char pad_20134[32]; //0x20134
	int32_t Drive3; //0x20154 //Iza bit cooldown //kokoTrapAvailable
	char pad_20158[4972]; //0x20158
}; //Size: 0x214C4 */
//211C