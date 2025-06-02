#include "include/first_app.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;
int main() {
  lve::FirstApp app{};
  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
