import os
import image_slicer

tiles = image_slicer.slice('npcs/pleb/pleb.png', 64, save=False)
image_slicer.save_tiles(tiles, directory='../resources/npcs/pleb')

for i in range(1, 9):
    os.rename('../resources/npcs/pleb/_01_0' + str(i) + '.png', '../resources/npcs/pleb/pleb_' + f'{(i - 1 + 8 * 6):03d}' + '.png')
    os.rename('../resources/npcs/pleb/_02_0' + str(i) + '.png', '../resources/npcs/pleb/pleb_' + f'{(i - 1 + 8 * 4):03d}' + '.png')
    os.rename('../resources/npcs/pleb/_03_0' + str(i) + '.png', '../resources/npcs/pleb/pleb_' + f'{(i - 1 + 8 * 0):03d}' + '.png')
    os.rename('../resources/npcs/pleb/_04_0' + str(i) + '.png', '../resources/npcs/pleb/pleb_' + f'{(i - 1 + 8 * 2):03d}' + '.png')
    os.rename('../resources/npcs/pleb/_05_0' + str(i) + '.png', '../resources/npcs/pleb/pleb_' + f'{(i - 1 + 8 * 5):03d}' + '.png')
    os.rename('../resources/npcs/pleb/_06_0' + str(i) + '.png', '../resources/npcs/pleb/pleb_' + f'{(i - 1 + 8 * 3):03d}' + '.png')
    os.rename('../resources/npcs/pleb/_07_0' + str(i) + '.png', '../resources/npcs/pleb/pleb_' + f'{(i - 1 + 8 * 7):03d}' + '.png')
    os.rename('../resources/npcs/pleb/_08_0' + str(i) + '.png', '../resources/npcs/pleb/pleb_' + f'{(i - 1 + 8 * 1):03d}' + '.png')
