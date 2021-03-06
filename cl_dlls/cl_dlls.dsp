# Microsoft Developer Studio Project File - Name="cl_dlls" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cl_dlls - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cl_dlls.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cl_dlls.mak" CFG="cl_dlls - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cl_dlls - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cl_dlls - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cl_dlls - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\utils\vgui\include" /I "..\cl_dlls\hud" /I "..\config\\" /I "." /I ".." /I "..\game_shared\util" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\game_shared" /I "..\pm_shared" /I "..\dlls\entity" /I "..\dlls\weapon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "CLIENT_DLL" /D "CLIENT_WEAPONS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib winmm.lib ../utils/vgui/lib/win32_vc6/vgui.lib wsock32.lib /nologo /subsystem:windows /dll /map /machine:I386 /out:".\Release\client.dll"

!ELSEIF  "$(CFG)" == "cl_dlls - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GR /GX /ZI /Od /I "." /I ".." /I "..\game_shared\util" /I "..\dlls" /I "..\common" /I "..\pm_shared" /I "..\engine" /I "..\utils\vgui\include" /I "..\game_shared" /I "..\cl_dlls\hud" /I "..\dlls\weapon" /I "..\dlls\entity" /I "..\config" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "CLIENT_DLL" /D "CLIENT_WEAPONS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib winmm.lib ../utils/vgui/lib/win32_vc6/vgui.lib wsock32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:".\Debug\client.dll"

!ENDIF 

# Begin Target

# Name "cl_dlls - Win32 Release"
# Name "cl_dlls - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Group "hl"

# PROP Default_Filter "*.CPP"
# Begin Source File

SOURCE=.\hl\cl_player.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\cl_weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\ev_hldm.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\hl_baseentity.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\hl_events.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\hl_objects.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\hl_weapons.cpp
# End Source File
# Begin Source File

SOURCE=..\common\interface.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_scrollbar2.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_slider2.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_banmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_status.cpp
# End Source File
# End Group
# Begin Group "Shared"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\game_shared\util\util_entity.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\util\util_printout.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\util\util_shared.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\util\util_version.cpp
# End Source File
# Begin Source File

SOURCE=..\common\vector.cpp
# End Source File
# End Group
# Begin Group "hud"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hud\ammo.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\ammo_secondary.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\ammohistory.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\battery.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\death.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\flashlight.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\geiger.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\health.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\hud_spectator.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\message.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\pain.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\saytext.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\status_icons.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\statusbar.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\text_message.cpp
# End Source File
# Begin Source File

SOURCE=.\hud\train.cpp
# End Source File
# End Group
# Begin Group "_root"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\config\external_lib_include.cpp
# End Source File
# End Group
# Begin Group "weapon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\dlls\weapon\chumtoadweapon.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\crossbow.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\crowbar.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\egon.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\gauss.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\glock.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\handgrenade.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\hornetgun.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\mp5.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\python.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\rpg.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\satchel.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\shotgun.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\squeak.cpp
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\tripmine.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\cdll_int.cpp
# End Source File
# Begin Source File

SOURCE=.\cl_util.cpp
# End Source File
# Begin Source File

SOURCE=.\custom_message.cpp
# End Source File
# Begin Source File

SOURCE=.\demo.cpp
# End Source File
# Begin Source File

SOURCE=.\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\ev_common.cpp
# End Source File
# Begin Source File

SOURCE=.\events.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStudioModelRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\hud.cpp
# End Source File
# Begin Source File

SOURCE=.\hud_redraw.cpp
# End Source File
# Begin Source File

SOURCE=.\hud_servers.cpp
# End Source File
# Begin Source File

SOURCE=.\hud_update.cpp
# End Source File
# Begin Source File

SOURCE=.\in_camera.cpp
# End Source File
# Begin Source File

SOURCE=.\input.cpp
# End Source File
# Begin Source File

SOURCE=.\inputw32.cpp
# End Source File
# Begin Source File

SOURCE=.\parsemsg.cpp
# End Source File
# Begin Source File

SOURCE=.\parsemsg.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_debug.c
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_math.c
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_shared.c
# End Source File
# Begin Source File

SOURCE=.\studio_util.cpp
# End Source File
# Begin Source File

SOURCE=.\StudioModelRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\tri.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_checkbutton2.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ClassMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ConsolePanel.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ControlConfigPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_CustomObjects.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_grid.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_helpers.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_int.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_listbox.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_loadtga.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_MOTDWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_SchemeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ScorePanel.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ServerBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_SpectatorPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_TeamFortressViewport.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_teammenu.cpp
# End Source File
# Begin Source File

SOURCE=.\view.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons_resource.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Group "Shared H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\game_shared\util\util_entity.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\util\util_printout.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\util\util_shared.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\util\util_version.h
# End Source File
# Begin Source File

SOURCE=..\common\vector.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\util\vector_shared.h
# End Source File
# End Group
# Begin Group "hud H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hud\ammo.h
# End Source File
# Begin Source File

SOURCE=.\hud\ammohistory.h
# End Source File
# Begin Source File

SOURCE=.\hud\health.h
# End Source File
# Begin Source File

SOURCE=.\hud\hud_spectator.h
# End Source File
# Begin Source File

SOURCE=.\hud\pain.h
# End Source File
# End Group
# Begin Group "_root H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\config\external_lib_include.h
# End Source File
# Begin Source File

SOURCE=..\config\version.h
# End Source File
# End Group
# Begin Group "weapon H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\dlls\weapon\chumtoadweapon.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\crossbow.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\crowbar.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\egon.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\gauss.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\glock.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\handgrenade.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\hornetgun.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\mp5.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\python.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\rpg.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\satchel.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\shotgun.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\squeak.h
# End Source File
# Begin Source File

SOURCE=..\dlls\weapon\tripmine.h
# End Source File
# End Group
# Begin Group "hl H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hl\hl_weapons.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\cl_dll.h
# End Source File
# Begin Source File

SOURCE=.\demo.h
# End Source File
# Begin Source File

SOURCE=.\ev_hldm.h
# End Source File
# Begin Source File

SOURCE=.\eventscripts.h
# End Source File
# Begin Source File

SOURCE=.\GameStudioModelRenderer.h
# End Source File
# Begin Source File

SOURCE=.\hud.h
# End Source File
# Begin Source File

SOURCE=.\hud_iface.h
# End Source File
# Begin Source File

SOURCE=.\hud_servers.h
# End Source File
# Begin Source File

SOURCE=.\hud_servers_priv.h
# End Source File
# Begin Source File

SOURCE=.\in_defs.h
# End Source File
# Begin Source File

SOURCE=..\common\itrackeruser.h
# End Source File
# Begin Source File

SOURCE=.\kbutton.h
# End Source File
# Begin Source File

SOURCE=.\overview.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_debug.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_defs.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_info.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_materials.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_movevars.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_shared.h
# End Source File
# Begin Source File

SOURCE=.\studio_util.h
# End Source File
# Begin Source File

SOURCE=.\StudioModelRenderer.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\util_vector.h
# End Source File
# Begin Source File

SOURCE=.\vgui_ConsolePanel.h
# End Source File
# Begin Source File

SOURCE=.\vgui_ControlConfigPanel.h
# End Source File
# Begin Source File

SOURCE=.\vgui_int.h
# End Source File
# Begin Source File

SOURCE=.\vgui_SchemeManager.h
# End Source File
# Begin Source File

SOURCE=.\vgui_ScorePanel.h
# End Source File
# Begin Source File

SOURCE=.\vgui_ServerBrowser.h
# End Source File
# Begin Source File

SOURCE=.\vgui_SpectatorPanel.h
# End Source File
# Begin Source File

SOURCE=.\vgui_TeamFortressViewport.h
# End Source File
# Begin Source File

SOURCE=.\view.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_banmgr.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_status.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_vgui_tweakdlg.h
# End Source File
# Begin Source File

SOURCE=.\weapons_resource.h
# End Source File
# Begin Source File

SOURCE=.\wrect.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
