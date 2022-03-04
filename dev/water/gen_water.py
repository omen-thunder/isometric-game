from PIL import Image

water_0 = Image.open("water/water_layer_0.png")
water_1 = Image.open("water/water_layer_1.png")
water_2 = Image.open("water/water_layer_2.png")
water_3 = Image.open("water/water_layer_3.png")
water_4 = Image.open("water/water_layer_4.png")
water_5 = Image.open("water/water_layer_5.png")
water_6 = Image.open("water/water_layer_6.png")
water_7 = Image.open("water/water_layer_7.png")
water_full = Image.open("water/water_layer_full.png")

for i in range(256):
    binary = '{0:08b}'.format(i)[::-1]
    if (binary[0] == '1' and binary[1] == '1' and binary[7] == '1'):
        continue
    if (binary[2] == '1' and binary[1] == '1' and binary [3] == '1'):
        continue
    if (binary[4] == '1' and binary[3] == '1' and binary[5] == '1'):
        continue
    if (binary[6] == '1' and binary[5] == '1' and binary[7] == '1'):
        continue

    water_base = Image.open("water/water_layer_base.png")

    if (binary[1] == '1'):
        water_base.paste(water_1, (0,0), water_1)
    if (binary[3] == '1'):
        water_base.paste(water_3, (0,0), water_3)
    if (binary[5] == '1'):
        water_base.paste(water_5, (0,0), water_5)
    if (binary[7] == '1'):
        water_base.paste(water_7, (0,0), water_7)
    if (binary[0] == '1' or (binary[1] == '1' and binary[7] == '1')):
        water_base.paste(water_0, (0,0), water_0)
    if (binary[2] == '1' or (binary[1] == '1' and binary [3] == '1')):
        water_base.paste(water_2, (0,0), water_2)
    if (binary[4] == '1' or (binary[3] == '1' and binary[5] == '1')):
        water_base.paste(water_4, (0,0), water_4)
    if (binary[6] == '1' or (binary[5] == '1' and binary[7] == '1')):
        water_base.paste(water_6, (0,0), water_6)
    if (all(char == '1' for char in binary)):
        water_base.paste(water_full, (0,0), water_full)

    water_base.save("../resources/tiles/water/water_%03d.png" % (i))
