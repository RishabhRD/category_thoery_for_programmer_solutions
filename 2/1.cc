#include <iostream>
#include <map>


template<typename... Args>
auto meomoize(auto &&func) requires std::invocable<decltype(func), Args...> {
  return [func = std::forward<decltype(func)>(func),
           cache = std::map<std::tuple<Args...>,
             std::invoke_result_t<decltype(func), Args...>>{}](
           Args &&...args) mutable {
    if (auto itr = cache.find(std::tuple{ std::forward<Args>(args)... });
        itr != std::end(cache)) {
      return itr->second;
    }
    auto res = func(args);
    cache[std::tuple{ std::forward<Args>(args)... }] = res;
    return res;
  };
}

auto func(int i, int c) {
  std::cout << i << " " << c << "\n";
  return i + c;
}

int main() {
  using namespace std;
  auto mem_func = meomoize<int, int>(func);
  cout << mem_func(1, 2) << endl;
  cout << mem_func(1, 2) << endl;
}
