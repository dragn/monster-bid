class Network < ActiveRecord::Base
	serialize :weights
	serialize :biases
	def self.create_from_file file
		puts "Creating network from file " + file.to_s
		f = File.open file, "r"
		iterator = f.gets.split.each
		n = Network.create( :name => File.basename( file, '.mbn' ) )
		n.size = iterator.next.to_i
		n.input_size = iterator.next.to_i
		n.output_size = iterator.next.to_i
		n.weights = []
		(n.size*n.size).times do
			n.weights << iterator.next.to_f
		end
		n.biases = []
		n.size.times do
			n.biases << iterator.next.to_f
		end		
		n.save
	end
end
