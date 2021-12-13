from PIL import Image

water_base_file = "water_layer_base.png"
water_1_file = "water_layer_1.png"
water_2_file = "water_layer_2.png"
water_3_file = "water_layer_3.png"
water_4_file = "water_layer_4.png"
water_5_file = "water_layer_5.png"
water_6_file = "water_layer_6.png"
water_7_file = "water_layer_7.png"
water_8_file = "water_layer_8.png"

water_1 = Image.open(water_1_file);
water_2 = Image.open(water_2_file);
water_3 = Image.open(water_3_file);
water_4 = Image.open(water_4_file);
water_5 = Image.open(water_5_file);
water_6 = Image.open(water_6_file);
water_7 = Image.open(water_7_file);
water_8 = Image.open(water_8_file);


for i in range(256):
	water_base = Image.open(water_base_file);
	binary = '{0:08b}'.format(i)

	if (binary[6] == '1'):
		water_base.paste(water_2, (0,0), water_2)
	if (binary[4] == '1'):
		water_base.paste(water_4, (0,0), water_4)
	if (binary[2] == '1'):
		water_base.paste(water_6, (0,0), water_6)
	if (binary[0] == '1'):
		water_base.paste(water_8, (0,0), water_8)
	if (binary[7] == '1'):
		water_base.paste(water_1, (0,0), water_1)
	if (binary[5] == '1'):
		water_base.paste(water_3, (0,0), water_3)
	if (binary[3] == '1'):
		water_base.paste(water_5, (0,0), water_5)
	if (binary[1] == '1'):
		water_base.paste(water_7, (0,0), water_7)

	water_base.save("test/water_%03d.png" % (i))
