// 원본 데이터
const input = "tsarslib;RavenCreek;hello;hello;PZGKO_Main;hello;hello;hello;hello;hello;hello;hello;hello;EerieCountry;lakeivytownship;Chinatown expansion;Navigation;GlassHats;GlassHats;GlassHats;improvedbuildmenu41;PZGKO_Gear;BB_CommonSense;BB_CommonSense;improvedbuildmenu41-2;FORTREDSTONE;PZGKO_KI5;STSPRINT;MiniHealthPanel;MiniHealthPanel;MiniHealthPanel;VISIBLE_BACKPACK_BACKGROUND;Brita;PZGKO_VFE;STNIMBLE;NVAPI;esIC;74amgeneralM151A2;ProximityInventory;ProximityInventory;STRELOAD;tctlBags;ZBAY;67commando;50%metalweight;50%metalweight;ReducedWoodWeight25p41;ReducedWoodWeight25p41;STPASSIVE;PZGKO_Snake;TheStar;KingsmouthKY;modoptions;RMH;simpleStatus;PZGKO_Brita;VehicleRecycling;90pierceArrow;TMC_Trolley;TakeAnyAmount;Dead_Space_Zombie;TheWorkshop(new version);ModGlassHats;ModGlassHats;ModGlassHats;OHKKK111;TheyKnew;CraftHelperContinued;P4HasBeenRead;P4HasBeenRead;ExtraSauceSac;PZGKO_SCKCO;STAIMING;improvedbuildmenu41-1;NewEkron;GunFighter_Radial_Menu;Fort Waterfront;NoLighterNeeded;NoLighterNeeded;SYMS;truemusic;SuperBulldozer;REORDER_CONTAINERS;REORDER_CONTAINERS;Grapeseed;Gun Stock Attack Remaster;PZGKO_Arsenal;rSemiTruck;Brita_K153;AutoReload;POM;OutTheWindow;UBPropFix";

// 중복 제거 및 중복 항목 추출 함수
function processDuplicates(data) {
    const items = data.split(';');
    const uniqueItems = new Set();
    const duplicates = new Set();
  
    for (const item of items) {
      if (uniqueItems.has(item)) {
        duplicates.add(item); // 중복된 데이터 기록
      } else {
        uniqueItems.add(item); // 고유 데이터 추가
      }
    }
  
    return {
      unique: Array.from(uniqueItems).sort(), // 고유 데이터 정렬
      duplicates: Array.from(duplicates).sort(), // 중복 데이터 정렬
    };
  }
  
  // 처리 실행
  const { unique, duplicates } = processDuplicates(input);
  
  // 결과 출력
  console.log("Unique Data:");
  console.log(unique.join(';'));
  console.log("\nDuplicated Data:");
  

  console.log(unique.join('\n'));