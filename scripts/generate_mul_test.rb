#!/usr/bin/ruby

require './mbtools.rb'

if ARGV.size > 0
	ts = TestSuite.new 'Multiplication testing', 3, 1
	rg = Random.new
	40.times do
		a = rg.rand 0.0..5.0 
		b = rg.rand 0.0..0.2 
		c = rg.rand 0.0..1.0
		ts.add_test [a, b, c], [a*b*c]
	end
	ts.write_to_file ARGV[0]
else
	puts "Usage: generate_mul_test.rb <FILE_NAME>" 
end
