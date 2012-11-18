class Dataset < ActiveRecord::Base
	has_many :tickers

	def self.create_from_file file
		puts 'Parsing dataset from file ' + file
		ds = Dataset.create :name => File.basename(file, '.txt')
		f = File.new file, 'r'
		header = f.gets
		f.each_line do |line|
			ds.parse_line line
		end
		ds.save
	end

	def parse_line line
		if line =~ /^\<.*/; 
			return 'Line starts with "<". Skipping...' ; 
		end;
		if line.split(',').size != 9;
			puts 'Wrong line format: ' + line;
			return; 
		end;
		t = self.tickers.create
		iterator = line.split(',').each
		name = iterator.next
		t.period = iterator.next		
		date = iterator.next
		time = iterator.next
		t.date = Date.strptime( date + ' ' + time, '%Y%m%d %H%M%S')
		t.open = iterator.next.to_f
		t.high = iterator.next.to_f
		t.low = iterator.next.to_f
		t.close = iterator.next.to_f
		t.volume = iterator.next.to_i
		t.save
	end
end
