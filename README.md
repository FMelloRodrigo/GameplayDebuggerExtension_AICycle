# Gameplay Debugger Extension - AI Cycle
A simple plugin that consists in a Gameplay Debugger Extension that allows cycling between AI agents when using the Unreal's Gameplay Debugger

Default keys are: 

Crtl+Page Up / Page Down: Cycle AI agents

Crtl+R: Reset current Cycling

 - Keys can be remapped under Project Settings->Gameplay Debugger->Addons->AICycleDebugger
 - Currently two options are available under Project Settings->Engine->Gameplay Debugger AI Cycle:
   - Seach Radius - Radius area around the player character to gather the possible AI Agents
   - AI Filter Classes - When added an entry, only AI agents of these classes will be gathered
   
This project is available on FAB here(link), but to install it directly:
 - Download (or clone) this repository.
 - Create a Plugins folder at the root of your Unreal Engine project if one doesn't already exist.
 - Create a folder named "GDebuggerExtension_AICycle" and paste the content inside
 - Copy the GDebuggerExtension_AICycle plugin folder into the Plugins folder.
 - Right-click your .uproject file and select Generate Visual Studio project files.
 - Open your project. Unreal Engine will ask to compile the new plugin. Click "Yes".
