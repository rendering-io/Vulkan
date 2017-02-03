template<typename F, typename I>
auto transform(I begin, I end, F f) {
  using result_type = decltype(f(*begin));
  std::vector<result_type> results;
  results.reserve(std::distance(begin, end));
  
  for (auto &item: vk::iterator_range<I>{begin, end}) {
    results.push_back(f(item));
  }
  return results;
}
