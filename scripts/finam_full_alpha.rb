#!/usr/bin/ruby

require './mbtools.rb'

HEADER='<TICKER>,<PER>,<DATE>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<VOL>'
EXP='[^,]*,[^,]*,[^,]*,[^,]*,([^,]*),([^,]*),([^,]*),([^,]*),[^,]*'
INPUT_SIZE=8
OUTPUT_SIZE=2
LINE_SIZE=4
RANGE_LEFT=10
RANGE_RIGHT=12
STEP=2
NORM_CURVE=10

def norm x
	# Normalization function - fl(x-1), where fl is a logistic function: fl(x) = 1/(1+N^(-x))
	# x should be around (0,+inf) value basically around 1
	1 / (1 + NORM_CURVE**(-x+1))
end

def parse_test data

	# linear mapping of input to the range [input.min, input.max]
	input = data[0...(INPUT_SIZE*LINE_SIZE)]
	input_min = input.min
	input_max = input.max
	input.collect! { |x| (x - input_min) / (input_max - input_min) }

	last = data[INPUT_SIZE*LINE_SIZE]
	range = data[RANGE_LEFT*LINE_SIZE...RANGE_RIGHT*LINE_SIZE]
	output = [ norm(range.min/last), norm(range.max/last) ]
	return TestCase.new	input, output
end

def parse file
	first = file.gets
	if first.chop != HEADER
		puts 'Unsupported header: ' + first + ' != ' + HEADER
		return nil
	end

	ts = TestSuite.new File.basename(file.path), INPUT_SIZE*LINE_SIZE, OUTPUT_SIZE
	input = []
	output = []
	buffer = []
	file.each_line do |line|
		m = line.match EXP
		if m == nil || m.size != LINE_SIZE + 1
			puts 'Invalid line: ' + line			
			puts 'Match size: ' + m.size.to_s
		else
			m[1..4].each do |str|
				buffer << str.to_f
			end
			if buffer.size >= RANGE_RIGHT * LINE_SIZE
				tc = parse_test buffer
				if not ts.add_testcase tc
					puts 'Test adding failed!'
				end
				buffer.slice! 0...STEP*LINE_SIZE
			end
		end
	end
	return ts
end

if ARGV.size < 1
	puts 'Usage: finam_full_alpha.rb <DATA_FILE>'
else
	begin
		f = File.open ARGV[0], 'r'
		ts = parse f
		if ts != nil
			puts ts.tests.size.to_s + ' tests parsed.'
			ts.write_to_file f.path
		end
		f.close()
	rescue IOError
		puts 'IO error'
	end
end
