#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h*h) + (s*s) + (l*l));
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_front(point);
  }

  /**
  * Removes the current Point in the bfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    work_list.pop_front();
    
  }

  /**
  * Removes the current Point in the dfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    work_list.pop_front();
  }

  /**
   * Returns the current Point in the bfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point bfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    return work_list.front();
  }

  /**
   * Returns the current Point in the dfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point dfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    return work_list.front();
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param fns the set of functions describing a traversal's operation
  */
   ImageTraversal::Iterator::Iterator(const PNG* image, const Point& start, double tolerance, const TraversalFunctions& functions)
    : image_(image), start_(start), tolerance_(tolerance), functions_(functions) {
    work_list_.push_back(start);
    visited_.resize(image_->width(), std::vector<bool>(image_->height(), false));
  }
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {  
    /** @todo [Part 1] */
    it = Iterator(&png, start, tolerance, fns);
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return it;
    //maybe make a new iterator with iterator private variable 
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return Iterator();
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
    image_ = nullptr;
    start_ = Point(0, 0);
    tolerance_ = 0.0;
    functions_ = {nullptr, nullptr, nullptr};
    work_list_.clear(); //add
    visited_.clear(); //add
  }


  /**
  * Iterator increment operator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    /** @todo [Part 1] */
        
    

    if(work_list_.empty()) {
      return *this;
    }
    Point currentPoint = functions_.peek(work_list_);

    visited_[currentPoint.x][currentPoint.y] = true;

    while(!work_list_.empty() && visited_[functions_.peek(work_list_).x][functions_.peek(work_list_).y]) {
      functions_.pop(work_list_);
    }

    
    
    
    Point neighbors[4] = {
        Point(currentPoint.x + 1, currentPoint.y),
        Point(currentPoint.x, currentPoint.y + 1),
        Point(currentPoint.x - 1, currentPoint.y),
        Point(currentPoint.x, currentPoint.y - 1)
    };

    for (const Point &neighbor : neighbors) {
        
        if (neighbor.x >= 0 && neighbor.x < image_->width() &&
            neighbor.y >= 0 && neighbor.y < image_->height() &&
            !visited_[neighbor.x][neighbor.y]) {
            
            const HSLAPixel &startPixel = image_->getPixel(start_.x, start_.y);
            const HSLAPixel &neighborPixel = image_->getPixel(neighbor.x, neighbor.y);
            double delta = calculateDelta(startPixel, neighborPixel);

            
            if (delta < tolerance_) {
                functions_.add(work_list_, neighbor);
               // visited_[neighbor.x][neighbor.y] = true;
            }
        }
    }
    
        return *this; 
    
  }

  /**
  * Iterator accessor operator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    
    return work_list_.front();
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    return !(work_list_.empty() && other.work_list_.empty());
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
    return work_list_.empty();
  }

}