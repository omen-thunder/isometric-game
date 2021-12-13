from PIL import Image

wall_1_file = "wall_layer_1.png"
wall_2_file = "wall_layer_2.png"
wall_3_file = "wall_layer_3.png"
wall_4_file = "wall_layer_4.png"
wall_base_file = "wall_layer_base.png"

wall_1 = Image.open(wall_1_file)
wall_2 = Image.open(wall_2_file)
wall_3 = Image.open(wall_3_file)
wall_4 = Image.open(wall_4_file)

for i in range(16):
	wall_base = Image.open(wall_base_file);
	binary = '{0:04b}'.format(i)

	if (binary[3] == '1'):
		wall_base.paste(wall_1, (0,0), wall_1)
	if (binary[2] == '1'):
		wall_base.paste(wall_2, (0,0), wall_2)
	if (binary[1] == '1'):
		wall_base.paste(wall_3, (0,0), wall_3)
	if (binary[0] == '1'):
		wall_base.paste(wall_4, (0,0), wall_4)

	wall_base.save("walls/wall_%02d.png" % (i))
