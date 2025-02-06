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
