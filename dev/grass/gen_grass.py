from PIL import Image

grass_0 = Image.open("grass/grass_layer_0.png")
grass_1 = Image.open("grass/grass_layer_1.png")
grass_2 = Image.open("grass/grass_layer_2.png")
grass_3 = Image.open("grass/grass_layer_3.png")

for i in range(256):
    binary = '{0:08b}'.format(i)[::-1]
    if (binary[0] == '1'):
        continue
    if (binary[2] == '1'):
        continue
    if (binary[4] == '1'):
        continue
    if (binary[6] == '1'):
        continue
    if not(binary[1] == '1' and binary[7] == '1') and not(binary[1] == '1' and binary[3] == '1') and not(binary[3] == '1' and binary[5] == '1') and not(binary[5] == '1' and binary[7] == '1') and i != 0:
        continue

    grass_base = Image.open("grass/grass_layer_base.png")

    if (binary[1] == '1' and binary[7] == '1'):
        grass_base.paste(grass_0, (0,0), grass_0)
    if (binary[1] == '1' and binary[3] == '1'):
        grass_base.paste(grass_1, (0,0), grass_1)
    if (binary[3] == '1' and binary[5] == '1'):
        grass_base.paste(grass_2, (0,0), grass_2)
    if (binary[5] == '1' and binary[7] == '1'):
        grass_base.paste(grass_3, (0,0), grass_3)

    grass_base.save("../resources/tiles/grass/grass_%03d.png" % (i))
