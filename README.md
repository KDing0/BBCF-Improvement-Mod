# BBCF-Improvement-Mod AI edition
This version of the BBCF Improvement mod adds an experimental AI that can learn from players and be used for a variety of training practices.
For a quick overview look at the video here: https://youtu.be/74DYdpBzIOs

Join the [BB Improvement Mod Discord Server](https://discord.gg/j2mCX9s) to discuss about the development and the game itself!

## What you can do with the AI system:
- Have it learn from you and your opponents while you play online, to create AI copies of your opponents.
- You can play against these AI opponents in training mode to practice against them. This allows you to practice matchups player and character matchups at your leisure.
- You can record your own actions in training mode to create AI bots that an be used for specific training scenarios, allowing for specialized practice.
- You can share your AI data in a global server, allowing you to easily access specialized training bots (assuming people in the community create and share these bots)

## Install:
Two versions available, the standalone version and a version that merges the AI with the features from:
https://github.com/libreofficecalc/BBCF-Improvement-Mod
Download whichever version you prefer (the libre version has far more features, in case you don't know the difference) and unpack all files in the .zip folder where the BBCF.exe is located.

## Detailed Guide:

### Main Menu Window:
![image](https://github.com/KDing0/BBCF-Improvement-Mod/assets/38357906/e14bdcc0-0162-4223-8bf3-ec6623084c68)

This window is only open while you are not in a battle.
- **CBR AI Explanation:** Hover the mouse over the "?" to get some basic info about how to use the AI.
- **Auto Record Myself:** When checked will generate AI data that will be saved locally after every played match. The mod will ask you to save the data or dismiss it by pressing F8/f9. These keys can be changed in the settings.ini file.
- **Auto Record Opponents:** Like the above but for any opponent you fight.
- **Auto Upload Data:** Saves the data immediately to the community server when checked.
- **Auto Saving in Lobby:** When returning to the lobby or main menu, saves all unsaved AI data as if the F8 key was pressed. Good if you always want to save data but don't want to keep pressing F8.

### AI Control Window:
![image](https://github.com/KDing0/BBCF-Improvement-Mod/assets/38357906/84aef291-3e10-4824-a0f2-f86db98b2ecd)

The AI control window is only available while in a match. This window is split into two sides, one for player 1 and the other for player 2.
Any button or info detailed on one side applies only to the character controlled by the respective player.

- **Replays:** Details how many AI recordings are stored in the AI file currently loaded for each player.
- **Frames Recorded:** This value counts up when recording new data for the AI to learn from.
- **Input:** The input each player is executing is stored as a value. Mostly useful for testing/debugging.
- **Record:** When pressed starts recording all actions the respective player executes until the recording ends. Pressing it again ends the recording and stores the data of the recording in the Replay data, which can be seen by the Replay counter going up. Restarting training mode while recording will also store the recording, but restart the recording.
- **Replaying:** Turns on the AI by using the replay data stored for this player, as indicated by the "Replays" counter. The AI will use any recordings given to it to determine how it should behave in any given situation, meaning that the AI can only do what someone taught it beforehand.
- **Instant Learning:** Only available in mirror matches. This is basically recording and replaying combined into one, but the recorded data is utilized by the AI system immediately 1 frame after observing it instead of requiring you to save it first.
- **Delete/Last:** Deletes a certain amount of replays from the replay data. The range can be determined at the bottom with Replay Deletion Range. Setting 0 and 10 in that setting would delete replays 0 to 10 for example. Pressing Last deletes the last replay from the data.
- **Save:** Saves the replay data to your hard drive, you can save it under a certain name by filling out the "Player Name" text box.
- **Load:** Opens a window to load AI data from your hard drive based on the character you are playing, which lists all AI data relevant for that character, which you can load by pressing the "Load" button.
- **Load By Name:** Like load except it immediately loads data based on the name written in "Player Name"
- **Ai Activity:** Lets you know what you are currently doing with the AI so you can turn it on/off.
  
### Weight Settings
![image](https://github.com/KDing0/BBCF-Improvement-Mod/assets/38357906/6b0a5fb8-3faf-4e41-ad56-b72cd6d47cf2)
Lets you adjust in-depth AI decision-making settings. Every value displayed corresponds to a variable the AI uses for decision-making and how heavily it is weighted. The weighting can be adjusted to change how important the AI considers the variable.
Higher values = more important. These values can be different for Neutral(Basic), Pressure, Blocking, and Combo.

### Ai Filehost
![image](https://github.com/KDing0/BBCF-Improvement-Mod/assets/38357906/abb8978a-a280-48bc-9076-caa9f84ede5f)
The Filehost window lets you search for files on the server as well as files stored on your local hard drive. You need to be logged into network mode to access the server data.
You can filter the data by checking for exact name matches for "Uploader" and  "Player Name". 
"Player Character" and "Opponent character" check for a shortened name of the character (Bullet is bl for example). 
And Replay count checks weather the AI data contains at least the set amount of replays.
![image](https://github.com/KDing0/BBCF-Improvement-Mod/assets/38357906/1f514c2b-cf9b-4188-b109-8ed4d69fe466)


## Compiling and usage
BBCF Improvement Mod is coded using Visual Studio 2019 (toolset v142). <br>
To compile, you should only need to load the sln file and compile as-is. No changes should be needed to the solution or source.<br>
Copy the compiled binary, settings.ini, and palettes.ini files from the bin/ folder into the game's root folder.

## Requirements
- Visual Studio 2019 (toolset v142) (Windows SDK 10)

## Thanks to the people who have helped the mod along the way
* KoviDomi
* Neptune
* Rouzel
* Dormin
* NeoStrayCat
* Microsoft Excel/libreofficecalc
* PC_volt
* Everybody in the BlazBlue PC community that has given support or feedback of any kind!

## Extra thanks to
Atom0s for their DirectX9.0 Hooking article<br>
Durante for their dsfix source

## Legal
```
BBCF Improvement Mod is NOT associated with Arc System Works or any of its partners / affiliates.
BBCF Improvement Mod is NOT intended for malicious use.
BBCF Improvement Mod is NOT intended to give players unfair advantages in online matches.
BBCF Improvement Mod is NOT intended to unlock unreleased / unpurchased contents of the game.
BBCF Improvement Mod should only be used on the official version that you legally purchased and own.

Use BBCF Improvement Mod at your own risk.
I, KDing, am not responsible for what happens while using BBCF Improvement Mod. You take full reponsibility for any outcome that happens to you while using this application.
```
