#include "Image.h"
#include "cs225/PNG.h"

    Image::Image():PNG() {}
    Image::Image(unsigned int width, unsigned int height) : PNG(width, height) {}

    void Image::darken() {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a, b).l <= 0.1) {
                    getPixel(a, b).l -= 0.1;
                }
                else {
                    getPixel(a, b).l = 0.0;
                }
            }
        }
    }

    void Image::darken(double amount) {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a, b).l >= amount) {
                    getPixel(a, b).l -= amount;
                }
                else {
                    getPixel(a, b).l = 0.0;
                }
            }
        }
    }

    void Image::lighten() {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a, b).l <= 0.9) {
                    getPixel(a, b).l += 0.1;
                }
                else {
                    getPixel(a, b).l = 1;
                }
            }
        }
    }

    void Image::lighten(double amount) {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a, b).l <= 1 - amount) {
                    getPixel(a, b).l += amount;
                }
                else {
                    getPixel(a, b).l = 1;
                }
            }
        }
    }

    void Image::desaturate() {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a, b).s >= 0.1) {
                    getPixel(a, b).s -= 0.1;
                }
                else {
                    getPixel(a, b).s = 0.0;
                }
            }
        }
    }

    void Image::desaturate(double amount) {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a, b).s >= amount) {
                    getPixel(a, b).s -= amount;
                }
                else {
                    getPixel(a, b).s = 0.0;
                }
            }
        }
    }

    void Image::saturate() {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a, b).s <= 0.9) {
                    getPixel(a, b).s += 0.1;
                }
                else {
                    getPixel(a, b).s = 1;
                }
            }
        }
    }

    void Image::saturate(double amount) {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a, b).s <= 1 - amount) {
                    getPixel(a, b).s += amount;
                }
                else {
                    getPixel(a, b).s = 1;
                }
            }
        }
    }

    void Image::greyscale() {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                getPixel(a, b).s = 0;
            }
        }
    }

    void Image::illinify() {
        double midPone = (216-11)/2;
        double midPtwo = ((360+11)+216)/2;

        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                if(getPixel(a,b).h <= midPone || getPixel(a,b).h > midPtwo) {
                    getPixel(a,b).h = 11;
                }
                else {
                    getPixel(a,b).h = 216;
                }
            }
        }
    }

    void Image::rotateColor(double degrees) {
        for(unsigned int a = 0 ; a < width() ; a++ ) {
            for(unsigned int b = 0 ; b < height() ; b++ ) {
                getPixel(a,b).h += degrees;

                if(getPixel(a,b).h > 360) {
                    getPixel(a,b).h -= 360;
                }
                else if (getPixel(a,b).h < 0){
                    getPixel(a,b).h += 360;
                }
            }
        }
    }

    void Image::scale(double factor) {
        if(factor != 1.0 && factor >= 0.0) {
            Image temp = *this;

            resize(((unsigned int)(factor*width())), ((unsigned int)(factor*height())));

            for(unsigned int a = 0 ; a < width() ; a++ ) {
                for(unsigned int b = 0 ; b < height() ; b++ ) {
                    getPixel(a,b) = temp.getPixel(((unsigned int)(a/factor)), ((unsigned int)(b/factor)));
                }
            }


        }
    }

    void Image::scale(unsigned w, unsigned h) {

        if( (double)w / (double)width() <= (double)h / (double)height()) {
            scale((double)w / (double)width());
        }
        else {
            scale((double)h / (double)height());
        }
    }