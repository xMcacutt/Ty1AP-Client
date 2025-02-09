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

Receiving items in an ap world will not lead to items being collected in the Ty world. Instead, it will give an item which is saved separately into the save data. These items can be tracked using the built in tracker which is opened whilst not on the main menu using F4. 

#### Rangs

If progressive rang (progressive elemental) is turned on in the yaml, you will receive progressive rang items instead of the rangs directly for the following rangs in order:

Second Rang -> Swim -> Aqua -> Dive -> Flame -> Frosty -> Zappy -> Doom

If you also have start with boomerang set to false, an additional progressive rang is added to the item pool and the first progressive rang item you get will unlock the boomerang. The remaining progressives will then unlock in the order listed above.

The technorangs will be unlocked via checks separately so you might be given the item "Infrarang" at some point for example.

If progressive rang is turned off, then all of the rangs and abilities will be given as checks.

#### Levels

The first portal (in Two Up vanilla location) is always open regardless of settings. If you have level shuffle turned on, the 9 core levels are randomised. Boss shuffle will randomise the first 3 bosses too. 

If you have level unlock style set to vanilla or vanilla bosses then you will need to obtain the amount of thunder eggs set in the yaml under "hub te counts" for a given hub to unlock that boss' portal. The portal will appear automatically once you do this.

If you have level unlock style set to checks or vanilla bosses, the 9 core levels (except the one in Two Up vanilla position) will need to be unlocked via checks.

With progressive level on, the portals will be unlocked in standard game order, otherwise, the order may be random.

#### Talisman

On being given a talisman, you unlock the ability to obtain a check on the elemental rangs. The conditions for the check are:

- Boss for the hub is defeated

- Talisman is obtained

- Thunder egg count for that hub is greater than 17 (may need adjusting)

- Return to Rainbow Cliffs

#### Thunder Eggs

When being given a thunder egg (thegg), the thegg will be either a Fire Thunder Egg, Ice Thunder Egg, or Air Thunder Egg for hub 1, 2, and 3 respectively.

These thunder eggs are saved separately into the save data and can be tracked with the in game tracker. These are used to determine if the boss portal is spawned in Vanilla and Vanilla Bosses level unlock style.

You will need to find or be given all 72 to complete the goals completion and all thunder eggs.

#### Golden Cogs

Golden cogs are stored similarly to thunder eggs. They can be redeemed for technorang checks in Julius' lab in Rainbow Cliffs.

Currently, in order for you to redeem a later rang check, you must have both checked the earlier rang checks and also received the rang for each earlier check.

#### Bilbies

When you receive a bilby, it will be named Bilby - LEVEL NAME. This will save a bilby to the save data separately from the normal save data. Once you have found or been given 5 bilbies for a level, you will automatically be given a thunder egg matching that level's vanilla hub type.

#### Stopwatches

Stopwatches are also added to the item pool by default. They unlock the time attack for their corresponding level.

## Known Issues

KNOWN CLIENT ISSUES

- Hard disk removed screen displays when loading in (VISUAL BUG)

- start_with_boom option set to false may crash the game

- Deathlink is not fully implemented yet

- Throwing the kaboomerang without having the second rang may crash the game

- Saving the game manually does not work

KNOWN APWORLD ISSUES

- Generation fails to add items to rang locations
- Needs overhaul to force certain checks preventing generation failures
- Picture frames do not logically require infrarang in any logic setting
- Patch to add archipelago branding

FEATURES TO ADD

- Text colouring to show items more clearly
- Option to make opal checks filler
- Extended hint info

SOLVED

- Tracker and logger windows made slightly bigger
- Aquarangs no longer require having swim
- Non progressive level items now spawn the correct portals
- All attribute checks now trigger even if the attribute is in inventory
- Fixed generation logic for starting with boomerang
- Prevented accidental save slot loading
- Fixed crash on lifepaw attribute check (25 rainbow scales)
- Portals now spawn without needing reload in Rainbow Cliffs on item get
- Time attack stopwatches no longer despawn after one use
