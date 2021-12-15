from PIL import Image

grass_0 = Image.open("grass_layer_0.png")
grass_1 = Image.open("grass_layer_1.png")
grass_2 = Image.open("grass_layer_2.png")
grass_3 = Image.open("grass_layer_3.png")

for i in range(16):
    grass_base = Image.open("grass_layer_base.png")
    binary = '{0:04b}'.format(i)[::-1]

    if (binary[0] == '1'):
        grass_base.paste(grass_0, (0,0), grass_0)
    if (binary[1] == '1'):
        grass_base.paste(grass_1, (0,0), grass_1)
    if (binary[2] == '1'):
        grass_base.paste(grass_2, (0,0), grass_2)
    if (binary[3] == '1'):
        grass_base.paste(grass_3, (0,0), grass_3)

    grass_base.save("grass/grass_%02d.png" % (i))
