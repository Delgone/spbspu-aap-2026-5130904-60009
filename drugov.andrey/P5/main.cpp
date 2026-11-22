#include <cstddef>
#include <iostream>

#include "shape.hpp"
#include "base-types.hpp"

namespace drugov
{
  const int error_input = 1;
  const double pi = 3.14159265358979323846;
  const double sqrt_two = 1.4142135623730951;
  const double two = 2.0;

  class Rectangle: public Shape
  {
  public:
    Rectangle(const point_t &pos, double width, double height):
      pos_(pos),
      width_(width),
      height_(height)
    {}

    double getArea() const override
    {
      return width_ * height_;
    }

    rectangle_t getFrameRect() const override
    {
      return {width_, height_, pos_};
    }

    void move(const point_t &point) override
    {
      pos_ = point;
    }

    void move(double dx, double dy) override
    {
      pos_.x += dx;
      pos_.y += dy;
    }

    void scale(double factor) override
    {
      width_ *= factor;
      height_ *= factor;
    }

  private:
    point_t pos_;
    double width_;
    double height_;
  };

  class Xquare: public Shape
  {
  public:
    Xquare(const point_t &pos, double side):
      pos_(pos),
      side_(side)
    {}

    double getArea() const override
    {
      return side_ * side_;
    }

    rectangle_t getFrameRect() const override
    {
      const double diagonal = side_ * sqrt_two;
      return {diagonal, diagonal, pos_};
    }

    void move(const point_t &point) override
    {
      pos_ = point;
    }

    void move(double dx, double dy) override
    {
      pos_.x += dx;
      pos_.y += dy;
    }

    void scale(double factor) override
    {
      side_ *= factor;
    }

  private:
    point_t pos_;
    double side_;
  };

  class Bubble: public Shape
  {
  public:
    Bubble(const point_t &pos, const point_t &circle_center, double radius):
      pos_(pos),
      circle_center_(circle_center),
      radius_(radius)
    {}

    double getArea() const override
    {
      return pi * radius_ * radius_;
    }

    rectangle_t getFrameRect() const override
    {
      const double diameter = two * radius_;
      return {diameter, diameter, circle_center_};
    }

    void move(const point_t &point) override
    {
      move(point.x - pos_.x, point.y - pos_.y);
    }

    void move(double dx, double dy) override
    {
      pos_.x += dx;
      pos_.y += dy;
      circle_center_.x += dx;
      circle_center_.y += dy;
    }

    void scale(double factor) override
    {
      circle_center_.x = pos_.x + (circle_center_.x - pos_.x) * factor;
      circle_center_.y = pos_.y + (circle_center_.y - pos_.y) * factor;
      radius_ *= factor;
    }

  private:
    point_t pos_;
    point_t circle_center_;
    double radius_;
  };

  void scaleAll(Shape *const *shapes, std::size_t count, const point_t &point, double factor)
  {
    for (std::size_t i = 0; i < count; ++i)
    {
      const point_t before = shapes[i]->getFrameRect().pos;
      shapes[i]->move(point);
      shapes[i]->scale(factor);
      const point_t after = shapes[i]->getFrameRect().pos;
      const double target_x = point.x + (before.x - point.x) * factor;
      const double target_y = point.y + (before.y - point.y) * factor;
      shapes[i]->move(target_x - after.x, target_y - after.y);
    }
  }

  rectangle_t getTotalFrame(const Shape *const *shapes, std::size_t count)
  {
    const rectangle_t first = shapes[0]->getFrameRect();
    double left = first.pos.x - first.width / two;
    double right = first.pos.x + first.width / two;
    double bottom = first.pos.y - first.height / two;
    double top = first.pos.y + first.height / two;
    for (std::size_t i = 1; i < count; ++i)
    {
      const rectangle_t frame = shapes[i]->getFrameRect();
      const double frame_left = frame.pos.x - frame.width / two;
      const double frame_right = frame.pos.x + frame.width / two;
      const double frame_bottom = frame.pos.y - frame.height / two;
      const double frame_top = frame.pos.y + frame.height / two;
      left = frame_left < left ? frame_left : left;
      right = frame_right > right ? frame_right : right;
      bottom = frame_bottom < bottom ? frame_bottom : bottom;
      top = frame_top > top ? frame_top : top;
    }
    const point_t center = {(left + right) / two, (bottom + top) / two};
    return {right - left, top - bottom, center};
  }

  void printFrame(const rectangle_t &frame)
  {
    std::cout << "frame: center (" << frame.pos.x << ", " << frame.pos.y << "), ";
    std::cout << "width " << frame.width << ", height " << frame.height << "\n";
  }

  void printShapes(const Shape *const *shapes, std::size_t count)
  {
    double total_area = 0.0;
    for (std::size_t i = 0; i < count; ++i)
    {
      const double area = shapes[i]->getArea();
      total_area += area;
      std::cout << "shape " << i + 1 << ": area " << area << ", ";
      printFrame(shapes[i]->getFrameRect());
    }
    std::cout << "total area: " << total_area << "\n";
    std::cout << "total ";
    printFrame(getTotalFrame(shapes, count));
  }

  int process()
  {
    point_t scale_point = {0.0, 0.0};
    double factor = 0.0;
    if (not(std::cin >> scale_point.x >> scale_point.y >> factor))
    {
      std::cerr << "invalid input\n";
      return error_input;
    }
    if (not(factor > 0.0))
    {
      std::cerr << "scale factor must be positive\n";
      return error_input;
    }

    const point_t rectangle_pos = {1.0, 2.0};
    const double rectangle_width = 4.0;
    const double rectangle_height = 2.0;
    const point_t xquare_pos = {-3.0, 0.5};
    const double xquare_side = 2.0;
    const point_t bubble_pos = {6.0, 6.0};
    const point_t bubble_circle_center = {6.5, 6.5};
    const double bubble_radius = 2.0;

    Rectangle rectangle(rectangle_pos, rectangle_width, rectangle_height);
    Xquare xquare(xquare_pos, xquare_side);
    Bubble bubble(bubble_pos, bubble_circle_center, bubble_radius);

    const std::size_t count = 3;
    Shape *const shapes[count] = {&rectangle, &xquare, &bubble};

    std::cout << "before scaling:\n";
    printShapes(shapes, count);
    scaleAll(shapes, count, scale_point, factor);
    std::cout << "after scaling:\n";
    printShapes(shapes, count);
    return 0;
  }
}

int main()
{
  return drugov::process();
}
