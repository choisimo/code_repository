const input= `
Mods=Chinatown expansion
Map=Chinatown_EX

Mods=TMC_Trolley
Map=No maps found

Mods=SuperBulldozer
Map=No maps found

Mods=TakeAnyAmount
Map=No maps found

Mods=FixCapacityOverlap
Map=No maps found

Mods=POM
Map=No maps found

Mods=EerieCountry
Map=Eerie Country;Eerie A;Eerie B;Eerie D;Eerie C

Mods=Navigation
Map=No maps found

Mods=GlassHats
Map=No maps found

Mods=GlassHats
Map=No maps found

Mods=GlassHats
Map=No maps found

Mods=ModGlassHats
Map=No maps found

Mods=ModGlassHats
Map=No maps found

Mods=ModGlassHats
Map=No maps found

Mods=STPASSIVE
Map=No maps found

Mods=NoLighterNeeded
Map=No maps found

Mods=NoLighterNeeded
Map=No maps found

Mods=FORTREDSTONE
Map=FortRedstone

Mods=90pierceArrow
Map=No maps found

Mods=ZBAY
Map=No maps found

Mods=74amgeneralM151A2
Map=No maps found

Mods=ExtraSauceSac
Map=No maps found

Mods=Gun Stock Attack Remaster
Map=No maps found

Mods=OHKKK111
Map=No maps found

Mods=P4HasBeenRead
Map=No maps found

Mods=P4HasBeenRead
Map=No maps found

Mods=SYMS
Map=No maps found

Mods=67commando
Map=No maps found

Mods=Dead_Space_Zombie
Map=No maps found

Mods=Fort Waterfront
Map=Fort Waterfront

Mods=VehicleRecycling
Map=No maps found

Mods=GunFighter_Radial_Menu
Map=No maps found

Mods=Grapeseed
Map=Grapeseed

Mods=CraftHelperContinued
Map=No maps found

Mods=STSPRINT
Map=No maps found

Mods=STAIMING
Map=No maps found

Mods=ReducedWoodWeight25p41
Map=No maps found

Mods=ReducedWoodWeight25p41
Map=No maps found

Mods=MiniHealthPanel
Map=No maps found

Mods=MiniHealthPanel
Map=No maps found

Mods=MiniHealthPanel
Map=No maps found

Mods=STRELOAD
Map=No maps found

Mods=TheWorkshop(new version)
Map=No maps found

Mods=50%metalweight
Map=No maps found

Mods=50%metalweight
Map=No maps found

Mods=STNIMBLE
Map=No maps found

Mods=AutoReload
Map=No maps found

Mods=NVAPI
Map=No maps found

Mods=Brita_K153
Map=No maps found

Mods=NewEkron
Map=NewEkron

Mods=KingsmouthKY
Map=Kingsmouth

Mods=OutTheWindow
Map=No maps found

Mods=TheyKnewB42
Map=No maps found

Mods=GenRange
Map=No maps found

Mods=GenRange
Map=No maps found

Mods=BB_CommonSense
Map=No maps found

Mods=BB_CommonSense
Map=No maps found

Mods=RMH
Map=Rosewood Military Hospital

Mods=REORDER_CONTAINERS
Map=No maps found

Mods=REORDER_CONTAINERS
Map=No maps found
`;

// Split the input into lines and process each block
const lines = input.split('\n');
let maps = [];

for (let i = 0; i < lines.length; i++) {
  if (lines[i].startsWith('Map=') && !lines[i].includes('No maps found')) {
    const mapLine = lines[i].replace('Map=', '').trim();
    maps.push(mapLine);
  }
}

// Combine maps into a single string with semicolon separation
const result = maps.join(';');
console.log(result);
