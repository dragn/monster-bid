# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended to check this file into your version control system.

ActiveRecord::Schema.define(:version => 20120322130024) do

  create_table "datasets", :force => true do |t|
    t.string   "name"
    t.date     "start_date"
    t.date     "end_date"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  create_table "networks", :force => true do |t|
    t.string   "name"
    t.integer  "size"
    t.integer  "input_size"
    t.integer  "output_size"
    t.string   "weights"
    t.string   "biases"
    t.datetime "created_at",  :null => false
    t.datetime "updated_at",  :null => false
  end

  create_table "tickers", :force => true do |t|
    t.integer  "dataset_id"
    t.date     "date"
    t.string   "period"
    t.float    "open"
    t.float    "low"
    t.float    "high"
    t.float    "close"
    t.integer  "volume"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

end
