#!/usr/bin/env ruby

require "serialport.so"
require "optparse"

port_baud = 115200
port_path = "/dev/ttyUSB0"

opts = OptionParser.new do |o|
	o.banner = "Usage: upload.rb [options] <file.srec>"

	o.on( "-b", "--baud BAUDRATE", Integer, 
			"Serial port baudrate (default: #{port_baud})" ) do |baud|
		port_baud = baud
	end

	o.on( "-s", "--serial SERIALPORT", 
	         "Path to serial port (default: #{port_path})" ) do |port|
		port_path = port
	end

	o.on_tail( "-h", "--help", "Display this help message" ) do
		puts o
		exit 0
	end
end

#############################################################################
# Check arguments, open serial port and file
begin
	opts.parse!(ARGV)

	raise "SREC file argument missing" if ARGV.length != 1;

	file_path = ARGV[0]

	file = File.open( file_path, "r" )
	raise "Could not SREC file." if not file;

	fileSize = File.size( file_path )

	port = SerialPort.new(port_path, port_baud, 8, 1, SerialPort::NONE)
	raise "Could not open serial port." if not port;

rescue => e
	STDERR.puts "\nERROR: #{e.message}"
	STDERR.puts
	STDERR.puts opts
	STDERR.puts
	exit 1
end


#############################################################################
# 
begin
    port.putc 'r'

    begin
        putc(ch = port.getc)
    end while ch != 62
    puts "Spike bootloader found"

	#
	#port.puts "v4000000040001000"
    #begin
    #    putc(ch = port.getc)
    #end while ch != 62
	#exit 0

	uploadFile = file
	sp = port
    
    ch = 0;
    # warte auf Eingabe
        
    error = 0
    puts "Transmitting..."
    uploadFile.each_line do |line|
		b = 1
		if line[0..1] == "S3"
            begin 
                sp.putc 'u' 
                sz = line[2..-5]
                sz.each_byte do |b|
                    putc '.'
                    sp.putc b
                end   
                c1 = sp.getc
                c2 = sp.getc  
                c  = sprintf("%c%c", c1, c2)
                if error > 3
                    puts "checksum error"
                    break
                end          
                error += 1    
            end while c.hex != line[-4..-3].hex
            begin
                putc(ch = sp.getc)
            end while ch != 62
            puts " "
            error = 0
		end
	end	
	puts "Upload done."
    		
    puts "Run..."
    sp.printf("g40000000")
    puts "Complete"

    begin
        putc(ch = sp.getc)
    end while ch != 62   
   
ensure 
    uploadFile.close unless uploadFile.nil?
    sp.close unless sp.nil?
end