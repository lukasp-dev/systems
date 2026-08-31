class Solution {
    private:
        struct Result{
            int minPrice;
            int maxPrice;
            int bestProfit;
        };
        
        Result getMaxProfit(vector<int>& prices, int left, int right){
            if(left == right) {
                return {prices[left], prices[left], 0};
            }
            
            int mid = left + (right - left) / 2;
            
            Result L = getMaxProfit(prices, left, mid);
            Result R = getMaxProfit(prices, mid + 1, right);
            
            Result result;
            result.minPrice = min(L.minPrice, R.minPrice);
            result.maxPrice = max(L.maxPrice, R.maxPrice);
            
            int crossing = R.maxPrice - L.minPrice;
            result.bestProfit = max({
                L.bestProfit,
                R.bestProfit, 
                crossing
            });
            
            return result;
        }
        
    public:
        int maxProfit(vector<int>& prices) {
            if(prices.empty()) return 0;
            
            return getMaxProfit(prices, 0, prices.size()-1).bestProfit;
        }
    };