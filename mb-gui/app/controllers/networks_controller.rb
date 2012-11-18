class NetworksController < ApplicationController
	def view
		@networks = Network.all
	end

	def run
		@networks = Network.all
		@datasets = Dataset.all

		if params.has_key? :net_id
			@network = Network.find_by_id params[:net_id]
		end

		if params.has_key? :ds_id
			@dataset = Dataset.find_by_id params[:ds_id]
		end

		@tickers = []
		if @network and @dataset			
			@dataset.tickers.all(:limit => 30).each do |t|
				@tickers << [t.date.to_s, t.low, t.open, t.close, t.high] 
			end
		end
	end
end
