from PIL import Image

wall_0 = Image.open("wall_layer_0.png")
wall_1 = Image.open("wall_layer_1.png")
wall_2 = Image.open("wall_layer_2.png")
wall_3 = Image.open("wall_layer_3.png")

for i in range(16):
    wall_base = Image.open("wall_layer_base.png")
    binary = '{0:04b}'.format(i)[::-1]

    if (binary[0] == '1'):
        wall_base.paste(wall_0, (0,0), wall_0)
    if (binary[1] == '1'):
        wall_base.paste(wall_1, (0,0), wall_1)
    if (binary[2] == '1'):
        wall_base.paste(wall_2, (0,0), wall_2)
    if (binary[3] == '1'):
        wall_base.paste(wall_3, (0,0), wall_3)

    wall_base.save("walls/wall_%02d.png" % (i))
