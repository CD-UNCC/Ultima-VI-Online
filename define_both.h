#ifndef DEFINE_BOTH_H
#define DEFINE_BOTH_H
#define U6O_DEBUG FALSE/* should probably put this on the compiler command line instead of here!! */
#include "random/random.h"

#define VIEW_X_SIZE 32; // to be 24 sooner or later, black area for stats screens etc.
#define VIEW_Y_SIZE 24;

#define U6O_VERSION 1.0 //version 5.5
#define SHA2_ENCRYPTION 1
#define PW_ENCRYPTION 1

#define rndold (((float)rand())/32768.0f)
//#define rnd ((float)((RandomMers()<<8)>>16)/65536.0f)
//#define rnd ((float)((Randomc()<<8)>>16)/65536.0f)
#define rnd Randomc()
//equipped item positions (REVISE) (warning: left and right refer to the character's hand hence they are reversed on screen)
#define helmx 52
#define helmy 132
#define wep_rightx 20
#define wep_righty 170
#define wep_leftx 84
#define wep_lefty 170
#define armourx 52
#define armoury 166
#define bootsx 52
#define bootsy 216
#define ring_rightx 22
#define ring_righty 202
#define ring_leftx 84
#define ring_lefty 202
#define neckx 20
#define necky 138
#define INVALID_NET 0xFFFFFFFF//used by net_send(),...
//mover flags
#define MVLISTLAST 2047
#define MV_LIGHTBRIGHT 1
#define MV_LIGHTGLOW 2
#define MV_INVISIBLE 4
#define MV_PARALYZE 8
#define MV_SLEEP 16
#define MV_SLEEPBEDVERTICAL 32
#define MV_SLEEPBEDHORIZONTAL 64
#define MV_PROTECT 128
#define MV_STORMCLOAK 256
#define MV_TYPING 512
#define MV_KEYBTARGET 1024
#define MV_SPEAKING 2048
//key flags (REVISE)
#define KEYup 1
#define KEYdown 2
#define KEYleft 4
#define KEYright 8
#define KEYmb1 16  //relates to mf
#define KEYu 32 //use
#define KEYl 64 //look
#define KEYmbclick 128 //relates to mf
#define KEYa 256 //attack
#define KEYup2 512
#define KEYdown2 1024
#define KEYleft2 2048
#define KEYright2 4096
#define KEYbreakcombat 8192
#define KEYquan 16384
#define KEYf1 32768
//number of possible socket threads for client and host (REVISE)
#define SOCKETLAST 65535
//movernew()
#define MOVERNEW_FLAG_NORESPAWN 1
#define MOVERNEW_FLAG_PASSIVE 2
#define MOVERNEW_FLAG_FRIENDLY 4
#define MOVERNEW_FLAG_AGRESSIVE 8
//wpf_pathfind()
#define PFleft 0
#define PFright 1
#define PFup 2
#define PFdown 3
#define WPF_NOPATHFOUND 0
#define WPF_PATHFOUND 1
//getinput return values
#define GETINPUT_TAB_PRESSED_STOP 0
#define GETINPUT_TAB_PRESSED_INIT 1
#define GETINPUT_TAB_PRESSED 2
//spell reagent flags /* luteijn: SIGH totally different than the original; hopefully these are consistantly used so we can eventually make them the same as the original */
#define BLAC 1
#define BLOO 2
#define GARL 4
#define GINS 8
#define MAND 16
#define NIGH 32
#define SPID 64
#define SULF 128
//used by gypsy to select correct vial
#define U6O2_HONESTY x4=0;
#define U6O2_COMPASSION x4=1;
#define U6O2_VALOR x4=2;
#define U6O2_JUSTICE x4=3;
#define U6O2_SACRIFICE x4=4;
#define U6O2_HONOR x4=5;
#define U6O2_SPIRITUALITY x4=6;
#define U6O2_HUMILITY x4=7;
//eXtra mouse buttons
#define WM_XBUTTONDOWN                  0x020B
#define WM_XBUTTONUP                    0x020C
#define XBUTTON1      0x0001
#define XBUTTON2      0x0002

/* special effect types TODO: use these everywhere */
#define SF_UNNAMED_0	(0) /* probably never used... */
#define SF_ATTACK_HIT	(1)
#define SF_ARROW	(2)
#define SF_TXT_PORTRAIT	(3)
#define SF_SOUND_EFFECT	(4)
#define SF_LIGHTNING	(5) 
#define SF_BOLT		(6)
#define SF_BOOMERANG	(7)
#define SF_FIRE_BOLT	(8)
#define SF_THROWN_OBJ	(9)
#define SF_DAGGER	(10)
#define SF_THROWING_AXE	(11)
#define SF_SPEAR	(12)
#define SF_SLING_STONE	(13)
#define SF_BLUE_BALL	(14)
#define SF_KILL_BOLT	(15)
#define SF_ATTACK_MISS	(16)
#define SF_CANNON_BALL	(17)
#define SF_GREEN_BALL	(18)
#define SF_BLUE_BOLT	(19)
#define SF_PINK_BALL	(20)
#define SF_STOLEN_ITEM	(21)
#define SF_FIREBALLS	(22)
#define SF_GREEN_BALLS	(23)
#define SF_LIGHTNINGS	(24)
#define SF_DEATH_BOLTS	(25)
#define SF_TREMOR	(26)
#define SF_INV_ADD	(27)
#define SF_RED_BOLT	(28)
#define SF_RED_BALL	(29)
/* unused so far */
#define SF_UNNAMED_30	(30)
#define SF_UNNAMED_31	(31)
#define SF_UNNAMED_32	(32)
#define SF_UNNAMED_33	(33)
#define SF_UNNAMED_34	(34)
#define SF_UNNAMED_35	(35)
#define SF_UNNAMED_36	(36)

/* protocol message types */

#define MSG_0			(0) /* probably never used */
#define MSG_OBSOLETE_SCENE	(1)
#define MSG_SF			(2)
#define MSG_CHARACTER		(3)
#define MSG_MOUSE_OBJECT	(4)
#define MSG_KEY_EVENT		(5)
#define MSG_LOCAL_TALK		(6)
#define MSG_DIRECTED_TALK	(7)
#define MSG_STATUS_MESSAGE	(8)
#define MSG_NAME_REQUEST	(9)
#define MSG_NAME_REPLY		(10)
#define MSG_BG_MIDI		(11)
#define MSG_GLOBAL_TALK		(12)
#define MSG_MULTIPLE_NUMBER	(13)
#define MSG_WIND_DIRECTION	(14)
/* TODO/FIXME etc, see messageformat.html and actual code! */

/* objects */

#define OBJ_NOTHING_0 (0)
#define OBJ_LEATHER_HELM (1)
#define OBJ_CHAIN_COIF (2)
#define OBJ_IRON_HELM (3)
#define OBJ_SPIKED_HELM (4)
#define OBJ_WINGED_HELM (5)
#define OBJ_BRASS_HELM (6)
#define OBJ_GARGOYLE_HELM (7)
#define OBJ_MAGIC_HELM (8)
#define OBJ_WOODEN_SHIELD (9)
#define OBJ_CURVED_HEATER (10)
#define OBJ_WINGED_SHIELD (11)
#define OBJ_KITE_SHIELD (12)
#define OBJ_SPIKED_SHIELD (13)
#define OBJ_BLACK_SHIELD (14)
#define OBJ_DOOR_SHIELD (15)
#define OBJ_MAGIC_SHIELD (16)
#define OBJ_CLOTH_ARMOUR (17)
#define OBJ_LEATHER_ARMOR (18)
#define OBJ_RING_MAIL (19)
#define OBJ_SCALE_MAIL (20)
#define OBJ_CHAIN_MAIL (21)
#define OBJ_PLATE_MAIL (22)
#define OBJ_MAGIC_ARMOUR (23)
#define OBJ_SPIKED_COLLAR (24)
#define OBJ_GUILD_BELT (25)
#define OBJ_GARGOYLE_BELT (26)
#define OBJ_LEATHER_BOOTS (27)
#define OBJ_SWAMP_BOOTS (28)
#define OBJ_EARTH (29)
#define OBJ_FLOOR_WOOD (30)
#define OBJ_FLOOR_TILE (31)
#define OBJ_FLOOR_BLUE (32)
#define OBJ_SLING (33)
#define OBJ_CLUB (34)
#define OBJ_MAIN_GAUCHE (35)
#define OBJ_SPEAR (36)
#define OBJ_THROWING_AXE (37)
#define OBJ_DAGGER (38)
#define OBJ_MACE (39)
#define OBJ_MORNING_STAR (40)
#define OBJ_BOW (41)
#define OBJ_CROSSBOW (42)
#define OBJ_SWORD (43)
#define OBJ_TWO_HANDED_HAMMER (44)
#define OBJ_TWO_HANDED_AXE (45)
#define OBJ_TWO_HANDED_SWORD (46)
#define OBJ_HALBERD (47)
#define OBJ_GLASS_SWORD (48)
#define OBJ_BOOMERANG (49)
#define OBJ_TRIPLE_CROSSBOW (50)
#define OBJ_FORCE_FIELD (51)
#define OBJ_WIZARD_EYE (52)
#define OBJ_WEB (53)
#define OBJ_MAGIC_BOW (54)
#define OBJ_ARROW (55)
#define OBJ_BOLT (56)
#define OBJ_SPELLBOOK (57)
#define OBJ_SPELL (58)
#define OBJ_CODEX (59)
#define OBJ_BOOK_OF_PROPHECIES (60)
#define OBJ_BOOK_OF_CIRCLES (61)
#define OBJ_VORTEX_CUBE (62)
#define OBJ_LOCK_PICK (63)
#define OBJ_KEY (64)
#define OBJ_BLACK_PEARL (65)
#define OBJ_BIT_OF_BLOOD_MOSS (66)
#define OBJ_BULB_OF_GARLIC (67)
#define OBJ_GINSENG_ROOT (68)
#define OBJ_MANDRAKE_ROOT (69)
#define OBJ_NIGHTSHADE_MUSHROOM (70)
#define OBJ_STRAND_OF_SPIDERSILK (71)
#define OBJ_BIT_OF_SULFUROUS_ASH (72)
#define OBJ_MOONSTONE (73)
#define OBJ_ANKH_AMULET (74)
#define OBJ_SNAKE_AMULET (75)
#define OBJ_AMULET_OF_SUBMISSION (76)
#define OBJ_GEM (77)
#define OBJ_STAFF (78)
#define OBJ_LIGHTNING_WAND (79)
#define OBJ_FIRE_WAND (80)
#define OBJ_STORM_CLOAK (81)
#define OBJ_RING (82)
#define OBJ_FLASK_OF_OIL (83)
#define OBJ_RED_GATE (84)
#define OBJ_MOONGATE (85)
#define OBJ_GAVEL (86)
#define OBJ_ORB_OF_THE_MOONS (87)
#define OBJ_GOLD_COIN (88)
#define OBJ_GOLD_NUGGET (89)
#define OBJ_TORCH (90)
#define OBJ_ZU_YLEM (91)
#define OBJ_SILVER_SNAKE_VENOM (92)
#define OBJ_SEXTANT (93)
#define OBJ_SPINNING_WHEEL (94)
#define OBJ_BUNCH_OF_GRAPES (95)
#define OBJ_BUTTER (96)
#define OBJ_GARGISH_VOCABULARY (97)
#define OBJ_CHEST (98)
#define OBJ_BACKPACK (99)
#define OBJ_SCYTHE (100)
#define OBJ_PITCHFORK (101)
#define OBJ_RAKE (102)
#define OBJ_PICK (103)
#define OBJ_SHOVEL (104)
#define OBJ_HOE (105)
#define OBJ_WOODEN_LADDER (106)
#define OBJ_YOKE (107)
#define OBJ_OVEN_SPATULA (108)
#define OBJ_ROLLING_PIN (109)
#define OBJ_SPATULA (110)
#define OBJ_LADLE (111)
#define OBJ_COOKING_SHEET (112)
#define OBJ_CLEAVER (113)
#define OBJ_KNIFE (114)
#define OBJ_WINE (115)
#define OBJ_MEAD (116)
#define OBJ_ALE (117)
#define OBJ_WINE_GLASS (118)
#define OBJ_PLATE (119)
#define OBJ_MUG (120)
#define OBJ_SILVERWARE (121)
#define OBJ_CANDLE (122)
#define OBJ_MIRROR (123)
#define OBJ_TUNIC (124)
#define OBJ_HANGER (125)
#define OBJ_DRESS (126)
#define OBJ_SKILLET (127)
#define OBJ_LOAF_OF_BREAD (128)
#define OBJ_PORTION_OF_MEAT (129)
#define OBJ_ROLLS (130)
#define OBJ_CAKE (131)
#define OBJ_CHEESE (132)
#define OBJ_HAM (133)
#define OBJ_HORSE_CARCASS (134)
#define OBJ_HORSE_CHOPS (135)
#define OBJ_SKEWER (136)
#define OBJ_PANTS (137)
#define OBJ_PLANT (138)
#define OBJ_FLOWERS (139)
#define OBJ_WALL_MOUNT (140)
#define OBJ_DECORATIVE_SWORD (141)
#define OBJ_DECORATIVE_SHIELD (142)
#define OBJ_PICTURE (143)
#define OBJ_TAPESTRY (144)
#define OBJ_CANDELABRA (145)
#define OBJ_PERSON_SLEEPING (146)
#define OBJ_CAULDRON (147)
#define OBJ_CAULDRON_FULL (148)
#define OBJ_SHIP_DEED (149)
#define OBJ_INKWELL (150)
#define OBJ_BOOK (151)
#define OBJ_SCROLL (152)
#define OBJ_PANPIPES (153)
#define OBJ_TELESCOPE (154)
#define OBJ_CRYSTAL_BALL (155)
#define OBJ_HARPSICHORD (156)
#define OBJ_HARP (157)
#define OBJ_LUTE (158)
#define OBJ_CLOCK (159)
#define OBJ_ENDTABLE (160)
#define OBJ_WATER_VASE (161)
#define OBJ_STOVE (162)
#define OBJ_BED (163)
#define OBJ_FIREPLACE (164)
#define OBJ_STALAGMITE (165)
#define OBJ_SACK_OF_GRAIN (166)
#define OBJ_SACK_OF_FLOUR (167)
#define OBJ_REMAINS (168)
#define OBJ_RUBBER_DUCKY (169)
#define OBJ_URN_OF_ASHES (170)
#define OBJ_FUMAROLE (171)
#define OBJ_SPIKES (172)
#define OBJ_TRAP (173)
#define OBJ_SWITCH (174)
#define OBJ_ELECTRIC_FIELD (175)
#define OBJ_CHEST_OF_DRAWERS (176)
#define OBJ_DESK (177)
#define OBJ_BUCKET (178)
#define OBJ_BUCKET_OF_WATER (179)
#define OBJ_BUCKET_OF_MILK (180)
#define OBJ_CHURN (181)
#define OBJ_BEEHIVE (182)
#define OBJ_HONEY_JAR (183)
#define OBJ_JAR_OF_HONEY (184)
#define OBJ_CLOTH (185)
#define OBJ_BARREL (186)
#define OBJ_JUG (187)
#define OBJ_BAG (188)
#define OBJ_CASK (189)
#define OBJ_BALE_OF_WOOL (190)
#define OBJ_BASKET (191)
#define OBJ_CRATE (192)
#define OBJ_SMALL_JUG (193)
#define OBJ_MILK_BOTTLE (194)
#define OBJ_WHEAT (195)
#define OBJ_VAT (196)
#define OBJ_WINE_CASK (197)
#define OBJ_CUTTING_TABLE (198)
#define OBJ_LOOM (199)
#define OBJ_HOOD (200)
#define OBJ_FIRE (201)
#define OBJ_HORSESHOES (202)
#define OBJ_PLIERS (203)
#define OBJ_HAMMER (204)
#define OBJ_WATER_TROUGH (205)
#define OBJ_BRAZIER (206)
#define OBJ_ROD (207)
#define OBJ_HOOK (208)
#define OBJ_MEAT (209)
#define OBJ_RIBS (210)
#define OBJ_DEAD_ANIMAL (211)
#define OBJ_FAN (212)
#define OBJ_MOUSE_HOLE (213)
#define OBJ_WINE_PRESS (214)
#define OBJ_STABLE (215)
#define OBJ_BOOKSHELF (216)
#define OBJ_ANVIL (217)
#define OBJ_BELLOWS (218)
#define OBJ_OVEN (219)
#define OBJ_FLAG (220)
#define OBJ_CANNON (221)
#define OBJ_CANNON_BALLS (222)
#define OBJ_POWDER_KEG (223)
#define OBJ_FOOT_RAIL (224)
#define OBJ_SPOOL_OF_THREAD (225)
#define OBJ_SPOOL_OF_SILK (226)
#define OBJ_PENNANT (227)
#define OBJ_TABLE_SQUARE_TOP (228)
#define OBJ_SHADOW_TABLE_SQUARE_TOP (229)
#define OBJ_TABLE_CIRCLE_TOP (230)
#define OBJ_SHADOW_TABLE_CIRCLE_TOP (231)
#define OBJ_SPITTOON (232)
#define OBJ_WELL (233)
#define OBJ_FOUNTAIN (234)
#define OBJ_SUNDIAL (235)
#define OBJ_BELL (236)
#define OBJ_TABLE_CENTRE (237)
#define OBJ_SHADOW_TABLE_CENTRE (238)
#define OBJ_TABLE_ROUND (239)
#define OBJ_SHADOW_TABLE_ROUND (240)
#define OBJ_SILK_CLOTH (241)
#define OBJ_RUNE_OF_HONESTY (242)
#define OBJ_RUNE_OF_COMPASSION (243)
#define OBJ_RUNE_OF_VALOR (244)
#define OBJ_RUNE_OF_JUSTICE (245)
#define OBJ_RUNE_OF_SACRIFICE (246)
#define OBJ_RUNE_OF_HONOR (247)
#define OBJ_RUNE_OF_SPIRITUALITY (248)
#define OBJ_RUNE_OF_HUMILITY (249)
#define OBJ_TABLE_SQUARE_BOTTOM (250)
#define OBJ_SHADOW_TABLE_SQUARE_BOTTOM (251)
#define OBJ_CHAIR (252)
#define OBJ_CAMPFIRE (253)
#define OBJ_CROSS (254)
#define OBJ_TOMBSTONE (255)
#define OBJ_PROTECTION_RING (256)
#define OBJ_REGENERATION_RING (257)
#define OBJ_INVISIBILITY_RING (258)
#define OBJ_TABLE_LEG_LEFT (259)
#define OBJ_SHADOW_TABLE_BOTTOM (260)
#define OBJ_TABLE_LEG_RIGHT (261)
#define OBJ_SHADOW_TABLE_LEG_RIGHT (262)
#define OBJ_STOCKS (263)
#define OBJ_FISHING_POLE (264)
#define OBJ_FISH (265)
#define OBJ_GRAVE (266)
#define OBJ_GUILLOTINE (267)
#define OBJ_LEVER (268)
#define OBJ_DRAWBRIDGE (269)
#define OBJ_BALLOON_PLANS (270)
#define OBJ_DOORSILL (271)
#define OBJ_STEPS_ENDS (272)
#define OBJ_TILE (273)
#define OBJ_YEW_LOG (274)
#define OBJ_POTION (275)
#define OBJ_STEPS_MAIN (276)
#define OBJ_YEW_BOARD (277)
#define OBJ_PASSTHROUGH_V (278)
#define OBJ_TABLE (279)
#define OBJ_PASSTHROUGH_H (280)
#define OBJ_FENCE (281)
#define OBJ_BARS (282)
#define OBJ_ANCHOR (283)
#define OBJ_ROPE (284)
#define OBJ_POLE (285)
#define OBJ_WALKWAY (286)
#define OBJ_WATER_WHEEL (287)
#define OBJ_CRANK (288)
#define OBJ_LOG_SAW (289)
#define OBJ_MILL_STONE (290)
#define OBJ_SHAFT (291)
#define OBJ_GEARWORK (292)
#define OBJ_CHAIN (293)
#define OBJ_LIGHTSOURCE (294)
#define OBJ_HEATSOURCE (295)
#define OBJ_XYLOPHONE (296)
#define OBJ_OAKEN_DOOR (297)
#define OBJ_WINDOWED_DOOR (298)
#define OBJ_CEDAR_DOOR (299)
#define OBJ_STEEL_DOOR (300)
#define OBJ_DOORWAY (301)
#define OBJ_ARCHWAY (302)
#define OBJ_CARPET (303)
#define OBJ_COOKFIRE (304)
#define OBJ_LADDER (305)
#define OBJ_TRELLIS (306)
#define OBJ_VOLCANO (307)
#define OBJ_HOLE (308)
#define OBJ_BONES (309)
#define OBJ_PORTCULLIS (310)
#define OBJ_STONE_TABLE (311)
#define OBJ_STONE_LION (312)
#define OBJ_SILVER_HORN (313)
#define OBJ_FLOOR_SLAB (314)
#define OBJ_STONE (315)
#define OBJ_LAMPPOST (316)
#define OBJ_FIRE_FIELD (317)
#define OBJ_POISON_FIELD (318)
#define OBJ_PROTECTION_FIELD (319)
#define OBJ_SLEEP_FIELD (320)
#define OBJ_STATUE (321)
#define OBJ_POOL (322)
#define OBJ_MONOLITH (323)
#define OBJ_PILLAR (324)
#define OBJ_BOOK_STAND (325)
#define OBJ_MINE_SHAFT (326)
#define OBJ_THRONE (327)
#define OBJ_ALTAR (328)
#define OBJ_ALTAR_OF_SINGULARITY (329)
#define OBJ_MAT (330)
#define OBJ_GOVERNMENT_SIGN (331)
#define OBJ_SIGN (332)
#define OBJ_GARGOYLE_SIGN (333)
#define OBJ_SECRET_DOOR (334)
#define OBJ_EGG (335)
#define OBJ_CHARGE (336)
#define OBJ_EFFECT (337)
#define OBJ_BLOOD (338)
#define OBJ_DEAD_BODY (339)
#define OBJ_DEAD_CYCLOPS (340)
#define OBJ_DEAD_GARGOYLE (341)
#define OBJ_GIANT_RAT (342)
#define OBJ_INSECTS (343)
#define OBJ_GIANT_BAT (344)
#define OBJ_GIANT_SQUID (345)
#define OBJ_SEA_SERPENT (346)
#define OBJ_REAPER (347)
#define OBJ_SHEEP (348)
#define OBJ_DOG (349)
#define OBJ_DEER (350)
#define OBJ_WOLF (351)
#define OBJ_GHOST (352)
#define OBJ_GREMLIN (353)
#define OBJ_MOUSE (354)
#define OBJ_GAZER (355)
#define OBJ_BIRD (356)
#define OBJ_CORPSER (357)
#define OBJ_SNAKE (358)
#define OBJ_RABBIT (359)
#define OBJ_ROT_WORMS (360)
#define OBJ_GIANT_SPIDER (361)
#define OBJ_GARGOYLE_WINGED (362)
#define OBJ_GARGOYLE_WINGLESS (363)
#define OBJ_ACID_SLUG (364)
#define OBJ_TANGLE_VINE_POD (365)
#define OBJ_TANGLE_VINE (366)
#define OBJ_DAEMON (367)
#define OBJ_SKELETON (368)
#define OBJ_DRAKE (369)
#define OBJ_HEADLESS (370)
#define OBJ_TROLL (371)
#define OBJ_MONGBAT (372)
#define OBJ_WISP (373)
#define OBJ_HYDRA_MOUTH (374)
#define OBJ_SLIME (375)
#define OBJ_FIGHTER (376)
#define OBJ_SWASHBUCKLER (377)
#define OBJ_MAGE (378)
#define OBJ_VILLAGER (379)
#define OBJ_MERCHANT (380)
#define OBJ_CHILD (381)
#define OBJ_GUARD (382)
#define OBJ_JESTER (383)
#define OBJ_PEASANT (384)
#define OBJ_FARMER (385)
#define OBJ_MUSICIAN (386)
#define OBJ_WOMAN (387)
#define OBJ_CAT (388)
#define OBJ_SILVER_TABLET (389)
#define OBJ_SILVER_FRAGMENT (390)
#define OBJ_FARMER_HOE (391)
#define OBJ_MUSICIAN_LUTE (392)
#define OBJ_SHRINE (393)
#define OBJ_BRITANNIAN_LENS (394)
#define OBJ_BROKEN_LENS (395)
#define OBJ_GARGOYLE_LENS (396)
#define OBJ_STATUE_OF_MONDAIN (397)
#define OBJ_STATUE_OF_MINAX (398)
#define OBJ_STATUE_OF_EXODUS (399)
#define OBJ_PART_OF_A_MAP_0 (400)
#define OBJ_PART_OF_A_MAP_1 (401)
#define OBJ_PART_OF_A_MAP_2 (402)
#define OBJ_PART_OF_A_MAP_3 (403)
#define OBJ_PART_OF_A_MAP_4 (404)
#define OBJ_PART_OF_A_MAP_5 (405)
#define OBJ_PART_OF_A_MAP_6 (406)
#define OBJ_PART_OF_A_MAP_7 (407)
#define OBJ_PART_OF_A_MAP_8 (408)
#define OBJ_LORD_BRITISH (409)
#define OBJ_AVATAR (410)
#define OBJ_DRAGON (411)
#define OBJ_SHIP (412)
#define OBJ_SILVER_SERPENT (413)
#define OBJ_SKIFF (414)
#define OBJ_RAFT (415)
#define OBJ_NOTHING (416)
#define OBJ_DRAGON_EGG (417)
#define OBJ_HATCHED_DRAGON_EGG (418)
#define OBJ_PULL_CHAIN (419)
#define OBJ_BALLOON_DEFLATED (420)
#define OBJ_MAMMOTH_SILK_BAG (421)
#define OBJ_BALLOON_BASKET (422)
#define OBJ_BALLOON (423)
#define OBJ_CYCLOPS (424)
#define OBJ_HYDRA_ARMS (425)
#define OBJ_GIANT_SCORPION (426)
#define OBJ_GIANT_ANT (427)
#define OBJ_COW (428)
#define OBJ_ALLIGATOR (429)
#define OBJ_HORSE (430)
#define OBJ_HORSE_WITH_RIDER (431)
#define OBJ_DEAD_GIANT_RAT (432)
#define OBJ_DEAD_GIANT_BAT (433)
#define OBJ_DEAD_REAPER (434)
#define OBJ_DEAD_SHEEP (435)
#define OBJ_DEAD_DOG (436)
#define OBJ_DEAD_DEER (437)
#define OBJ_DEAD_WOLF (438)
#define OBJ_DEAD_GREMLIN (439)
#define OBJ_DEAD_MOUSE (440)
#define OBJ_DEAD_BIRD (441)
#define OBJ_DEAD_SNAKE (442)
#define OBJ_DEAD_RABBIT (443)
#define OBJ_DEAD_SPIDER (444)
#define OBJ_DEAD_DRAKE (445)
#define OBJ_DEAD_MONGBAT (446)
#define OBJ_DEAD_CAT (447)
#define OBJ_HORSE_PAPERS (448)
#define OBJ_SCORPION_EGG (449)
#define OBJ_MULTI_LENS_TELESCOPE (450)
#define OBJ_TMAP (451)
#define OBJ_POCKET_WATCH (452)


// r666 (obsolete; r999 *mostly* replaced this)
/*
#define ACTIONBAR_MAX 2
#define ACTIONTALKBAR_MAX 2
#define ACTIONBUTTON_MAX 5
#define ACTIONTALKBUTTON_MAX 10
#define ACTIONBAR_HEIGHT 52

#define ACTIONBUTTON_ATTACK 0
#define ACTIONBUTTON_TALK 1
#define ACTIONBUTTON_LOOK 2
#define ACTIONBUTTON_USE 3
#define ACTIONBUTTON_DROP 4

#define ACTIONBUTTON_FOOD 0
#define ACTIONBUTTON_MINIMAP 3
#define ACTIONBUTTON_QUIT 4

#define ACTIONTALKBUTTON_NAME 0
#define ACTIONTALKBUTTON_JOB 1
#define ACTIONTALKBUTTON_BUY 2
#define ACTIONTALKBUTTON_SELL 3
#define ACTIONTALKBUTTON_YES 4
#define ACTIONTALKBUTTON_NO 5
#define ACTIONTALKBUTTON_BULK 6
#define ACTIONTALKBUTTON_SPELL 7
#define ACTIONTALKBUTTON_REAGENT 8
#define ACTIONTALKBUTTON_POTION 9

#define ACTIONTALKBUTTON_DEPOSIT 0
#define ACTIONTALKBUTTON_WITHDRAW 1
#define ACTIONTALKBUTTON_BALANCE 2
#define ACTIONTALKBUTTON_EXCHANGE 3
#define ACTIONTALKBUTTON_HOUSE 4
#define ACTIONTALKBUTTON_PAYMENT 5
#define ACTIONTALKBUTTON_HEAL 6
#define ACTIONTALKBUTTON_CURE 7
#define ACTIONTALKBUTTON_RESURRECT 8
#define ACTIONTALKBUTTON_HELP 9
*/

// r777 mx and my locations (mostly for 1024 res)
// DO NOT USE.  These locations are not always correct.
/*
#define MX_TILE 30 // tile size in pixel (1024 res)
#define MY_TILE 30 // tile size in pixel (1024 res)
#define MX_AVATAR 495 // 480 to 510
#define MY_AVATAR 365 // 350 to 380
#define MX_AVATAR_TOP MX_AVATAR
#define MY_AVATAR_TOP MY_AVATAR - MY_TILE
#define MX_AVATAR_BOTTOM MX_AVATAR
#define MY_AVATAR_BOTTOM MY_AVATAR + MY_TILE
#define MX_AVATAR_LEFT MX_AVATAR - MY_TILE
#define MY_AVATAR_LEFT MY_AVATAR
#define MX_AVATAR_RIGHT MX_AVATAR + MY_TILE
#define MY_AVATAR_RIGHT MY_AVATAR

#define POS_AVATAR_MX 476 // 480 to 510
#define POS_AVATAR_MY MY_AVATAR // 350 to 380
#define POS_AVATAR_MX1 508 // 480 to 510
#define POS_AVATAR_MY1 350 // 350 to 380
#define POS_AVATAR_MX2 510 // 480 to 510
#define POS_AVATAR_MY2 386 // 350 to 380
*/


// r999
#define PANEL_MAX 10 // obsolete

// r999 new
#define UI_PANEL_MAX 20
#define UI_PANELWIDGET_MAX 10+1 // +1 because widget[0] is the panel itself
#define UI_WIDGETSTATE_MAX 6

#define UI_PANEL_IMG_MAX UI_PANEL_MAX
#define UI_PANELWIDGET_IMG_MAX 20
#define UI_WIDGETSTATE_IMG_MAX UI_WIDGETSTATE_MAX
#define UI_IMGI_ACTIONBAR 0
//#define UI_IMGI_OPTIONBAR 5
#define UI_IMGI_ACTIONTALKBAR 5
#define UI_IMGI_WIDGET 0
#define UI_IMGI_HOVER UI_IMGI_WIDGET+13


#define UI_NOSCALE -1.0f
#define UI_WIDGET_DEF 0
#define UI_STATE_DEF 0

//#define ACTIONBAR_MAX 2
//#define ACTIONTALKBAR_MAX 2
#define UI_WIDGET_ACTIONBUTTON_MAX 5+1 // +1 because widget[0] is the panel itself
#define UI_WIDGET_ACTIONTALKBUTTON_MAX 10+1 // +1 because widget[0] is the panel itself
//#define UI_ACTIONBUTTON_SIZEX 51
//#define UI_ACTIONBUTTON_SIZEY 51

#define UI_WIDGET_ACTIONBUTTON_ATTACK 1
#define UI_WIDGET_ACTIONBUTTON_TALK 2
#define UI_WIDGET_ACTIONBUTTON_LOOK 3
#define UI_WIDGET_ACTIONBUTTON_USE 4
#define UI_WIDGET_ACTIONBUTTON_DROP 5

#define UI_WIDGET_ACTIONBUTTON_FOOD 1
#define UI_WIDGET_ACTIONBUTTON_WORLDMAP 4
#define UI_WIDGET_ACTIONBUTTON_COMBATLOG 5

#define UI_WIDGET_OPTIONBUTTON_ENHANCE 1
#define UI_WIDGET_OPTIONBUTTON_SOUND 2
#define UI_WIDGET_OPTIONBUTTON_COMBATSOUND 3
#define UI_WIDGET_OPTIONBUTTON_MINIMAP 4
#define UI_WIDGET_OPTIONBUTTON_QUIT 5

#define UI_WIDGET_MAPBUTTON_U6CLOTH 1
#define UI_WIDGET_MAPBUTTON_U6P 2
#define UI_WIDGET_MAPBUTTON_U6G 3
#define UI_WIDGET_MAPBUTTON_U6RUNE 4
#define UI_WIDGET_MAPBUTTON_CLOSE 5

#define UI_WIDGET_ACTIONTALKBUTTON_NAME 1
#define UI_WIDGET_ACTIONTALKBUTTON_JOB 2
#define UI_WIDGET_ACTIONTALKBUTTON_HEAL 3
#define UI_WIDGET_ACTIONTALKBUTTON_CURE 4
#define UI_WIDGET_ACTIONTALKBUTTON_RESURRECT 5
#define UI_WIDGET_ACTIONTALKBUTTON_INSURANCE 6
#define UI_WIDGET_ACTIONTALKBUTTON_SPELL 7
#define UI_WIDGET_ACTIONTALKBUTTON_REAGENT 8
#define UI_WIDGET_ACTIONTALKBUTTON_POTION 9
#define UI_WIDGET_ACTIONTALKBUTTON_STAFF 10

#define UI_WIDGET_ACTIONTALKBUTTON_YES 1
#define UI_WIDGET_ACTIONTALKBUTTON_NO 2
#define UI_WIDGET_ACTIONTALKBUTTON_SELL 3
#define UI_WIDGET_ACTIONTALKBUTTON_BULK 4
#define UI_WIDGET_ACTIONTALKBUTTON_BUY 5
#define UI_WIDGET_ACTIONTALKBUTTON_ARMOR 6
#define UI_WIDGET_ACTIONTALKBUTTON_ARMS 7
#define UI_WIDGET_ACTIONTALKBUTTON_WEAPON 8
#define UI_WIDGET_ACTIONTALKBUTTON_ARROWS 9
#define UI_WIDGET_ACTIONTALKBUTTON_BOLTS 10

#define UI_WIDGET_ACTIONTALKBUTTON_DEPOSIT 1
#define UI_WIDGET_ACTIONTALKBUTTON_WITHDRAW 2
#define UI_WIDGET_ACTIONTALKBUTTON_BALANCE 3
#define UI_WIDGET_ACTIONTALKBUTTON_EXCHANGE 4
#define UI_WIDGET_ACTIONTALKBUTTON_HOUSE 5
#define UI_WIDGET_ACTIONTALKBUTTON_PAYMENT 6
#define UI_WIDGET_ACTIONTALKBUTTON_JOIN 7
#define UI_WIDGET_ACTIONTALKBUTTON_LEAVE 8
#define UI_WIDGET_ACTIONTALKBUTTON_BAGS 9
#define UI_WIDGET_ACTIONTALKBUTTON_HELP 10



// s222
#define SOUND_LEVER 6
#define SOUND_DOOROPEN 1
#define SOUND_DOORCLOSE 2
#define SOUND_FIELD 5

#define SOUND_COMBAT_HIT 0
#define SOUND_COMBAT_BOW 7
#define SOUND_COMBAT_CROSSBOW 8
#define SOUND_COMBAT_FIREBALL 20

#define SOUND_LEVERN1 45
#define SOUND_DOOROPENN1 47
#define SOUND_DOORCLOSEN1 48
#define SOUND_DOOROPENN2 54
#define SOUND_DOORCLOSEN2 55
#define SOUND_FIELDN1 61

#define SOUND_STATUSMESSAGE 59 // 49
#define SOUND_STEALINGWARNING 49
#define SOUND_STOLENITEM 62
#define SOUND_STATUSMESSAGELOWPRIORITY 51
#define SOUND_USERACTION 50
#define SOUND_USERACTIONDENIED 46
#define SOUND_UIACTION 50
#define SOUND_EAT 56
#define SOUND_DRINK 57
#define SOUND_DROP 58
#define SOUND_NOTIFY 59
#define SOUND_PLAYERENTERED 60

#define SOUND_COMBAT_BOWN1 44
#define SOUND_COMBAT_CROSSBOWN1 53
#define SOUND_COMBAT_FIREBALLN1 39
#define SOUND_COMBAT_MISS1N1 36
#define SOUND_COMBAT_MISS2N1 37
#define SOUND_COMBAT_PLAYERHURTN1 38
#define SOUND_COMBAT_HITN1 52

#define SOUND_MAGIC_READY 41
#define SOUND_MAGIC_NOTREADY 46
#define SOUND_MAGIC_FREEZE 42
#define SOUND_MAGIC_UNFREEZE 43
#define SOUND_MAGIC_FAILED1 62
#define SOUND_MAGIC_FAILED2 63
#define SOUND_MAGIC_FAILED3 64
#define SOUND_MAGIC_SUCCESS 65


// b111
#define WALL_NORTH 1
#define WALL_SOUTH 2
#define WALL_WEST 3
#define WALL_EAST 4

#define BUILD_TYPE_HORIZONTAL 10
#define BUILD_TYPE_VERTICAL 20
#define BUILD_TYPE_NORTH 1
#define BUILD_TYPE_SOUTH 2
#define BUILD_TYPE_WEST 3
#define BUILD_TYPE_EAST 4

#define BUILD_TYPE_H BUILD_TYPE_HORIZONTAL
#define BUILD_TYPE_V BUILD_TYPE_VERTICAL
#define BUILD_TYPE_N BUILD_TYPE_NORTH
#define BUILD_TYPE_S BUILD_TYPE_SOUTH
#define BUILD_TYPE_W BUILD_TYPE_WEST
#define BUILD_TYPE_E BUILD_TYPE_EAST

#define BUILD_WALLSTYLE_STONE1 1110
#define BUILD_WALLSTYLE_STONE2 1111

#define BUILD_FLOORSTYLE_BLUE1 2110
#define BUILD_FLOORSTYLE_BLUE2 2111

#define BUILD_SKIPSIDE_NONE 0
#define BUILD_SKIPSIDE_NORTH 1
#define BUILD_SKIPSIDE_SOUTH 2
#define BUILD_SKIPSIDE_WEST 4
#define BUILD_SKIPSIDE_EAST 8
#define BUILD_SKIPSIDE_ENDPOINTS 16


#define BUILD_TILEOPTION_D 1
#define BUILD_TILEOPTION_N 11
#define BUILD_TILEOPTION_S 12
#define BUILD_TILEOPTION_W 13
#define BUILD_TILEOPTION_E 14
#define BUILD_TILEOPTION_NW 21
#define BUILD_TILEOPTION_NE 22
#define BUILD_TILEOPTION_SW 23
#define BUILD_TILEOPTION_SE 24
#define BUILD_TILEOPTION_H 31
#define BUILD_TILEOPTION_V 32

#define BUILD_TABLEWITHSTORAGE_NO 0
#define BUILD_TABLEWITHSTORAGE_YES 1


#define BUILD_HOUSEID_MAX 50
#define BUILD_SECTIONID_MAX 50
#define BUILD_OBJPOINTER_MAX 10
#define BUILD_ARENAID_MAX 10

#define BUILD_HOUSEID_MANOR1 21
#define BUILD_HOUSEID_MANOR2 22
#define BUILD_SECTIONID_BASE 0
#define BUILD_SECTIONID_WALL 1
#define BUILD_SECTIONID_OBJ 11
#define BUILD_SECTIONID_STORAGE1 21



#endif /* DEFINE_BOTH_H */
