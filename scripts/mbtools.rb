class TestCase
	def initialize input, output
		@input = input;
		@output = output;
	end
	attr_accessor :input;
	attr_accessor :output;
end

class TestSuite
	attr_accessor :name
	attr_reader :input_size
	attr_reader :output_size
	attr_accessor :tests

	def initialize name, input_size, output_size, tests = []
		@name = name
		@input_size = input_size
		@output_size = output_size
		@tests = tests
	end

	def add_testcase test
		if test.is_a? TestCase and 
			test.input.size == self.input_size and
			test.output.size == self.output_size
			self.tests << test
			return true
		else return false
		end
	end

	def add_test input, output
		if input.size == self.input_size and
			output.size == self.output_size
			@tests << TestCase.new(input, output)
			return true;
		else return false;
		end
	end

	def write_to_file filename 
		filename += '.tsc'
		if File.exist? filename
			STDOUT.write "File " + filename + " already exists. Overwrite?[Y/N]: ";
			answer = STDIN.getc
			if answer.chr != 'Y' and answer.chr != 'y'; 
				puts "Got '" + answer + "' - will not ovewrite"
				return; 
			end;
		end
		f = File.new filename, "w";
		write_to f
	rescue IOError
		puts "Error: " + error;
	ensure
		f.close unless f.nil?
	end

	def write_to io
		write_header io
		write_data io
	end

	private
	def write_header io
		io.puts "# -------------- #"
		io.puts "# Header section #"
		io.puts "# -------------- #"
		io.puts "FILENAME: " + @name
		io.puts "INPUT_SIZE: " + @input_size.to_s
		io.puts "OUTPUT_SIZE: " + @output_size.to_s
	end

	def write_data io
		io.puts "# -------------- #"
		io.puts "# Data section   #"
		io.puts "# -------------- #"
		tests.each { |test|
			io.write "\nIN: "
			test.input.each { |a| io.write a.to_s + " " }
			io.write "\nOUT: "
			test.output.each { |a| io.write a.to_s + " " }
		}
	end
end
