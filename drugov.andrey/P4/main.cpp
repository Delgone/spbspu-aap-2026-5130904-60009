#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace drugov
{
  const int ERROR_MEMORY = 1;
  const std::size_t LATIN_COUNT = 26;

  std::size_t length(const char *s) noexcept
  {
    std::size_t len = 0;
    while (s[len] != '\0')
    {
      ++len;
    }
    return len;
  }

  char *uni_two(const char *first, const char *second, char *result) noexcept
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

  void mark_letters(const char *s, int *present) noexcept
  {
    for (std::size_t i = 0; s[i] != '\0'; ++i)
    {
      unsigned char c = static_cast< unsigned char >(s[i]);
      if (std::isalpha(c))
      {
        int low = std::tolower(c);
        if (low >= 'a' and low <= 'z')
        {
          present[low - 'a'] = 1;
        }
      }
    }
  }

  char *lat_two(const char *first, const char *second, char *result) noexcept
  {
    int present[LATIN_COUNT] = {};
    mark_letters(first, present);
    mark_letters(second, present);
    std::size_t k = 0;
    for (std::size_t i = 0; i < LATIN_COUNT; ++i)
    {
      if (present[i] != 0)
      {
        result[k++] = static_cast< char >('a' + i);
      }
    }
    result[k] = '\0';
    return result;
  }

  void fill_zero(char *s, std::size_t size) noexcept
  {
    for (std::size_t i = 0; i < size; ++i)
    {
      s[i] = '\0';
    }
  }

  char *read_line(std::istream &in)
  {
    std::size_t capacity = 16;
    std::size_t size = 0;
    char *buf = static_cast< char * >(std::malloc(capacity));
    if (buf == nullptr)
    {
      return nullptr;
    }
    fill_zero(buf, capacity);
    char c = 0;
    while (in.get(c) and c != '\n')
    {
      if (size + 1 == capacity)
      {
        if (capacity > std::numeric_limits< std::size_t >::max() / 2)
        {
          std::free(buf);
          return nullptr;
        }
        std::size_t new_capacity = capacity * 2;
        char *bigger = static_cast< char * >(std::malloc(new_capacity));
        if (bigger == nullptr)
        {
          std::free(buf);
          return nullptr;
        }
        fill_zero(bigger, new_capacity);
        for (std::size_t i = 0; i < size; ++i)
        {
          bigger[i] = buf[i];
        }
        std::free(buf);
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
    const char second_uni[] = "def_";
    const char second_lat[] = "def_ghk";

    char *line = read_line(in);
    if (line == nullptr)
    {
      err << "not enough memory\n";
      return ERROR_MEMORY;
    }

    const std::size_t len = length(line);
    const std::size_t max_len = std::numeric_limits< std::size_t >::max();
    if (len > max_len - length(second_uni) - 1)
    {
      std::free(line);
      err << "not enough memory\n";
      return ERROR_MEMORY;
    }

    char *uni = static_cast< char * >(std::malloc(len + length(second_uni) + 1));
    char *lat = static_cast< char * >(std::malloc(LATIN_COUNT + 1));
    if (uni == nullptr or lat == nullptr)
    {
      std::free(line);
      std::free(uni);
      std::free(lat);
      err << "not enough memory\n";
      return ERROR_MEMORY;
    }

    fill_zero(uni, len + length(second_uni) + 1);
    fill_zero(lat, LATIN_COUNT + 1);
    out << uni_two(line, second_uni, uni) << "\n";
    out << lat_two(line, second_lat, lat) << "\n";

    std::free(line);
    std::free(uni);
    std::free(lat);
    return 0;
  }
}

int main()
{
  return drugov::process(std::cin, std::cout, std::cerr);
}
