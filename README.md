# Ty1 - Archipelago Client Plugin

This plugin is based on the TygerFramework created by Kana (Elusive Fluffy). It is the Ty1 implementation of a client to connect to an archipelago randomiser server and handle items and checks.

For the APWorld for Ty the Tasmanian Tiger see [Ty the Tasmanian Tiger APWorld](https://github.com/xMcacutt/Archipelago-TyTheTasmanianTiger/releases/latest)

## Setup

To set up, first you'll need to install TygerFramework as well as TygerMemory. These are simple dlls that sit in your Ty directory. Information on their setup can be found at the respective pages below

- [TygerFramework](https://github.com/ElusiveFluffy/TygerFramework)

- [TygerMemory](https://github.com/xMcacutt/TygerMemory1) VERSION 1.0.3+

Next, you'll need to add the Ty1 AP Client dll to your plugins directory that should have been created when installing TygerFramework.

Once you launch the game, the client should be up and running.

Optionally, you can place the Patch_PC.rkv from the releases page into your Ty folder to have some extra Archipelago branding.

**as of v1.1.4, libcrypto and libssl are no longer required as dependencies**

## Connecting

To open and close the AP client windows you can press F2. If you wish to hide the connection window but keep the item log in the bottom left, simply press F3.

To interact with any window in TygerFramework, you'll need the TygerFramework window to be open. You can toggle the TygerFramework window with F1.

With the TygerFramework window and the Ty1 AP Client connection window open, enter your server address, password, and slot name and press connect. The load game button on the main menu should now be active. Upon pressing this, you will bypass the load select screen, a save is created in the saves directory in your Ty folder and you will be able to load directly into the save for the seed of the world.

## Collectible Info Window

As of 1.1.3, a new window is available and can be toggled by pressing F5. This window gives information on every collectible so hints showing locations such as 

`Two Up - Golden Cog 7`

Can be found more easily. Simply press F5 and reference the guide.

## How This Rando Works

### Locations

In Ty-AP the following locations are considered checks by default

- All 72 Thunder Eggs

- All 90 Golden Cogs

- Cog completion for each level (find all 10 cogs)

- All 45 Bilbies

- Bilby completion for finding each level's 5 bilbies throughout the world

- All 13 Rangs / Abilities

- All 5 Talismans

#### Picture Frames

There are 373 picture frames in the game. Only 127 of these can be activated as checks. All of the bonus world picture frames are excluded since they are postgame content. The remaining frames can be enabled as checks through framesanity. If framesanity is set to all, then all 127 frames will be checks. If it is set to per_level then collecting all of the picture frames in a level will grant a single check. 

#### Extra Lives

There are 22 extra lives in the game. They can be enabled as checks through lifesanity. Then, collecting each life will grant a check on the first time it is collected.

#### Signposts

There are 25 sign posts which Maurie sits on in the game and which can be hit with the boomerang. They can be enabled as checks through signsanity. Then, hitting each sign with the boomerang will grant a check on the first time it is hit.

#### Attributes

There are 15 attributes in the game. These are the rangs and abillities that can be unlocked for Ty including swim, dive, second rang, extra health paw, etc. 

For each attribute its unlock to grant the check varies. Using scalesanity, you can get the extra health paw and its corresponding check. To get the check, you must collect all 25 rainbow scales in Rainbow Cliffs. Swim and Dive are split such that speaking to Rex in Ship Rex grants the Swim check, while speaking to Rex in Bridge on the River Ty grants the Dive check. On getting an attribute check, you will not receive the attribute.

### Items

Receiving items in an ap world will not lead to items being collected in the Ty world. Instead, it will give an item which is saved separately into the save data. These items can be tracked using the built in tracker which is opened whilst not on the main menu using F4. 

#### Rangs

If progressive rang (progressive elemental) is turned on in the yaml, you will receive progressive rang items instead of the rangs directly for the following rangs in order:

Second Rang -> Swim -> Aqua -> Dive -> Flame -> Frosty -> Zappy -> Doom

If you also have start with boomerang set to false, an additional progressive rang is added to the item pool and the first progressive rang item you get will unlock the boomerang. The remaining progressives will then unlock in the order listed above.

The technorangs will be unlocked via checks separately so you might be given the item "Infrarang" at some point for example.

If progressive rang is turned off, then all of the rangs and abilities will be given as checks.

#### Levels

The first portal (in Two Up vanilla location) is always open regardless of settings. If you have level shuffle turned on, the 9 core levels are randomised. 

If you have level unlock style set to "vanilla" or "checks no bosses" then you will need to obtain the amount of thunder eggs set in the yaml under "theggGating" for a given hub to unlock that boss' portal. The portal will appear automatically once you do this.

If you have level unlock style set to checks or vanilla bosses, the 9 core levels (except the one in Two Up vanilla position) will need to be unlocked via checks.

With progressive level on, the portals will be unlocked in standard game order, otherwise, the order may be random.

#### Talisman

On being given a talisman, you unlock the ability to obtain a check on the elemental rangs. The conditions for the check are:

- Boss for the hub is defeated

- Talisman is obtained

- Thunder egg count for that hub is greater than the count set by "theggGating"

- Return to Rainbow Cliffs

#### Thunder Eggs

When being given a thunder egg (thegg), the thegg will be either a Fire Thunder Egg, Ice Thunder Egg, or Air Thunder Egg for hub 1, 2, and 3 respectively.

These thunder eggs are saved separately into the save data and can be tracked with the in game tracker. These are used to determine if the boss portal is spawned in Vanilla and "Checks - No Bosses" level unlock style.

#### Golden Cogs

Golden cogs are stored similarly to thunder eggs. They can be redeemed for technorang checks in Julius' lab in Rainbow Cliffs. You will need to redeem multiple times if you have a high enough cog count. The amount of cogs required for each stage is set using "Cog Gating".

#### Bilbies

When you receive a bilby, it will be named Bilby - LEVEL NAME. This will save a bilby to the save data separately from the normal save data. Once you have found or been given 5 bilbies for a level, you will automatically be given a thunder egg matching that level's vanilla hub type.

#### Stopwatches

Stopwatches can be added to the pool using "Gate Time Attacks". They unlock the time attack for their corresponding level and add a harder time attack challenge check to each level. The time attack challenge can be completed on the same attempt as obtaining the time attack thunder egg check but you will only see the time you are trying to beat after obtaining the thunder egg.

## Known Issues

KNOWN CLIENT ISSUES

- Throwing the kaboomerang without having the second rang may crash the game

FEATURES TO ADD

- More log text colouring

SOLVED

- Fixed portals... for real this time
- Added level codes to tracker on portals
