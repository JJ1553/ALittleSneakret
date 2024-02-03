

#include <iostream>
#include "Image.h"
#include "StickerSheet.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

int main() {
  Image alma; alma.readFromFile("../data/alma.png");
  Image i;    i.readFromFile("../data/i.png");
  Image expected; expected.readFromFile("../data/expected.png");
  Image expected2; expected2.readFromFile("../data/expected-2.png");

  StickerSheet s1(alma, 5);
  s1.addSticker(i, 20, 200);
  s1.addSticker(i, 40, 200);

  StickerSheet s2(alma, 5);
  s2 = s1;
  s2.removeSticker(1);

  s1.render().writeToFile("JoshsTest.png");
  s2.render().writeToFile("JoshsTest1.png");
/*
  Image alma; alma.readFromFile("../data/alma.png");
  Image i;    i.readFromFile("../data/i.png");

  StickerSheet sheet(alma, 5);
  std::cout << sheet.layers() << std::endl;
  
  sheet.addSticker(i, 20, 200);

  Image expected;
  expected.readFromFile("../data/expected.png");
  sheet.render().writeToFile("JoshsTest.png");
  */
  /*
  Image alma;

  std::cout << "Using Image::lighten to create `lighten.png`..." << std::endl;
  alma.readFromFile("../data/alma.png");
  alma.lighten(0.2);
  alma.writeToFile("lighten.png");

  std::cout << "Using Image::saturate to create `saturate.png`..." << std::endl;
  alma.readFromFile("../data/alma.png");
  alma.saturate(0.2);
  alma.writeToFile("saturate.png");

  std::cout << "Using Image::scale to create `scale.png`..." << std::endl;
  alma.readFromFile("../data/alma.png");
  alma.scale(2);
  alma.writeToFile("scale2x.png");
  */
  return 0;
}
