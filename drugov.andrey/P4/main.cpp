#include <cctype>
#include <cstddef>
#include <exception>
#include <iostream>

namespace drugov
{
  const int error_memory = 1;
  const std::size_t latin_count = 26;
  const std::size_t initial_capacity = 16;
  const std::size_t growth_factor = 2;

  std::size_t length(const char *s) noexcept
  {
    std::size_t len = 0;
    while (s[len] != '\0')
    {
      ++len;
    }
    return len;
  }

  void markLetters(const char *s, int *present) noexcept
  {
    for (std::size_t i = 0; s[i] != '\0'; ++i)
    {
      const unsigned char c = static_cast< unsigned char >(s[i]);
      if (std::isalpha(c) != 0)
      {
        const int low = std::tolower(c);
        if (low >= 'a' and low <= 'z')
        {
          present[low - 'a'] = 1;
        }
      }
    }
  }

  char *latTwo(const char *first, const char *second, char *result) noexcept
  {
    int present[latin_count] = {};
    markLetters(first, present);
    markLetters(second, present);
    std::size_t k = 0;
    for (std::size_t i = 0; i < latin_count; ++i)
    {
      if (present[i] != 0)
      {
        result[k++] = static_cast< char >('a' + i);
      }
    }
    result[k] = '\0';
    return result;
  }

  char *uniTwo(const char *first, const char *second, char *result) noexcept
  {
    const std::size_t len_first = length(first);
    const std::size_t len_second = length(second);
    std::size_t k = 0;
    for (std::size_t i = 0; i < len_first or i < len_second; ++i)
    {
      if (i < len_first)
      {
        result[k++] = first[i];
      }
      if (i < len_second)
      {
        result[k++] = second[i];
      }
    }
    result[k] = '\0';
    return result;
  }

  char *readLine(std::istream &in)
  {
    std::size_t capacity = initial_capacity;
    std::size_t size = 0;
    char *buf = new char[capacity]{};
    char c = 0;
    while (in.get(c) and c != '\n')
    {
      if (size + 1 == capacity)
      {
        const std::size_t new_capacity = capacity * growth_factor;
        char *bigger = nullptr;
        try
        {
          bigger = new char[new_capacity]{};
        }
        catch (...)
        {
          delete[] buf;
          throw;
        }
        for (std::size_t i = 0; i < size; ++i)
        {
          bigger[i] = buf[i];
        }
        delete[] buf;
        buf = bigger;
        capacity = new_capacity;
      }
      buf[size++] = c;
    }
    buf[size] = '\0';
    return buf;
  }

  int process(std::istream &in, std::ostream &out, std::ostream &err)
  {
    const char second_lat[] = "def_ghk";
    const char second_uni[] = "def_";

    const char *line = nullptr;
    char *lat = nullptr;
    char *uni = nullptr;
    try
    {
      line = readLine(in);
      lat = new char[latin_count + 1]{};
      uni = new char[length(line) + length(second_uni) + 1]{};
    }
    catch (const std::exception &)
    {
      delete[] line;
      delete[] lat;
      delete[] uni;
      err << "not enough memory\n";
      return error_memory;
    }

    out << latTwo(line, second_lat, lat) << "\n";
    out << uniTwo(line, second_uni, uni) << "\n";

    delete[] line;
    delete[] lat;
    delete[] uni;
    return 0;
  }
}

int main()
{
  return drugov::process(std::cin, std::cout, std::cerr);
}
