#include "Map.h"

namespace Map {
  void initializeMapInfo(std::multimap<BWAPI::UnitType, int> dStructMap,
                         std::list<int>& mapVariables,
                         std::vector<std::pair<BWAPI::TilePosition,
                         BWAPI::TilePosition>>&noBuildZones,
                         std::vector<std::pair<BWAPI::TilePosition,
                         BWAPI::TilePosition>>&buildZones) {
    mapVariables.push_back(findMineralSetup(dStructMap));
    initializeNoBuildZones(noBuildZones);
    initializeBuildZones(dStructMap, mapVariables, noBuildZones, buildZones);
  }

  int findMineralSetup(std::multimap<BWAPI::UnitType, int> dStructMap) {
    int startingCC_X = 0;
    int startingCC_Y = 0;
    bool allAbove = true;
    bool allRight = true;
    bool allBelow = true;
    bool allLeft = true;

    BWAPI::Unit startingCC = UnitIDMap::getUnitFromID(
      UnitIDMap::getFirstUnitID(dStructMap, kCommandCenter));
    startingCC_X = startingCC->getTilePosition().x;
    startingCC_Y = startingCC->getTilePosition().y;

    for (auto &u : BWAPI::Broodwar->getNeutralUnits()) {
      if (u->getType().isMineralField() && u->isVisible()) {
        if (u->getTilePosition().x < startingCC_X) allRight = false;
        if (u->getTilePosition().x > startingCC_X) allLeft = false;
        if (u->getTilePosition().y < startingCC_Y) allAbove = false;
        if (u->getTilePosition().y > startingCC_Y) allBelow = false;
      }
    }

    if (allAbove == true) return 12;
    if (allRight == true) return 3;
    if (allBelow == true) return 6;
    if (allLeft == true) return 9;

    // error
    return -1;
  }

  void initializeNoBuildZones(std::vector<std::pair<BWAPI::TilePosition,
                              BWAPI::TilePosition>>&noBuildZones) {
    // Designate box around mineral line, gas, and CC as a no build zone
    BWAPI::TilePosition topLeft = BWAPI::TilePosition(-1, 1);
    BWAPI::TilePosition botRight = BWAPI::TilePosition(-1, -1);

    // set topLeft and botRight to CC
    for (auto &u : BWAPI::Broodwar->self()->getUnits()) {
      if (u->getType() == BWAPI::UnitTypes::Terran_Command_Center) {
        topLeft = u->getTilePosition();
        botRight = BWAPI::TilePosition(u->getTilePosition().x +
                                       u->getType().tileWidth(),
                                       u->getTilePosition().y +
                                       u->getType().tileHeight());
      }
    }

    // iterate through mineral fields and vespene gas to expand first
    // no build zone
    for (auto &u : BWAPI::Broodwar->getNeutralUnits()) {
      if ((u->getType().isMineralField() || u->getType() ==
           BWAPI::UnitTypes::Resource_Vespene_Geyser) && u->isVisible()) {
        topLeft = TileComparator::findTopLeft(topLeft, u->getTilePosition());
        botRight = TileComparator::findBotRight(botRight, u->getTilePosition(),
                                                u->getType());
      }
    }

    noBuildZones.push_back(std::make_pair(topLeft, botRight));
  }

  void initializeBuildZones(std::multimap<BWAPI::UnitType, int> dStructMap,
                            std::list<int>& mapVariables,
                            std::vector<std::pair<BWAPI::TilePosition,
                            BWAPI::TilePosition>>&noBuildZones,
                            std::vector<std::pair<BWAPI::TilePosition,
                            BWAPI::TilePosition>>&buildZones) {
    int mapOrientation = mapVariables.front();


  }

}