class CreateTickers < ActiveRecord::Migration
  def change
    create_table :tickers do |t|
	  t.integer :dataset_id
	  t.date :date
	  t.string :period
	  t.float :open
	  t.float :low
	  t.float :high
	  t.float :close
	  t.integer :volume
      t.timestamps
    end
  end
end
