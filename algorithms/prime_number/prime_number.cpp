#include <bits/stdc++.h>
using namespace std;

void print(const vector<int>& numbers, const vector<bool>& sieve, int N){
    for(int i=0; i<N; ++i){
        if(sieve[numbers[i]] == true) cout << numbers[i] << " ";
    }

    cout << "\n";
}

int main(){
    vector<int> numbers = {
        523, 88, 997, 4, 101, 762, 31, 881, 120, 443,
        9, 607, 224, 13, 853, 911, 55, 67, 317, 941,
        2, 400, 613, 71, 829, 144, 461, 53, 900, 239,
        17, 777, 359, 66, 499, 151, 83, 619, 12, 733,
        293, 8, 577, 641, 93, 227, 463, 79, 503, 311
    };
    int N = numbers.size();
    if(N == 0) {
        cout << "there is no number to sieve\n";
        return -1;
    }

    auto it = max_element(numbers.begin(), numbers.end());
    int _max = *it;
    vector<bool> sieve(_max+1, true);
    
    if(_max >= 0) sieve[0] = false;
    if(_max >= 1) sieve[1] = false;

    for(int i=2; i<=sqrt(_max); ++i){
        if(sieve[i]){
            for(int j=i*i; j<=_max; j += i){
                sieve[j] = false;
            }
        }
    }

    cout << "The prime numbers are: ";
    print(numbers, sieve, N);

    return 0;
}

/**
 * 1. 먼저 주어진 범위까지 배열을 생성한다. 1은 소수가 아니므로 삭제하고, 배열은 2부터 시작한다.
 * 2. 선택한 수의 배수를 모두 삭제한다 (선택한 수는 그대로). 현재의 경우 2의 배수를 모두 삭제.
 * 3. 다음 지워지지 않은 수를 선택한다. 즉, 3을 선택하고 선택한 수의 모든 배수를 삭제한다.
 * 4. 앞의 과정을 배열의 끝까지 반복한다.
 * 5. 삭제되지 않은 수를 모두 출력한다.
 * 
 * 몇 번 그려보면 알 수 있는데, 배수 삭제를 진행할 때, 예를 들어 i = 2 라고 하면
 * i*i 부터 시작해서 +i, +2i 이렇게 해나가는게 더 효율적이고 빠르다.
 * 
 * 에라토스테네스의 체의 시간 복잡도
 * 일반적으로 에라토스테네스의 체를 구현하려면 이중 for 문을 이용하므로 시간 복잡도가 O(N^2) 정도라고
 * 판별할 수 있다. 하지만 실제 시간 복잡도는 최적화의 정도에 따라 다르겠지만, 일반적으로 O(Nlog(logN)) 이다.
 * 그 이유는 배수를 삭제하는 연산으로 실제 구현에서 바깥쪽 for문을 생략하는 경우가 빈번하게 발생하기 때문.
 * 이러한 이유 때문에 에라토스테네스의 체 기법은 현재에도 코딩 테스트에서 소수를 구하는 일반적인 방법으로 통용되고 있다.
 */