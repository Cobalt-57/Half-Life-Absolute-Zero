

// NOTE TO SELF:
// for release to compile right, something in the HASH list needs to
// have the CLIENTSENDOFF_BROADCAST constants, orrrrrrr else.
// That is, if something is serverside only, it should not be in the hash
// list at all.  No exceptions.

// something that is _CLIENTONLY can use the _AC_ cvar tags for saving.
// everything else uses _A_. I think that's how it works.

// for something shared, it mainly uses server preferences where required
// and uses _CLIENTSENDOFF_BROADCAST for most areas below. Must also be
// part of the HASH list as said above.




#define strobeDurationMin_ID 0
#define strobeDurationMax_ID 1
#define strobeRadiusMin_ID 2
#define strobeRadiusMax_ID 3
#define strobeSpawnDistHori_ID 4
#define strobeSpawnDistVertMin_ID 5
#define strobeSpawnDistVertMax_ID 6
#define strobeMultiColor_ID 7
#define raveLaserEnabled_ID 8
#define raveLaserSpawnFreq_ID 9
#define raveLaserLength_ID 10
#define raveLaserSpawnDistHoriMin_ID 11
#define raveLaserSpawnDistHoriMax_ID 12
#define raveLaserSpawnDistVertMin_ID 13
#define raveLaserSpawnDistVertMax_ID 14
#define raveLaserBrightnessMin_ID 15
#define raveLaserBrightnessMax_ID 16
#define raveLaserDurationMin_ID 17
#define raveLaserDurationMax_ID 18
#define raveLaserThicknessMin_ID 19
#define raveLaserThicknessMax_ID 20
#define raveLaserNoiseMin_ID 21
#define raveLaserNoiseMax_ID 22
#define raveLaserFrameRateMin_ID 23
#define raveLaserFrameRateMax_ID 24
#define raveLaserMultiColor_ID 25
#define cameraPosFixedX_ID 26
#define cameraPosFixedY_ID 27
#define cameraPosFixedZ_ID 28
#define cameraPosOffX_ID 29
#define cameraPosOffY_ID 30
#define cameraPosOffZ_ID 31
#define cameraRotFixedX_ID 32
#define cameraRotFixedY_ID 33
#define cameraRotFixedZ_ID 34
#define cameraRotOffX_ID 35
#define cameraRotOffY_ID 36
#define cameraRotOffZ_ID 37
#define imAllFuckedUp_ID 38
#define thatWasntGrass_ID 39
#define thatWasntPunch_ID 40
#define fogNear_ID 41
#define fogFar_ID 42
#define myCameraSucks_ID 43
#define muteBulletHitSounds_ID 44
#define geigerChannel_ID 45
#define muteTempEntityGroundHitSound_ID 46
#define muteRicochetSound_ID 47
#define mutePlayerWeaponFire_ID 48
#define muteCrowbarSounds_ID 49
#define seeMonsterHealth_ID 50
#define event5011Allowed_ID 51
#define event5021Allowed_ID 52
#define event5031Allowed_ID 53
#define event5002Allowed_ID 54
#define event5004Allowed_ID 55
#define eventsAreFabulous_ID 56
#define glockOldReloadLogic_ID 57
#define testVar_ID 58
#define painArrowColorMode_ID 59
#define painFlashColorMode_ID 60
#define painFlashSuitless_ID 61
#define painFlashDrownMode_ID 62
#define firstPersonIdleDelayMin_ID 63
#define firstPersonIdleDelayMax_ID 64
#define forceDrawBatteryNumber_ID 65
#define canShowWeaponSelectAtDeath_ID 66
#define endlessFlashlightBattery_ID 67
#define preE3UsesFailColors_ID 68
#define E3UsesFailColors_ID 69
#define preE3ShowsDamageIcons_ID 70
#define E3ShowsDamageIcons_ID 71
#define playerCrossbowMode_ID 72
#define tripmineAnimWaitsForFinish_ID 73
#define revolverLaserScope_ID 74
//#define python_zoomfov_ID 75
//#define crossbow_zoomfov_ID 76
//#define canApplyDefaultFOV_ID 77
#define auto_adjust_fov_ID 78
//#define auto_adjust_zoomfov_ID 79
#define cheat_infiniteclip_ID 80
#define cheat_infiniteammo_ID 81
#define cheat_minimumfiredelay_ID 82
#define cheat_minimumfiredelaycustom_ID 83
#define cheat_nogaussrecoil_ID 84
#define gaussRecoilSendsUpInSP_ID 85
#define raveEffectSpawnInterval_ID 86
#define fogTest_ID 87
//#define useAlphaCrosshair_ID 88
#define allowAlphaCrosshairWithoutGuns_ID 89
#define alphaCrosshairBlockedOnFrozen_ID 90
#define mirrorsReflectOnlyNPCs_ID 91
#define mirrorsDoNotReflectPlayer_ID 92
#define sv_germancensorship_ID 93
#define egonEffectsMode_ID 94
#define egonHitCloud_ID 95
//#define enableModPrintouts_ID 96
#define handGrenadePickupYieldsOne_ID 97
//#define skipTFDamageTextures_ID 98
#define weaponSelectSoundPlayOnMousewheel_ID 99
#define timedDamageDeathRemoveMode_ID 100
#define weaponSelectUsesReloadSounds_ID 101
#define glockUseLastBulletAnim_ID 102
#define playerBarnacleVictimViewOffset_ID 103
#define trailTypeTest_ID 104
#define hornetTrail_ID 105
#define hornetTrailSolidColor_ID 106
#define hornetDeathModEasy_ID 107
#define hornetDeathModMedium_ID 108
#define hornetDeathModHard_ID 109
#define hornetZoomPuff_ID 110
#define hornetSpiral_ID 111
#define hornetSpeedMulti_ID 112
#define hornetSpeedDartMulti_ID 113
#define rocketTrailAlphaInterval_ID 114
#define rocketTrailAlphaScale_ID 115
#define rocketSkipIgnite_ID 116
#define gauss_primaryonly_ID 117
#define gauss_reflectdealsdamage_ID 118
#define gauss_chargeanimdelay_ID 119
#define gauss_chargeworkdelay_ID 120
#define gauss_secondarychargetimereq_ID 121
#define gauss_primaryreflects_ID 122
#define gauss_primarypierces_ID 123
#define gauss_secondaryreflects_ID 124
#define gauss_secondarypierces_ID 125
#define gauss_primarypunchthrough_ID 126
#define gauss_secondarypunchthrough_ID 127
#define gauss_betweenattackdelay_ID 128
#define gauss_secondarychargemindelay_ID 129
#define gauss_chargeMaxAmmo_SP_ID 130
#define gauss_chargeMaxAmmo_MP_ID 131
#define gauss_chargeInterval_SP_ID 132
#define gauss_chargeInterval_MP_ID 133
#define painFlashDmgMin_ID 134
#define painFlashDmgExMult_ID 135
#define painFlashCumulativeMinDrowning_ID 136
#define painFlashCumulativeMax_ID 137
#define painFlashDrawOpacityMax_ID 138
#define painArrowDrawOpacityMin_ID 139
#define painArrowDrawOpacityMax_ID 140
#define painFlashFadeMult_ID 141
#define painArrowFadeMult_ID 142
#define painFlashArmorBlock_ID 143
#define painFlashDirTolerance_ID 144
#define painArrowCumulativeAppearMin_ID 145
#define painArrowCumulativeDmgJump_ID 146
#define painFlashPrintouts_ID 147
#define itemFlashCumulativeJump_ID 148
#define itemFlashDrawOpacityMin_ID 149
#define itemFlashDrawOpacityMax_ID 150
#define itemFlashFadeMult_ID 151
#define crossbowReloadSoundDelay_ID 152
#define crossbowFirePlaysReloadSound_ID 153
#define iHaveAscended_ID 154
#define drawViewModel_ID 155
#define drawHUD_ID 156
#define playerBulletHitEffectForceServer_ID 157
#define forceAllowServersideTextureSounds_ID 158
#define playerWeaponSpreadMode_ID 159
#define viewModelPrintouts_ID 160
#define viewModelSyncFixPrintouts_ID 161
#define textureHitSoundPrintouts_ID 162
//#define cl_holster_ID 163
#define playerWeaponTracerMode_ID 164
#define decalTracerExclusivity_ID 165
#define healthcolor_fullRedMin_ID 166
#define healthcolor_brightness_ID 167
#define healthcolor_yellowMark_ID 168
#define cl_drawExtraZeros_ID 169
#define hideDamage_ID 170
#define timedDamage_brightnessMax_ID 171
#define timedDamage_brightnessMin_ID 172
#define timedDamage_brightnessCap_ID 173
#define timedDamage_brightnessFloor_ID 174
#define timedDamage_flashSpeed_ID 175
#define timedDamage_debug_ID 176
#define myRocketsAreBarney_ID 177
#define wpn_glocksilencer_ID 178



//Please.  Update this on changing the count above.
// Or bad things?     They will happen.
//  They.
//           WilL.
//                          haPpen.
//                                                          Thanks.
#define CVAR_CLIENTSENDOFF_COUNT 179







#define DEFAULT_gruntsCanHaveMP5Grenade 0
#define DEFAULT_hud_version 2
#define DEFAULT_hud_batterydraw 1
#define DEFAULT_strobeDurationMin 0.08
#define DEFAULT_strobeDurationMax 0.14
#define DEFAULT_strobeRadiusMin 280
#define DEFAULT_strobeRadiusMax 410
#define DEFAULT_strobeSpawnDistHori 180
#define DEFAULT_strobeSpawnDistVertMin 25
#define DEFAULT_strobeSpawnDistVertMax 75
#define DEFAULT_strobeMultiColor 1
#define DEFAULT_raveLaserEnabled 1
#define DEFAULT_raveLaserSpawnFreq 2.3
#define DEFAULT_raveLaserLength 1800
#define DEFAULT_raveLaserSpawnDistHoriMin 50
#define DEFAULT_raveLaserSpawnDistHoriMax 110
#define DEFAULT_raveLaserSpawnDistVertMin -5
#define DEFAULT_raveLaserSpawnDistVertMax 45
#define DEFAULT_raveLaserBrightnessMin 0.8
#define DEFAULT_raveLaserBrightnessMax 1.0
#define DEFAULT_raveLaserDurationMin 0.06
#define DEFAULT_raveLaserDurationMax 0.12
#define DEFAULT_raveLaserThicknessMin 0.8
#define DEFAULT_raveLaserThicknessMax 2.6
#define DEFAULT_raveLaserNoiseMin 0
#define DEFAULT_raveLaserNoiseMax 0
#define DEFAULT_raveLaserFrameRateMin 10
#define DEFAULT_raveLaserFrameRateMax 10
#define DEFAULT_raveLaserMultiColor 1
#define DEFAULT_cameraPosFixedX -1
#define DEFAULT_cameraPosFixedY -1
#define DEFAULT_cameraPosFixedZ -1
#define DEFAULT_cameraPosOffX 0
#define DEFAULT_cameraPosOffY 0
#define DEFAULT_cameraPosOffZ 260
#define DEFAULT_cameraRotFixedX 90
#define DEFAULT_cameraRotFixedY -1
#define DEFAULT_cameraRotFixedZ -1
#define DEFAULT_cameraRotOffX 0
#define DEFAULT_cameraRotOffY 0
#define DEFAULT_cameraRotOffZ 0
#define DEFAULT_imAllFuckedUp 0
#define DEFAULT_thatWasntGrass 0
#define DEFAULT_thatWasntPunch 0
#define DEFAULT_fogNear 380
#define DEFAULT_fogFar 1500
#define DEFAULT_NPCsTalkMore 0
#define DEFAULT_myCameraSucks 0
#define DEFAULT_shutupstuka 0
#define DEFAULT_chromeEffect 0
#define DEFAULT_hassaultSpinMovement 1
#define DEFAULT_hassaultIdleSpinSound 1
#define DEFAULT_hassaultFireSound 1
#define DEFAULT_muteBulletHitSounds 0
#define DEFAULT_mutePlayerPainSounds 0
#define DEFAULT_hassaultIdleSpinSoundChannel 6
#define DEFAULT_hassaultSpinUpDownSoundChannel 3
#define DEFAULT_hassaultFireSoundChannel 1
#define DEFAULT_geigerChannel 6
#define DEFAULT_hassaultWaitTime 4.0
#define DEFAULT_hassaultSpinupRemainTime 8.5
#define DEFAULT_hassaultResidualAttackTime 0.34
#define DEFAULT_hassaultSpinupStartTime 1.5
#define DEFAULT_hassaultVoicePitchMin 86
#define DEFAULT_hassaultVoicePitchMax 93
#define DEFAULT_hassaultFireSpread 0.060
#define DEFAULT_muteTempEntityGroundHitSound 0
#define DEFAULT_houndeyeAttackMode 1
#define DEFAULT_muteRicochetSound 0
#define DEFAULT_mutePlayerWeaponFire 0
#define DEFAULT_muteCrowbarSounds 0
#define DEFAULT_scientistHealNPC 0
#define DEFAULT_scientistHealNPCDebug 0
#define DEFAULT_bulletholeAlertRange 300
#define DEFAULT_fleshhitmakessound 1
#define DEFAULT_nothingHurts 0
#define DEFAULT_seeMonsterHealth 0
#define DEFAULT_scientistHealNPCFract 0.8
#define DEFAULT_bullsquidRangeDisabled 0
#define DEFAULT_applyLKPPathFixToAll 0
#define DEFAULT_timedDamageAffectsMonsters 1
#define DEFAULT_scientistHealCooldown 44
#define DEFAULT_crazyMonsterPrintouts 0
#define DEFAULT_movementIsCompletePrintout 0
#define DEFAULT_bulletHoleAlertPrintout 0
#define DEFAULT_bulletholeAlertStukaOnly 1
#define DEFAULT_barneyPrintouts 0
#define DEFAULT_monsterSpawnPrintout 0
#define DEFAULT_zombieBulletResistance 1
#define DEFAULT_zombieBulletPushback 1
#define DEFAULT_houndeyePrintout 0
#define DEFAULT_quakeExplosionSound 2
#define DEFAULT_explosionDebrisSoundVolume 0.6
#define DEFAULT_noFlinchOnHard 1
#define DEFAULT_hassaultDrawLKP 0
#define DEFAULT_meleeDrawBloodModeA 9
#define DEFAULT_meleeDrawBloodModeB 9
#define DEFAULT_drawDebugBloodTrace 0
#define DEFAULT_meleeDrawBloodModeBFix 1
#define DEFAULT_meleeDrawBloodModeAOffset 5
#define DEFAULT_meleeDrawBloodModeBOffset 5
#define DEFAULT_wpn_glocksilencer 1
#define DEFAULT_panthereyeHasCloakingAbility 0
#define DEFAULT_hgruntSpeedMulti 1.0
#define DEFAULT_hgruntForceStrafeFireAnim 1
#define DEFAULT_hgruntLockRunAndGunTime 0.5
#define DEFAULT_hgruntHeadshotGore 1
#define DEFAULT_hgruntAllowStrafeFire 1
#define DEFAULT_thoroughHitBoxUpdates 1
#define DEFAULT_hgruntTinyClip 0
#define DEFAULT_hgruntStrafeAlwaysHasAmmo 0
#define DEFAULT_hgruntBrassEjectForwardOffset 18
#define DEFAULT_cl_muzzleflash 1
#define DEFAULT_agrunt_muzzleflash 1
#define DEFAULT_event5011Allowed 1
#define DEFAULT_event5021Allowed 1
#define DEFAULT_event5031Allowed 1
#define DEFAULT_event5002Allowed 1
#define DEFAULT_event5004Allowed 1
#define DEFAULT_eventsAreFabulous 0
#define DEFAULT_glockOldReloadLogic 0
#define DEFAULT_glockOldReloadLogicBarney 0
#define DEFAULT_barneyDroppedGlockAmmoCap 12
#define DEFAULT_drawCollisionBoundsAtDeath 0
#define DEFAULT_drawHitBoundsAtDeath 0
#define DEFAULT_islaveReviveFriendMode 0
#define DEFAULT_islaveReviveFriendChance 0.4
#define DEFAULT_islaveReviveFriendRange 230
#define DEFAULT_islaveReviveSelfMinDelay 14
#define DEFAULT_islaveReviveSelfMaxDelay 32
#define DEFAULT_islaveReviveSelfChance 0
#define DEFAULT_hgruntRunAndGunDistance 390
#define DEFAULT_hgruntPrintout 0
#define DEFAULT_testVar 0
#define DEFAULT_painArrowColorMode 1
#define DEFAULT_painFlashColorMode 2
#define DEFAULT_painFlashSuitless 0
#define DEFAULT_painFlashDrownMode 2
#define DEFAULT_firstPersonIdleDelayMin 5
#define DEFAULT_firstPersonIdleDelayMax 10
#define DEFAULT_forceDrawBatteryNumber 0
#define DEFAULT_offsetgivedistance 160
#define DEFAULT_offsetgivelookvertical 2
#define DEFAULT_canShowWeaponSelectAtDeath 1
#define DEFAULT_endlessFlashlightBattery 1
#define DEFAULT_preE3UsesFailColors 1
#define DEFAULT_E3UsesFailColors 1
#define DEFAULT_preE3ShowsDamageIcons 1
#define DEFAULT_E3ShowsDamageIcons 1
#define DEFAULT_playerCrossbowMode 1
#define DEFAULT_hassassinCrossbowMode 1
#define DEFAULT_tripmineAnimWaitsForFinish 0
#define DEFAULT_revolverLaserScope 1
//#define DEFAULT_python_zoomfov 0
//#define DEFAULT_crossbow_zoomfov 0
//#define DEFAULT_canApplyDefaultFOV 1
#define DEFAULT_auto_adjust_fov 1
//#define DEFAULT_auto_adjust_zoomfov 1
#define DEFAULT_cheat_infiniteclip 0
#define DEFAULT_cheat_infiniteammo 0
#define DEFAULT_cheat_minimumfiredelay 0
#define DEFAULT_cheat_minimumfiredelaycustom 0.048
#define DEFAULT_cheat_nogaussrecoil 0
#define DEFAULT_autoSneaky 0
#define DEFAULT_infiniteLongJumpCharge 0
#define DEFAULT_cheat_touchNeverExplodes 0
#define DEFAULT_gaussRecoilSendsUpInSP 0
#define DEFAULT_drawDebugPathfinding 0
#define DEFAULT_STUcheckDistH 18
#define DEFAULT_STUcheckDistV 32
#define DEFAULT_STUcheckDistD 38
#define DEFAULT_STUextraTriangH 1.2
#define DEFAULT_STUextraTriangV 1.4
#define DEFAULT_STUrepelMulti 1.4
#define DEFAULT_STUSpeedMulti 1
#define DEFAULT_STUExplodeTest 0
#define DEFAULT_STUYawSpeedMulti 0.88
#define DEFAULT_STUDetection 1
#define DEFAULT_stukaAdvancedCombat 0
#define DEFAULT_drawDebugPathfinding2 0
#define DEFAULT_hassaultFriendlyFire 1
#define DEFAULT_myStrobe 0
#define DEFAULT_peopleStrobe 0
#define DEFAULT_forceWorldLightOff 0
#define DEFAULT_wildHeads 0
#define DEFAULT_raveEffectSpawnInterval 0.22
#define DEFAULT_drawBarnacleDebug 0
#define DEFAULT_fogTest 0
#define DEFAULT_sparksAllMulti 1
#define DEFAULT_sparksEnvMulti 0.5
#define DEFAULT_sparksButtonMulti 1
#define DEFAULT_sparksPlayerCrossbowMulti 1
#define DEFAULT_sparksComputerHitMulti 1
#define DEFAULT_sparksTurretDeathMulti 1
#define DEFAULT_sparksOspreyHitMulti 1
#define DEFAULT_sparksExplosionMulti 0.5
#define DEFAULT_sparksBeamMulti 1
#define DEFAULT_sparksAIFailMulti 1
#define DEFAULT_normalSpeedMulti 1
#define DEFAULT_noclipSpeedMulti 2.5
#define DEFAULT_jumpForceMulti 1
#define DEFAULT_apacheForceCinBounds 0
#define DEFAULT_apacheBottomBoundAdj 0
#define DEFAULT_apacheInfluence 0
#define DEFAULT_allowAlphaCrosshairWithoutGuns 0
#define DEFAULT_alphaCrosshairBlockedOnFrozen 1
#define DEFAULT_hgruntRunAndGunDotMin 0.91
#define DEFAULT_panthereyeJumpDotTol 1.2
#define DEFAULT_panthereyePrintout 0
#define DEFAULT_gargantuaPrintout 0
#define DEFAULT_squadmonsterPrintout 0
#define DEFAULT_hassaultPrintout 0
#define DEFAULT_barnaclePrintout 0
#define DEFAULT_friendlyPrintout 0
#define DEFAULT_stukaPrintout 0
#define DEFAULT_timedDamageEndlessOnHard 0
#define DEFAULT_mirrorsReflectOnlyNPCs 1
#define DEFAULT_mirrorsDoNotReflectPlayer 1
#define DEFAULT_r_shadows 0
#define DEFAULT_altSquadRulesRuntime 0
#define DEFAULT_hgruntLockStrafeTime 0.67
#define DEFAULT_ignoreIsolatedNodes 1
#define DEFAULT_drawNodeAll 0
#define DEFAULT_drawNodeSpecial -1
#define DEFAULT_drawNodeConnections 0
#define DEFAULT_drawNodeAlternateTime 0
#define DEFAULT_nodeSearchStartVerticalOffset 30
#define DEFAULT_blockChangeLevelTrigger 0
#define DEFAULT_blockMusicTrigger 0
#define DEFAULT_blockMultiTrigger 0
#define DEFAULT_blockTeleportTrigger 0
#define DEFAULT_blockHurtTrigger 0
#define DEFAULT_blockAutosaveTrigger 0
#define DEFAULT_hideNodeGraphRebuildNotice 1
#define DEFAULT_barnacleTongueRetractDelay 2
#define DEFAULT_sv_germancensorship 0
#define DEFAULT_allowGermanModels 1
#define DEFAULT_germanRobotGibs 1
#define DEFAULT_germanRobotBleedsOil 1
#define DEFAULT_germanRobotDamageDecal 1
#define DEFAULT_germanRobotGibsDecal 1
#define DEFAULT_egonEffectsMode 3
#define DEFAULT_egonHitCloud 1
//#define DEFAULT_enableModPrintouts 0
#define DEFAULT_handGrenadePickupYieldsOne 1
#define DEFAULT_handGrenadesUseOldBounceSound 1
//#define DEFAULT_skipTFDamageTextures 1
#define DEFAULT_weaponSelectSoundPlayOnMousewheel 1
#define DEFAULT_timedDamageDeathRemoveMode 1
#define DEFAULT_weaponSelectUsesReloadSounds 0
#define DEFAULT_barnacleCanGib 0
#define DEFAULT_sentryCanGib 0
#define DEFAULT_miniturretCanGib 0
#define DEFAULT_turretCanGib 0
#define DEFAULT_turretBleedsOil 0
#define DEFAULT_turretDamageDecal 0
#define DEFAULT_turretGibDecal 0
#define DEFAULT_canDropInSinglePlayer 0
#define DEFAULT_timedDamageIgnoresArmor 0
#define DEFAULT_itemBatteryPrerequisite 0
#define DEFAULT_playerExtraPainSoundsMode 2
#define DEFAULT_timedDamageDisableViewPunch 0
#define DEFAULT_batteryDrainsAtDeath 1
#define DEFAULT_batteryDrainsAtAdrenalineMode 1
#define DEFAULT_canTakeLongJump 2
#define DEFAULT_printOutCommonTimables 0
#define DEFAULT_playerBrightLight 0
#define DEFAULT_disablePainPunchAutomatic 1
#define DEFAULT_gargantuaCorpseDeath 1
#define DEFAULT_gargantuaFallSound 1
#define DEFAULT_gargantuaBleeds 1
#define DEFAULT_shrapMode 1
#define DEFAULT_shrapRand 190
#define DEFAULT_shrapRandHeightExtra 140
#define DEFAULT_explosionShrapnelMulti 1
#define DEFAULT_useAlphaSparks 1
#define DEFAULT_emergencyFix 0
#define DEFAULT_timedDamageReviveRemoveMode 1
//#define DEFAULT_forceAllowMonsterSpawning 0
#define DEFAULT_ospreyIgnoresGruntCount 0
#define DEFAULT_mp5GrenadeInheritsPlayerVelocity 0.11
#define DEFAULT_crossbowInheritsPlayerVelocity 0.06
#define DEFAULT_fastHornetsInheritsPlayerVelocity 0.03
#define DEFAULT_snarkInheritsPlayerVelocity 0.17
#define DEFAULT_chumtoadInheritsPlayerVelocity 0.15
#define DEFAULT_weaponPickupPlaysAnyReloadSounds 1
#define DEFAULT_glockUseLastBulletAnim 1
#define DEFAULT_playerBarnacleVictimViewOffset -6
#define DEFAULT_hgruntMovementDeltaCheck 1
#define DEFAULT_hiddenMemPrintout 0
#define DEFAULT_hassaultExtraMuzzleFlashRadius 0.85
#define DEFAULT_hassaultExtraMuzzleFlashBrightness 0.78
#define DEFAULT_hassaultExtraMuzzleFlashForward 4.6
#define DEFAULT_leaderlessSquadAllowed 1
#define DEFAULT_nodeConnectionBreakableCheck 1
#define DEFAULT_playerReviveInvincibilityTime 1.6
#define DEFAULT_playerReviveBuddhaMode 1
#define DEFAULT_playerReviveTimeBlocksTimedDamage 1
#define DEFAULT_hassaultBulletDamageMulti 1.6
#define DEFAULT_hassaultBulletsPerShot 2
#define DEFAULT_hassaultFireAnimSpeedMulti 2.4
#define DEFAULT_hassaultMeleeAnimSpeedMulti 1.36
#define DEFAULT_hassassinCrossbowReloadSoundDelay 1.1
#define DEFAULT_hgruntStrafeAnimSpeedMulti 1.25
#define DEFAULT_hgruntRunAndGunAnimSpeedMulti 1.14
#define DEFAULT_cheat_iwantguts 0
#define DEFAULT_nodeDetailPrintout 0
#define DEFAULT_soundAttenuationStuka 0.85
#define DEFAULT_soundVolumeStuka 1
#define DEFAULT_cineChangelevelFix 3
#define DEFAULT_drawDebugCine 0
#define DEFAULT_cineAllowSequenceOverwrite 0
#define DEFAULT_stukaInflictsBleeding 0
#define DEFAULT_animationKilledBoundsRemoval 1
#define DEFAULT_gargantuaKilledBoundsAssist 1
#define DEFAULT_bullsquidSpitTrajTimeMin 1.0
#define DEFAULT_bullsquidSpitTrajTimeMax 1.5
#define DEFAULT_bullsquidSpitTrajDistMin 900
#define DEFAULT_bullsquidSpitTrajDistMax 1400
#define DEFAULT_bullsquidSpitGravityMulti 1
#define DEFAULT_cl_bullsquidspit 0
#define DEFAULT_cl_bullsquidspitarc 0
#define DEFAULT_bullsquidSpitUseAlphaModel 1
#define DEFAULT_bullsquidSpitUseAlphaEffect 1
#define DEFAULT_bullsquidSpitEffectSpread 0.37
#define DEFAULT_bullsquidSpitEffectMin 90
#define DEFAULT_bullsquidSpitEffectMax 120
#define DEFAULT_bullsquidSpitEffectHitMin 60
#define DEFAULT_bullsquidSpitEffectHitMax 80
#define DEFAULT_bullsquidSpitEffectSpawn 4
#define DEFAULT_bullsquidSpitEffectHitSpawn 4
#define DEFAULT_bullsquidSpitSpriteScale 0.5
#define DEFAULT_bullsquidSpitAlphaScale 0.8
#define DEFAULT_scientistBravery 1
#define DEFAULT_barneyUnholsterTime 18
#define DEFAULT_barneyUnholsterAnimChoice 1
#define DEFAULT_trailTypeTest -1
#define DEFAULT_hornetTrail 0
#define DEFAULT_hornetTrailSolidColor 1
#define DEFAULT_hornetDeathModEasy 2
#define DEFAULT_hornetDeathModMedium 2
#define DEFAULT_hornetDeathModHard 0
#define DEFAULT_hornetZoomPuff 0
#define DEFAULT_hornetSpiral 0
#define DEFAULT_hornetSpeedMulti 1
#define DEFAULT_hornetSpeedDartMulti 2
#define DEFAULT_cl_rockettrail 0
#define DEFAULT_rocketTrailAlphaInterval 0.035
#define DEFAULT_rocketTrailAlphaScale 0.21
#define DEFAULT_rocketSkipIgnite 1
#define DEFAULT_agruntHornetRandomness 0.1
#define DEFAULT_hornetSpiralPeriod 0.11
#define DEFAULT_hornetSpiralAmplitude 59
#define DEFAULT_cl_hornetspiral 0
#define DEFAULT_cl_hornettrail 0
#define DEFAULT_hud_drawammobar 0
#define DEFAULT_hud_weaponselecthideslower 1
#define DEFAULT_hud_drawsidebarmode 0
#define DEFAULT_gaussmode 0
#define DEFAULT_gauss_primaryonly 0
#define DEFAULT_gauss_reflectdealsdamage 1
#define DEFAULT_gauss_chargeanimdelay 0
#define DEFAULT_gauss_chargeworkdelay 0
#define DEFAULT_gauss_secondarychargetimereq 0.5
#define DEFAULT_gauss_primaryreflects 1
#define DEFAULT_gauss_primarypierces 1
#define DEFAULT_gauss_secondaryreflects 1
#define DEFAULT_gauss_secondarypierces 1
#define DEFAULT_gauss_primarypunchthrough 0
#define DEFAULT_gauss_secondarypunchthrough 1
#define DEFAULT_gauss_betweenattackdelay 0.2
#define DEFAULT_gauss_secondarychargemindelay 0.5
#define DEFAULT_gauss_chargeMaxAmmo_SP 13
#define DEFAULT_gauss_chargeMaxAmmo_MP 10
#define DEFAULT_gauss_chargeInterval_SP 0.3
#define DEFAULT_gauss_chargeInterval_MP 0.1
#define DEFAULT_painFlashDmgMin 0.3
#define DEFAULT_painFlashDmgExMult 0.08
#define DEFAULT_painFlashCumulativeMinDrowning 0.5
#define DEFAULT_painFlashCumulativeMax 1.4
#define DEFAULT_painFlashDrawOpacityMax 0.8
#define DEFAULT_painArrowDrawOpacityMin 0.5
#define DEFAULT_painArrowDrawOpacityMax 1.0
#define DEFAULT_painFlashFadeMult 1.7
#define DEFAULT_painArrowFadeMult 1.5
#define DEFAULT_painFlashArmorBlock 0.5
#define DEFAULT_painFlashDirTolerance 0.8
#define DEFAULT_painArrowCumulativeAppearMin 0.4
#define DEFAULT_painArrowCumulativeDmgJump 1.2
#define DEFAULT_painFlashPrintouts 0
#define DEFAULT_itemFlashCumulativeJump 1.15
#define DEFAULT_itemFlashDrawOpacityMin 0.0
#define DEFAULT_itemFlashDrawOpacityMax 0.75
#define DEFAULT_itemFlashFadeMult 1.5
#define DEFAULT_chumtoadPrintout 0
#define DEFAULT_crossbowReloadSoundDelay -1
#define DEFAULT_crossbowFirePlaysReloadSound 0
#define DEFAULT_pathfindFidgetFailTime 0.2
#define DEFAULT_pathfindPrintout 0
#define DEFAULT_pathfindTopRampFixDistance 100
#define DEFAULT_pathfindTopRampFixDraw 0
#define DEFAULT_iHaveAscended 0
#define DEFAULT_pathfindLooseMapNodes 0
#define DEFAULT_pathfindRampFix 0
#define DEFAULT_chumtoadPlayDeadFoolChance 0.5
#define DEFAULT_animationFramerateMulti 1
#define DEFAULT_pathfindNodeToleranceMulti 1
#define DEFAULT_friendlyPianoFollowVolume 0.7
#define DEFAULT_friendlyPianoOtherVolume 1.0
#define DEFAULT_showtriggers 0
#define DEFAULT_tentacleAlertSound 1
#define DEFAULT_tentacleSwingSound1 0
#define DEFAULT_tentacleSwingSound2 0
#define DEFAULT_playerFollowerMax 1
#define DEFAULT_announcerIsAJerk 0
#define DEFAULT_playerUseDrawDebug 0
#define DEFAULT_playerChumtoadThrowDrawDebug 0
#define DEFAULT_peaceOut 0
#define DEFAULT_drawViewModel 1
#define DEFAULT_drawHUD 1
#define DEFAULT_disablePauseSinglePlayer 0
#define DEFAULT_playerBulletHitEffectForceServer 0
#define DEFAULT_forceAllowServersideTextureSounds 0
#define DEFAULT_playerWeaponSpreadMode 0
#define DEFAULT_monsterAIForceFindDistance 0
#define DEFAULT_baseEntityDamagePushNormalMulti 0
#define DEFAULT_baseEntityDamagePushVerticalBoost 0
#define DEFAULT_baseEntityDamagePushVerticalMulti 0
#define DEFAULT_baseEntityDamagePushVerticalMinimum 0
#define DEFAULT_viewModelPrintouts 0
#define DEFAULT_viewModelSyncFixPrintouts 0
#define DEFAULT_textureHitSoundPrintouts 0
#define DEFAULT_hgruntAllowGrenades 1
#define DEFAULT_scheduleInterruptPrintouts 0
#define DEFAULT_animationPrintouts 0
#define DEFAULT_hassaultMeleeAttackEnabled 1
#define DEFAULT_pathfindStumpedWaitTime 6
#define DEFAULT_pathfindStumpedMode 2
#define DEFAULT_pathfindStumpedForgetEnemy 0
#define DEFAULT_pathfindEdgeCheck 1
#define DEFAULT_RadiusDamageDrawDebug 0
#define DEFAULT_AlienRadiationImmunity 0
#define DEFAULT_customLogoSprayMode 0
#define DEFAULT_monsterFadeOutRate 120
#define DEFAULT_playerFadeOutRate 100
#define DEFAULT_drawDebugEnemyLKP 0
#define DEFAULT_trackchangePrintouts 0
#define DEFAULT_trackTrainPrintouts 0
#define DEFAULT_cl_holster 0
#define DEFAULT_playerWeaponTracerMode 5
#define DEFAULT_monsterWeaponTracerMode 2
#define DEFAULT_decalTracerExclusivity 0
#define DEFAULT_monsterToPlayerHitgroupSpecial 0
#define DEFAULT_multiplayerCrowbarHitSoundMode 1
#define DEFAULT_healthcolor_fullRedMin 15
#define DEFAULT_healthcolor_brightness 125
#define DEFAULT_healthcolor_yellowMark 70
#define DEFAULT_cl_drawExtraZeros 0
#define DEFAULT_pathfindLargeBoundFix 0
#define DEFAULT_flyerKilledFallingLoop 1
#define DEFAULT_floaterDummy 0
#define DEFAULT_barneyDummy 0
#define DEFAULT_ladderCycleMulti 1
#define DEFAULT_ladderSpeedMulti 1
#define DEFAULT_barnacleGrabNoInterpolation 0
#define DEFAULT_hideDamage 0
#define DEFAULT_timedDamage_brightnessMax 256
#define DEFAULT_timedDamage_brightnessMin 0
#define DEFAULT_timedDamage_brightnessCap -1
#define DEFAULT_timedDamage_brightnessFloor -1
#define DEFAULT_timedDamage_flashSpeed 2
#define DEFAULT_timedDamage_debug 0
#define DEFAULT_wallHealthDoor_closeDelay 0.75
#define DEFAULT_houndeye_attack_canGib 0
#define DEFAULT_myRocketsAreBarney 0
#define DEFAULT_hassassinCrossbowDebug 0
#define DEFAULT_crossbowBoltDirectionAffectedByWater 0
#define DEFAULT_kingpinDebug 0
#define DEFAULT_minimumRespawnDelay 2
#define DEFAULT_r_glowshell_debug 0
#define DEFAULT_cl_viewpunch 1
#define DEFAULT_cl_explosion 0
#define DEFAULT_soundSentenceSave 1
#define DEFAULT_pissedNPCs 0

#define DEFAULT_hud_logo 0
#define DEFAULT_hud_brokentrans 0
#define DEFAULT_cl_fvox 1
#define DEFAULT_cl_ladder 1
#define DEFAULT_precacheAll 1
#define DEFAULT_cl_interp_entity 1

#define DEFAULT_hud_swapFirstTwoBuckets 0
#define DEFAULT_ignoreMultiplayerSkillOverride 1

#define DEFAULT_m_rawinput 0
#define DEFAULT_cl_earlyaccess 1
#define DEFAULT_cl_viewroll 1
#define DEFAULT_drawDebugCrowbar 0

#define DEFAULT_blastExtraArmorDamageMode 0
#define DEFAULT_hud_batteryhiddendead 1
#define DEFAULT_hyperBarney 0
#define DEFAULT_monsterKilledToss 2
#define DEFAULT_interpolation_movetypestep_mindelta 0.06
#define DEFAULT_sv_bloodparticlemode 1
#define DEFAULT_cl_interp_view_extra 2
#define DEFAULT_cl_interp_viewmodel 1
#define DEFAULT_sv_explosionknockback 1
#define DEFAULT_cl_gaussfollowattachment 1
#define DEFAULT_cl_breakholster 1




#define EASY_CVAR_HASH_MASS\
	EASY_CVAR_HASH(strobeDurationMin, 0)\
	EASY_CVAR_HASH(strobeDurationMax, 1)\
	EASY_CVAR_HASH(strobeRadiusMin, 2)\
	EASY_CVAR_HASH(strobeRadiusMax, 3)\
	EASY_CVAR_HASH(strobeSpawnDistHori, 4)\
	EASY_CVAR_HASH(strobeSpawnDistVertMin, 5)\
	EASY_CVAR_HASH(strobeSpawnDistVertMax, 6)\
	EASY_CVAR_HASH(strobeMultiColor, 7)\
	EASY_CVAR_HASH(raveLaserEnabled, 8)\
	EASY_CVAR_HASH(raveLaserSpawnFreq, 9)\
	EASY_CVAR_HASH(raveLaserLength, 10)\
	EASY_CVAR_HASH(raveLaserSpawnDistHoriMin, 11)\
	EASY_CVAR_HASH(raveLaserSpawnDistHoriMax, 12)\
	EASY_CVAR_HASH(raveLaserSpawnDistVertMin, 13)\
	EASY_CVAR_HASH(raveLaserSpawnDistVertMax, 14)\
	EASY_CVAR_HASH(raveLaserBrightnessMin, 15)\
	EASY_CVAR_HASH(raveLaserBrightnessMax, 16)\
	EASY_CVAR_HASH(raveLaserDurationMin, 17)\
	EASY_CVAR_HASH(raveLaserDurationMax, 18)\
	EASY_CVAR_HASH(raveLaserThicknessMin, 19)\
	EASY_CVAR_HASH(raveLaserThicknessMax, 20)\
	EASY_CVAR_HASH(raveLaserNoiseMin, 21)\
	EASY_CVAR_HASH(raveLaserNoiseMax, 22)\
	EASY_CVAR_HASH(raveLaserFrameRateMin, 23)\
	EASY_CVAR_HASH(raveLaserFrameRateMax, 24)\
	EASY_CVAR_HASH(raveLaserMultiColor, 25)\
	EASY_CVAR_HASH(cameraPosFixedX, 26)\
	EASY_CVAR_HASH(cameraPosFixedY, 27)\
	EASY_CVAR_HASH(cameraPosFixedZ, 28)\
	EASY_CVAR_HASH(cameraPosOffX, 29)\
	EASY_CVAR_HASH(cameraPosOffY, 30)\
	EASY_CVAR_HASH(cameraPosOffZ, 31)\
	EASY_CVAR_HASH(cameraRotFixedX, 32)\
	EASY_CVAR_HASH(cameraRotFixedY, 33)\
	EASY_CVAR_HASH(cameraRotFixedZ, 34)\
	EASY_CVAR_HASH(cameraRotOffX, 35)\
	EASY_CVAR_HASH(cameraRotOffY, 36)\
	EASY_CVAR_HASH(cameraRotOffZ, 37)\
	EASY_CVAR_HASH(imAllFuckedUp, 38)\
	EASY_CVAR_HASH(thatWasntGrass, 39)\
	EASY_CVAR_HASH(thatWasntPunch, 40)\
	EASY_CVAR_HASH(fogNear, 41)\
	EASY_CVAR_HASH(fogFar, 42)\
	EASY_CVAR_HASH(myCameraSucks, 43)\
	EASY_CVAR_HASH(muteBulletHitSounds, 44)\
	EASY_CVAR_HASH(geigerChannel, 45)\
	EASY_CVAR_HASH(muteTempEntityGroundHitSound, 46)\
	EASY_CVAR_HASH(muteRicochetSound, 47)\
	EASY_CVAR_HASH(mutePlayerWeaponFire, 48)\
	EASY_CVAR_HASH(muteCrowbarSounds, 49)\
	EASY_CVAR_HASH(seeMonsterHealth, 50)\
	EASY_CVAR_HASH(event5011Allowed, 51)\
	EASY_CVAR_HASH(event5021Allowed, 52)\
	EASY_CVAR_HASH(event5031Allowed, 53)\
	EASY_CVAR_HASH(event5002Allowed, 54)\
	EASY_CVAR_HASH(event5004Allowed, 55)\
	EASY_CVAR_HASH(eventsAreFabulous, 56)\
	EASY_CVAR_HASH(glockOldReloadLogic, 57)\
	EASY_CVAR_HASH(testVar, 58)\
	EASY_CVAR_HASH(painArrowColorMode, 59)\
	EASY_CVAR_HASH(painFlashColorMode, 60)\
	EASY_CVAR_HASH(painFlashSuitless, 61)\
	EASY_CVAR_HASH(painFlashDrownMode, 62)\
	EASY_CVAR_HASH(firstPersonIdleDelayMin, 63)\
	EASY_CVAR_HASH(firstPersonIdleDelayMax, 64)\
	EASY_CVAR_HASH(forceDrawBatteryNumber, 65)\
	EASY_CVAR_HASH(canShowWeaponSelectAtDeath, 66)\
	EASY_CVAR_HASH(endlessFlashlightBattery, 67)\
	EASY_CVAR_HASH(preE3UsesFailColors, 68)\
	EASY_CVAR_HASH(E3UsesFailColors, 69)\
	EASY_CVAR_HASH(preE3ShowsDamageIcons, 70)\
	EASY_CVAR_HASH(E3ShowsDamageIcons, 71)\
	EASY_CVAR_HASH(playerCrossbowMode, 72)\
	EASY_CVAR_HASH(tripmineAnimWaitsForFinish, 73)\
	EASY_CVAR_HASH(revolverLaserScope, 74)\
	DUMMY\
	DUMMY\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_HASH(cheat_infiniteclip, 80)\
	EASY_CVAR_HASH(cheat_infiniteammo, 81)\
	EASY_CVAR_HASH(cheat_minimumfiredelay, 82)\
	EASY_CVAR_HASH(cheat_minimumfiredelaycustom, 83)\
	EASY_CVAR_HASH(cheat_nogaussrecoil, 84)\
	EASY_CVAR_HASH(gaussRecoilSendsUpInSP, 85)\
	EASY_CVAR_HASH(raveEffectSpawnInterval, 86)\
	EASY_CVAR_HASH(fogTest, 87)\
	DUMMY\
	EASY_CVAR_HASH(allowAlphaCrosshairWithoutGuns, 89)\
	EASY_CVAR_HASH(alphaCrosshairBlockedOnFrozen, 90)\
	EASY_CVAR_HASH(mirrorsReflectOnlyNPCs, 91)\
	EASY_CVAR_HASH(mirrorsDoNotReflectPlayer, 92)\
	EASY_CVAR_HASH(sv_germancensorship, 93)\
	EASY_CVAR_HASH(egonEffectsMode, 94)\
	EASY_CVAR_HASH(egonHitCloud, 95)\
	DUMMY\
	EASY_CVAR_HASH(handGrenadePickupYieldsOne, 97)\
	DUMMY\
	EASY_CVAR_HASH(weaponSelectSoundPlayOnMousewheel, 99)\
	EASY_CVAR_HASH(timedDamageDeathRemoveMode, 100)\
	EASY_CVAR_HASH(weaponSelectUsesReloadSounds, 101)\
	EASY_CVAR_HASH(glockUseLastBulletAnim, 102)\
	EASY_CVAR_HASH(playerBarnacleVictimViewOffset, 103)\
	EASY_CVAR_HASH(trailTypeTest, 104)\
	EASY_CVAR_HASH(hornetTrail, 105)\
	EASY_CVAR_HASH(hornetTrailSolidColor, 106)\
	EASY_CVAR_HASH(hornetDeathModEasy, 107)\
	EASY_CVAR_HASH(hornetDeathModMedium, 108)\
	EASY_CVAR_HASH(hornetDeathModHard, 109)\
	EASY_CVAR_HASH(hornetZoomPuff, 110)\
	EASY_CVAR_HASH(hornetSpiral, 111)\
	EASY_CVAR_HASH(hornetSpeedMulti, 112)\
	EASY_CVAR_HASH(hornetSpeedDartMulti, 113)\
	EASY_CVAR_HASH(rocketTrailAlphaInterval, 114)\
	EASY_CVAR_HASH(rocketTrailAlphaScale, 115)\
	EASY_CVAR_HASH(rocketSkipIgnite, 116)\
	EASY_CVAR_HASH(gauss_primaryonly, 117)\
	EASY_CVAR_HASH(gauss_reflectdealsdamage, 118)\
	EASY_CVAR_HASH(gauss_chargeanimdelay, 119)\
	EASY_CVAR_HASH(gauss_chargeworkdelay, 120)\
	EASY_CVAR_HASH(gauss_secondarychargetimereq, 121)\
	EASY_CVAR_HASH(gauss_primaryreflects, 122)\
	EASY_CVAR_HASH(gauss_primarypierces, 123)\
	EASY_CVAR_HASH(gauss_secondaryreflects, 124)\
	EASY_CVAR_HASH(gauss_secondarypierces, 125)\
	EASY_CVAR_HASH(gauss_primarypunchthrough, 126)\
	EASY_CVAR_HASH(gauss_secondarypunchthrough, 127)\
	EASY_CVAR_HASH(gauss_betweenattackdelay, 128)\
	EASY_CVAR_HASH(gauss_secondarychargemindelay, 129)\
	EASY_CVAR_HASH(gauss_chargeMaxAmmo_SP, 130)\
	EASY_CVAR_HASH(gauss_chargeMaxAmmo_MP, 131)\
	EASY_CVAR_HASH(gauss_chargeInterval_SP, 132)\
	EASY_CVAR_HASH(gauss_chargeInterval_MP, 133)\
	EASY_CVAR_HASH(painFlashDmgMin, 134)\
	EASY_CVAR_HASH(painFlashDmgExMult, 135)\
	EASY_CVAR_HASH(painFlashCumulativeMinDrowning, 136)\
	EASY_CVAR_HASH(painFlashCumulativeMax, 137)\
	EASY_CVAR_HASH(painFlashDrawOpacityMax, 138)\
	EASY_CVAR_HASH(painArrowDrawOpacityMin, 139)\
	EASY_CVAR_HASH(painArrowDrawOpacityMax, 140)\
	EASY_CVAR_HASH(painFlashFadeMult, 141)\
	EASY_CVAR_HASH(painArrowFadeMult, 142)\
	EASY_CVAR_HASH_CLIENTONLY(painFlashArmorBlock, 143)\
	EASY_CVAR_HASH(painFlashDirTolerance, 144)\
	EASY_CVAR_HASH(painArrowCumulativeAppearMin, 145)\
	EASY_CVAR_HASH(painArrowCumulativeDmgJump, 146)\
	EASY_CVAR_HASH(painFlashPrintouts, 147)\
	EASY_CVAR_HASH(itemFlashCumulativeJump, 148)\
	EASY_CVAR_HASH(itemFlashDrawOpacityMin, 149)\
	EASY_CVAR_HASH(itemFlashDrawOpacityMax, 150)\
	EASY_CVAR_HASH(itemFlashFadeMult, 151)\
	EASY_CVAR_HASH(crossbowReloadSoundDelay, 152)\
	EASY_CVAR_HASH(crossbowFirePlaysReloadSound, 153)\
	EASY_CVAR_HASH(iHaveAscended, 154)\
	EASY_CVAR_HASH_CLIENTONLY(drawViewModel, 155)\
	EASY_CVAR_HASH_CLIENTONLY(drawHUD, 156)\
	EASY_CVAR_HASH(playerBulletHitEffectForceServer, 157)\
	EASY_CVAR_HASH(forceAllowServersideTextureSounds, 158)\
	EASY_CVAR_HASH(playerWeaponSpreadMode, 159)\
	EASY_CVAR_HASH(viewModelPrintouts, 160)\
	EASY_CVAR_HASH(viewModelSyncFixPrintouts, 161)\
	EASY_CVAR_HASH(textureHitSoundPrintouts, 162)\
	DUMMY\
	EASY_CVAR_HASH(playerWeaponTracerMode, 164)\
	EASY_CVAR_HASH(decalTracerExclusivity, 165)\
	EASY_CVAR_HASH_CLIENTONLY(healthcolor_fullRedMin, 166)\
	EASY_CVAR_HASH_CLIENTONLY(healthcolor_brightness, 167)\
	EASY_CVAR_HASH_CLIENTONLY(healthcolor_yellowMark, 168)\
	EASY_CVAR_HASH_CLIENTONLY(cl_drawExtraZeros, 169)\
	EASY_CVAR_HASH(hideDamage, 170)\
	EASY_CVAR_HASH_CLIENTONLY(timedDamage_brightnessMax, 171)\
	EASY_CVAR_HASH_CLIENTONLY(timedDamage_brightnessMin, 172)\
	EASY_CVAR_HASH_CLIENTONLY(timedDamage_brightnessCap, 173)\
	EASY_CVAR_HASH_CLIENTONLY(timedDamage_brightnessFloor, 174)\
	EASY_CVAR_HASH_CLIENTONLY(timedDamage_flashSpeed, 175)\
	EASY_CVAR_HASH_CLIENTONLY(timedDamage_debug, 176)\
	EASY_CVAR_HASH(myRocketsAreBarney, 177)\
	EASY_CVAR_HASH(wpn_glocksilencer, wpn_glocksilencer_ID)\
	DUMMY



#define EASY_CVAR_HIDDEN_LIST\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(gruntsCanHaveMP5Grenade, gruntscanhavemp5grenade)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMin, strobedurationmin, 0)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMax, strobedurationmax, 1)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMin, stroberadiusmin, 2)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMax, stroberadiusmax, 3)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistHori, strobespawndisthori, 4)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMin, strobespawndistvertmin, 5)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMax, strobespawndistvertmax, 6)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeMultiColor, strobemulticolor, 7)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserEnabled, ravelaserenabled, 8)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnFreq, ravelaserspawnfreq, 9)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserLength, ravelaserlength, 10)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMin, ravelaserspawndisthorimin, 11)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMax, ravelaserspawndisthorimax, 12)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMin, ravelaserspawndistvertmin, 13)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMax, ravelaserspawndistvertmax, 14)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMin, ravelaserbrightnessmin, 15)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMax, ravelaserbrightnessmax, 16)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMin, ravelaserdurationmin, 17)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMax, ravelaserdurationmax, 18)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMin, ravelaserthicknessmin, 19)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMax, ravelaserthicknessmax, 20)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMin, ravelasernoisemin, 21)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMax, ravelasernoisemax, 22)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMin, ravelaserframeratemin, 23)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMax, ravelaserframeratemax, 24)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserMultiColor, ravelasermulticolor, 25)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedX, cameraposfixedx, 26)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedY, cameraposfixedy, 27)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedZ, cameraposfixedz, 28)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffX, cameraposoffx, 29)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffY, cameraposoffy, 30)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffZ, cameraposoffz, 31)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedX, camerarotfixedx, 32)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedY, camerarotfixedy, 33)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedZ, camerarotfixedz, 34)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffX, camerarotoffx, 35)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffY, camerarotoffy, 36)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffZ, camerarotoffz, 37)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(imAllFuckedUp, imallfuckedup, 38)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntGrass, thatwasntgrass, 39)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntPunch, thatwasntpunch, 40)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogNear, fognear, 41)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogFar, fogfar, 42)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(NPCsTalkMore, npcstalkmore)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myCameraSucks, mycamerasucks, 43)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(shutupstuka, shutupstuka)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultSpinMovement, hassaultspinmovement)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultIdleSpinSound, hassaultidlespinsound)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultFireSound, hassaultfiresound)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteBulletHitSounds, mutebullethitsounds, 44)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(mutePlayerPainSounds, muteplayerpainsounds)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultIdleSpinSoundChannel, hassaultidlespinsoundchannel)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultSpinUpDownSoundChannel, hassaultspinupdownsoundchannel)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultFireSoundChannel, hassaultfiresoundchannel)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(geigerChannel, geigerchannel, 45)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultWaitTime, hassaultwaittime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultSpinupRemainTime, hassaultspinupremaintime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultResidualAttackTime, hassaultresidualattacktime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultSpinupStartTime, hassaultspinupstarttime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultVoicePitchMin, hassaultvoicepitchmin)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultVoicePitchMax, hassaultvoicepitchmax)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultFireSpread, hassaultfirespread)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteTempEntityGroundHitSound, mutetempentitygroundhitsound, 46)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(houndeyeAttackMode, houndeyeattackmode)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteRicochetSound, mutericochetsound, 47)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mutePlayerWeaponFire, muteplayerweaponfire, 48)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteCrowbarSounds, mutecrowbarsounds, 49)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(scientistHealNPC, scientisthealnpc)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(scientistHealNPCDebug, scientisthealnpcdebug)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bulletholeAlertRange, bulletholealertrange)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(fleshhitmakessound, fleshhitmakessound)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(nothingHurts, nothinghurts)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(seeMonsterHealth, seemonsterhealth, 50)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(scientistHealNPCFract, scientisthealnpcfract)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidRangeDisabled, bullsquidrangedisabled)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(applyLKPPathFixToAll, applylkppathfixtoall)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(timedDamageAffectsMonsters, timeddamageaffectsmonsters)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(scientistHealCooldown, scientisthealcooldown)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(crazyMonsterPrintouts, crazymonsterprintouts)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(movementIsCompletePrintout, movementiscompleteprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bulletHoleAlertPrintout, bulletholealertprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bulletholeAlertStukaOnly, bulletholealertstukaonly)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barneyPrintouts, barneyprintouts)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(monsterSpawnPrintout, monsterspawnprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(zombieBulletResistance, zombiebulletresistance)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(zombieBulletPushback, zombiebulletpushback)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(houndeyePrintout, houndeyeprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(quakeExplosionSound, quakeexplosionsound)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(explosionDebrisSoundVolume, explosiondebrissoundvolume)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(noFlinchOnHard, noflinchonhard)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultDrawLKP, hassaultdrawlkp)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(meleeDrawBloodModeA, meleedrawbloodmodea)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(meleeDrawBloodModeB, meleedrawbloodmodeb)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawDebugBloodTrace, drawdebugbloodtrace)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(meleeDrawBloodModeBFix, meleedrawbloodmodebfix)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(meleeDrawBloodModeAOffset, meleedrawbloodmodeaoffset)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(meleeDrawBloodModeBOffset, meleedrawbloodmodeboffset)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(panthereyeHasCloakingAbility, panthereyehascloakingability)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntSpeedMulti, hgruntspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntForceStrafeFireAnim, hgruntforcestrafefireanim)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntLockRunAndGunTime, hgruntlockrunandguntime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntHeadshotGore, hgruntheadshotgore)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntAllowStrafeFire, hgruntallowstrafefire)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(thoroughHitBoxUpdates, thoroughhitboxupdates)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntTinyClip, hgrunttinyclip)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntStrafeAlwaysHasAmmo, hgruntstrafealwayshasammo)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntBrassEjectForwardOffset, hgruntbrassejectforwardoffset)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(agrunt_muzzleflash, agrunt_muzzleflash)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5011Allowed, event5011allowed, 51)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5021Allowed, event5021allowed, 52)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5031Allowed, event5031allowed, 53)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5002Allowed, event5002allowed, 54)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5004Allowed, event5004allowed, 55)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(eventsAreFabulous, eventsarefabulous, 56)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockOldReloadLogic, glockoldreloadlogic, 57)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(glockOldReloadLogicBarney, glockoldreloadlogicbarney)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barneyDroppedGlockAmmoCap, barneydroppedglockammocap)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawCollisionBoundsAtDeath, drawcollisionboundsatdeath)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawHitBoundsAtDeath, drawhitboundsatdeath)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(islaveReviveFriendMode, islaverevivefriendmode)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(islaveReviveFriendChance, islaverevivefriendchance)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(islaveReviveFriendRange, islaverevivefriendrange)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(islaveReviveSelfMinDelay, islavereviveselfmindelay)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(islaveReviveSelfMaxDelay, islavereviveselfmaxdelay)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(islaveReviveSelfChance, islavereviveselfchance)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntRunAndGunDistance, hgruntrunandgundistance)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntPrintout, hgruntprintout)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(testVar, testvar, 58)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowColorMode, painarrowcolormode, 59)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashColorMode, painflashcolormode, 60)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashSuitless, painflashsuitless, 61)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrownMode, painflashdrownmode, 62)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMin, firstpersonidledelaymin, 63)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMax, firstpersonidledelaymax, 64)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceDrawBatteryNumber, forcedrawbatterynumber, 65)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(offsetgivedistance, offsetgivedistance)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(offsetgivelookvertical, offsetgivelookvertical)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(canShowWeaponSelectAtDeath, canshowweaponselectatdeath, 66)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(endlessFlashlightBattery, endlessflashlightbattery, 67)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3UsesFailColors, pree3usesfailcolors, 68)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3UsesFailColors, e3usesfailcolors, 69)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3ShowsDamageIcons, pree3showsdamageicons, 70)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3ShowsDamageIcons, e3showsdamageicons, 71)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerCrossbowMode, playercrossbowmode, 72)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassassinCrossbowMode, hassassincrossbowmode)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(tripmineAnimWaitsForFinish, tripmineanimwaitsforfinish, 73)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(revolverLaserScope, revolverlaserscope, 74)\
	DUMMY\
	DUMMY\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip, cheat_infiniteclip, 80)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo, cheat_infiniteammo, 81)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay, cheat_minimumfiredelay, 82)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom, cheat_minimumfiredelaycustom, 83)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil, cheat_nogaussrecoil, 84)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(autoSneaky, autosneaky)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(infiniteLongJumpCharge, infinitelongjumpcharge)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(cheat_touchNeverExplodes, cheat_touchneverexplodes)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP, gaussrecoilsendsupinsp, 85)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawDebugPathfinding, drawdebugpathfinding)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUcheckDistH, stucheckdisth)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUcheckDistV, stucheckdistv)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUcheckDistD, stucheckdistd)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUextraTriangH, stuextratriangh)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUextraTriangV, stuextratriangv)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUrepelMulti, sturepelmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUSpeedMulti, stuspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUExplodeTest, stuexplodetest)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUYawSpeedMulti, stuyawspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(STUDetection, studetection)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(stukaAdvancedCombat, stukaadvancedcombat)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawDebugPathfinding2, drawdebugpathfinding2)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultFriendlyFire, hassaultfriendlyfire)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(myStrobe, mystrobe)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(peopleStrobe, peoplestrobe)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(forceWorldLightOff, forceworldlightoff)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(wildHeads, wildheads)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveEffectSpawnInterval, raveeffectspawninterval, 86)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawBarnacleDebug, drawbarnacledebug)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogTest, fogtest, 87)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksAllMulti, sparksallmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksEnvMulti, sparksenvmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksButtonMulti, sparksbuttonmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksPlayerCrossbowMulti, sparksplayercrossbowmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksComputerHitMulti, sparkscomputerhitmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksTurretDeathMulti, sparksturretdeathmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksOspreyHitMulti, sparksospreyhitmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksExplosionMulti, sparksexplosionmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksBeamMulti, sparksbeammulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sparksAIFailMulti, sparksaifailmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(normalSpeedMulti, normalspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(noclipSpeedMulti, noclipspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(jumpForceMulti, jumpforcemulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(apacheForceCinBounds, apacheforcecinbounds)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(apacheBottomBoundAdj, apachebottomboundadj)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(apacheInfluence, apacheinfluence)\
	DUMMY\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(allowAlphaCrosshairWithoutGuns, allowalphacrosshairwithoutguns, 89)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(alphaCrosshairBlockedOnFrozen, alphacrosshairblockedonfrozen, 90)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntRunAndGunDotMin, hgruntrunandgundotmin)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(panthereyeJumpDotTol, panthereyejumpdottol)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(panthereyePrintout, panthereyeprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(gargantuaPrintout, gargantuaprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(squadmonsterPrintout, squadmonsterprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultPrintout, hassaultprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barnaclePrintout, barnacleprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(friendlyPrintout, friendlyprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(stukaPrintout, stukaprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(timedDamageEndlessOnHard, timeddamageendlessonhard)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsReflectOnlyNPCs, mirrorsreflectonlynpcs, 91)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsDoNotReflectPlayer, mirrorsdonotreflectplayer, 92)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(altSquadRulesRuntime, altsquadrulesruntime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntLockStrafeTime, hgruntlockstrafetime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(ignoreIsolatedNodes, ignoreisolatednodes)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawNodeAll, drawnodeall)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawNodeSpecial, drawnodespecial)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawNodeConnections, drawnodeconnections)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawNodeAlternateTime, drawnodealternatetime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(nodeSearchStartVerticalOffset, nodesearchstartverticaloffset)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(blockChangeLevelTrigger, blockchangeleveltrigger)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(blockMusicTrigger, blockmusictrigger)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(blockMultiTrigger, blockmultitrigger)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(blockTeleportTrigger, blockteleporttrigger)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(blockHurtTrigger, blockhurttrigger)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(blockAutosaveTrigger, blockautosavetrigger)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hideNodeGraphRebuildNotice, hidenodegraphrebuildnotice)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barnacleTongueRetractDelay, barnacletongueretractdelay)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(sv_germancensorship, sv_germancensorship, 93)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(allowGermanModels, allowgermanmodels)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(germanRobotGibs, germanrobotgibs)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(germanRobotBleedsOil, germanrobotbleedsoil)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(germanRobotDamageDecal, germanrobotdamagedecal)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(germanRobotGibsDecal, germanrobotgibsdecal)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode, egoneffectsmode, 94)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud, egonhitcloud, 95)\
	DUMMY\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(handGrenadePickupYieldsOne, handgrenadepickupyieldsone, 97)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(handGrenadesUseOldBounceSound, handgrenadesuseoldbouncesound)\
	DUMMY\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectSoundPlayOnMousewheel, weaponselectsoundplayonmousewheel, 99)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(timedDamageDeathRemoveMode, timeddamagedeathremovemode, 100)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectUsesReloadSounds, weaponselectusesreloadsounds, 101)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barnacleCanGib, barnaclecangib)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(sentryCanGib, sentrycangib)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(miniturretCanGib, miniturretcangib)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(turretCanGib, turretcangib)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(turretBleedsOil, turretbleedsoil)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(turretDamageDecal, turretdamagedecal)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(turretGibDecal, turretgibdecal)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(canDropInSinglePlayer, candropinsingleplayer)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(timedDamageIgnoresArmor, timeddamageignoresarmor)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(itemBatteryPrerequisite, itembatteryprerequisite)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerExtraPainSoundsMode, playerextrapainsoundsmode)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(timedDamageDisableViewPunch, timeddamagedisableviewpunch)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(batteryDrainsAtDeath, batterydrainsatdeath)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(batteryDrainsAtAdrenalineMode, batterydrainsatadrenalinemode)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(canTakeLongJump, cantakelongjump)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(printOutCommonTimables, printoutcommontimables)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerBrightLight, playerbrightlight)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(disablePainPunchAutomatic, disablepainpunchautomatic)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(gargantuaCorpseDeath, gargantuacorpsedeath)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(gargantuaFallSound, gargantuafallsound)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(gargantuaBleeds, gargantuableeds)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(shrapMode, shrapmode)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(shrapRand, shraprand)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(shrapRandHeightExtra, shraprandheightextra)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(explosionShrapnelMulti, explosionshrapnelmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(useAlphaSparks, usealphasparks)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(emergencyFix, emergencyfix)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(timedDamageReviveRemoveMode, timeddamagereviveremovemode)\
	DUMMY\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(ospreyIgnoresGruntCount, ospreyignoresgruntcount)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity, mp5grenadeinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(crossbowInheritsPlayerVelocity, crossbowinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(fastHornetsInheritsPlayerVelocity, fasthornetsinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(snarkInheritsPlayerVelocity, snarkinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(chumtoadInheritsPlayerVelocity, chumtoadinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(weaponPickupPlaysAnyReloadSounds, weaponpickupplaysanyreloadsounds)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockUseLastBulletAnim, glockuselastbulletanim, 102)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBarnacleVictimViewOffset, playerbarnaclevictimviewoffset, 103)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntMovementDeltaCheck, hgruntmovementdeltacheck)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultExtraMuzzleFlashRadius, hassaultextramuzzleflashradius)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultExtraMuzzleFlashBrightness, hassaultextramuzzleflashbrightness)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultExtraMuzzleFlashForward, hassaultextramuzzleflashforward)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(leaderlessSquadAllowed, leaderlesssquadallowed)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(nodeConnectionBreakableCheck, nodeconnectionbreakablecheck)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerReviveInvincibilityTime, playerreviveinvincibilitytime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerReviveBuddhaMode, playerrevivebuddhamode)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerReviveTimeBlocksTimedDamage, playerrevivetimeblockstimeddamage)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultBulletDamageMulti, hassaultbulletdamagemulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultBulletsPerShot, hassaultbulletspershot)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultFireAnimSpeedMulti, hassaultfireanimspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultMeleeAnimSpeedMulti, hassaultmeleeanimspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassassinCrossbowReloadSoundDelay, hassassincrossbowreloadsounddelay)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntStrafeAnimSpeedMulti, hgruntstrafeanimspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti, hgruntrunandgunanimspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(cheat_iwantguts, cheat_iwantguts)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(nodeDetailPrintout, nodedetailprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(soundAttenuationStuka, soundattenuationstuka)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(soundVolumeStuka, soundvolumestuka)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(cineChangelevelFix, cinechangelevelfix)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawDebugCine, drawdebugcine)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(cineAllowSequenceOverwrite, cineallowsequenceoverwrite)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(stukaInflictsBleeding, stukainflictsbleeding)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(animationKilledBoundsRemoval, animationkilledboundsremoval)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(gargantuaKilledBoundsAssist, gargantuakilledboundsassist)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitTrajTimeMin, bullsquidspittrajtimemin)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitTrajTimeMax, bullsquidspittrajtimemax)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitTrajDistMin, bullsquidspittrajdistmin)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitTrajDistMax, bullsquidspittrajdistmax)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitGravityMulti, bullsquidspitgravitymulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitUseAlphaModel, bullsquidspitusealphamodel)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitUseAlphaEffect, bullsquidspitusealphaeffect)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitEffectSpread, bullsquidspiteffectspread)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitEffectMin, bullsquidspiteffectmin)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitEffectMax, bullsquidspiteffectmax)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitEffectHitMin, bullsquidspiteffecthitmin)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitEffectHitMax, bullsquidspiteffecthitmax)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitEffectSpawn, bullsquidspiteffectspawn)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitEffectHitSpawn, bullsquidspiteffecthitspawn)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitSpriteScale, bullsquidspitspritescale)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(bullsquidSpitAlphaScale, bullsquidspitalphascale)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(scientistBravery, scientistbravery)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barneyUnholsterTime, barneyunholstertime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barneyUnholsterAnimChoice, barneyunholsteranimchoice)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(trailTypeTest, trailtypetest, 104)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrail, hornettrail, 105)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrailSolidColor, hornettrailsolidcolor, 106)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModEasy, hornetdeathmodeasy, 107)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModMedium, hornetdeathmodmedium, 108)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModHard, hornetdeathmodhard, 109)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetZoomPuff, hornetzoompuff, 110)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpiral, hornetspiral, 111)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedMulti, hornetspeedmulti, 112)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedDartMulti, hornetspeeddartmulti, 113)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaInterval, rockettrailalphainterval, 114)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaScale, rockettrailalphascale, 115)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketSkipIgnite, rocketskipignite, 116)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(agruntHornetRandomness, agrunthornetrandomness)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hornetSpiralPeriod, hornetspiralperiod)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hornetSpiralAmplitude, hornetspiralamplitude)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly, gauss_primaryonly, 117)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_reflectdealsdamage, gauss_reflectdealsdamage, 118)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay, gauss_chargeanimdelay, 119)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay, gauss_chargeworkdelay, 120)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq, gauss_secondarychargetimereq, 121)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects, gauss_primaryreflects, 122)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces, gauss_primarypierces, 123)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects, gauss_secondaryreflects, 124)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces, gauss_secondarypierces, 125)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough, gauss_primarypunchthrough, 126)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough, gauss_secondarypunchthrough, 127)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay, gauss_betweenattackdelay, 128)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay, gauss_secondarychargemindelay, 129)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP, gauss_chargemaxammo_sp, 130)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP, gauss_chargemaxammo_mp, 131)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP, gauss_chargeinterval_sp, 132)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP, gauss_chargeinterval_mp, 133)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgMin, painflashdmgmin, 134)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgExMult, painflashdmgexmult, 135)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMinDrowning, painflashcumulativemindrowning, 136)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMax, painflashcumulativemax, 137)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrawOpacityMax, painflashdrawopacitymax, 138)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMin, painarrowdrawopacitymin, 139)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMax, painarrowdrawopacitymax, 140)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashFadeMult, painflashfademult, 141)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowFadeMult, painarrowfademult, 142)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(painFlashArmorBlock, painflasharmorblock, 143)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDirTolerance, painflashdirtolerance, 144)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeAppearMin, painarrowcumulativeappearmin, 145)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeDmgJump, painarrowcumulativedmgjump, 146)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashPrintouts, painflashprintouts, 147)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashCumulativeJump, itemflashcumulativejump, 148)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMin, itemflashdrawopacitymin, 149)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMax, itemflashdrawopacitymax, 150)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashFadeMult, itemflashfademult, 151)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(chumtoadPrintout, chumtoadprintout)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowReloadSoundDelay, crossbowreloadsounddelay, 152)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowFirePlaysReloadSound, crossbowfireplaysreloadsound, 153)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindFidgetFailTime, pathfindfidgetfailtime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindPrintout, pathfindprintout)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindTopRampFixDistance, pathfindtoprampfixdistance)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindTopRampFixDraw, pathfindtoprampfixdraw)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(iHaveAscended, ihaveascended, 154)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindLooseMapNodes, pathfindloosemapnodes)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindRampFix, pathfindrampfix)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(chumtoadPlayDeadFoolChance, chumtoadplaydeadfoolchance)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(animationFramerateMulti, animationframeratemulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindNodeToleranceMulti, pathfindnodetolerancemulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(friendlyPianoFollowVolume, friendlypianofollowvolume)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(friendlyPianoOtherVolume, friendlypianoothervolume)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(showtriggers, showtriggers)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(tentacleAlertSound, tentaclealertsound)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(tentacleSwingSound1, tentacleswingsound1)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(tentacleSwingSound2, tentacleswingsound2)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerFollowerMax, playerfollowermax)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(announcerIsAJerk, announcerisajerk)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerUseDrawDebug, playerusedrawdebug)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerChumtoadThrowDrawDebug, playerchumtoadthrowdrawdebug)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(peaceOut, peaceout)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(drawViewModel, drawviewmodel, 155)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(drawHUD, drawhud, 156)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(disablePauseSinglePlayer, disablepausesingleplayer)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBulletHitEffectForceServer, playerbullethiteffectforceserver, 157)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceAllowServersideTextureSounds, forceallowserversidetexturesounds, 158)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponSpreadMode, playerweaponspreadmode, 159)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(monsterAIForceFindDistance, monsteraiforcefinddistance)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(baseEntityDamagePushNormalMulti, baseentitydamagepushnormalmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(baseEntityDamagePushVerticalBoost, baseentitydamagepushverticalboost)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(baseEntityDamagePushVerticalMulti, baseentitydamagepushverticalmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(baseEntityDamagePushVerticalMinimum, baseentitydamagepushverticalminimum)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts, viewmodelprintouts, 160)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelSyncFixPrintouts, viewmodelsyncfixprintouts, 161)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(textureHitSoundPrintouts, texturehitsoundprintouts, 162)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hgruntAllowGrenades, hgruntallowgrenades)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(scheduleInterruptPrintouts, scheduleinterruptprintouts)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(animationPrintouts, animationprintouts)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassaultMeleeAttackEnabled, hassaultmeleeattackenabled)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindStumpedWaitTime, pathfindstumpedwaittime)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindStumpedMode, pathfindstumpedmode)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindStumpedForgetEnemy, pathfindstumpedforgetenemy)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindEdgeCheck, pathfindedgecheck)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(RadiusDamageDrawDebug, radiusdamagedrawdebug)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(AlienRadiationImmunity, alienradiationimmunity)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(customLogoSprayMode, customlogospraymode)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(monsterFadeOutRate, monsterfadeoutrate)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(playerFadeOutRate, playerfadeoutrate)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawDebugEnemyLKP, drawdebugenemylkp)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(trackchangePrintouts, trackchangeprintouts)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(trackTrainPrintouts, tracktrainprintouts)\
	DUMMY\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponTracerMode, playerweapontracermode, 164)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(monsterWeaponTracerMode, monsterweapontracermode)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(decalTracerExclusivity, decaltracerexclusivity, 165)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(monsterToPlayerHitgroupSpecial, monstertoplayerhitgroupspecial)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(multiplayerCrowbarHitSoundMode, multiplayercrowbarhitsoundmode)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin, healthcolor_fullredmin, 166)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(healthcolor_brightness, healthcolor_brightness, 167)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark, healthcolor_yellowmark, 168)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros, cl_drawextrazeros, 169)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(pathfindLargeBoundFix, pathfindlargeboundfix)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(flyerKilledFallingLoop, flyerkilledfallingloop)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(floaterDummy, floaterdummy)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barneyDummy, barneydummy)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(ladderCycleMulti, laddercyclemulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(ladderSpeedMulti, ladderspeedmulti)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(barnacleGrabNoInterpolation, barnaclegrabnointerpolation)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hideDamage, hidedamage, 170)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax, timeddamage_brightnessmax, 171)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin, timeddamage_brightnessmin, 172)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap, timeddamage_brightnesscap, 173)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor, timeddamage_brightnessfloor, 174)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed, timeddamage_flashspeed, 175)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTONLY_DEBUGONLY(timedDamage_debug, timeddamage_debug, 176)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(wallHealthDoor_closeDelay, wallhealthdoor_closedelay)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(houndeye_attack_canGib, houndeye_attack_cangib)\
	EASY_CVAR_HIDDEN_ACCESS_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myRocketsAreBarney, myrocketsarebarney, 177)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(hassassinCrossbowDebug, hassassincrossbowdebug)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(crossbowBoltDirectionAffectedByWater, crossbowboltdirectionaffectedbywater)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(kingpinDebug, kingpindebug)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(minimumRespawnDelay, minimumrespawndelay)\
	EASY_CVAR_HIDDEN_ACCESS_DEBUGONLY(drawDebugCrowbar, drawdebugcrowbar)\
	DUMMY


#define EASY_CVAR_HIDDEN_SAVE_MASS\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(strobeDurationMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(strobeDurationMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(strobeRadiusMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(strobeRadiusMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(strobeSpawnDistHori)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(strobeMultiColor)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserEnabled)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserSpawnFreq)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserLength)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserBrightnessMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserBrightnessMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserDurationMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserDurationMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserThicknessMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserThicknessMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserNoiseMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserNoiseMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserFrameRateMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserFrameRateMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveLaserMultiColor)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraPosFixedX)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraPosFixedY)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraPosFixedZ)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraPosOffX)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraPosOffY)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraPosOffZ)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraRotFixedX)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraRotFixedY)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraRotFixedZ)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraRotOffX)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraRotOffY)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cameraRotOffZ)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(imAllFuckedUp)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(thatWasntGrass)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(thatWasntPunch)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(fogNear)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(fogFar)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(NPCsTalkMore)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(myCameraSucks)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(shutupstuka)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultSpinMovement)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultIdleSpinSound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultFireSound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(muteBulletHitSounds)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(mutePlayerPainSounds)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultFireSoundChannel)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(geigerChannel)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultWaitTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultResidualAttackTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultSpinupStartTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultVoicePitchMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultVoicePitchMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultFireSpread)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(houndeyeAttackMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(muteRicochetSound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(mutePlayerWeaponFire)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(muteCrowbarSounds)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(scientistHealNPC)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(scientistHealNPCDebug)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bulletholeAlertRange)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(fleshhitmakessound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(nothingHurts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(seeMonsterHealth)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(scientistHealNPCFract)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidRangeDisabled)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(applyLKPPathFixToAll)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(scientistHealCooldown)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(crazyMonsterPrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(movementIsCompletePrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barneyPrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(monsterSpawnPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(zombieBulletResistance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(zombieBulletPushback)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(houndeyePrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(quakeExplosionSound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(noFlinchOnHard)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultDrawLKP)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(meleeDrawBloodModeA)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(meleeDrawBloodModeB)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawDebugBloodTrace)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntHeadshotGore)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntTinyClip)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(agrunt_muzzleflash)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(event5011Allowed)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(event5021Allowed)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(event5031Allowed)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(event5002Allowed)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(event5004Allowed)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(eventsAreFabulous)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(glockOldReloadLogic)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(islaveReviveFriendMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(islaveReviveFriendChance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(islaveReviveFriendRange)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(islaveReviveSelfChance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(testVar)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painArrowColorMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashColorMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashSuitless)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashDrownMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(forceDrawBatteryNumber)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(offsetgivedistance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(offsetgivelookvertical)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(endlessFlashlightBattery)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(preE3UsesFailColors)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(E3UsesFailColors)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(E3ShowsDamageIcons)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerCrossbowMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassassinCrossbowMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cheat_infiniteclip)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cheat_infiniteammo)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cheat_minimumfiredelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cheat_nogaussrecoil)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(autoSneaky)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(infiniteLongJumpCharge)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawDebugPathfinding)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUcheckDistH)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUcheckDistV)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUcheckDistD)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUextraTriangH)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUextraTriangV)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUrepelMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUExplodeTest)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUYawSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(STUDetection)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(stukaAdvancedCombat)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawDebugPathfinding2)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultFriendlyFire)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(myStrobe)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(peopleStrobe)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(forceWorldLightOff)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(wildHeads)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(raveEffectSpawnInterval)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawBarnacleDebug)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(fogTest)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksAllMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksEnvMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksButtonMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksComputerHitMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksTurretDeathMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksOspreyHitMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksExplosionMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksBeamMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sparksAIFailMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(normalSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(noclipSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(jumpForceMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(apacheForceCinBounds)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(apacheBottomBoundAdj)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(panthereyeJumpDotTol)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(panthereyePrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gargantuaPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(squadmonsterPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barnaclePrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(friendlyPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(stukaPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(altSquadRulesRuntime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntLockStrafeTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(ignoreIsolatedNodes)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawNodeAll)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawNodeSpecial)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawNodeConnections)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawNodeAlternateTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(blockChangeLevelTrigger)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(blockMusicTrigger)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(blockMultiTrigger)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(blockTeleportTrigger)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(blockHurtTrigger)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(blockAutosaveTrigger)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sv_germancensorship)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(allowGermanModels)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(germanRobotGibs)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(germanRobotBleedsOil)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(germanRobotDamageDecal)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(germanRobotGibsDecal)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(egonEffectsMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barnacleCanGib)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(sentryCanGib)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(miniturretCanGib)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(turretCanGib)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(turretBleedsOil)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(turretDamageDecal)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(turretGibDecal)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(canDropInSinglePlayer)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(itemBatteryPrerequisite)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(batteryDrainsAtDeath)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(canTakeLongJump)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(printOutCommonTimables)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerBrightLight)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(disablePainPunchAutomatic)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gargantuaCorpseDeath)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gargantuaFallSound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gargantuaBleeds)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(shrapMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(shrapRand)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(shrapRandHeightExtra)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(explosionShrapnelMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(useAlphaSparks)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(emergencyFix)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(glockUseLastBulletAnim)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(leaderlessSquadAllowed)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerReviveBuddhaMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultBulletsPerShot)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cheat_iwantguts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(nodeDetailPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(soundAttenuationStuka)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(soundVolumeStuka)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cineChangelevelFix)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawDebugCine)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(stukaInflictsBleeding)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(scientistBravery)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barneyUnholsterTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(trailTypeTest)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetTrail)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetTrailSolidColor)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetDeathModEasy)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetDeathModMedium)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetDeathModHard)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetZoomPuff)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetSpiral)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetSpeedDartMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(rocketTrailAlphaScale)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(rocketSkipIgnite)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(agruntHornetRandomness)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetSpiralPeriod)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hornetSpiralAmplitude)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_primaryonly)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_chargeanimdelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_chargeworkdelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_primaryreflects)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_primarypierces)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_secondaryreflects)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_secondarypierces)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_primarypunchthrough)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_betweenattackdelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashDmgMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashDmgExMult)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashCumulativeMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashFadeMult)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painArrowFadeMult)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(painFlashArmorBlock)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashDirTolerance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(painFlashPrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(itemFlashCumulativeJump)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(itemFlashFadeMult)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(chumtoadPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindFidgetFailTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindPrintout)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(iHaveAscended)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindLooseMapNodes)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindRampFix)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(animationFramerateMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(showtriggers)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(tentacleAlertSound)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(tentacleSwingSound1)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(tentacleSwingSound2)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerFollowerMax)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(announcerIsAJerk)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerUseDrawDebug)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(peaceOut)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(drawViewModel)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(drawHUD)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(disablePauseSinglePlayer)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerWeaponSpreadMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(monsterAIForceFindDistance)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(viewModelPrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(textureHitSoundPrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hgruntAllowGrenades)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(animationPrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindStumpedMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindEdgeCheck)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(AlienRadiationImmunity)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(customLogoSprayMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(monsterFadeOutRate)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerFadeOutRate)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawDebugEnemyLKP)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(trackchangePrintouts)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(trackTrainPrintouts)\
	DUMMY\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(playerWeaponTracerMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(monsterWeaponTracerMode)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(decalTracerExclusivity)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(healthcolor_fullRedMin)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(healthcolor_brightness)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(healthcolor_yellowMark)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(cl_drawExtraZeros)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(pathfindLargeBoundFix)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(flyerKilledFallingLoop)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(floaterDummy)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barneyDummy)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(ladderCycleMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(ladderSpeedMulti)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hideDamage)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(timedDamage_brightnessMax)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(timedDamage_brightnessMin)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(timedDamage_brightnessCap)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(timedDamage_flashSpeed)\
	EASY_CVAR_HIDDEN_SAVE_CLIENTONLY(timedDamage_debug)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(houndeye_attack_canGib)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(myRocketsAreBarney)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(hassassinCrossbowDebug)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(kingpinDebug)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(minimumRespawnDelay)\
	EASY_CVAR_HIDDEN_SAVE_SERVERONLY(drawDebugCrowbar)\
	DUMMY

#define EASY_CVAR_HIDDEN_LOAD_MASS\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gruntsCanHaveMP5Grenade, gruntscanhavemp5grenade)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(strobeDurationMin, strobedurationmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(strobeDurationMax, strobedurationmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(strobeRadiusMin, stroberadiusmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(strobeRadiusMax, stroberadiusmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(strobeSpawnDistHori, strobespawndisthori)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(strobeSpawnDistVertMin, strobespawndistvertmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(strobeSpawnDistVertMax, strobespawndistvertmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(strobeMultiColor, strobemulticolor)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserEnabled, ravelaserenabled)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserSpawnFreq, ravelaserspawnfreq)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserLength, ravelaserlength)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserSpawnDistHoriMin, ravelaserspawndisthorimin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserSpawnDistHoriMax, ravelaserspawndisthorimax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserSpawnDistVertMin, ravelaserspawndistvertmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserSpawnDistVertMax, ravelaserspawndistvertmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserBrightnessMin, ravelaserbrightnessmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserBrightnessMax, ravelaserbrightnessmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserDurationMin, ravelaserdurationmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserDurationMax, ravelaserdurationmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserThicknessMin, ravelaserthicknessmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserThicknessMax, ravelaserthicknessmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserNoiseMin, ravelasernoisemin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserNoiseMax, ravelasernoisemax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserFrameRateMin, ravelaserframeratemin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserFrameRateMax, ravelaserframeratemax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveLaserMultiColor, ravelasermulticolor)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraPosFixedX, cameraposfixedx)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraPosFixedY, cameraposfixedy)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraPosFixedZ, cameraposfixedz)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraPosOffX, cameraposoffx)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraPosOffY, cameraposoffy)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraPosOffZ, cameraposoffz)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraRotFixedX, camerarotfixedx)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraRotFixedY, camerarotfixedy)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraRotFixedZ, camerarotfixedz)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraRotOffX, camerarotoffx)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraRotOffY, camerarotoffy)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cameraRotOffZ, camerarotoffz)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(imAllFuckedUp, imallfuckedup)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(thatWasntGrass, thatwasntgrass)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(thatWasntPunch, thatwasntpunch)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(fogNear, fognear)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(fogFar, fogfar)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(NPCsTalkMore, npcstalkmore)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(myCameraSucks, mycamerasucks)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(shutupstuka, shutupstuka)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultSpinMovement, hassaultspinmovement)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultIdleSpinSound, hassaultidlespinsound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultFireSound, hassaultfiresound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(muteBulletHitSounds, mutebullethitsounds)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(mutePlayerPainSounds, muteplayerpainsounds)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultIdleSpinSoundChannel, hassaultidlespinsoundchannel)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultSpinUpDownSoundChannel, hassaultspinupdownsoundchannel)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultFireSoundChannel, hassaultfiresoundchannel)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(geigerChannel, geigerchannel)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultWaitTime, hassaultwaittime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultSpinupRemainTime, hassaultspinupremaintime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultResidualAttackTime, hassaultresidualattacktime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultSpinupStartTime, hassaultspinupstarttime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultVoicePitchMin, hassaultvoicepitchmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultVoicePitchMax, hassaultvoicepitchmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultFireSpread, hassaultfirespread)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(muteTempEntityGroundHitSound, mutetempentitygroundhitsound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(houndeyeAttackMode, houndeyeattackmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(muteRicochetSound, mutericochetsound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(mutePlayerWeaponFire, muteplayerweaponfire)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(muteCrowbarSounds, mutecrowbarsounds)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(scientistHealNPC, scientisthealnpc)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(scientistHealNPCDebug, scientisthealnpcdebug)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bulletholeAlertRange, bulletholealertrange)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(fleshhitmakessound, fleshhitmakessound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(nothingHurts, nothinghurts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(seeMonsterHealth, seemonsterhealth)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(scientistHealNPCFract, scientisthealnpcfract)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidRangeDisabled, bullsquidrangedisabled)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(applyLKPPathFixToAll, applylkppathfixtoall)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(timedDamageAffectsMonsters, timeddamageaffectsmonsters)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(scientistHealCooldown, scientisthealcooldown)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(crazyMonsterPrintouts, crazymonsterprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(movementIsCompletePrintout, movementiscompleteprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bulletHoleAlertPrintout, bulletholealertprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bulletholeAlertStukaOnly, bulletholealertstukaonly)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barneyPrintouts, barneyprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(monsterSpawnPrintout, monsterspawnprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(zombieBulletResistance, zombiebulletresistance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(zombieBulletPushback, zombiebulletpushback)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(houndeyePrintout, houndeyeprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(quakeExplosionSound, quakeexplosionsound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(explosionDebrisSoundVolume, explosiondebrissoundvolume)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(noFlinchOnHard, noflinchonhard)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultDrawLKP, hassaultdrawlkp)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(meleeDrawBloodModeA, meleedrawbloodmodea)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(meleeDrawBloodModeB, meleedrawbloodmodeb)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawDebugBloodTrace, drawdebugbloodtrace)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(meleeDrawBloodModeBFix, meleedrawbloodmodebfix)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(meleeDrawBloodModeAOffset, meleedrawbloodmodeaoffset)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(meleeDrawBloodModeBOffset, meleedrawbloodmodeboffset)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(panthereyeHasCloakingAbility, panthereyehascloakingability)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntSpeedMulti, hgruntspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntForceStrafeFireAnim, hgruntforcestrafefireanim)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntLockRunAndGunTime, hgruntlockrunandguntime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntHeadshotGore, hgruntheadshotgore)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntAllowStrafeFire, hgruntallowstrafefire)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(thoroughHitBoxUpdates, thoroughhitboxupdates)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntTinyClip, hgrunttinyclip)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntStrafeAlwaysHasAmmo, hgruntstrafealwayshasammo)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntBrassEjectForwardOffset, hgruntbrassejectforwardoffset)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(agrunt_muzzleflash, agrunt_muzzleflash)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(event5011Allowed, event5011allowed)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(event5021Allowed, event5021allowed)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(event5031Allowed, event5031allowed)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(event5002Allowed, event5002allowed)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(event5004Allowed, event5004allowed)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(eventsAreFabulous, eventsarefabulous)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(glockOldReloadLogic, glockoldreloadlogic)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(glockOldReloadLogicBarney, glockoldreloadlogicbarney)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barneyDroppedGlockAmmoCap, barneydroppedglockammocap)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawCollisionBoundsAtDeath, drawcollisionboundsatdeath)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawHitBoundsAtDeath, drawhitboundsatdeath)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(islaveReviveFriendMode, islaverevivefriendmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(islaveReviveFriendChance, islaverevivefriendchance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(islaveReviveFriendRange, islaverevivefriendrange)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(islaveReviveSelfMinDelay, islavereviveselfmindelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(islaveReviveSelfMaxDelay, islavereviveselfmaxdelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(islaveReviveSelfChance, islavereviveselfchance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntRunAndGunDistance, hgruntrunandgundistance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntPrintout, hgruntprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(testVar, testvar)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painArrowColorMode, painarrowcolormode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashColorMode, painflashcolormode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashSuitless, painflashsuitless)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashDrownMode, painflashdrownmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(firstPersonIdleDelayMin, firstpersonidledelaymin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(firstPersonIdleDelayMax, firstpersonidledelaymax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(forceDrawBatteryNumber, forcedrawbatterynumber)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(offsetgivedistance, offsetgivedistance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(offsetgivelookvertical, offsetgivelookvertical)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(canShowWeaponSelectAtDeath, canshowweaponselectatdeath)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(endlessFlashlightBattery, endlessflashlightbattery)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(preE3UsesFailColors, pree3usesfailcolors)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(E3UsesFailColors, e3usesfailcolors)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(preE3ShowsDamageIcons, pree3showsdamageicons)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(E3ShowsDamageIcons, e3showsdamageicons)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerCrossbowMode, playercrossbowmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassassinCrossbowMode, hassassincrossbowmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(tripmineAnimWaitsForFinish, tripmineanimwaitsforfinish)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(revolverLaserScope, revolverlaserscope)\
	DUMMY\
	DUMMY\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cheat_infiniteclip, cheat_infiniteclip)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cheat_infiniteammo, cheat_infiniteammo)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cheat_minimumfiredelay, cheat_minimumfiredelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cheat_minimumfiredelaycustom, cheat_minimumfiredelaycustom)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cheat_nogaussrecoil, cheat_nogaussrecoil)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(autoSneaky, autosneaky)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(infiniteLongJumpCharge, infinitelongjumpcharge)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cheat_touchNeverExplodes, cheat_touchneverexplodes)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gaussRecoilSendsUpInSP, gaussrecoilsendsupinsp)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawDebugPathfinding, drawdebugpathfinding)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUcheckDistH, stucheckdisth)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUcheckDistV, stucheckdistv)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUcheckDistD, stucheckdistd)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUextraTriangH, stuextratriangh)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUextraTriangV, stuextratriangv)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUrepelMulti, sturepelmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUSpeedMulti, stuspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUExplodeTest, stuexplodetest)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUYawSpeedMulti, stuyawspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(STUDetection, studetection)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(stukaAdvancedCombat, stukaadvancedcombat)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawDebugPathfinding2, drawdebugpathfinding2)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultFriendlyFire, hassaultfriendlyfire)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(myStrobe, mystrobe)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(peopleStrobe, peoplestrobe)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(forceWorldLightOff, forceworldlightoff)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(wildHeads, wildheads)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(raveEffectSpawnInterval, raveeffectspawninterval)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawBarnacleDebug, drawbarnacledebug)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(fogTest, fogtest)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksAllMulti, sparksallmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksEnvMulti, sparksenvmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksButtonMulti, sparksbuttonmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksPlayerCrossbowMulti, sparksplayercrossbowmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksComputerHitMulti, sparkscomputerhitmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksTurretDeathMulti, sparksturretdeathmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksOspreyHitMulti, sparksospreyhitmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksExplosionMulti, sparksexplosionmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksBeamMulti, sparksbeammulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sparksAIFailMulti, sparksaifailmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(normalSpeedMulti, normalspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(noclipSpeedMulti, noclipspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(jumpForceMulti, jumpforcemulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(apacheForceCinBounds, apacheforcecinbounds)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(apacheBottomBoundAdj, apachebottomboundadj)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(apacheInfluence, apacheinfluence)\
	DUMMY\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(allowAlphaCrosshairWithoutGuns, allowalphacrosshairwithoutguns)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(alphaCrosshairBlockedOnFrozen, alphacrosshairblockedonfrozen)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntRunAndGunDotMin, hgruntrunandgundotmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(panthereyeJumpDotTol, panthereyejumpdottol)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(panthereyePrintout, panthereyeprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gargantuaPrintout, gargantuaprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(squadmonsterPrintout, squadmonsterprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultPrintout, hassaultprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barnaclePrintout, barnacleprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(friendlyPrintout, friendlyprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(stukaPrintout, stukaprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(timedDamageEndlessOnHard, timeddamageendlessonhard)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(mirrorsReflectOnlyNPCs, mirrorsreflectonlynpcs)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(mirrorsDoNotReflectPlayer, mirrorsdonotreflectplayer)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(altSquadRulesRuntime, altsquadrulesruntime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntLockStrafeTime, hgruntlockstrafetime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(ignoreIsolatedNodes, ignoreisolatednodes)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawNodeAll, drawnodeall)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawNodeSpecial, drawnodespecial)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawNodeConnections, drawnodeconnections)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawNodeAlternateTime, drawnodealternatetime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(nodeSearchStartVerticalOffset, nodesearchstartverticaloffset)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(blockChangeLevelTrigger, blockchangeleveltrigger)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(blockMusicTrigger, blockmusictrigger)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(blockMultiTrigger, blockmultitrigger)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(blockTeleportTrigger, blockteleporttrigger)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(blockHurtTrigger, blockhurttrigger)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(blockAutosaveTrigger, blockautosavetrigger)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hideNodeGraphRebuildNotice, hidenodegraphrebuildnotice)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barnacleTongueRetractDelay, barnacletongueretractdelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sv_germancensorship, sv_germancensorship)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(allowGermanModels, allowgermanmodels)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(germanRobotGibs, germanrobotgibs)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(germanRobotBleedsOil, germanrobotbleedsoil)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(germanRobotDamageDecal, germanrobotdamagedecal)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(germanRobotGibsDecal, germanrobotgibsdecal)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(egonEffectsMode, egoneffectsmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(egonHitCloud, egonhitcloud)\
	DUMMY\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(handGrenadePickupYieldsOne, handgrenadepickupyieldsone)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(handGrenadesUseOldBounceSound, handgrenadesuseoldbouncesound)\
	DUMMY\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(weaponSelectSoundPlayOnMousewheel, weaponselectsoundplayonmousewheel)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(timedDamageDeathRemoveMode, timeddamagedeathremovemode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(weaponSelectUsesReloadSounds, weaponselectusesreloadsounds)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barnacleCanGib, barnaclecangib)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(sentryCanGib, sentrycangib)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(miniturretCanGib, miniturretcangib)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(turretCanGib, turretcangib)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(turretBleedsOil, turretbleedsoil)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(turretDamageDecal, turretdamagedecal)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(turretGibDecal, turretgibdecal)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(canDropInSinglePlayer, candropinsingleplayer)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(timedDamageIgnoresArmor, timeddamageignoresarmor)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(itemBatteryPrerequisite, itembatteryprerequisite)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerExtraPainSoundsMode, playerextrapainsoundsmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(timedDamageDisableViewPunch, timeddamagedisableviewpunch)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(batteryDrainsAtDeath, batterydrainsatdeath)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(batteryDrainsAtAdrenalineMode, batterydrainsatadrenalinemode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(canTakeLongJump, cantakelongjump)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(printOutCommonTimables, printoutcommontimables)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerBrightLight, playerbrightlight)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(disablePainPunchAutomatic, disablepainpunchautomatic)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gargantuaCorpseDeath, gargantuacorpsedeath)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gargantuaFallSound, gargantuafallsound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gargantuaBleeds, gargantuableeds)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(shrapMode, shrapmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(shrapRand, shraprand)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(shrapRandHeightExtra, shraprandheightextra)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(explosionShrapnelMulti, explosionshrapnelmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(useAlphaSparks, usealphasparks)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(emergencyFix, emergencyfix)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(timedDamageReviveRemoveMode, timeddamagereviveremovemode)\
	DUMMY\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(ospreyIgnoresGruntCount, ospreyignoresgruntcount)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(mp5GrenadeInheritsPlayerVelocity, mp5grenadeinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(crossbowInheritsPlayerVelocity, crossbowinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(fastHornetsInheritsPlayerVelocity, fasthornetsinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(snarkInheritsPlayerVelocity, snarkinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(chumtoadInheritsPlayerVelocity, chumtoadinheritsplayervelocity)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(weaponPickupPlaysAnyReloadSounds, weaponpickupplaysanyreloadsounds)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(glockUseLastBulletAnim, glockuselastbulletanim)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerBarnacleVictimViewOffset, playerbarnaclevictimviewoffset)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntMovementDeltaCheck, hgruntmovementdeltacheck)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultExtraMuzzleFlashRadius, hassaultextramuzzleflashradius)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultExtraMuzzleFlashBrightness, hassaultextramuzzleflashbrightness)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultExtraMuzzleFlashForward, hassaultextramuzzleflashforward)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(leaderlessSquadAllowed, leaderlesssquadallowed)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(nodeConnectionBreakableCheck, nodeconnectionbreakablecheck)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerReviveInvincibilityTime, playerreviveinvincibilitytime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerReviveBuddhaMode, playerrevivebuddhamode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerReviveTimeBlocksTimedDamage, playerrevivetimeblockstimeddamage)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultBulletDamageMulti, hassaultbulletdamagemulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultBulletsPerShot, hassaultbulletspershot)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultFireAnimSpeedMulti, hassaultfireanimspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultMeleeAnimSpeedMulti, hassaultmeleeanimspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassassinCrossbowReloadSoundDelay, hassassincrossbowreloadsounddelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntStrafeAnimSpeedMulti, hgruntstrafeanimspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntRunAndGunAnimSpeedMulti, hgruntrunandgunanimspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cheat_iwantguts, cheat_iwantguts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(nodeDetailPrintout, nodedetailprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(soundAttenuationStuka, soundattenuationstuka)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(soundVolumeStuka, soundvolumestuka)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cineChangelevelFix, cinechangelevelfix)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawDebugCine, drawdebugcine)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(cineAllowSequenceOverwrite, cineallowsequenceoverwrite)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(stukaInflictsBleeding, stukainflictsbleeding)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(animationKilledBoundsRemoval, animationkilledboundsremoval)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gargantuaKilledBoundsAssist, gargantuakilledboundsassist)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitTrajTimeMin, bullsquidspittrajtimemin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitTrajTimeMax, bullsquidspittrajtimemax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitTrajDistMin, bullsquidspittrajdistmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitTrajDistMax, bullsquidspittrajdistmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitGravityMulti, bullsquidspitgravitymulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitUseAlphaModel, bullsquidspitusealphamodel)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitUseAlphaEffect, bullsquidspitusealphaeffect)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitEffectSpread, bullsquidspiteffectspread)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitEffectMin, bullsquidspiteffectmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitEffectMax, bullsquidspiteffectmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitEffectHitMin, bullsquidspiteffecthitmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitEffectHitMax, bullsquidspiteffecthitmax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitEffectSpawn, bullsquidspiteffectspawn)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitEffectHitSpawn, bullsquidspiteffecthitspawn)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitSpriteScale, bullsquidspitspritescale)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(bullsquidSpitAlphaScale, bullsquidspitalphascale)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(scientistBravery, scientistbravery)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barneyUnholsterTime, barneyunholstertime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barneyUnholsterAnimChoice, barneyunholsteranimchoice)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(trailTypeTest, trailtypetest)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetTrail, hornettrail)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetTrailSolidColor, hornettrailsolidcolor)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetDeathModEasy, hornetdeathmodeasy)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetDeathModMedium, hornetdeathmodmedium)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetDeathModHard, hornetdeathmodhard)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetZoomPuff, hornetzoompuff)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetSpiral, hornetspiral)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetSpeedMulti, hornetspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetSpeedDartMulti, hornetspeeddartmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(rocketTrailAlphaInterval, rockettrailalphainterval)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(rocketTrailAlphaScale, rockettrailalphascale)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(rocketSkipIgnite, rocketskipignite)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(agruntHornetRandomness, agrunthornetrandomness)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetSpiralPeriod, hornetspiralperiod)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hornetSpiralAmplitude, hornetspiralamplitude)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_primaryonly, gauss_primaryonly)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_reflectdealsdamage, gauss_reflectdealsdamage)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_chargeanimdelay, gauss_chargeanimdelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_chargeworkdelay, gauss_chargeworkdelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_secondarychargetimereq, gauss_secondarychargetimereq)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_primaryreflects, gauss_primaryreflects)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_primarypierces, gauss_primarypierces)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_secondaryreflects, gauss_secondaryreflects)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_secondarypierces, gauss_secondarypierces)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_primarypunchthrough, gauss_primarypunchthrough)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_secondarypunchthrough, gauss_secondarypunchthrough)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_betweenattackdelay, gauss_betweenattackdelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_secondarychargemindelay, gauss_secondarychargemindelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_chargeMaxAmmo_SP, gauss_chargemaxammo_sp)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_chargeMaxAmmo_MP, gauss_chargemaxammo_mp)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_chargeInterval_SP, gauss_chargeinterval_sp)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(gauss_chargeInterval_MP, gauss_chargeinterval_mp)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashDmgMin, painflashdmgmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashDmgExMult, painflashdmgexmult)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashCumulativeMinDrowning, painflashcumulativemindrowning)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashCumulativeMax, painflashcumulativemax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashDrawOpacityMax, painflashdrawopacitymax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painArrowDrawOpacityMin, painarrowdrawopacitymin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painArrowDrawOpacityMax, painarrowdrawopacitymax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashFadeMult, painflashfademult)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painArrowFadeMult, painarrowfademult)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(painFlashArmorBlock, painflasharmorblock)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashDirTolerance, painflashdirtolerance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painArrowCumulativeAppearMin, painarrowcumulativeappearmin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painArrowCumulativeDmgJump, painarrowcumulativedmgjump)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(painFlashPrintouts, painflashprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(itemFlashCumulativeJump, itemflashcumulativejump)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(itemFlashDrawOpacityMin, itemflashdrawopacitymin)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(itemFlashDrawOpacityMax, itemflashdrawopacitymax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(itemFlashFadeMult, itemflashfademult)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(chumtoadPrintout, chumtoadprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(crossbowReloadSoundDelay, crossbowreloadsounddelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(crossbowFirePlaysReloadSound, crossbowfireplaysreloadsound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindFidgetFailTime, pathfindfidgetfailtime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindPrintout, pathfindprintout)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindTopRampFixDistance, pathfindtoprampfixdistance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindTopRampFixDraw, pathfindtoprampfixdraw)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(iHaveAscended, ihaveascended)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindLooseMapNodes, pathfindloosemapnodes)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindRampFix, pathfindrampfix)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(chumtoadPlayDeadFoolChance, chumtoadplaydeadfoolchance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(animationFramerateMulti, animationframeratemulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindNodeToleranceMulti, pathfindnodetolerancemulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(friendlyPianoFollowVolume, friendlypianofollowvolume)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(friendlyPianoOtherVolume, friendlypianoothervolume)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(showtriggers, showtriggers)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(tentacleAlertSound, tentaclealertsound)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(tentacleSwingSound1, tentacleswingsound1)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(tentacleSwingSound2, tentacleswingsound2)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerFollowerMax, playerfollowermax)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(announcerIsAJerk, announcerisajerk)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerUseDrawDebug, playerusedrawdebug)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerChumtoadThrowDrawDebug, playerchumtoadthrowdrawdebug)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(peaceOut, peaceout)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(drawViewModel, drawviewmodel)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(drawHUD, drawhud)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(disablePauseSinglePlayer, disablepausesingleplayer)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerBulletHitEffectForceServer, playerbullethiteffectforceserver)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(forceAllowServersideTextureSounds, forceallowserversidetexturesounds)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerWeaponSpreadMode, playerweaponspreadmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(monsterAIForceFindDistance, monsteraiforcefinddistance)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(baseEntityDamagePushNormalMulti, baseentitydamagepushnormalmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(baseEntityDamagePushVerticalBoost, baseentitydamagepushverticalboost)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(baseEntityDamagePushVerticalMulti, baseentitydamagepushverticalmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(baseEntityDamagePushVerticalMinimum, baseentitydamagepushverticalminimum)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(viewModelPrintouts, viewmodelprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(viewModelSyncFixPrintouts, viewmodelsyncfixprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(textureHitSoundPrintouts, texturehitsoundprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hgruntAllowGrenades, hgruntallowgrenades)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(scheduleInterruptPrintouts, scheduleinterruptprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(animationPrintouts, animationprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassaultMeleeAttackEnabled, hassaultmeleeattackenabled)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindStumpedWaitTime, pathfindstumpedwaittime)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindStumpedMode, pathfindstumpedmode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindStumpedForgetEnemy, pathfindstumpedforgetenemy)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindEdgeCheck, pathfindedgecheck)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(RadiusDamageDrawDebug, radiusdamagedrawdebug)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(AlienRadiationImmunity, alienradiationimmunity)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(customLogoSprayMode, customlogospraymode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(monsterFadeOutRate, monsterfadeoutrate)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerFadeOutRate, playerfadeoutrate)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawDebugEnemyLKP, drawdebugenemylkp)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(trackchangePrintouts, trackchangeprintouts)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(trackTrainPrintouts, tracktrainprintouts)\
	DUMMY\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(playerWeaponTracerMode, playerweapontracermode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(monsterWeaponTracerMode, monsterweapontracermode)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(decalTracerExclusivity, decaltracerexclusivity)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(monsterToPlayerHitgroupSpecial, monstertoplayerhitgroupspecial)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(multiplayerCrowbarHitSoundMode, multiplayercrowbarhitsoundmode)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(healthcolor_fullRedMin, healthcolor_fullredmin)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(healthcolor_brightness, healthcolor_brightness)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(healthcolor_yellowMark, healthcolor_yellowmark)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(cl_drawExtraZeros, cl_drawextrazeros)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(pathfindLargeBoundFix, pathfindlargeboundfix)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(flyerKilledFallingLoop, flyerkilledfallingloop)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(floaterDummy, floaterdummy)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barneyDummy, barneydummy)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(ladderCycleMulti, laddercyclemulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(ladderSpeedMulti, ladderspeedmulti)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(barnacleGrabNoInterpolation, barnaclegrabnointerpolation)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hideDamage, hidedamage)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(timedDamage_brightnessMax, timeddamage_brightnessmax)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(timedDamage_brightnessMin, timeddamage_brightnessmin)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(timedDamage_brightnessCap, timeddamage_brightnesscap)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(timedDamage_brightnessFloor, timeddamage_brightnessfloor)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(timedDamage_flashSpeed, timeddamage_flashspeed)\
	EASY_CVAR_HIDDEN_LOAD_CLIENTONLY(timedDamage_debug, timeddamage_debug)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(wallHealthDoor_closeDelay, wallhealthdoor_closedelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(houndeye_attack_canGib, houndeye_attack_cangib)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(myRocketsAreBarney, myrocketsarebarney)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(hassassinCrossbowDebug, hassassincrossbowdebug)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(crossbowBoltDirectionAffectedByWater, crossbowboltdirectionaffectedbywater)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(kingpinDebug, kingpindebug)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(minimumRespawnDelay, minimumrespawndelay)\
	EASY_CVAR_HIDDEN_LOAD_SERVERONLY(drawDebugCrowbar, drawdebugcrowbar)\
	DUMMY





#define EASY_CVAR_DECLARATION_SERVER_MASS\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_DECLARATION_SERVER(hud_version)\
	EASY_CVAR_DECLARATION_SERVER(hud_batterydraw)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistHori)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeMultiColor)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserEnabled)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnFreq)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserLength)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserMultiColor)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedX)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedY)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedZ)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffX)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffY)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffZ)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedX)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedY)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedZ)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffX)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffY)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffZ)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(imAllFuckedUp)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntGrass)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntPunch)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogNear)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogFar)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myCameraSucks)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(shutupstuka)\
	EASY_CVAR_DECLARATION_SERVER(chromeEffect)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteBulletHitSounds)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(geigerChannel)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteRicochetSound)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mutePlayerWeaponFire)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteCrowbarSounds)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(nothingHurts)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(seeMonsterHealth)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST(wpn_glocksilencer)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_DECLARATION_SERVER(cl_muzzleflash)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5011Allowed)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5021Allowed)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5031Allowed)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5002Allowed)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5004Allowed)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(eventsAreFabulous)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockOldReloadLogic)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(testVar)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowColorMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashColorMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashSuitless)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrownMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceDrawBatteryNumber)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(endlessFlashlightBattery)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3UsesFailColors)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3UsesFailColors)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3ShowsDamageIcons)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerCrossbowMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(autoSneaky)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(STUDetection)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(myStrobe)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(wildHeads)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveEffectSpawnInterval)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogTest)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_DECLARATION_SERVER(r_shadows)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(sv_germancensorship)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(turretCanGib)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(shrapMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(shrapRand)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(emergencyFix)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockUseLastBulletAnim)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hiddenMemPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_DECLARATION_SERVER(cl_bullsquidspit)\
	EASY_CVAR_DECLARATION_SERVER(cl_bullsquidspitarc)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(scientistBravery)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(trailTypeTest)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrail)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrailSolidColor)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModEasy)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModMedium)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModHard)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetZoomPuff)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpiral)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedDartMulti)\
	EASY_CVAR_DECLARATION_SERVER(cl_rockettrail)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaScale)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketSkipIgnite)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_DECLARATION_SERVER(cl_hornetspiral)\
	EASY_CVAR_DECLARATION_SERVER(cl_hornettrail)\
	EASY_CVAR_DECLARATION_SERVER(hud_drawammobar)\
	EASY_CVAR_DECLARATION_SERVER(hud_weaponselecthideslower)\
	EASY_CVAR_DECLARATION_SERVER(hud_drawsidebarmode)\
	EASY_CVAR_DECLARATION_SERVER(gaussmode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgExMult)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashFadeMult)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowFadeMult)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(painFlashArmorBlock)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDirTolerance)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashPrintouts)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashCumulativeJump)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashFadeMult)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(iHaveAscended)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(showtriggers)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(peaceOut)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(drawViewModel)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(drawHUD)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponSpreadMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(textureHitSoundPrintouts)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_DECLARATION_SERVER(cl_holster)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponTracerMode)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(decalTracerExclusivity)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(healthcolor_brightness)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(floaterDummy)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barneyDummy)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hideDamage)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_debug)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_DECLARATION_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myRocketsAreBarney)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_DECLARATION_SERVER(r_glowshell_debug)\
	EASY_CVAR_DECLARATION_SERVER(cl_viewpunch)\
	EASY_CVAR_DECLARATION_SERVER(cl_explosion)\
	EASY_CVAR_DECLARATION_SERVER(soundSentenceSave)\
	EASY_CVAR_DECLARATION_SERVER(pissedNPCs)\
	EASY_CVAR_DECLARATION_SERVER(hud_logo)\
	EASY_CVAR_DECLARATION_SERVER(hud_brokentrans)\
	EASY_CVAR_DECLARATION_SERVER(cl_fvox)\
	EASY_CVAR_DECLARATION_SERVER(cl_ladder)\
	EASY_CVAR_DECLARATION_SERVER(precacheAll)\
	EASY_CVAR_DECLARATION_SERVER(cl_interp_entity)\
	EASY_CVAR_DECLARATION_SERVER(hud_swapFirstTwoBuckets)\
	EASY_CVAR_DECLARATION_SERVER(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_DECLARATION_SERVER(m_rawinput)\
	EASY_CVAR_DECLARATION_SERVER(cl_earlyaccess)\
	EASY_CVAR_DECLARATION_SERVER(cl_viewroll)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_DECLARATION_SERVER(blastExtraArmorDamageMode)\
	EASY_CVAR_DECLARATION_SERVER(hud_batteryhiddendead)\
	EASY_CVAR_DECLARATION_SERVER_DEBUGONLY(hyperBarney)\
	EASY_CVAR_DECLARATION_SERVER(monsterKilledToss)\
	EASY_CVAR_DECLARATION_SERVER(interpolation_movetypestep_mindelta)\
	EASY_CVAR_DECLARATION_SERVER(sv_bloodparticlemode)\
	EASY_CVAR_DECLARATION_SERVER(cl_interp_view_extra)\
	EASY_CVAR_DECLARATION_SERVER(cl_interp_viewmodel)\
	EASY_CVAR_DECLARATION_SERVER(sv_explosionknockback)\
	EASY_CVAR_DECLARATION_SERVER(cl_gaussfollowattachment)\
	EASY_CVAR_DECLARATION_SERVER(cl_breakholster)\
	DUMMY




#define EASY_CVAR_DECLARATION_CLIENT_MASS\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_DECLARATION_CLIENT(hud_version)\
	EASY_CVAR_DECLARATION_CLIENT(hud_batterydraw)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistHori)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeMultiColor)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserEnabled)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnFreq)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserLength)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserMultiColor)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedX)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedY)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedZ)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffX)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffY)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffZ)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedX)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedY)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedZ)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffX)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffY)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffZ)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(imAllFuckedUp)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntGrass)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntPunch)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogNear)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogFar)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myCameraSucks)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(shutupstuka)\
	EASY_CVAR_DECLARATION_CLIENT(chromeEffect)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteBulletHitSounds)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(geigerChannel)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteRicochetSound)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mutePlayerWeaponFire)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteCrowbarSounds)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(nothingHurts)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(seeMonsterHealth)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST(wpn_glocksilencer)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_DECLARATION_CLIENT(cl_muzzleflash)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5011Allowed)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5021Allowed)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5031Allowed)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5002Allowed)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5004Allowed)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(eventsAreFabulous)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockOldReloadLogic)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(testVar)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowColorMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashColorMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashSuitless)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrownMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceDrawBatteryNumber)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(endlessFlashlightBattery)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3UsesFailColors)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3UsesFailColors)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3ShowsDamageIcons)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerCrossbowMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(autoSneaky)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(STUDetection)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(myStrobe)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(wildHeads)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveEffectSpawnInterval)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogTest)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_DECLARATION_CLIENT(r_shadows)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(sv_germancensorship)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(turretCanGib)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(shrapMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(shrapRand)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(emergencyFix)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockUseLastBulletAnim)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(hiddenMemPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_DECLARATION_CLIENT(cl_bullsquidspit)\
	EASY_CVAR_DECLARATION_CLIENT(cl_bullsquidspitarc)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(scientistBravery)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(trailTypeTest)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrail)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrailSolidColor)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModEasy)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModMedium)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModHard)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetZoomPuff)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpiral)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedDartMulti)\
	EASY_CVAR_DECLARATION_CLIENT(cl_rockettrail)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaScale)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketSkipIgnite)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_DECLARATION_CLIENT(cl_hornetspiral)\
	EASY_CVAR_DECLARATION_CLIENT(cl_hornettrail)\
	EASY_CVAR_DECLARATION_CLIENT(hud_drawammobar)\
	EASY_CVAR_DECLARATION_CLIENT(hud_weaponselecthideslower)\
	EASY_CVAR_DECLARATION_CLIENT(hud_drawsidebarmode)\
	EASY_CVAR_DECLARATION_CLIENT(gaussmode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgExMult)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashFadeMult)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowFadeMult)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(painFlashArmorBlock)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDirTolerance)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashCumulativeJump)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashFadeMult)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(iHaveAscended)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(showtriggers)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(peaceOut)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(drawViewModel)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(drawHUD)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponSpreadMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(textureHitSoundPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_DECLARATION_CLIENT(cl_holster)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponTracerMode)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(decalTracerExclusivity)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(healthcolor_brightness)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(floaterDummy)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barneyDummy)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hideDamage)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_debug)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_DECLARATION_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myRocketsAreBarney)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_DECLARATION_CLIENT(r_glowshell_debug)\
	EASY_CVAR_DECLARATION_CLIENT(cl_viewpunch)\
	EASY_CVAR_DECLARATION_CLIENT(cl_explosion)\
	EASY_CVAR_DECLARATION_CLIENT(soundSentenceSave)\
	EASY_CVAR_DECLARATION_CLIENT(pissedNPCs)\
	EASY_CVAR_DECLARATION_CLIENT(hud_logo)\
	EASY_CVAR_DECLARATION_CLIENT(hud_brokentrans)\
	EASY_CVAR_DECLARATION_CLIENT(cl_fvox)\
	EASY_CVAR_DECLARATION_CLIENT(cl_ladder)\
	EASY_CVAR_DECLARATION_CLIENT(precacheAll)\
	EASY_CVAR_DECLARATION_CLIENT(cl_interp_entity)\
	EASY_CVAR_DECLARATION_CLIENT(hud_swapFirstTwoBuckets)\
	EASY_CVAR_DECLARATION_CLIENT(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_DECLARATION_CLIENT(m_rawinput)\
	EASY_CVAR_DECLARATION_CLIENT(cl_earlyaccess)\
	EASY_CVAR_DECLARATION_CLIENT(cl_viewroll)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_DECLARATION_CLIENT(blastExtraArmorDamageMode)\
	EASY_CVAR_DECLARATION_CLIENT(hud_batteryhiddendead)\
	EASY_CVAR_DECLARATION_CLIENT_DEBUGONLY(hyperBarney)\
	EASY_CVAR_DECLARATION_CLIENT(monsterKilledToss)\
	EASY_CVAR_DECLARATION_CLIENT(interpolation_movetypestep_mindelta)\
	EASY_CVAR_DECLARATION_CLIENT(sv_bloodparticlemode)\
	EASY_CVAR_DECLARATION_CLIENT(cl_interp_view_extra)\
	EASY_CVAR_DECLARATION_CLIENT(cl_interp_viewmodel)\
	EASY_CVAR_DECLARATION_CLIENT(sv_explosionknockback)\
	EASY_CVAR_DECLARATION_CLIENT(cl_gaussfollowattachment)\
	EASY_CVAR_DECLARATION_CLIENT(cl_breakholster)\
	DUMMY

#define EASY_CVAR_UPDATE_SERVER_MASS\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_UPDATE_SERVER(hud_version)\
	EASY_CVAR_UPDATE_SERVER(hud_batterydraw)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMin, 0)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMax, 1)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMin, 2)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMax, 3)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistHori, 4)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMin, 5)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMax, 6)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeMultiColor, 7)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserEnabled, 8)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnFreq, 9)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserLength, 10)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMin, 11)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMax, 12)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMin, 13)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMax, 14)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMin, 15)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMax, 16)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMin, 17)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMax, 18)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMin, 19)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMax, 20)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMin, 21)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMax, 22)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMin, 23)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMax, 24)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserMultiColor, 25)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedX, 26)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedY, 27)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedZ, 28)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffX, 29)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffY, 30)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffZ, 31)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedX, 32)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedY, 33)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedZ, 34)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffX, 35)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffY, 36)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffZ, 37)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(imAllFuckedUp, 38)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntGrass, 39)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntPunch, 40)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogNear, 41)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogFar, 42)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myCameraSucks, 43)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(shutupstuka)\
	EASY_CVAR_UPDATE_SERVER(chromeEffect)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteBulletHitSounds, 44)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(geigerChannel, 45)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteTempEntityGroundHitSound, 46)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteRicochetSound, 47)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mutePlayerWeaponFire, 48)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteCrowbarSounds, 49)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(nothingHurts)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(seeMonsterHealth, 50)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST(wpn_glocksilencer, wpn_glocksilencer_ID)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_UPDATE_SERVER(cl_muzzleflash)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5011Allowed, 51)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5021Allowed, 52)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5031Allowed, 53)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5002Allowed, 54)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5004Allowed, 55)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(eventsAreFabulous, 56)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockOldReloadLogic, 57)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(testVar, 58)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowColorMode, 59)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashColorMode, 60)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashSuitless, 61)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrownMode, 62)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMin, 63)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMax, 64)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceDrawBatteryNumber, 65)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(canShowWeaponSelectAtDeath, 66)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(endlessFlashlightBattery, 67)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3UsesFailColors, 68)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3UsesFailColors, 69)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3ShowsDamageIcons, 70)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3ShowsDamageIcons, 71)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerCrossbowMode, 72)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(tripmineAnimWaitsForFinish, 73)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(revolverLaserScope, 74)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip, 80)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo, 81)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay, 82)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom, 83)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil, 84)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(autoSneaky)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP, 85)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(STUDetection)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(myStrobe)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(wildHeads)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveEffectSpawnInterval, 86)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogTest, 87)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(allowAlphaCrosshairWithoutGuns, 89)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(alphaCrosshairBlockedOnFrozen, 90)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsReflectOnlyNPCs, 91)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsDoNotReflectPlayer, 92)\
	EASY_CVAR_UPDATE_SERVER(r_shadows)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(sv_germancensorship, 93)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode, 94)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud, 95)\
	DUMMY\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(handGrenadePickupYieldsOne, 97)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectSoundPlayOnMousewheel, 99)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(timedDamageDeathRemoveMode, 100)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectUsesReloadSounds, 101)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(turretCanGib)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(shrapMode)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(shrapRand)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(emergencyFix)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockUseLastBulletAnim, 102)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBarnacleVictimViewOffset, 103)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hiddenMemPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_UPDATE_SERVER(cl_bullsquidspit)\
	EASY_CVAR_UPDATE_SERVER(cl_bullsquidspitarc)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(scientistBravery)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(trailTypeTest, 104)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrail, 105)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrailSolidColor, 106)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModEasy, 107)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModMedium, 108)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModHard, 109)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetZoomPuff, 110)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpiral, 111)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedMulti, 112)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedDartMulti, 113)\
	EASY_CVAR_UPDATE_SERVER(cl_rockettrail)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaInterval, 114)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaScale, 115)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketSkipIgnite, 116)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_UPDATE_SERVER(cl_hornetspiral)\
	EASY_CVAR_UPDATE_SERVER(cl_hornettrail)\
	EASY_CVAR_UPDATE_SERVER(hud_drawammobar)\
	EASY_CVAR_UPDATE_SERVER(hud_weaponselecthideslower)\
	EASY_CVAR_UPDATE_SERVER(hud_drawsidebarmode)\
	EASY_CVAR_UPDATE_SERVER(gaussmode)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly, 117)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_reflectdealsdamage, 118)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay, 119)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay, 120)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq, 121)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects, 122)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces, 123)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects, 124)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces, 125)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough, 126)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough, 127)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay, 128)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay, 129)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP, 130)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP, 131)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP, 132)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP, 133)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgMin, 134)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgExMult, 135)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMinDrowning, 136)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMax, 137)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrawOpacityMax, 138)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMin, 139)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMax, 140)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashFadeMult, 141)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowFadeMult, 142)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(painFlashArmorBlock, 143)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDirTolerance, 144)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeAppearMin, 145)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeDmgJump, 146)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashPrintouts, 147)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashCumulativeJump, 148)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMin, 149)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMax, 150)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashFadeMult, 151)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowReloadSoundDelay, 152)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowFirePlaysReloadSound, 153)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(iHaveAscended, 154)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(showtriggers)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(peaceOut)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(drawViewModel, 155)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(drawHUD, 156)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBulletHitEffectForceServer, 157)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceAllowServersideTextureSounds, 158)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponSpreadMode, 159)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts, 160)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelSyncFixPrintouts, 161)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(textureHitSoundPrintouts, 162)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_UPDATE_SERVER(cl_holster)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponTracerMode, 164)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(decalTracerExclusivity, 165)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin, 166)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(healthcolor_brightness, 167)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark, 168)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros, 169)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(floaterDummy)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barneyDummy)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hideDamage, 170)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax, 171)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin, 172)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap, 173)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor, 174)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed, 175)\
	EASY_CVAR_UPDATE_SERVER_CLIENTONLY_DEBUGONLY(timedDamage_debug, 176)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_UPDATE_SERVER_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myRocketsAreBarney, 177)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_UPDATE_SERVER(r_glowshell_debug)\
	EASY_CVAR_UPDATE_SERVER(cl_viewpunch)\
	EASY_CVAR_UPDATE_SERVER(cl_explosion)\
	EASY_CVAR_UPDATE_SERVER(soundSentenceSave)\
	EASY_CVAR_UPDATE_SERVER(pissedNPCs)\
	EASY_CVAR_UPDATE_SERVER(hud_logo)\
	EASY_CVAR_UPDATE_SERVER(hud_brokentrans)\
	EASY_CVAR_UPDATE_SERVER(cl_fvox)\
	EASY_CVAR_UPDATE_SERVER(cl_ladder)\
	EASY_CVAR_UPDATE_SERVER(precacheAll)\
	EASY_CVAR_UPDATE_SERVER(cl_interp_entity)\
	EASY_CVAR_UPDATE_SERVER(hud_swapFirstTwoBuckets)\
	EASY_CVAR_UPDATE_SERVER(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_UPDATE_SERVER(m_rawinput)\
	EASY_CVAR_UPDATE_SERVER(cl_earlyaccess)\
	EASY_CVAR_UPDATE_SERVER(cl_viewroll)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_UPDATE_SERVER(blastExtraArmorDamageMode)\
	EASY_CVAR_UPDATE_SERVER(hud_batteryhiddendead)\
	EASY_CVAR_UPDATE_SERVER_DEBUGONLY(hyperBarney)\
	EASY_CVAR_UPDATE_SERVER(monsterKilledToss)\
	EASY_CVAR_UPDATE_SERVER(interpolation_movetypestep_mindelta)\
	EASY_CVAR_UPDATE_SERVER(sv_bloodparticlemode)\
	EASY_CVAR_UPDATE_SERVER(cl_interp_view_extra)\
	EASY_CVAR_UPDATE_SERVER(cl_interp_viewmodel)\
	EASY_CVAR_UPDATE_SERVER(sv_explosionknockback)\
	EASY_CVAR_UPDATE_SERVER(cl_gaussfollowattachment)\
	EASY_CVAR_UPDATE_SERVER(cl_breakholster)\
	DUMMY

#define EASY_CVAR_UPDATE_CLIENT_MASS\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_UPDATE_CLIENT(hud_version)\
	EASY_CVAR_UPDATE_CLIENT(hud_batterydraw)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistHori)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeMultiColor)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserEnabled)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnFreq)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserLength)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserMultiColor)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedX)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedY)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedZ)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffX)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffY)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffZ)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedX)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedY)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedZ)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffX)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffY)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffZ)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(imAllFuckedUp)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntGrass)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntPunch)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogNear)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogFar)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myCameraSucks)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(shutupstuka)\
	EASY_CVAR_UPDATE_CLIENT(chromeEffect)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteBulletHitSounds)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(geigerChannel)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteRicochetSound)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mutePlayerWeaponFire)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteCrowbarSounds)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(nothingHurts)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(seeMonsterHealth)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST(wpn_glocksilencer)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_UPDATE_CLIENT(cl_muzzleflash)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5011Allowed)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5021Allowed)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5031Allowed)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5002Allowed)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5004Allowed)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(eventsAreFabulous)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockOldReloadLogic)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(testVar)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowColorMode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashColorMode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashSuitless)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrownMode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceDrawBatteryNumber)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(endlessFlashlightBattery)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3UsesFailColors)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3UsesFailColors)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3ShowsDamageIcons)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerCrossbowMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(autoSneaky)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(STUDetection)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(myStrobe)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(wildHeads)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveEffectSpawnInterval)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogTest)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_UPDATE_CLIENT(r_shadows)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(sv_germancensorship)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(turretCanGib)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(shrapMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(shrapRand)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(emergencyFix)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockUseLastBulletAnim)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hiddenMemPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_UPDATE_CLIENT(cl_bullsquidspit)\
	EASY_CVAR_UPDATE_CLIENT(cl_bullsquidspitarc)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(scientistBravery)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(trailTypeTest)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrail)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrailSolidColor)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModEasy)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModMedium)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModHard)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetZoomPuff)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpiral)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedDartMulti)\
	EASY_CVAR_UPDATE_CLIENT(cl_rockettrail)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaScale)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketSkipIgnite)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_UPDATE_CLIENT(cl_hornetspiral)\
	EASY_CVAR_UPDATE_CLIENT(cl_hornettrail)\
	EASY_CVAR_UPDATE_CLIENT(hud_drawammobar)\
	EASY_CVAR_UPDATE_CLIENT(hud_weaponselecthideslower)\
	EASY_CVAR_UPDATE_CLIENT(hud_drawsidebarmode)\
	EASY_CVAR_UPDATE_CLIENT(gaussmode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgExMult)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashFadeMult)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowFadeMult)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(painFlashArmorBlock)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDirTolerance)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashPrintouts)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashCumulativeJump)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashFadeMult)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(iHaveAscended)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(showtriggers)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(peaceOut)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(drawViewModel)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(drawHUD)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponSpreadMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(textureHitSoundPrintouts)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_UPDATE_CLIENT(cl_holster)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponTracerMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(decalTracerExclusivity)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(healthcolor_fullRedMin)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(healthcolor_brightness)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(healthcolor_yellowMark)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(cl_drawExtraZeros)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(floaterDummy)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barneyDummy)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hideDamage)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTONLY_DEBUGONLY(timedDamage_debug)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_UPDATE_CLIENT_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myRocketsAreBarney)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_UPDATE_CLIENT(r_glowshell_debug)\
	EASY_CVAR_UPDATE_CLIENT(cl_viewpunch)\
	EASY_CVAR_UPDATE_CLIENT(cl_explosion)\
	EASY_CVAR_UPDATE_CLIENT(soundSentenceSave)\
	EASY_CVAR_UPDATE_CLIENT(pissedNPCs)\
	EASY_CVAR_UPDATE_CLIENT(hud_logo)\
	EASY_CVAR_UPDATE_CLIENT(hud_brokentrans)\
	EASY_CVAR_UPDATE_CLIENT(cl_fvox)\
	EASY_CVAR_UPDATE_CLIENT(cl_ladder)\
	EASY_CVAR_UPDATE_CLIENT(precacheAll)\
	EASY_CVAR_UPDATE_CLIENT(cl_interp_entity)\
	EASY_CVAR_UPDATE_CLIENT(hud_swapFirstTwoBuckets)\
	EASY_CVAR_UPDATE_CLIENT(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_UPDATE_CLIENT(m_rawinput)\
	EASY_CVAR_UPDATE_CLIENT(cl_earlyaccess)\
	EASY_CVAR_UPDATE_CLIENT(cl_viewroll)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_UPDATE_CLIENT(blastExtraArmorDamageMode)\
	EASY_CVAR_UPDATE_CLIENT(hud_batteryhiddendead)\
	EASY_CVAR_UPDATE_CLIENT_DEBUGONLY(hyperBarney)\
	EASY_CVAR_UPDATE_CLIENT(monsterKilledToss)\
	EASY_CVAR_UPDATE_CLIENT(interpolation_movetypestep_mindelta)\
	EASY_CVAR_UPDATE_CLIENT(sv_bloodparticlemode)\
	EASY_CVAR_UPDATE_CLIENT(cl_interp_view_extra)\
	EASY_CVAR_UPDATE_CLIENT(cl_interp_viewmodel)\
	EASY_CVAR_UPDATE_CLIENT(sv_explosionknockback)\
	EASY_CVAR_UPDATE_CLIENT(cl_gaussfollowattachment)\
	EASY_CVAR_UPDATE_CLIENT(cl_breakholster)\
	DUMMY

#define EASY_CVAR_CREATE_SERVER_SETUP_MASS\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_version)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_batterydraw)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(strobeDurationMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(strobeDurationMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(strobeRadiusMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(strobeRadiusMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistHori)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(strobeMultiColor)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserEnabled)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnFreq)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserLength)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserBrightnessMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserBrightnessMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserDurationMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserDurationMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserThicknessMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserThicknessMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserNoiseMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserNoiseMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserFrameRateMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserFrameRateMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(raveLaserMultiColor)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraPosFixedX)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraPosFixedY)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraPosFixedZ)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraPosOffX)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraPosOffY)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraPosOffZ)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraRotFixedX)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraRotFixedY)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraRotFixedZ)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraRotOffX)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraRotOffY)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cameraRotOffZ)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(imAllFuckedUp)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(thatWasntGrass)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(thatWasntPunch)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(fogNear)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(fogFar)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(myCameraSucks)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(shutupstuka)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(chromeEffect)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(muteBulletHitSounds)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(geigerChannel)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(muteRicochetSound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(mutePlayerWeaponFire)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(muteCrowbarSounds)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(nothingHurts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(seeMonsterHealth)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(wpn_glocksilencer)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_muzzleflash)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(event5011Allowed)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(event5021Allowed)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(event5031Allowed)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(event5002Allowed)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(event5004Allowed)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(eventsAreFabulous)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(glockOldReloadLogic)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(testVar)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(painArrowColorMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(painFlashColorMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(painFlashSuitless)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(painFlashDrownMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(forceDrawBatteryNumber)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(endlessFlashlightBattery)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(preE3UsesFailColors)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(E3UsesFailColors)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(E3ShowsDamageIcons)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerCrossbowMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cheat_infiniteclip)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cheat_infiniteammo)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cheat_minimumfiredelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cheat_nogaussrecoil)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(autoSneaky)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(STUDetection)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(myStrobe)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(wildHeads)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(raveEffectSpawnInterval)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(fogTest)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(r_shadows)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sv_germancensorship)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(egonEffectsMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(turretCanGib)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(shrapMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(shrapRand)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(emergencyFix)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(glockUseLastBulletAnim)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_CREATE_SERVER_SETUP_SERVERONLY_DEBUGONLY(hiddenMemPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(cl_bullsquidspit)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(cl_bullsquidspitarc)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(scientistBravery)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(trailTypeTest)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetTrail)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetTrailSolidColor)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetDeathModEasy)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetDeathModMedium)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetDeathModHard)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetZoomPuff)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetSpiral)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetSpeedDartMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(cl_rockettrail)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(rocketTrailAlphaScale)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(rocketSkipIgnite)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(cl_hornetspiral)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(cl_hornettrail)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_drawammobar)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_weaponselecthideslower)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_drawsidebarmode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(gaussmode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_primaryonly)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_chargeanimdelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_chargeworkdelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_primaryreflects)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_primarypierces)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_secondaryreflects)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_secondarypierces)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_primarypunchthrough)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_betweenattackdelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painFlashDmgMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painFlashDmgExMult)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painFlashCumulativeMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painFlashFadeMult)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painArrowFadeMult)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(painFlashArmorBlock)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painFlashDirTolerance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(painFlashPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(itemFlashCumulativeJump)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(itemFlashFadeMult)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(iHaveAscended)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(showtriggers)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(peaceOut)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(drawViewModel)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(drawHUD)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerWeaponSpreadMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(viewModelPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(textureHitSoundPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_holster)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(playerWeaponTracerMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(decalTracerExclusivity)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(healthcolor_brightness)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(floaterDummy)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barneyDummy)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hideDamage)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY_DEBUGONLY(timedDamage_debug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(myRocketsAreBarney)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(r_glowshell_debug)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_viewpunch)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(cl_explosion)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(soundSentenceSave)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(pissedNPCs)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_logo)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_brokentrans)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_fvox)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_ladder)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(precacheAll)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_interp_entity)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_swapFirstTwoBuckets)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(m_rawinput)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_earlyaccess)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_viewroll)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(blastExtraArmorDamageMode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(hud_batteryhiddendead)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY_DEBUGONLY(hyperBarney)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(monsterKilledToss)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(interpolation_movetypestep_mindelta)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(sv_bloodparticlemode)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_interp_view_extra)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_interp_viewmodel)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_SERVERONLY(sv_explosionknockback)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_gaussfollowattachment)\
	EASY_CVAR_CREATE_SERVER_SETUP_A_CLIENTONLY(cl_breakholster)\
	DUMMY

#define EASY_CVAR_CREATE_SERVER_MASS\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_version)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_batterydraw)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(strobeDurationMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(strobeDurationMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(strobeRadiusMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(strobeRadiusMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistHori)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(strobeMultiColor)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserEnabled)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnFreq)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserLength)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserBrightnessMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserBrightnessMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserDurationMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserDurationMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserThicknessMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserThicknessMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserNoiseMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserNoiseMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserFrameRateMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserFrameRateMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(raveLaserMultiColor)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraPosFixedX)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraPosFixedY)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraPosFixedZ)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraPosOffX)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraPosOffY)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraPosOffZ)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraRotFixedX)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraRotFixedY)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraRotFixedZ)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraRotOffX)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraRotOffY)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cameraRotOffZ)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(imAllFuckedUp)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(thatWasntGrass)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(thatWasntPunch)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(fogNear)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(fogFar)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(myCameraSucks)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(shutupstuka)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(chromeEffect)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(muteBulletHitSounds)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(geigerChannel)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(muteRicochetSound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(mutePlayerWeaponFire)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(muteCrowbarSounds)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(nothingHurts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(seeMonsterHealth)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(wpn_glocksilencer)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_muzzleflash)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(event5011Allowed)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(event5021Allowed)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(event5031Allowed)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(event5002Allowed)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(event5004Allowed)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(eventsAreFabulous)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(glockOldReloadLogic)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(testVar)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(painArrowColorMode)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(painFlashColorMode)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(painFlashSuitless)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(painFlashDrownMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(forceDrawBatteryNumber)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(endlessFlashlightBattery)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(preE3UsesFailColors)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(E3UsesFailColors)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(E3ShowsDamageIcons)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerCrossbowMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cheat_infiniteclip)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cheat_infiniteammo)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cheat_minimumfiredelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cheat_nogaussrecoil)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(autoSneaky)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(STUDetection)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(myStrobe)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(wildHeads)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(raveEffectSpawnInterval)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(fogTest)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(r_shadows)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sv_germancensorship)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(egonEffectsMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(turretCanGib)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(shrapMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(shrapRand)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(emergencyFix)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(glockUseLastBulletAnim)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_CREATE_SERVER_SERVERONLY_DEBUGONLY(hiddenMemPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(cl_bullsquidspit)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(cl_bullsquidspitarc)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(scientistBravery)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(trailTypeTest)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetTrail)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetTrailSolidColor)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetDeathModEasy)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetDeathModMedium)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetDeathModHard)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetZoomPuff)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetSpiral)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetSpeedDartMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(cl_rockettrail)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(rocketTrailAlphaScale)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(rocketSkipIgnite)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(cl_hornetspiral)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(cl_hornettrail)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_drawammobar)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_weaponselecthideslower)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_drawsidebarmode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(gaussmode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_primaryonly)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_chargeanimdelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_chargeworkdelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_primaryreflects)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_primarypierces)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_secondaryreflects)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_secondarypierces)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_primarypunchthrough)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_betweenattackdelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painFlashDmgMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painFlashDmgExMult)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painFlashCumulativeMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painFlashFadeMult)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painArrowFadeMult)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(painFlashArmorBlock)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painFlashDirTolerance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(painFlashPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(itemFlashCumulativeJump)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(itemFlashFadeMult)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(iHaveAscended)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(showtriggers)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(peaceOut)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(drawViewModel)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(drawHUD)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerWeaponSpreadMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(viewModelPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(textureHitSoundPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_holster)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(playerWeaponTracerMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(decalTracerExclusivity)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(healthcolor_brightness)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(floaterDummy)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barneyDummy)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hideDamage)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY_DEBUGONLY(timedDamage_debug)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(myRocketsAreBarney)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(r_glowshell_debug)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_viewpunch)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(cl_explosion)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(soundSentenceSave)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(pissedNPCs)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_logo)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_brokentrans)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_fvox)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_ladder)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(precacheAll)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_interp_entity)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_swapFirstTwoBuckets)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(m_rawinput)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_earlyaccess)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_viewroll)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(blastExtraArmorDamageMode)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(hud_batteryhiddendead)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY_DEBUGONLY(hyperBarney)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(monsterKilledToss)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(interpolation_movetypestep_mindelta)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(sv_bloodparticlemode)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_interp_view_extra)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_interp_viewmodel)\
	EASY_CVAR_CREATE_SERVER_A_SERVERONLY(sv_explosionknockback)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_gaussfollowattachment)\
	EASY_CVAR_CREATE_SERVER_A_CLIENTONLY(cl_breakholster)\
	DUMMY


#define EASY_CVAR_CREATE_CLIENT_MASS\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_version)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_batterydraw)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(strobeDurationMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(strobeDurationMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(strobeRadiusMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(strobeRadiusMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistHori)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(strobeMultiColor)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserEnabled)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnFreq)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserLength)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserBrightnessMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserBrightnessMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserDurationMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserDurationMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserThicknessMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserThicknessMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserNoiseMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserNoiseMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserFrameRateMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserFrameRateMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(raveLaserMultiColor)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraPosFixedX)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraPosFixedY)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraPosFixedZ)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraPosOffX)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraPosOffY)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraPosOffZ)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraRotFixedX)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraRotFixedY)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraRotFixedZ)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraRotOffX)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraRotOffY)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cameraRotOffZ)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(imAllFuckedUp)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(thatWasntGrass)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(thatWasntPunch)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(fogNear)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(fogFar)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(myCameraSucks)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(shutupstuka)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(chromeEffect)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(muteBulletHitSounds)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(geigerChannel)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(muteRicochetSound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(mutePlayerWeaponFire)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(muteCrowbarSounds)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(nothingHurts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(seeMonsterHealth)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(wpn_glocksilencer)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_muzzleflash)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(event5011Allowed)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(event5021Allowed)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(event5031Allowed)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(event5002Allowed)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(event5004Allowed)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(eventsAreFabulous)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(glockOldReloadLogic)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(testVar)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(painArrowColorMode)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(painFlashColorMode)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(painFlashSuitless)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(painFlashDrownMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(forceDrawBatteryNumber)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(endlessFlashlightBattery)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(preE3UsesFailColors)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(E3UsesFailColors)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(E3ShowsDamageIcons)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerCrossbowMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cheat_infiniteclip)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cheat_infiniteammo)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cheat_minimumfiredelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cheat_nogaussrecoil)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(autoSneaky)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(STUDetection)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(myStrobe)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(wildHeads)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(raveEffectSpawnInterval)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(fogTest)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(r_shadows)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sv_germancensorship)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(egonEffectsMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(turretCanGib)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(shrapMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(shrapRand)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(emergencyFix)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(glockUseLastBulletAnim)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_CREATE_CLIENT_SERVERONLY_DEBUGONLY(hiddenMemPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(cl_bullsquidspit)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(cl_bullsquidspitarc)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(scientistBravery)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(trailTypeTest)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetTrail)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetTrailSolidColor)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetDeathModEasy)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetDeathModMedium)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetDeathModHard)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetZoomPuff)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetSpiral)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetSpeedDartMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(cl_rockettrail)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(rocketTrailAlphaScale)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(rocketSkipIgnite)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(cl_hornetspiral)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(cl_hornettrail)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_drawammobar)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_weaponselecthideslower)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_drawsidebarmode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(gaussmode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_primaryonly)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_chargeanimdelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_chargeworkdelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_primaryreflects)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_primarypierces)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_secondaryreflects)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_secondarypierces)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_primarypunchthrough)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_betweenattackdelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painFlashDmgMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painFlashDmgExMult)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painFlashCumulativeMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painFlashFadeMult)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painArrowFadeMult)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(painFlashArmorBlock)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painFlashDirTolerance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(painFlashPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(itemFlashCumulativeJump)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(itemFlashFadeMult)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(iHaveAscended)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(showtriggers)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(peaceOut)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(drawViewModel)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(drawHUD)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerWeaponSpreadMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(viewModelPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(textureHitSoundPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_holster)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(playerWeaponTracerMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(decalTracerExclusivity)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(healthcolor_brightness)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(floaterDummy)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barneyDummy)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hideDamage)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY_DEBUGONLY(timedDamage_debug)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(myRocketsAreBarney)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(r_glowshell_debug)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_viewpunch)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(cl_explosion)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(soundSentenceSave)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(pissedNPCs)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_logo)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_brokentrans)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_fvox)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_ladder)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(precacheAll)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_interp_entity)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_swapFirstTwoBuckets)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(m_rawinput)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_earlyaccess)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_viewroll)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(blastExtraArmorDamageMode)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(hud_batteryhiddendead)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY_DEBUGONLY(hyperBarney)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(monsterKilledToss)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(interpolation_movetypestep_mindelta)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(sv_bloodparticlemode)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_interp_view_extra)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_interp_viewmodel)\
	EASY_CVAR_CREATE_CLIENT_A_SERVERONLY(sv_explosionknockback)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_gaussfollowattachment)\
	EASY_CVAR_CREATE_CLIENT_A_CLIENTONLY(cl_breakholster)\
	DUMMY


#define EASY_CVAR_RESET_MASS\
	EASY_CVAR_RESET_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_RESET(hud_version)\
	EASY_CVAR_RESET(hud_batterydraw)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistHori)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeMultiColor)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserEnabled)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnFreq)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserLength)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserMultiColor)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedX)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedY)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedZ)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffX)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffY)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffZ)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedX)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedY)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedZ)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffX)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffY)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffZ)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(imAllFuckedUp)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntGrass)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntPunch)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogNear)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogFar)\
	EASY_CVAR_RESET_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myCameraSucks)\
	EASY_CVAR_RESET_DEBUGONLY(shutupstuka)\
	EASY_CVAR_RESET(chromeEffect)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteBulletHitSounds)\
	EASY_CVAR_RESET_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(geigerChannel)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_RESET_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteRicochetSound)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mutePlayerWeaponFire)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteCrowbarSounds)\
	EASY_CVAR_RESET_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_RESET_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_RESET_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_RESET_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_RESET_DEBUGONLY(nothingHurts)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(seeMonsterHealth)\
	EASY_CVAR_RESET_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_RESET_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_RESET_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_RESET_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_RESET_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_RESET_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_RESET_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_RESET_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_RESET_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_RESET_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_RESET_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_RESET_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_RESET_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_RESET_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_RESET_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_RESET_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_RESET_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_RESET_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_RESET_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_RESET_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST(wpn_glocksilencer)\
	EASY_CVAR_RESET_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_RESET_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_RESET(cl_muzzleflash)\
	EASY_CVAR_RESET_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5011Allowed)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5021Allowed)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5031Allowed)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5002Allowed)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5004Allowed)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(eventsAreFabulous)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockOldReloadLogic)\
	EASY_CVAR_RESET_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_RESET_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_RESET_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_RESET_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_RESET_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_RESET_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_RESET_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_RESET_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_RESET_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_RESET_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(testVar)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowColorMode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashColorMode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashSuitless)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrownMode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceDrawBatteryNumber)\
	EASY_CVAR_RESET_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_RESET_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(endlessFlashlightBattery)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3UsesFailColors)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3UsesFailColors)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3ShowsDamageIcons)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerCrossbowMode)\
	EASY_CVAR_RESET_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_RESET_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil)\
	EASY_CVAR_RESET_DEBUGONLY(autoSneaky)\
	EASY_CVAR_RESET_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_RESET_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_RESET_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_RESET_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_RESET_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_RESET_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_RESET_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_RESET_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_RESET_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_RESET_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_RESET_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(STUDetection)\
	EASY_CVAR_RESET_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_RESET_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_RESET_DEBUGONLY(myStrobe)\
	EASY_CVAR_RESET_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_RESET_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_RESET_DEBUGONLY(wildHeads)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveEffectSpawnInterval)\
	EASY_CVAR_RESET_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogTest)\
	EASY_CVAR_RESET_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_RESET_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_RESET_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_RESET_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_RESET_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_RESET_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_RESET_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_RESET_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_RESET_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_RESET_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_RESET(r_shadows)\
	EASY_CVAR_RESET_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_RESET_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_RESET_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_RESET_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_RESET_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_RESET_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_RESET_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_RESET_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_RESET_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_RESET_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_RESET_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_RESET_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_RESET_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_RESET_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_RESET_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(sv_germancensorship)\
	EASY_CVAR_RESET_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_RESET_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_RESET_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_RESET_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_RESET_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_RESET_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_RESET_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_RESET_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_RESET_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_RESET_DEBUGONLY(turretCanGib)\
	EASY_CVAR_RESET_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_RESET_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_RESET_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_RESET_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_RESET_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_RESET_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_RESET_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_RESET_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_RESET_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_RESET_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_RESET_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_RESET_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_RESET_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_RESET_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_RESET_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_RESET_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_RESET_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_RESET_DEBUGONLY(shrapMode)\
	EASY_CVAR_RESET_DEBUGONLY(shrapRand)\
	EASY_CVAR_RESET_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_RESET_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_RESET_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_RESET_DEBUGONLY(emergencyFix)\
	EASY_CVAR_RESET_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_RESET_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_RESET_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_RESET_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_RESET_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_RESET_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_RESET_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_RESET_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockUseLastBulletAnim)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_RESET_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_RESET_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_RESET_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_RESET_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_RESET_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_RESET_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_RESET_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_RESET_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_RESET_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_RESET_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_RESET_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_RESET_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_RESET_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_RESET(cl_bullsquidspit)\
	EASY_CVAR_RESET(cl_bullsquidspitarc)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_RESET_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_RESET_DEBUGONLY(scientistBravery)\
	EASY_CVAR_RESET_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_RESET_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(trailTypeTest)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrail)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrailSolidColor)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModEasy)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModMedium)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModHard)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetZoomPuff)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpiral)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedMulti)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedDartMulti)\
	EASY_CVAR_RESET(cl_rockettrail)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaScale)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketSkipIgnite)\
	EASY_CVAR_RESET_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_RESET_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_RESET_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_RESET(cl_hornetspiral)\
	EASY_CVAR_RESET(cl_hornettrail)\
	EASY_CVAR_RESET(hud_drawammobar)\
	EASY_CVAR_RESET(hud_weaponselecthideslower)\
	EASY_CVAR_RESET(hud_drawsidebarmode)\
	EASY_CVAR_RESET(gaussmode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgExMult)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashFadeMult)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowFadeMult)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(painFlashArmorBlock)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDirTolerance)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashPrintouts)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashCumulativeJump)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashFadeMult)\
	EASY_CVAR_RESET_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(iHaveAscended)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_RESET_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_RESET_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_RESET_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_RESET_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_RESET_DEBUGONLY(showtriggers)\
	EASY_CVAR_RESET_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_RESET_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_RESET_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_RESET_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_RESET_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_RESET_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_RESET_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_RESET_DEBUGONLY(peaceOut)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(drawViewModel)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(drawHUD)\
	EASY_CVAR_RESET_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponSpreadMode)\
	EASY_CVAR_RESET_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_RESET_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_RESET_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_RESET_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_RESET_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(textureHitSoundPrintouts)\
	EASY_CVAR_RESET_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_RESET_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_RESET_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_RESET_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_RESET_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_RESET_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_RESET_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_RESET_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_RESET_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_RESET_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_RESET_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_RESET_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_RESET_CLIENTONLY(cl_holster)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponTracerMode)\
	EASY_CVAR_RESET_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(decalTracerExclusivity)\
	EASY_CVAR_RESET_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_RESET_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(healthcolor_brightness)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros)\
	EASY_CVAR_RESET_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_RESET_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_RESET_DEBUGONLY(floaterDummy)\
	EASY_CVAR_RESET_DEBUGONLY(barneyDummy)\
	EASY_CVAR_RESET_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_RESET_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_RESET_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hideDamage)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed)\
	EASY_CVAR_RESET_CLIENTONLY_DEBUGONLY(timedDamage_debug)\
	EASY_CVAR_RESET_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_RESET_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_RESET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myRocketsAreBarney)\
	EASY_CVAR_RESET_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_RESET_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_RESET_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_RESET_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_RESET(r_glowshell_debug)\
	EASY_CVAR_RESET(cl_viewpunch)\
	EASY_CVAR_RESET(cl_explosion)\
	EASY_CVAR_RESET(soundSentenceSave)\
	EASY_CVAR_RESET(pissedNPCs)\
	EASY_CVAR_RESET(hud_logo)\
	EASY_CVAR_RESET_CLIENTONLY(hud_brokentrans)\
	EASY_CVAR_RESET_CLIENTONLY(cl_fvox)\
	EASY_CVAR_RESET_CLIENTONLY(cl_ladder)\
	EASY_CVAR_RESET(precacheAll)\
	EASY_CVAR_RESET(cl_interp_entity)\
	EASY_CVAR_RESET(hud_swapFirstTwoBuckets)\
	EASY_CVAR_RESET(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_RESET_CLIENTONLY(m_rawinput)\
	EASY_CVAR_RESET_CLIENTONLY(cl_earlyaccess)\
	EASY_CVAR_RESET_CLIENTONLY(cl_viewroll)\
	EASY_CVAR_RESET_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_RESET(blastExtraArmorDamageMode)\
	EASY_CVAR_RESET(hud_batteryhiddendead)\
	EASY_CVAR_RESET_DEBUGONLY(hyperBarney)\
	EASY_CVAR_RESET(monsterKilledToss)\
	EASY_CVAR_RESET(interpolation_movetypestep_mindelta)\
	EASY_CVAR_RESET(sv_bloodparticlemode)\
	EASY_CVAR_RESET(cl_interp_view_extra)\
	EASY_CVAR_RESET(cl_interp_viewmodel)\
	EASY_CVAR_RESET(sv_explosionknockback)\
	EASY_CVAR_RESET(cl_gaussfollowattachment)\
	EASY_CVAR_RESET(cl_breakholster)\
	DUMMY


#define EASY_CVAR_EXTERN_MASS\
	EASY_CVAR_EXTERN_DEBUGONLY(gruntsCanHaveMP5Grenade)\
	EASY_CVAR_EXTERN(hud_version)\
	EASY_CVAR_EXTERN(hud_batterydraw)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeDurationMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeRadiusMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistHori)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeSpawnDistVertMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(strobeMultiColor)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserEnabled)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnFreq)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserLength)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistHoriMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserSpawnDistVertMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserBrightnessMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserDurationMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserThicknessMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserNoiseMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserFrameRateMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveLaserMultiColor)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedX)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedY)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosFixedZ)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffX)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffY)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraPosOffZ)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedX)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedY)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotFixedZ)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffX)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffY)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cameraRotOffZ)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(imAllFuckedUp)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntGrass)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(thatWasntPunch)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogNear)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogFar)\
	EASY_CVAR_EXTERN_DEBUGONLY(NPCsTalkMore)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myCameraSucks)\
	EASY_CVAR_EXTERN_DEBUGONLY(shutupstuka)\
	EASY_CVAR_EXTERN(chromeEffect)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultSpinMovement)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultIdleSpinSound)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultFireSound)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteBulletHitSounds)\
	EASY_CVAR_EXTERN_DEBUGONLY(mutePlayerPainSounds)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultIdleSpinSoundChannel)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultSpinUpDownSoundChannel)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultFireSoundChannel)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(geigerChannel)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultWaitTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultSpinupRemainTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultResidualAttackTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultSpinupStartTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultVoicePitchMin)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultVoicePitchMax)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultFireSpread)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteTempEntityGroundHitSound)\
	EASY_CVAR_EXTERN_DEBUGONLY(houndeyeAttackMode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteRicochetSound)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mutePlayerWeaponFire)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(muteCrowbarSounds)\
	EASY_CVAR_EXTERN_DEBUGONLY(scientistHealNPC)\
	EASY_CVAR_EXTERN_DEBUGONLY(scientistHealNPCDebug)\
	EASY_CVAR_EXTERN_DEBUGONLY(bulletholeAlertRange)\
	EASY_CVAR_EXTERN_DEBUGONLY(fleshhitmakessound)\
	EASY_CVAR_EXTERN_DEBUGONLY(nothingHurts)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(seeMonsterHealth)\
	EASY_CVAR_EXTERN_DEBUGONLY(scientistHealNPCFract)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidRangeDisabled)\
	EASY_CVAR_EXTERN_DEBUGONLY(applyLKPPathFixToAll)\
	EASY_CVAR_EXTERN_DEBUGONLY(timedDamageAffectsMonsters)\
	EASY_CVAR_EXTERN_DEBUGONLY(scientistHealCooldown)\
	EASY_CVAR_EXTERN_DEBUGONLY(crazyMonsterPrintouts)\
	EASY_CVAR_EXTERN_DEBUGONLY(movementIsCompletePrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(bulletHoleAlertPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(bulletholeAlertStukaOnly)\
	EASY_CVAR_EXTERN_DEBUGONLY(barneyPrintouts)\
	EASY_CVAR_EXTERN_DEBUGONLY(monsterSpawnPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(zombieBulletResistance)\
	EASY_CVAR_EXTERN_DEBUGONLY(zombieBulletPushback)\
	EASY_CVAR_EXTERN_DEBUGONLY(houndeyePrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(quakeExplosionSound)\
	EASY_CVAR_EXTERN_DEBUGONLY(explosionDebrisSoundVolume)\
	EASY_CVAR_EXTERN_DEBUGONLY(noFlinchOnHard)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultDrawLKP)\
	EASY_CVAR_EXTERN_DEBUGONLY(meleeDrawBloodModeA)\
	EASY_CVAR_EXTERN_DEBUGONLY(meleeDrawBloodModeB)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawDebugBloodTrace)\
	EASY_CVAR_EXTERN_DEBUGONLY(meleeDrawBloodModeBFix)\
	EASY_CVAR_EXTERN_DEBUGONLY(meleeDrawBloodModeAOffset)\
	EASY_CVAR_EXTERN_DEBUGONLY(meleeDrawBloodModeBOffset)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST(wpn_glocksilencer)\
	EASY_CVAR_EXTERN_DEBUGONLY(panthereyeHasCloakingAbility)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntForceStrafeFireAnim)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntLockRunAndGunTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntHeadshotGore)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntAllowStrafeFire)\
	EASY_CVAR_EXTERN_DEBUGONLY(thoroughHitBoxUpdates)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntTinyClip)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntStrafeAlwaysHasAmmo)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntBrassEjectForwardOffset)\
	EASY_CVAR_EXTERN(cl_muzzleflash)\
	EASY_CVAR_EXTERN_DEBUGONLY(agrunt_muzzleflash)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5011Allowed)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5021Allowed)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5031Allowed)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5002Allowed)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(event5004Allowed)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(eventsAreFabulous)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockOldReloadLogic)\
	EASY_CVAR_EXTERN_DEBUGONLY(glockOldReloadLogicBarney)\
	EASY_CVAR_EXTERN_DEBUGONLY(barneyDroppedGlockAmmoCap)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawCollisionBoundsAtDeath)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawHitBoundsAtDeath)\
	EASY_CVAR_EXTERN_DEBUGONLY(islaveReviveFriendMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(islaveReviveFriendChance)\
	EASY_CVAR_EXTERN_DEBUGONLY(islaveReviveFriendRange)\
	EASY_CVAR_EXTERN_DEBUGONLY(islaveReviveSelfMinDelay)\
	EASY_CVAR_EXTERN_DEBUGONLY(islaveReviveSelfMaxDelay)\
	EASY_CVAR_EXTERN_DEBUGONLY(islaveReviveSelfChance)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntRunAndGunDistance)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntPrintout)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(testVar)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowColorMode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashColorMode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashSuitless)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrownMode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(firstPersonIdleDelayMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceDrawBatteryNumber)\
	EASY_CVAR_EXTERN_DEBUGONLY(offsetgivedistance)\
	EASY_CVAR_EXTERN_DEBUGONLY(offsetgivelookvertical)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(canShowWeaponSelectAtDeath)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(endlessFlashlightBattery)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3UsesFailColors)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3UsesFailColors)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(preE3ShowsDamageIcons)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(E3ShowsDamageIcons)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerCrossbowMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassassinCrossbowMode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(tripmineAnimWaitsForFinish)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(revolverLaserScope)\
	DUMMY\
	DUMMY\
	DUMMY\
	EASY_CVAR_EXTERN_CLIENTONLY(auto_adjust_fov)\
	DUMMY\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil)\
	EASY_CVAR_EXTERN_DEBUGONLY(autoSneaky)\
	EASY_CVAR_EXTERN_DEBUGONLY(infiniteLongJumpCharge)\
	EASY_CVAR_EXTERN_DEBUGONLY(cheat_touchNeverExplodes)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawDebugPathfinding)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUcheckDistH)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUcheckDistV)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUcheckDistD)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUextraTriangH)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUextraTriangV)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUrepelMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUExplodeTest)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUYawSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(STUDetection)\
	EASY_CVAR_EXTERN_DEBUGONLY(stukaAdvancedCombat)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawDebugPathfinding2)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultFriendlyFire)\
	EASY_CVAR_EXTERN_DEBUGONLY(myStrobe)\
	EASY_CVAR_EXTERN_DEBUGONLY(peopleStrobe)\
	EASY_CVAR_EXTERN_DEBUGONLY(forceWorldLightOff)\
	EASY_CVAR_EXTERN_DEBUGONLY(wildHeads)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(raveEffectSpawnInterval)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawBarnacleDebug)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(fogTest)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksAllMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksEnvMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksButtonMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksPlayerCrossbowMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksComputerHitMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksTurretDeathMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksOspreyHitMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksExplosionMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksBeamMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(sparksAIFailMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(normalSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(noclipSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(jumpForceMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(apacheForceCinBounds)\
	EASY_CVAR_EXTERN_DEBUGONLY(apacheBottomBoundAdj)\
	EASY_CVAR_EXTERN_DEBUGONLY(apacheInfluence)\
	DUMMY\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(allowAlphaCrosshairWithoutGuns)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(alphaCrosshairBlockedOnFrozen)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntRunAndGunDotMin)\
	EASY_CVAR_EXTERN_DEBUGONLY(panthereyeJumpDotTol)\
	EASY_CVAR_EXTERN_DEBUGONLY(panthereyePrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(gargantuaPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(squadmonsterPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(barnaclePrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(friendlyPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(stukaPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(timedDamageEndlessOnHard)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsReflectOnlyNPCs)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(mirrorsDoNotReflectPlayer)\
	EASY_CVAR_EXTERN(r_shadows)\
	EASY_CVAR_EXTERN_DEBUGONLY(altSquadRulesRuntime)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntLockStrafeTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(ignoreIsolatedNodes)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawNodeAll)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawNodeSpecial)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawNodeConnections)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawNodeAlternateTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(nodeSearchStartVerticalOffset)\
	EASY_CVAR_EXTERN_DEBUGONLY(blockChangeLevelTrigger)\
	EASY_CVAR_EXTERN_DEBUGONLY(blockMusicTrigger)\
	EASY_CVAR_EXTERN_DEBUGONLY(blockMultiTrigger)\
	EASY_CVAR_EXTERN_DEBUGONLY(blockTeleportTrigger)\
	EASY_CVAR_EXTERN_DEBUGONLY(blockHurtTrigger)\
	EASY_CVAR_EXTERN_DEBUGONLY(blockAutosaveTrigger)\
	EASY_CVAR_EXTERN_DEBUGONLY(hideNodeGraphRebuildNotice)\
	EASY_CVAR_EXTERN_DEBUGONLY(barnacleTongueRetractDelay)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(sv_germancensorship)\
	EASY_CVAR_EXTERN_DEBUGONLY(allowGermanModels)\
	EASY_CVAR_EXTERN_DEBUGONLY(germanRobotGibs)\
	EASY_CVAR_EXTERN_DEBUGONLY(germanRobotBleedsOil)\
	EASY_CVAR_EXTERN_DEBUGONLY(germanRobotDamageDecal)\
	EASY_CVAR_EXTERN_DEBUGONLY(germanRobotGibsDecal)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud)\
	DUMMY\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(handGrenadePickupYieldsOne)\
	EASY_CVAR_EXTERN_DEBUGONLY(handGrenadesUseOldBounceSound)\
	DUMMY\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectSoundPlayOnMousewheel)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(timedDamageDeathRemoveMode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(weaponSelectUsesReloadSounds)\
	EASY_CVAR_EXTERN_DEBUGONLY(barnacleCanGib)\
	EASY_CVAR_EXTERN_DEBUGONLY(sentryCanGib)\
	EASY_CVAR_EXTERN_DEBUGONLY(miniturretCanGib)\
	EASY_CVAR_EXTERN_DEBUGONLY(turretCanGib)\
	EASY_CVAR_EXTERN_DEBUGONLY(turretBleedsOil)\
	EASY_CVAR_EXTERN_DEBUGONLY(turretDamageDecal)\
	EASY_CVAR_EXTERN_DEBUGONLY(turretGibDecal)\
	EASY_CVAR_EXTERN_DEBUGONLY(canDropInSinglePlayer)\
	EASY_CVAR_EXTERN_DEBUGONLY(timedDamageIgnoresArmor)\
	EASY_CVAR_EXTERN_DEBUGONLY(itemBatteryPrerequisite)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerExtraPainSoundsMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(timedDamageDisableViewPunch)\
	EASY_CVAR_EXTERN_DEBUGONLY(batteryDrainsAtDeath)\
	EASY_CVAR_EXTERN_DEBUGONLY(batteryDrainsAtAdrenalineMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(canTakeLongJump)\
	EASY_CVAR_EXTERN_DEBUGONLY(printOutCommonTimables)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerBrightLight)\
	EASY_CVAR_EXTERN_DEBUGONLY(disablePainPunchAutomatic)\
	EASY_CVAR_EXTERN_DEBUGONLY(gargantuaCorpseDeath)\
	EASY_CVAR_EXTERN_DEBUGONLY(gargantuaFallSound)\
	EASY_CVAR_EXTERN_DEBUGONLY(gargantuaBleeds)\
	EASY_CVAR_EXTERN_DEBUGONLY(shrapMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(shrapRand)\
	EASY_CVAR_EXTERN_DEBUGONLY(shrapRandHeightExtra)\
	EASY_CVAR_EXTERN_DEBUGONLY(explosionShrapnelMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(useAlphaSparks)\
	EASY_CVAR_EXTERN_DEBUGONLY(emergencyFix)\
	EASY_CVAR_EXTERN_DEBUGONLY(timedDamageReviveRemoveMode)\
	DUMMY\
	EASY_CVAR_EXTERN_DEBUGONLY(ospreyIgnoresGruntCount)\
	EASY_CVAR_EXTERN_DEBUGONLY(mp5GrenadeInheritsPlayerVelocity)\
	EASY_CVAR_EXTERN_DEBUGONLY(crossbowInheritsPlayerVelocity)\
	EASY_CVAR_EXTERN_DEBUGONLY(fastHornetsInheritsPlayerVelocity)\
	EASY_CVAR_EXTERN_DEBUGONLY(snarkInheritsPlayerVelocity)\
	EASY_CVAR_EXTERN_DEBUGONLY(chumtoadInheritsPlayerVelocity)\
	EASY_CVAR_EXTERN_DEBUGONLY(weaponPickupPlaysAnyReloadSounds)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(glockUseLastBulletAnim)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBarnacleVictimViewOffset)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntMovementDeltaCheck)\
	EASY_CVAR_EXTERN_DEBUGONLY(hiddenMemPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultExtraMuzzleFlashRadius)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultExtraMuzzleFlashBrightness)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultExtraMuzzleFlashForward)\
	EASY_CVAR_EXTERN_DEBUGONLY(leaderlessSquadAllowed)\
	EASY_CVAR_EXTERN_DEBUGONLY(nodeConnectionBreakableCheck)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerReviveInvincibilityTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerReviveBuddhaMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerReviveTimeBlocksTimedDamage)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultBulletDamageMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultBulletsPerShot)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultFireAnimSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultMeleeAnimSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassassinCrossbowReloadSoundDelay)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntStrafeAnimSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntRunAndGunAnimSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(cheat_iwantguts)\
	EASY_CVAR_EXTERN_DEBUGONLY(nodeDetailPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(soundAttenuationStuka)\
	EASY_CVAR_EXTERN_DEBUGONLY(soundVolumeStuka)\
	EASY_CVAR_EXTERN_DEBUGONLY(cineChangelevelFix)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawDebugCine)\
	EASY_CVAR_EXTERN_DEBUGONLY(cineAllowSequenceOverwrite)\
	EASY_CVAR_EXTERN_DEBUGONLY(stukaInflictsBleeding)\
	EASY_CVAR_EXTERN_DEBUGONLY(animationKilledBoundsRemoval)\
	EASY_CVAR_EXTERN_DEBUGONLY(gargantuaKilledBoundsAssist)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitTrajTimeMin)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitTrajTimeMax)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitTrajDistMin)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitTrajDistMax)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitGravityMulti)\
	EASY_CVAR_EXTERN(cl_bullsquidspit)\
	EASY_CVAR_EXTERN(cl_bullsquidspitarc)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitUseAlphaModel)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitUseAlphaEffect)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitEffectSpread)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitEffectMin)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitEffectMax)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitEffectHitMin)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitEffectHitMax)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitEffectSpawn)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitEffectHitSpawn)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitSpriteScale)\
	EASY_CVAR_EXTERN_DEBUGONLY(bullsquidSpitAlphaScale)\
	EASY_CVAR_EXTERN_DEBUGONLY(scientistBravery)\
	EASY_CVAR_EXTERN_DEBUGONLY(barneyUnholsterTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(barneyUnholsterAnimChoice)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(trailTypeTest)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrail)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetTrailSolidColor)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModEasy)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModMedium)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetDeathModHard)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetZoomPuff)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpiral)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedMulti)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hornetSpeedDartMulti)\
	EASY_CVAR_EXTERN(cl_rockettrail)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaInterval)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketTrailAlphaScale)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(rocketSkipIgnite)\
	EASY_CVAR_EXTERN_DEBUGONLY(agruntHornetRandomness)\
	EASY_CVAR_EXTERN_DEBUGONLY(hornetSpiralPeriod)\
	EASY_CVAR_EXTERN_DEBUGONLY(hornetSpiralAmplitude)\
	EASY_CVAR_EXTERN(cl_hornetspiral)\
	EASY_CVAR_EXTERN(cl_hornettrail)\
	EASY_CVAR_EXTERN(hud_drawammobar)\
	EASY_CVAR_EXTERN(hud_weaponselecthideslower)\
	EASY_CVAR_EXTERN(hud_drawsidebarmode)\
	EASY_CVAR_EXTERN(gaussmode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_reflectdealsdamage)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDmgExMult)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMinDrowning)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashCumulativeMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDrawOpacityMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowDrawOpacityMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashFadeMult)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowFadeMult)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(painFlashArmorBlock)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashDirTolerance)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeAppearMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painArrowCumulativeDmgJump)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(painFlashPrintouts)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashCumulativeJump)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMin)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashDrawOpacityMax)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(itemFlashFadeMult)\
	EASY_CVAR_EXTERN_DEBUGONLY(chumtoadPrintout)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowReloadSoundDelay)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(crossbowFirePlaysReloadSound)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindFidgetFailTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindPrintout)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindTopRampFixDistance)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindTopRampFixDraw)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(iHaveAscended)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindLooseMapNodes)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindRampFix)\
	EASY_CVAR_EXTERN_DEBUGONLY(chumtoadPlayDeadFoolChance)\
	EASY_CVAR_EXTERN_DEBUGONLY(animationFramerateMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindNodeToleranceMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(friendlyPianoFollowVolume)\
	EASY_CVAR_EXTERN_DEBUGONLY(friendlyPianoOtherVolume)\
	EASY_CVAR_EXTERN_DEBUGONLY(showtriggers)\
	EASY_CVAR_EXTERN_DEBUGONLY(tentacleAlertSound)\
	EASY_CVAR_EXTERN_DEBUGONLY(tentacleSwingSound1)\
	EASY_CVAR_EXTERN_DEBUGONLY(tentacleSwingSound2)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerFollowerMax)\
	EASY_CVAR_EXTERN_DEBUGONLY(announcerIsAJerk)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerUseDrawDebug)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerChumtoadThrowDrawDebug)\
	EASY_CVAR_EXTERN_DEBUGONLY(peaceOut)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(drawViewModel)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(drawHUD)\
	EASY_CVAR_EXTERN_DEBUGONLY(disablePauseSinglePlayer)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerBulletHitEffectForceServer)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(forceAllowServersideTextureSounds)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponSpreadMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(monsterAIForceFindDistance)\
	EASY_CVAR_EXTERN_DEBUGONLY(baseEntityDamagePushNormalMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(baseEntityDamagePushVerticalBoost)\
	EASY_CVAR_EXTERN_DEBUGONLY(baseEntityDamagePushVerticalMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(baseEntityDamagePushVerticalMinimum)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelSyncFixPrintouts)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(textureHitSoundPrintouts)\
	EASY_CVAR_EXTERN_DEBUGONLY(hgruntAllowGrenades)\
	EASY_CVAR_EXTERN_DEBUGONLY(scheduleInterruptPrintouts)\
	EASY_CVAR_EXTERN_DEBUGONLY(animationPrintouts)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassaultMeleeAttackEnabled)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindStumpedWaitTime)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindStumpedMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindStumpedForgetEnemy)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindEdgeCheck)\
	EASY_CVAR_EXTERN_DEBUGONLY(RadiusDamageDrawDebug)\
	EASY_CVAR_EXTERN_DEBUGONLY(AlienRadiationImmunity)\
	EASY_CVAR_EXTERN_DEBUGONLY(customLogoSprayMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(monsterFadeOutRate)\
	EASY_CVAR_EXTERN_DEBUGONLY(playerFadeOutRate)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawDebugEnemyLKP)\
	EASY_CVAR_EXTERN_DEBUGONLY(trackchangePrintouts)\
	EASY_CVAR_EXTERN_DEBUGONLY(trackTrainPrintouts)\
	EASY_CVAR_EXTERN(cl_holster)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(playerWeaponTracerMode)\
	EASY_CVAR_EXTERN_DEBUGONLY(monsterWeaponTracerMode)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(decalTracerExclusivity)\
	EASY_CVAR_EXTERN_DEBUGONLY(monsterToPlayerHitgroupSpecial)\
	EASY_CVAR_EXTERN_DEBUGONLY(multiplayerCrowbarHitSoundMode)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(healthcolor_fullRedMin)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(healthcolor_brightness)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(healthcolor_yellowMark)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(cl_drawExtraZeros)\
	EASY_CVAR_EXTERN_DEBUGONLY(pathfindLargeBoundFix)\
	EASY_CVAR_EXTERN_DEBUGONLY(flyerKilledFallingLoop)\
	EASY_CVAR_EXTERN_DEBUGONLY(floaterDummy)\
	EASY_CVAR_EXTERN_DEBUGONLY(barneyDummy)\
	EASY_CVAR_EXTERN_DEBUGONLY(ladderCycleMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(ladderSpeedMulti)\
	EASY_CVAR_EXTERN_DEBUGONLY(barnacleGrabNoInterpolation)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(hideDamage)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMax)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(timedDamage_brightnessMin)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(timedDamage_brightnessCap)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(timedDamage_brightnessFloor)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(timedDamage_flashSpeed)\
	EASY_CVAR_EXTERN_CLIENTONLY_DEBUGONLY(timedDamage_debug)\
	EASY_CVAR_EXTERN_DEBUGONLY(wallHealthDoor_closeDelay)\
	EASY_CVAR_EXTERN_DEBUGONLY(houndeye_attack_canGib)\
	EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(myRocketsAreBarney)\
	EASY_CVAR_EXTERN_DEBUGONLY(hassassinCrossbowDebug)\
	EASY_CVAR_EXTERN_DEBUGONLY(crossbowBoltDirectionAffectedByWater)\
	EASY_CVAR_EXTERN_DEBUGONLY(kingpinDebug)\
	EASY_CVAR_EXTERN_DEBUGONLY(minimumRespawnDelay)\
	EASY_CVAR_EXTERN(r_glowshell_debug)\
	EASY_CVAR_EXTERN(cl_viewpunch)\
	EASY_CVAR_EXTERN(cl_explosion)\
	EASY_CVAR_EXTERN(soundSentenceSave)\
	EASY_CVAR_EXTERN(pissedNPCs)\
	EASY_CVAR_EXTERN(hud_logo)\
	EASY_CVAR_EXTERN(hud_brokentrans)\
	EASY_CVAR_EXTERN(cl_fvox)\
	EASY_CVAR_EXTERN(cl_ladder)\
	EASY_CVAR_EXTERN(precacheAll)\
	EASY_CVAR_EXTERN(cl_interp_entity)\
	EASY_CVAR_EXTERN(hud_swapFirstTwoBuckets)\
	EASY_CVAR_EXTERN(ignoreMultiplayerSkillOverride)\
	EASY_CVAR_EXTERN(m_rawinput)\
	EASY_CVAR_EXTERN(cl_earlyaccess)\
	EASY_CVAR_EXTERN(cl_viewroll)\
	EASY_CVAR_EXTERN_DEBUGONLY(drawDebugCrowbar)\
	EASY_CVAR_EXTERN(blastExtraArmorDamageMode)\
	EASY_CVAR_EXTERN(hud_batteryhiddendead)\
	EASY_CVAR_EXTERN_DEBUGONLY(hyperBarney)\
	EASY_CVAR_EXTERN(monsterKilledToss)\
	EASY_CVAR_EXTERN(interpolation_movetypestep_mindelta)\
	EASY_CVAR_EXTERN(sv_bloodparticlemode)\
	EASY_CVAR_EXTERN(cl_interp_view_extra)\
	EASY_CVAR_EXTERN(cl_interp_viewmodel)\
	EASY_CVAR_EXTERN(sv_explosionknockback)\
	EASY_CVAR_EXTERN(cl_gaussfollowattachment)\
	EASY_CVAR_EXTERN(cl_breakholster)\
	DUMMY

