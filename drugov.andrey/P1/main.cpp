#include <iostream>

namespace drugov
{
  int process()
  {
    int N = 0;
    int oldN = 0;
    std::cin >> N;

    if (!std::cin)
    {
      std::cerr << "invalid input\n";
      return 1;
    }

    if (N == 0)
    {
      std::cout << 0 << "\n" << 0 << "\n";
      return 0;
    }

    int max_four = 1;
    int cur_four = 1;

    int max_six = 1;
    int cur_six = 1;

    oldN = N;
    std::cin >> N;

    while (N != 0)
    {
      if (!std::cin)
      {
        std::cerr << "invalid input\n";
        return 1;
      }

      if (N == oldN)
      {
        cur_four = cur_four + 1;
      }
      else
      {
        cur_four = 1;
      }

      if (cur_four > max_four)
      {
        max_four = cur_four;
      }

      if (N >= oldN)
      {
        cur_six = cur_six + 1;
      }
      else
      {
        cur_six = 1;
      }

      if (cur_six > max_six)
      {
        max_six = cur_six;
      }

      oldN = N;
      std::cin >> N;
    }

    std::cout << max_four << "\n" << max_six << "\n";

    return 0;
  }
}

int main()
{
  return drugov::process();
}

