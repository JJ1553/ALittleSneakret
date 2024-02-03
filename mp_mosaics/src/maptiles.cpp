/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */

        MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    vector<Point<3>> points;
    for (const auto& tile : theTiles) {
        points.push_back(convertToXYZ(tile.getAverageColor()));
    }

    KDTree<3> tree(points);

    for (int row = 0; row < theSource.getRows(); row++) {
        for (int col = 0; col < theSource.getColumns(); col++) {
            LUVAPixel getColor = theSource.getRegionColor(row, col);
            Point<3> targetColor = convertToXYZ(getColor);
            Point<3> nearestColor = tree.findNearestNeighbor(targetColor);

            // Find the index of the nearest tile in theTiles vector
            int nearestIndex = -1;
            size_t i = 0;
            while(i < points.size()) {
                if (points[i] == nearestColor) {
                    nearestIndex = (int)(i);
                    break;
                }
                i++;
            }

            if (nearestIndex != -1) {
                TileImage& bestMatchTile = theTiles[nearestIndex];
                canvas->setTile(row, col, &bestMatchTile);
            } 
        }
    }

    return canvas;
}

