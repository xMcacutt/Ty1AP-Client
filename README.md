# Ty1 - Archipelago Client Plugin

This plugin is based on the TygerFramework created by Kana (Elusive Fluffy). It is the Ty1 implementation of a client to connect to an archipelago randomiser server and handle items and checks.

For the APWorld for Ty the Tasmanian Tiger see [Ty the Tasmanian Tiger APWorld](https://github.com/xMcacutt/Archipelago-TyTheTasmanianTiger/releases/latest)

## Setup

To set up, first you'll need to install TygerFramework as well as TygerMemory. These are simple dlls that sit in your Ty directory. Information on their setup can be found at the respective pages below

- [TygerFramework](https://github.com/ElusiveFluffy/TygerFramework)

- [TygerMemory](https://github.com/xMcacutt/TygerMemory1)

Next, you'll need to add the Ty1 AP Client dll to your plugins directory that should have been created when installing TygerFramework. You'll also need to copy libcrpyto-3.dll and libssl-3.dll from the Ty1 AP Client release into your dependencies directory.

Once you launch the game, the client should be up and running.

## Connecting

To open and close the AP client windows you can press F2. If you wish to hide the connection window but keep the item log in the bottom left, simply press F3.

To interact with any window in TygerFramework, you'll need the TygerFramework window to be open. You can toggle the TygerFramework window with F1.

With the TygerFramework window and the Ty1 AP Client connection window open, enter your server address, password, and slot name and press connect. The load game button on the main menu should now be active. Upon pressing this, you will bypass the load select screen, a save is created in the saves directory in your Ty folder and you will be able to load directly into the save for the seed of the world.

## How This Rando Works

### Locations

In Ty-AP the following locations are considered checks by default

- All 72 Thunder Eggs

- All 5 Talismans

#### Cogs

There are 90 golden cogs in the game. These can be enabled as checks through cogsanity. If cogsanity is set to all, then all 90 cogs will be checks. If cogsanity is set to per_level, then once you collect all 10 cogs in a level, you will be granted a check.

#### Bilbies

There are 45 bilbies in the game. These can be enabled as checks through bilbysanity. If bilbysanity is set to all_no_te then the thunder egg for collecting all 5 bilbies in a level will not trigger a check to avoid a double check on thunder egg collection. all_with_te allows the double check.

#### Picture Frames

There are 373 picture frames in the game. Only 127 of these can be activated as checks. All of the bonus world picture frames are excluded since they are postgame content. The remaining frames can be enabled as checks through framesanity. If framesanity is set to all, then all 127 frames will be checks. If it is set to per_level then collecting all of the picture frames in a level will grant a single check. 

It is recommended to leave picture frames on "all" due to generation failures from lack of locations.

#### Attributes

There are 15 attributes in the game. These are the rangs and abillities that can be unlocked for Ty including swim, dive, second rang, extra health paw, etc. 

For each attribute its unlock to grant the check varies. For extra health paw, you must collect all 25 rainbow scales in Rainbow Cliffs. Swim and Dive are split such that speaking to Rex in Ship Rex grants Swim while speaking to Rex in Bridge on the River Ty grants the Dive check. On getting an attribute check, you will not receive the attribute.

If attributesanity is set to skip_elementals, you will not be given a check for the flamerang, frostyrang, or zappyrang. This avoids a double check due to the boss completion talisman check.

### Items

TO BE COMPLETED

## Known Issues

KNOWN CLIENT ISSUES

- Lifepaw crash

KNOWN APWORLD ISSUES

- Junk fill bad
- Plenty of generation issues
- (Might be worth switching Theggs to non progression if on goal Final Battle with progressive rang and checks on level unlock style)

FEATURES TO ADD

- Text colouring to show items more clearly
- Overlay showing current totals since gameinfo is used to track checks
- Option to turn opal checks off or make them filler
- Add text to hard disk removed thing
- Text guide explaining how rando works
- Extended hint info
