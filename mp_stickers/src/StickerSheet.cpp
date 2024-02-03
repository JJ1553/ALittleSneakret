#include "StickerSheet.h"
#include <vector>
#include <tuple>
#include "cs225/PNG.h"

        StickerSheet::StickerSheet(const Image &picture, unsigned max) {
            baseImg = new Image(picture);
            max_ = max;
        }

        StickerSheet::~StickerSheet() {
            delete baseImg;
        }

        StickerSheet::StickerSheet(const StickerSheet &other) {
            baseImg = new Image(*other.baseImg);
            max_ = other.max_;

            stickerList.clear();
            coordinates.clear();

            coordinates = other.coordinates;
            stickerList = other.stickerList;
            /*
            for(unsigned i = 0 ; i < other.coordinates.size() ; i++) {
                coordinates.push_back(other.coordinates[i]);
            }
            for(unsigned i = 0 ; i < other.stickerList.size() ; i++) {
                stickerList.push_back(other.stickerList[i]);
            }
            */
        }

        const StickerSheet& StickerSheet::operator=(const StickerSheet &other){
            if (this != &other) {
                
                stickerList.clear();
                coordinates.clear();

                
                delete baseImg; 
                baseImg = new Image(*other.baseImg);
                max_ = other.max_;

                
                coordinates = other.coordinates;
                stickerList = other.stickerList;
            }

            return *this;
        }

        void  StickerSheet::changeMaxStickers(unsigned max) {
            if(max_ == max) {
                return;
            }
            unsigned x = stickerList.size();  
                if (x > 0) {
                    x -= 1;  
                } else {
    
                }
            if((max >= 0) && (max < stickerList.size())) {
                for(int i = (int)x ; i > (int)max-1 ; i--) { 
                    removeSticker(i);
                }
            }
            /*
             if (max < max_) {
        
                for (unsigned i = max; i < max_; i++) {
                    removeSticker(i);
                }
            }
            */
            max_ = max;
        }

        int StickerSheet::addSticker(Image &sticker, int x, int y){
            
            int layer = (int)stickerList.size();

            coordinates.emplace_back(x, y);

            stickerList.push_back(&sticker);

            if(stickerList.size() > max_) {
                max_ = stickerList.size();
            }

            return layer;
        }

        int  StickerSheet::setStickerAtLayer(Image &sticker, unsigned layer, int x, int y) {
              
            if(layer >= 0  && layer < stickerList.size()) {
                stickerList[layer] = &sticker;
                coordinates[layer] = std::make_tuple(x, y);
                return layer;
            }
            else {
                return -1;
            }
        }

        bool  StickerSheet::translate(unsigned index, int x, int y) {
            //stickerList[index] == nullptr ||
            if( index < 0 || index > stickerList.size() || index > coordinates.size()) {
                return false;
            }
            else {
                coordinates[index] = std::make_tuple(x, y);
                return true;
            }
        }

        void  StickerSheet::removeSticker(unsigned index) {
                if(index >=0 && index < stickerList.size()) {
                    stickerList.erase(stickerList.begin() + index);
                    coordinates.erase(coordinates.begin() + index);
                }
        }

        Image* StickerSheet::getSticker(unsigned index) {
            if(index >= 0 && index < stickerList.size()) {
                return stickerList[index];
            }
            else {
                return nullptr;
            }
        }

        int  StickerSheet::layers() const {
            return max_;
        }

        Image StickerSheet::render() const {
            
            int minX = 0, minY = 0;
            int maxX = (int)(baseImg->width()), maxY = (int)(baseImg->height());
            
            for (int i = 0; i < (int)coordinates.size(); i++) {
                int x, y;
                std::tie(x, y) = coordinates[i];

                
                minX = std::min(minX, x);
                minY = std::min(minY, y);
                maxX = std::max(maxX, x + (int)(stickerList[i]->width()));
                maxY = std::max(maxY, y + (int)(stickerList[i]->height()));
            }

            unsigned int resultWidth = maxX - minX;
            unsigned int resultHeight = maxY - minY;

            Image result = Image((resultWidth), (resultHeight));
            //result.resize(resultWidth, resultHeight);

            
            for (int x = 0; x < (int)baseImg->width(); x++) {
                for (int y = 0; y < (int)baseImg->height(); y++) {
                    result.getPixel(x - minX, y - minY) = baseImg->getPixel(x, y);
                }
            }

            
            for (unsigned int i = 0; i < coordinates.size(); i++) {
                int x, y;
                std::tie(x, y) = coordinates[i];
                
                for (int stickerX = 0; stickerX < (int)stickerList[i]->width(); stickerX++) {
                    for (int stickerY = 0; stickerY < (int)stickerList[i]->height(); stickerY++) {
                        if ((x + stickerX) >= minX && (x + stickerX) < maxX &&
                            (y + stickerY) >= minY && (y + stickerY) < maxY) {
                                if(stickerList[i]->getPixel(stickerX, stickerY).a != 0 ) {
                                    result.getPixel(x + stickerX - minX, y + stickerY - minY) =
                                    stickerList[i]->getPixel(stickerX, stickerY);
                                }
                        }
                    }
                }
            }

            return result;
        }