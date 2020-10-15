---HLS1 AZ Codebase---

Oops, never got around to updating this readme did I.


Derived from the official Half-Life SDK.

Decription of folders:

_docs       - NEW; info about the codebase/mod and 'template_monster' files.
cl_dlls     - clientside script, builds client.dll.
common      - mostly commonly-used headers for constants or very simple methods (common/const.h is the most modified).  Redundant/inconsistent vector-related script combined into common/vector.h/.cpp.
config      - NEW; mod-specific settings or system includes
dlls        - serverside script, builds az.dll.  (Some script is read by cl_dlls, mainly to be able to run some weapon logic between server pings while dummying out most entity-related script)
engine      - engine-related headers that tie into precompiled code.
game_shared - script that may be included by both client and server projects; game_shared/util has most new mod-related script
pm_shared   - c-compiled player movement-related script and utilities (the rest of the codebase is mostly C++).
utils       - unknown;  rarely touched by the mod and most global methods are already in game_shared/util/util_shared.cpp, cl_dlls/cl_util.cpp, and dlls/util.cpp.

