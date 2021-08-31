#include <type_traits>
#include <cstdlib>
#include <iostream>
#include <map>


template<typename... Args> auto memoize(std::invocable<Args...> auto &&func) {
  return [func = std::forward<decltype(func)>(func),
           cache = std::map<std::tuple<Args...>,
             std::invoke_result_t<decltype(func), Args...>>{}](
           Args &&...args) mutable {
    if (auto itr =
          cache.find(std::tuple<Args...>{ std::forward<Args>(args)... });
        itr != std::end(cache)) {
      return itr->second;
    }
    auto res = func(args...);
    cache[std::tuple<Args...>{ std::forward<Args>(args)... }] = res;
    return res;
  };
}

int main() {
  using namespace std;
  auto mem_rand = memoize<>(rand);
  std::cout << mem_rand() << std::endl;
  std::cout << mem_rand() << std::endl;
  std::cout << mem_rand() << std::endl;
  std::cout << mem_rand() << std::endl;
  std::cout << mem_rand() << std::endl;
}
