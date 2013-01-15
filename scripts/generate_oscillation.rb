#!/usr/bin/ruby

require './mbtools.rb'

if ARGV.size > 0
    ts = TestSuite.new 'Oscillation function', 10, 0
    x = 0
    while x < 100 do
	a = [] 
	10.times do
	    a.push Math.sin(x).round 4
	    x += 0.2
	end
	ts.add_test a, []
    end
    ts.write_to_file ARGV[0]
else
    puts "Usage: generate_mul_test.rb <FILE_NAME>" 
end
