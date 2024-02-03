/* Your code here! */

#include "maze.h"
#include "dsets.h"
#include <vector>
#include <algorithm>
#include <random>
#include <queue>

SquareMaze::SquareMaze():width_(0), height_(0) {
    
    dSet.addelements(0);
}
void SquareMaze::makeMaze(int width, int height){
    width_ = width;
    height_ = height;
    dSet.addelements(width_ * height_);
    totalMaze.clear();

    std::vector<int> temp;

    for (int i = 0; i < width_ * height_; ++i) {
        totalMaze.push_back(Cell());  
        temp.push_back(i);
    }

    std::random_device rd;
    auto rng = std::default_random_engine(rd());
    std::shuffle(std::begin(temp), std::end(temp), rng);

    for(auto i : temp) {
         int x = i % width_;
        int y = i / width_;

        if(((i + 1) % width_ != 0) && (dSet.find(i) != dSet.find(i + 1))){
            dSet.setunion(i, i + 1);
            setWall(x, y, 0, false);
        }

        if(((i + width_) < width_ * height_) && (dSet.find(i) != dSet.find(i + width_))){
            dSet.setunion(i, i + width_);
            setWall(x, y, 1, false);
        }
    }
    

}
bool SquareMaze::canTravel(int x, int y, int dir) const{
    int index = y * width_ + x; 

    
    if (dir == 0) {
        if (x < width_ - 1 && totalMaze[index].rightWall) {
            return false; 
        }
    }
    
    else if (dir == 1) {
        if (y < height_ - 1 && totalMaze[index].bottomWall) {
            return false; 
        }
    }
    
    else if (dir == 2) {
        if (x > 0 && totalMaze[index - 1].rightWall) {
            return false; 
        }
    }
    
    else if (dir == 3) {
        if (y > 0 && totalMaze[x+(y-1)* width_].bottomWall) {
            return false; 
        }
    }

    return true;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists){
    int index = y * width_ + x;

    
    if (dir == 0) {
        
        totalMaze[index].rightWall = exists;
    } else if (dir == 1) {
        
        totalMaze[index].bottomWall = exists;
    }
}

std::vector<int> SquareMaze::solveMaze(){
     
     
    std::vector<int> distances(width_ * height_, 0); 
    std::vector<int> parent(width_ * height_, -1);
    
    std::queue<int> q;
    q.push(0); 
    
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        int x = curr % width_;
        int y = curr / width_;
        
        
        for (int dir = 0; dir < 4; ++dir) {
            int nx = x;
            int ny = y;
            if (dir == 0) nx += 1; // Right
            else if (dir == 1) ny += 1; // Down
            else if (dir == 2) nx -= 1; // Left
            else if (dir == 3) ny -= 1; // Up
            
            
            if (nx >= 0 && nx < width_ && ny >= 0 && ny < height_ && canTravel(x, y, dir) && distances[ny * width_ + nx] == 0) {
                distances[ny * width_ + nx] = distances[curr] + 1;
                parent[ny * width_ + nx] = curr;
                q.push(ny * width_ + nx);
            }
        }
    }
    
    
    int destination = 0;
    int maxDistance = 0;
    for (int x = 0; x < width_; ++x) {
        int curr = (height_ - 1) * width_ + x;
        if (distances[curr] > maxDistance) {
            maxDistance = distances[curr];
            destination = curr;
        }
    }
    
    
    std::vector<int> path;
    while (destination != 0) {
        int parentCell = parent[destination];
        int dx = (destination % width_) - (parentCell % width_);
        int dy = (destination / width_) - (parentCell / width_);
        if (dx == 1) path.push_back(0); // Right
        else if (dx == -1) path.push_back(2); // Left
        else if (dy == 1) path.push_back(1); // Down
        else if (dy == -1) path.push_back(3); // Up
        destination = parentCell;
    }
    
    
    reverse(path.begin(), path.end());
    return path;
}

cs225::PNG* SquareMaze::drawMaze() const{
    
    cs225::PNG *mazeImage = new cs225::PNG(width_ * 10 + 1, height_ * 10 + 1);
    
    
    for (int x = 10; x <= width_ * 10; x++) {
        mazeImage->getPixel(x, 0).l = 0; 
    }
    for (int x = 0; x <= width_ * 10; x++) {
        mazeImage->getPixel(x, height_*10).l = 0; 
    }
    
    for (int y = 0; y <= height_ * 10; y++) {
        mazeImage->getPixel(0, y).l = 0; 
    }
    for (int y = 0; y <= height_ * 10; y++) {
        mazeImage->getPixel(width_*10, y).l = 0; 
    }
    
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            
            if (!canTravel(x, y, 0)) {
                
                for (int k = 0; k <= 10; k++) {
                    mazeImage->getPixel((x + 1) * 10, y * 10 + k).l = 0; 
                }
            }
            
            if (!canTravel(x, y, 1)) {
                
                for (int k = 0; k <= 10; k++) {
                    mazeImage->getPixel(x * 10 + k, (y + 1) * 10).l = 0; 
                }
            }
        }
    }
    
    return mazeImage;
}

cs225::PNG* SquareMaze::drawMazeWithSolution() {
    
    cs225::PNG* mazeImage = drawMaze();

    std::vector<int> solution = solveMaze();
    
    int x = 5;
    int y = 5;
    int exitX = 0;
    int exitY = 0;;
    int count = 0;
    for (int i = 0; i < (int)solution.size(); i++) {
        int dir = solution[i];
       
        for (int k = 0; k < 10; k++) {
            

                    cs225::HSLAPixel& pixel = mazeImage->getPixel(x, y);
                    pixel.h = 0;
                    pixel.s = 1;
                    pixel.l = 0.5;
                    pixel.a = 1;
            
                
            
            if (dir == 0) {
                x++; // Right
                exitX++;
            } else if (dir == 1) {
                y++; // Down
                exitY++;
            } else if (dir == 2) {
                x--; // Left
                exitX--;
            } else if (dir == 3) {
                y--; // Up
                exitY--;
            }
            
            if (i == (int)solution.size()-1 && count == 0) {
                    
                    int lastStep = solution[i];
                    int dx = 0;
                    int dy = 0;
                    if (lastStep == 0) {
                        dx = 9; // Right
                    } else if (lastStep == 1) {
                        dy = 9; // Down
                    } else if (lastStep == 2) {
                        dx = -9; // Left
                    } else if (lastStep == 3) {
                        dy = -9; // Up
                    }

                    
                    if (x + k + dx >= 0 && x + k + dx < (int)mazeImage->width() &&
                        y + k + dy >= 0 && y + k + dy < (int)mazeImage->height()) {
                        cs225::HSLAPixel& extraPixel = mazeImage->getPixel(x + k + dx, y + k + dy);
                        extraPixel.h = 0;
                        extraPixel.s = 1;
                        extraPixel.l = 0.5;
                        extraPixel.a = 1;
                        
                    }
                    count++;
                }
        }
    }

    for (int k = 1; k <= 9; ++k) {
            
            mazeImage->getPixel(exitX + k, (exitY + 10)).l = 1;
    }

    return mazeImage;
}


