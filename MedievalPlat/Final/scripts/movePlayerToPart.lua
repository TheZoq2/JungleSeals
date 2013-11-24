-- Getting the name of the target from labels
lastActive = getLastActivePart()

-- Getting the part to move and the target from the labels

part = getLabelValue(lastActive, 0)

movePlayerToPart(part)