class Networks < ActiveRecord::Migration
	def up
		create_table :networks do |t|
			t.string :name
			t.integer :size
			t.integer :input_size
			t.integer :output_size
			t.string :weights
			t.string :biases
			t.timestamps
		end
	end

	def down
		drop_table :networks
	end
end
