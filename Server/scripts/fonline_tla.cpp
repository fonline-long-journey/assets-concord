#include "fonline_tla.h"

// Slot/parameters allowing
EXPORT bool allowSlot_Hand1( uint8, Item &, Critter &, Critter & toCr );

// Initialization

// In this functions (DllMain and DllLoad) all global variables is NOT initialized, use FONLINE_DLL_ENTRY instead
#if defined ( FO_WINDOWS )
int __stdcall DllMain( void* module, unsigned long reason, void* reserved ) { return 1; }
#elif defined ( FO_LINUX )
void __attribute__( ( constructor ) ) DllLoad()   {}
void __attribute__( ( destructor ) )  DllUnload() {}
#endif

FONLINE_DLL_ENTRY( isCompiler )
{}

// Slot/parameters allowing

EXPORT bool allowSlot_Hand1( uint8, Item&, Critter&, Critter& toCr )
{
    return toCr.Params[ PE_AWARENESS ] != 0;
}

#ifdef __SERVER

uint GetTiles( Map& map, uint16 hexX, uint16 hexY, bool is_roof, vector< uint >& finded )
{
    ProtoMap::TileVec& tiles = const_cast< ProtoMap::TileVec& >( map.Proto->Tiles );

    for( uint i = 0, j = tiles.size(); i < j; i++ )
    {
        if( tiles[ i ].HexX != hexX || tiles[ i ].HexY != hexY || tiles[ i ].IsRoof != is_roof )
            continue;
        finded.push_back( tiles[ i ].NameHash );
    }
    return finded.size();
}

EXPORT uint Map_GetTile( Map& map, uint16 tx, uint16 ty )
{
	vector< uint > finded;

    return GetTiles( map, tx, ty, false, finded );
}

EXPORT uint Map_GetRoof( Map& map, uint16 tx, uint16 ty )
{
    vector< uint > finded;

    return GetTiles( map, tx, ty, true, finded );
}
#endif // __SERVER

#ifdef __CLIENT
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

EXPORT int GetHardware(ScriptString& output)
{
	std::string hr = exec("wmic path win32_physicalmedia get SerialNumber");
	output = hr;
    // Копируем информацию о железе в структуру SYSTEM_INFO.
	/*
    SYSTEM_INFO siSysInfo;
    GetSystemInfo( &siSysInfo );
    uint hardwareId = uint(siSysInfo.dwOemId);

    // Отображаем содержимое структуры SYSTEM_INFO.
    array.Resize(0);
    array.Grow(siSysInfo.dwOemId);
    array.Grow(siSysInfo.dwNumberOfProcessors);
    array.Grow(siSysInfo.dwPageSize);
    array.Grow(siSysInfo.dwProcessorType);
	*/
    return 0;
}

#endif