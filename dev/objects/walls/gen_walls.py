from PIL import Image

wall_0 = Image.open("objects/walls/wall_layer_0.png")
wall_1 = Image.open("objects/walls/wall_layer_1.png")
wall_2 = Image.open("objects/walls/wall_layer_2.png")
wall_3 = Image.open("objects/walls/wall_layer_3.png")

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

    wall_base = Image.open("objects/walls/wall_layer_base.png")

    if (binary[1] == '1'):
        wall_base.paste(wall_0, (0,0), wall_0)
    if (binary[3] == '1'):
        wall_base.paste(wall_1, (0,0), wall_1)
    if (binary[5] == '1'):
        wall_base.paste(wall_2, (0,0), wall_2)
    if (binary[7] == '1'):
        wall_base.paste(wall_3, (0,0), wall_3)

    wall_base.save("../resources/objects/walls/wall_%03d.png" % (i))
