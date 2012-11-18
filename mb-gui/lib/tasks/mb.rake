namespace :mb do
	task :load_networks, [:dir] => :environment do |t,args|
		f = args[:dir]
		if File.exists? f and File.directory? f
			Dir.foreach f do |file|
				if file =~ /.*\.mbn$/
					Network.create_from_file File.expand_path ( File.join f, file )
				end
			end
		end
	end

	task :load_datasets, [:dir] => :environment do |t,args|
		f = args[:dir]
		if File.exists? f and File.directory? f
			Dir.foreach f do |file|
				if file =~ /.*\.txt$/
					Dataset.create_from_file File.expand_path ( File.join f, file )
				end
			end
		end
	end
end
