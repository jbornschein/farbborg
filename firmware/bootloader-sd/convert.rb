i = 0
to0  = File.open("zImage.h", "w") do |fto0|
to1  = File.open("zImage.c", "w") do |fto1|
from = File.open("image.bin", "r") do |ffrom|
	fto1.puts("unsigned char zImage[] = {");
	while c = ffrom.getc
		if ((i % 16) == 0) 
			fto1.puts("\n\t\t")
		fto1.printf("0x%X, ", c)	
		i = i + 1
	end
	fto1.puts("};\n\n");
	fto0.puts("#ifndef Z_IMAGE_H\n#define Z_IMAGE_H\n\n");
	fto0.printf("#define Z_IMAGE_SIZE %d", i);
	fto0.puts("extern unsigned char zImage[];");
	fto0.puts("#endif // Z_IMAGE_H\n");
end
end
end

