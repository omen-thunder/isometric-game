import os

for i in range(1, 9):
    os.rename('pleb_01_0' + str(i) + '.png', 'pleb/pleb_' + f'{(i - 1 + 8 * 6):02d}' + '.png')
    os.rename('pleb_02_0' + str(i) + '.png', 'pleb/pleb_' + f'{(i - 1 + 8 * 4):02d}' + '.png')
    os.rename('pleb_03_0' + str(i) + '.png', 'pleb/pleb_' + f'{(i - 1 + 8 * 0):02d}' + '.png')
    os.rename('pleb_04_0' + str(i) + '.png', 'pleb/pleb_' + f'{(i - 1 + 8 * 2):02d}' + '.png')
    os.rename('pleb_05_0' + str(i) + '.png', 'pleb/pleb_' + f'{(i - 1 + 8 * 5):02d}' + '.png')
    os.rename('pleb_06_0' + str(i) + '.png', 'pleb/pleb_' + f'{(i - 1 + 8 * 3):02d}' + '.png')
    os.rename('pleb_07_0' + str(i) + '.png', 'pleb/pleb_' + f'{(i - 1 + 8 * 7):02d}' + '.png')
    os.rename('pleb_08_0' + str(i) + '.png', 'pleb/pleb_' + f'{(i - 1 + 8 * 1):02d}' + '.png')
