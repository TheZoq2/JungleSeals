
-- Getting the part that activated the script
lastActive = getLastActivePart()

-- Getting the part to move and the target from the labels

part = getLabelValue(lastActive, 0)
target = getLabelValue(lastActive, 1)

speed = 0.15 -- The speed that the part should move at

movePartToPart(part, target, speed)