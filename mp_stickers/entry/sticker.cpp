#include "Image.h"
#include "StickerSheet.h"

int main() {
  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  Image test = Image (2219, 1521);
  test.readFromFile("/workspaces/CS225/cs225git/mp_stickers/data/Andromeda2_Original.png");
  Image test1 = Image (600, 500);
  test1.readFromFile("/workspaces/CS225/cs225git/mp_stickers/data/Subject.png");
  test1.scale(600,500);
  Image test2 = Image (340, 860);
  test2.readFromFile("/workspaces/CS225/cs225git/mp_stickers/data/Subject 3.png");
  test2.scale(340,860);
  Image test3 = Image (600, 560);
  test3.readFromFile("/workspaces/CS225/cs225git/mp_stickers/data/Subject 4.png");
  test3.scale(600,560);
  StickerSheet final = StickerSheet(test, 5);
  final.addSticker (test1, 0 , 0);
  final.addSticker(test2, 1800, 0); 
  final.addSticker (test3, 1600, 900);

  final.render().writeToFile("FinalTestImageJaJenks2.png");

  return 0;
}
