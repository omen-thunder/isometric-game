from PIL import Image

water_1_file = "water_layer_1.png"
water_2_file = "water_layer_2.png"
water_3_file = "water_layer_3.png"
water_4_file = "water_layer_4.png"
water_5_file = "water_layer_5.png"
water_6_file = "water_layer_6.png"
water_7_file = "water_layer_7.png"
water_8_file = "water_layer_8.png"
water_base_file = "water_layer_base.png"
water_full_file = "water_layer_full.png"

water_1 = Image.open(water_1_file)
water_2 = Image.open(water_2_file)
water_3 = Image.open(water_3_file)
water_4 = Image.open(water_4_file)
water_5 = Image.open(water_5_file)
water_6 = Image.open(water_6_file)
water_7 = Image.open(water_7_file)
water_8 = Image.open(water_8_file)
water_full = Image.open(water_full_file)


for i in range(256):
	water_base = Image.open(water_base_file);
	binary = '{0:08b}'.format(i)
	memory = [0, 0, 0, 0, 0, 0, 0, 0]

	if (binary[6] == '1'):
		water_base.paste(water_2, (0,0), water_2)
		memory[1] = 1
	if (binary[4] == '1'):
		water_base.paste(water_4, (0,0), water_4)
		memory[3] = 1
	if (binary[2] == '1'):
		water_base.paste(water_6, (0,0), water_6)
		memory[5] = 1
	if (binary[0] == '1'):
		water_base.paste(water_8, (0,0), water_8)
		memory[7] = 1
	if (binary[7] == '1' or (binary[6] == '1' and binary[0] == '1')):
		water_base.paste(water_1, (0,0), water_1)
		memory[0] = 1
	if (binary[5] == '1' or (binary[6] == '1' and binary [4] == '1')):
		water_base.paste(water_3, (0,0), water_3)
		memory[2] = 1
	if (binary[3] == '1' or (binary[4] == '1' and binary[2] == '1')):
		water_base.paste(water_5, (0,0), water_5)
		memory[4] = 1
	if (binary[1] == '1' or (binary[2] == '1' and binary[0] == '1')):
		water_base.paste(water_7, (0,0), water_7)
		memory[6] = 1
	if (all(ele == 1 for ele in memory)):
		water_base.paste(water_full, (0,0), water_full)

	water_base.save("water/water_%03d.png" % (i))
