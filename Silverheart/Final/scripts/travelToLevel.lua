-- Getting the part that was most likley activated to run this
lastActive = getLastActivePart()

-- Getting the labels
levelName = getLabelValue(lastActive, 0)
spawnPoint = getLabelValue(lastActive, 1)

loadNewLevel(levelName)
movePlayerToPart(spawnPoint)

