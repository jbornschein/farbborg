
to0  = File.open("image0.ram", "w") do |fto0|
to1  = File.open("image1.ram", "w") do |fto1|

from = File.open("image.ram", "r") do |ffrom|
	while line = ffrom.gets
		fto1.puts line[0..3]
		fto0.puts line[4..7]
	end
end
end
end

